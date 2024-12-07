#include "FileDownloader.h"

namespace fs = std::filesystem;

FileDownloader::FileDownloader(QObject* parent) 
	: QObject(parent), m_NetworkAccessManager(new QNetworkAccessManager(this))
{
	connect(m_NetworkAccessManager, &QNetworkAccessManager::finished, this, &FileDownloader::downloadFinished);
}

void FileDownloader::DownloadFile(InstallData& installData, std::string targetFolder)
{
	m_installData = installData;

	// Check if a file already exists
	if (fs::exists("cache") && fs::is_directory("cache"))
	{
		fs::path p("cache/" + m_installData.fileName);
		if (QFile::exists(p) && fs::file_size(p) == installData.fileSize)
		{
			// We already have it downloaded here
			emit fileDownloaded(p.string());
			return;
		}
	}
	else
	{
		// Create the cache folder if it doesn't exist
		fs::create_directory("cache");
	}

	// This is rather horrible???
	QUrl downloadURL(QString(installData.downloadURL.c_str()));

	QNetworkRequest request(downloadURL);

	auto reply = m_NetworkAccessManager->get(request);

	connect(reply, &QNetworkReply::downloadProgress, this, &FileDownloader::sendDownloadProgress);
}

void FileDownloader::sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	emit downloadProgress(bytesReceived, bytesTotal);
}

void FileDownloader::downloadFinished(QNetworkReply* reply)
{
	fs::path p("cache/" + m_installData.fileName);

	QFile file(p);

	file.open(QIODevice::WriteOnly);

	auto data = reply->readAll();
	file.write(data);

	file.close();

	emit fileDownloaded(p.string());
}