#pragma once

#include "InstallData.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QURL>
//#include <QtCore/QSaveFile>
#include <QtCore/QFile>

#include <filesystem>

#include <string>

class FileDownloader : public QObject
{
	Q_OBJECT

public:
	FileDownloader(QObject* parent = nullptr);


	void DownloadFile(InstallData& installData, std::string targetFolder);

signals:
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void fileDownloaded(std::string downloadedFilePath);

private slots:
	void sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadFinished(QNetworkReply* reply);

private:
	QNetworkAccessManager* m_NetworkAccessManager;

	InstallData m_installData;
};