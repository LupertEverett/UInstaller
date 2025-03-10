#pragma once

#include <QObject>

#include <string>
#include <filesystem>

#include "Thirdparty/libarchive/libarchive/archive.h"
#include "Thirdparty/libarchive/libarchive/archive_entry.h"

namespace fs = std::filesystem;

class FileExtractor : public QObject
{
	Q_OBJECT

public:
	FileExtractor(QObject* parent = nullptr);
	~FileExtractor();

	void ExtractTo(const std::string& pathToFile, const std::string& dstPath);

	void OpenStreams();
	void CloseStreams();

signals:
	void ExtractionFinished();

private:
	int copyData();
	archive* m_Reader = nullptr;
	archive* m_Writer = nullptr;
};
