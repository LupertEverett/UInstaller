/* Map Decompress Dialog */

#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QProcess>
#include <QtCore/QDir>

#include <filesystem>

namespace fs = std::filesystem;


class MapDecompressDialog : public QDialog
{
public:
    MapDecompressDialog(QWidget* parent = nullptr, fs::path mapFolderPath = fs::path(""));

private:
    QPlainTextEdit* m_processOutputTextEdit;
    QPushButton* m_closeButton;

    QProcess* m_decompressProcess;

    void setupConnections();
    void startProcess(fs::path mapFolderPath);

private slots:
    void handleProcessReadStandardOutput();
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
};
