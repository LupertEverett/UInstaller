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

signals:
	void ExtractionFinished();

private:
	int copyData();

	archive* m_Reader;
	archive* m_Writer;
};