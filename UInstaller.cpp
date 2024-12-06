#include "UInstaller.h"

#include <QtWidgets/QMessageBox>

#include <algorithm>
#include <cctype>
#include <string>

UInstaller::UInstaller(QWidget* parent)
    : QWidget(parent), 
    m_CurrentInstallData(&UnrealGoldInstallData),
    m_CurrentCommunityPatchInstallData(&UG227kInstallData),
    m_fileDownloader(new FileDownloader(this)),
    m_CommunityPatchDownloader(new FileDownloader(this)),
    m_BonusPackDownloader(new FileDownloader(this)),
    m_ISOExtractor(new FileExtractor(this)),
    m_CommunityPatchExtractor(new FileExtractor(this)),
    m_BonusPackExtractor(new FileExtractor(this))
{
    this->setWindowTitle("UInstaller");

    m_GamePickerGroupBox = new QGroupBox("Game");
    m_GameSettingsGroupBox = new QGroupBox("Game Settings");

    m_GamePickerComboBox = new QComboBox();
    m_GamePickerComboBox->addItem("Unreal Gold");
    m_GamePickerComboBox->addItem("Unreal Tournament");

    m_TargetPathLineEdit = new QLineEdit();
    m_TargetPathLineEdit->setEnabled(false);

    m_StartInstallationButton = new QPushButton("Start");
    m_StartInstallationButton->setEnabled(false);
    m_BrowseFolderButton = new QPushButton("...");
    m_BrowseFolderButton->setToolTip("Browse...");

    m_DownloadProgressBar = new QProgressBar();
    m_DownloadProgressBar->setMinimum(0);

    m_StatusLabel = new QLabel("Status: Idle");

    m_InstallCommunityPatchCheckBox = new QCheckBox("Also install the community patches (227/469)?");
    m_BonusPackCheckBox = new QCheckBox("Also install Bonus Pack 4 (UT only)?");
    m_BonusPackCheckBox->setEnabled(false);

    prepareLayout();
    setupConnections();
}

UInstaller::~UInstaller()
{}

void UInstaller::prepareLayout()
{
    main_layout = new QVBoxLayout(this);

    bottom_button_layout = new QHBoxLayout(this);

    game_picker_group_layout = new QVBoxLayout(this);

    game_picker_group_layout->addWidget(m_GamePickerComboBox);

    game_settings_group_layout = new QVBoxLayout(this);

    game_settings_group_layout->addWidget(m_InstallCommunityPatchCheckBox);
    game_settings_group_layout->addWidget(m_BonusPackCheckBox);

    m_GamePickerGroupBox->setLayout(game_picker_group_layout);
    m_GameSettingsGroupBox->setLayout(game_settings_group_layout);

    auto install_path_layout = new QHBoxLayout(this);

    install_path_layout->addWidget(new QLabel("Install to:"));
    install_path_layout->addWidget(m_TargetPathLineEdit);
    install_path_layout->addWidget(m_BrowseFolderButton);

    bottom_button_layout->addWidget(m_StatusLabel);
    bottom_button_layout->addStretch();
    bottom_button_layout->addWidget(m_StartInstallationButton);

    main_layout->addWidget(m_GamePickerGroupBox);
    main_layout->addWidget(m_GameSettingsGroupBox);
    main_layout->addLayout(install_path_layout);
    main_layout->addWidget(m_DownloadProgressBar);
    main_layout->addLayout(bottom_button_layout);

    main_layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

    this->setLayout(main_layout);
}

void UInstaller::setupConnections()
{
    connect(m_GamePickerComboBox, &QComboBox::currentIndexChanged, this, &UInstaller::handleGamePickerSelectionChanged);

    connect(m_TargetPathLineEdit, &QLineEdit::textChanged, this, &UInstaller::handleTargetPathLineEditChanged);

    connect(m_BrowseFolderButton, &QPushButton::pressed, this, &UInstaller::handleBrowseButtonPressed);
    connect(m_StartInstallationButton, &QPushButton::pressed, this, &UInstaller::handleStartButtonPressed);

    connect(m_fileDownloader, &FileDownloader::downloadProgress, this, &UInstaller::handleDownloadProgress);
    connect(m_fileDownloader, &FileDownloader::fileDownloaded, this, &UInstaller::handleDownloadFinished);

    connect(m_CommunityPatchDownloader, &FileDownloader::downloadProgress, this, &UInstaller::handleCommunityPatchDownloadProgress);
    connect(m_CommunityPatchDownloader, &FileDownloader::fileDownloaded, this, &UInstaller::handleCommunityPatchDownloadFinished);

    connect(m_BonusPackDownloader, &FileDownloader::fileDownloaded, this, &UInstaller::handleBonusPackDownloadFinished);

    connect(m_ISOExtractor, &FileExtractor::ExtractionFinished, this, &UInstaller::handleISOExtractionFinished);
    connect(m_CommunityPatchExtractor, &FileExtractor::ExtractionFinished, this, &UInstaller::handleCommunityPatchExtractionFinished);
    connect(m_BonusPackExtractor, &FileExtractor::ExtractionFinished, this, &UInstaller::handleBP4ExtractionFinished);
}

void UInstaller::handleGamePickerSelectionChanged()
{
    auto choice = m_GamePickerComboBox->currentText();

    if (choice == "Unreal Gold")
    {
        m_CurrentInstallData = &UnrealGoldInstallData;
        m_CurrentCommunityPatchInstallData = &UG227kInstallData;
        m_BonusPackCheckBox->setEnabled(false);
    }
    else
    {
        m_CurrentInstallData = &UnrealTournamentInstallData;
        m_CurrentCommunityPatchInstallData = &UT469dInstallData;
        m_BonusPackCheckBox->setEnabled(true);
    }
}

void UInstaller::handleTargetPathLineEditChanged()
{
    m_StartInstallationButton->setEnabled(!m_TargetPathLineEdit->text().isEmpty());
}

void UInstaller::handleBrowseButtonPressed()
{
    auto dirpath = QFileDialog::getExistingDirectory(this, "Choose a folder");

    if (dirpath != "")
    {
        m_TargetPathLineEdit->setText(dirpath);
    }
}

void UInstaller::handleStartButtonPressed()
{
    SetEnableForAllFields(false);

    m_StatusLabel->setText("Status: Downloading");
    m_fileDownloader->DownloadFile(*m_CurrentInstallData, "");
}

void UInstaller::handleDownloadProgress(qint64 downloaded, qint64 total)
{
    m_DownloadProgressBar->setMaximum(total);
    m_DownloadProgressBar->setValue(downloaded);
}

void UInstaller::handleDownloadFinished(std::string downloadedFilePath)
{
    try
    {
        ExtractISOFile(downloadedFilePath);
    }
    catch (std::exception& e)
    {
        QMessageBox::critical(this, "Error", e.what(), QMessageBox::Ok);
        m_StatusLabel->setText("Status: Idle");
        SetEnableForAllFields(true);
    }
}

void UInstaller::handleCommunityPatchDownloadProgress(qint64 downloaded, qint64 total)
{
    m_DownloadProgressBar->setMaximum(total);
    m_DownloadProgressBar->setValue(downloaded);
}

void UInstaller::handleCommunityPatchDownloadFinished(std::string downloadedFilePath)
{
    m_StatusLabel->setText("Status: Extracting Community Patch");

    // Extract all of the content to a subfolder instead
    fs::path finalPath(m_TargetPathLineEdit->text().toStdString());
    finalPath /= m_CurrentInstallData->gameFolderName;

    m_CommunityPatchExtractor->ExtractTo(downloadedFilePath, finalPath.string());
}

void UInstaller::handleBonusPackDownloadFinished(std::string downloadedFilePath)
{
    m_StatusLabel->setText("Status: Extracting Bonus Pack 4");
    
    // Extract all of the content to a subfolder instead
    fs::path finalPath(m_TargetPathLineEdit->text().toStdString());
    finalPath /= m_CurrentInstallData->gameFolderName;

    m_BonusPackExtractor->ExtractTo(downloadedFilePath, finalPath.string());
}

void UInstaller::handleCommunityPatchExtractionFinished()
{
    if (m_CurrentInstallData == &UnrealTournamentInstallData && m_BonusPackCheckBox->isChecked())
        m_BonusPackDownloader->DownloadFile(UTBonusPack4InstallData, "");
    else
        AllDone();
}

void UInstaller::handleBP4ExtractionFinished()
{
    AllDone();
}

void UInstaller::handleISOExtractionFinished()
{
    PerformPostExtraction();
}

void UInstaller::ExtractISOFile(std::string& downloadedISOPath)
{
    m_StatusLabel->setText("Status: Extracting");

    // Extract all of the content to a subfolder instead
    fs::path finalPath(m_TargetPathLineEdit->text().toStdString());
    finalPath /= m_CurrentInstallData->gameFolderName;

    m_ISOExtractor->ExtractTo(downloadedISOPath, finalPath.string());
}

void UInstaller::PerformPostExtraction()
{
    RenameRootFolders();

    if (m_InstallCommunityPatchCheckBox->isChecked())
    {
        // TODO: Also handle installing community patches
        m_CommunityPatchDownloader->DownloadFile(*m_CurrentCommunityPatchInstallData, "");
        CleanupSystemFolder();
    }
    else if (m_CurrentInstallData == &UnrealTournamentInstallData && m_BonusPackCheckBox->isChecked())
    {
        m_BonusPackDownloader->DownloadFile(UTBonusPack4InstallData, "");
    }
    else
    {
        AllDone();
    }
}

void UInstaller::AllDone()
{
    m_StatusLabel->setText("Status: Done!");

    fs::path gamePath(m_TargetPathLineEdit->text().toStdString());

    gamePath /= m_CurrentInstallData->gameFolderName;

    QDesktopServices::openUrl(QUrl::fromLocalFile(QString(gamePath.c_str())));

    SetEnableForAllFields(true);
}

void UInstaller::RenameRootFolders()
{
    fs::path extracted_path(m_TargetPathLineEdit->text().toStdString());

    extracted_path /= m_CurrentInstallData->gameFolderName;

    // Make all the root folders lowercase.

    auto dirIter = fs::directory_iterator(extracted_path);

    for (auto& dir : dirIter)
    {
        if (dir.is_directory())
        {
            fs::path dirPath = dir.path();
            std::string dirName = dirPath.filename().string();
            fs::path dirParent = dirPath.parent_path();

            std::transform(dirName.begin() + 1, dirName.end(), dirName.begin() + 1, [](unsigned char c) { return std::tolower(c); });

            fs::rename(dirPath, dirParent / dirName);
        }
    }
}

void UInstaller::CleanupSystemFolder()
{
    fs::path systemPath(m_TargetPathLineEdit->text().toStdString());
    systemPath /= m_CurrentInstallData->gameFolderName;
    systemPath /= "System";

    auto dirIter = fs::directory_iterator(systemPath);

    std::vector<std::string> langExts = { ".int", ".det", ".est", ".frt", ".itt" };

    for (auto& entry : dirIter)
    {
        if (entry.is_regular_file())
        {
            for (auto& ext : langExts)
            {
                if (entry.path().extension() == ext)
                {
                    fs::remove(entry.path());
                    break;
                }
            }
        }
    }
}

void UInstaller::SetEnableForAllFields(bool value)
{
    m_GamePickerComboBox->setEnabled(value);

    m_InstallCommunityPatchCheckBox->setEnabled(value);
    m_BonusPackCheckBox->setEnabled(value);
    m_StartInstallationButton->setEnabled(value);
    m_BrowseFolderButton->setEnabled(value);
}
