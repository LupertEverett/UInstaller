#include "FileExtractor.h"
#include "InstallData.h"

#include <stdexcept>
#include <iostream>

FileExtractor::FileExtractor(QObject* parent)
	: QObject(parent)
{
}

FileExtractor::~FileExtractor()
{
	CloseStreams();
}

void FileExtractor::ExtractTo(const std::string& pathToFile, const std::string& dstPath)
{
	OpenStreams();

	archive_entry* entry;

	int res = archive_read_open_filename(m_Reader, pathToFile.c_str(), 10240);

	if (res != 0)
	{
		std::cout << "Couldn\'t open file " + pathToFile << std::endl;
		throw std::runtime_error("Couldn\'t open file " + pathToFile);
	}
		
	for (;;)
	{
		res = archive_read_next_header(m_Reader, &entry);

		if (res == ARCHIVE_EOF)
			break;

		if (res < ARCHIVE_OK)
		{
			std::cout << archive_error_string(m_Reader) << std::endl;
			throw std::runtime_error(archive_error_string(m_Reader));
		}
			

		// Move the file to the destination path
		const char* currentFile = archive_entry_pathname(entry);

		// Check if it is one of the ones we should skip
		bool skipThisFile = false;
		fs::path currentFilePath(currentFile);

		for (std::string skipFile : skipFiles)
		{
			if (currentFilePath.filename() == skipFile)
			{
				skipThisFile = true;
				break;
			}
		}

		if (skipThisFile)
			continue;

		fs::path fullPath(dstPath);
		fullPath /= currentFile;
		archive_entry_set_pathname(entry, fullPath.string().c_str());

		res = archive_write_header(m_Writer, entry);

		if (res < ARCHIVE_OK)
		{
			std::cout << archive_error_string(m_Writer) << std::endl;
			throw std::runtime_error(archive_error_string(m_Writer));
		}
		else if (archive_entry_size(entry) > 0)
		{
			res = copyData();

			if (res == ARCHIVE_FATAL)
			{
				// Only throw an exception on FATAL errors 
				throw std::runtime_error(archive_error_string(m_Writer));
			}
		}
		
		res = archive_write_finish_entry(m_Writer);

		if (res < ARCHIVE_OK)
		{
			std::cout << archive_error_string(m_Writer) << std::endl;
			throw std::runtime_error(archive_error_string(m_Writer));
		}
	}

	CloseStreams();

	emit ExtractionFinished();
}

void FileExtractor::OpenStreams()
{
	int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_FFLAGS;

	m_Reader = archive_read_new();

	archive_read_support_format_all(m_Reader);
	archive_read_support_filter_all(m_Reader);

	m_Writer = archive_write_disk_new();

	archive_write_disk_set_options(m_Writer, flags);
	archive_write_disk_set_standard_lookup(m_Writer);
}

void FileExtractor::CloseStreams()
{
	if (m_Reader)
		archive_read_close(m_Reader);
	if (m_Writer)
		archive_write_close(m_Writer);
}

int FileExtractor::copyData()
{
	int res;
	const void* buff;
	size_t size;
	la_int64_t offset;

	while (true)
	{
		res = archive_read_data_block(m_Reader, &buff, &size, &offset);

		if (res == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (res < ARCHIVE_OK)
			return res;
		
		res = archive_write_data_block(m_Writer, buff, size, offset);

		if ( res < ARCHIVE_OK )
		{
			std::cout << archive_error_string(m_Writer) << std::endl;
			throw std::runtime_error(archive_error_string(m_Writer));
		}
	}
}
