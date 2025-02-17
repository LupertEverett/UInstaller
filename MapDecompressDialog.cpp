#include "MapDecompressDialog.h"

MapDecompressDialog::MapDecompressDialog(QWidget* parent, fs::path mapFolderPath)
    : QDialog(parent),
    m_processOutputTextEdit(new QPlainTextEdit(this)),
    m_closeButton(new QPushButton("Close", this)),
    m_decompressProcess(new QProcess(this))
{
    m_processOutputTextEdit->setReadOnly(true);
    m_closeButton->setEnabled(false);

    auto mainLayout = new QVBoxLayout();
    auto closeButtonLayout = new QHBoxLayout();

    closeButtonLayout->addStretch();
    closeButtonLayout->addWidget(m_closeButton);

    mainLayout->addWidget(new QLabel("<h3>Please wait while the map files are decompressed</h3><p>This dialog will close automatically when there are no errors.</p>"));
    mainLayout->addWidget(m_processOutputTextEdit);
    mainLayout->addLayout(closeButtonLayout);

    setLayout(mainLayout);

    setupConnections();
    // Start the process and stuff...
    startProcess(mapFolderPath);
}

void MapDecompressDialog::setupConnections()
{
    connect(m_decompressProcess, &QProcess::readyReadStandardOutput, this, &MapDecompressDialog::handleProcessReadStandardOutput);
    connect(m_decompressProcess, &QProcess::finished, this, &MapDecompressDialog::handleProcessFinished);
    connect(m_closeButton, &QPushButton::pressed, this, &QDialog::reject);
}

void MapDecompressDialog::startProcess(fs::path mapFolderPath)
{
#ifdef WIN32
    m_decompressProcess->setProgram("uz.exe");
#else
    m_decompressProcess->setProgram("./uz");
#endif

    auto mapDirIter = fs::directory_iterator(mapFolderPath);

    QStringList argList;

    argList += "decompress";

    for (auto& entry: mapDirIter)
    {
        if (entry.is_regular_file() && entry.path().extension() == ".uz")
            argList.push_back(QString(entry.path().c_str()));
    }

    QDir::setCurrent("uz");

    m_decompressProcess->setArguments(argList);

    m_decompressProcess->setProcessChannelMode(QProcess::MergedChannels);

    m_decompressProcess->start();
}

void MapDecompressDialog::handleProcessReadStandardOutput()
{
    auto stdOutput = m_decompressProcess->readAllStandardOutput();

    m_processOutputTextEdit->appendPlainText(QString::fromUtf8(stdOutput));
}

void MapDecompressDialog::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // Don't forget to set the working directory path back afterwards
    // Otherwise UInstaller will try to make a cache folder in uz and download everything again
    QDir::setCurrent("..");

    if (exitCode == 0)
    {
        // Close dialog immediately
        this->accept();
    }
    else
    {
        // Let user see what went wrong. Closing this dialog manually should stop installation.
        m_processOutputTextEdit->appendPlainText("Error(s) occured while trying to decompress maps.\n");
        m_closeButton->setEnabled(true);
    }
}
