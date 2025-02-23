#include "UInstaller.h"

#include <QtWidgets/QMessageBox>

#include <algorithm>
#include <cctype>
#include <string>

UInstaller::UInstaller(QWidget* parent)
    : QWidget(parent),
    m_CurrentInstallData(&UnrealGoldInstallData),
    m_CurrentCommunityPatchInstallData(nullptr),
    m_CurrentBonusPackInstallData(&UGFusionMapPackInstallData),
    m_fileDownloader(new FileDownloader(this)),
    m_CommunityPatchDownloader(new FileDownloader(this)),
    m_BonusPackDownloader(new FileDownloader(this)),
    m_ISOExtractor(new FileExtractor(this)),
    m_CommunityPatchExtractor(new FileExtractor(this)),
    m_BonusPackExtractor(new FileExtractor(this))
{
    this->setWindowTitle("UInstaller v2");

    m_GamePickerGroupBox = new QGroupBox("Game");
    m_GameSettingsGroupBox = new QGroupBox("Game Settings");

    m_GamePickerComboBox = new QComboBox();
    m_GamePickerComboBox->addItem("Unreal Gold");
    m_GamePickerComboBox->addItem("Unreal Tournament");

    m_CommunityPatchPickerComboBox = new QComboBox();
    PopulatePatchPicker();
    m_CommunityPatchPickerComboBox->setEnabled(false);

    m_TargetPathLineEdit = new QLineEdit();
    m_TargetPathLineEdit->setEnabled(false);

    m_StartInstallationButton = new QPushButton("Start");
    m_StartInstallationButton->setEnabled(false);
    m_BrowseFolderButton = new QPushButton("...");
    m_BrowseFolderButton->setToolTip("Browse...");

    m_DownloadProgressBar = new QProgressBar();
    m_DownloadProgressBar->setMinimum(0);

    m_StatusLabel = new QLabel("Status: Idle");
#ifndef WIN32
    m_wineRequiredLabel = new QLabel("<b>Note:</b> You\'ll need to use Wine to run the game.");
#endif

    m_InstallCommunityPatchCheckBox = new QCheckBox("Also install the community patches (227/469)?");
    m_BonusPackCheckBox = new QCheckBox("Also install Fusion Map Pack?");

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

    patch_picker_group_layout = new QHBoxLayout(this);

    patch_picker_group_layout->addWidget(new QLabel("Select Patch:"));
    patch_picker_group_layout->addWidget(m_CommunityPatchPickerComboBox);

    game_settings_group_layout = new QVBoxLayout(this);

    game_settings_group_layout->addWidget(m_InstallCommunityPatchCheckBox);
#ifndef WIN32
    game_settings_group_layout->addWidget(m_wineRequiredLabel);
#endif
    game_settings_group_layout->addLayout(patch_picker_group_layout);
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
    connect(m_CommunityPatchPickerComboBox, &QComboBox::currentIndexChanged, this, &UInstaller::handleCommunityPatchPickerSelectionChanged);

    connect(m_InstallCommunityPatchCheckBox, &QCheckBox::toggled, this, &UInstaller::handleCommunityPatchCheckboxToggled);

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
    connect(m_BonusPackExtractor, &FileExtractor::ExtractionFinished, this, &UInstaller::handleBonusPackExtractionFinished);
}

void UInstaller::handleGamePickerSelectionChanged()
{
    auto choice = m_GamePickerComboBox->currentText();

    if (choice == "Unreal Gold")
    {
        m_CurrentInstallData = &UnrealGoldInstallData;
        //m_CurrentCommunityPatchInstallData = &UG227kInstallData;
        m_CurrentBonusPackInstallData = &UGFusionMapPackInstallData;
        m_BonusPackCheckBox->setText("Also install Fusion Map Pack?");
    }
    else
    {
        m_CurrentInstallData = &UnrealTournamentInstallData;
        //m_CurrentCommunityPatchInstallData = &UT469dInstallData;
        m_CurrentBonusPackInstallData = &UTBonusPack4InstallData;
        m_BonusPackCheckBox->setText("Also install Bonus Pack 4?");
    }

    PopulatePatchPicker();
}

void UInstaller::PopulatePatchPicker()
{
    m_CommunityPatchPickerComboBox->clear();

    auto selectedGame = m_GamePickerComboBox->currentText();

    std::vector<std::string> patchesListStr = getPatchesList(selectedGame.toStdString());

    QStringList patchesListQStr;

    for (auto& patch : patchesListStr)
    {
        patchesListQStr.push_back(QString(patch.c_str()));
    }

    m_CommunityPatchPickerComboBox->addItems(patchesListQStr);

    handleCommunityPatchPickerSelectionChanged();
}

void UInstaller::handleCommunityPatchPickerSelectionChanged()
{
    auto selectedPatch = m_CommunityPatchPickerComboBox->currentText();
    m_CurrentCommunityPatchInstallData = getPatchData(selectedPatch.toStdString());
}

void UInstaller::handleCommunityPatchCheckboxToggled(bool toggle)
{
    m_CommunityPatchPickerComboBox->setEnabled(toggle);
#ifndef WIN32
    m_wineRequiredLabel->setVisible(!toggle);
#endif
}

void UInstaller::handleTargetPathLineEditChanged()
{
    QString lineEditText = m_TargetPathLineEdit->text();

    if (m_lastInstallationSuccessful)
        m_StartInstallationButton->setEnabled(!lineEditText.isEmpty() || lineEditText != m_lastTargetPath);
    else
        m_StartInstallationButton->setEnabled(!lineEditText.isEmpty());
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
    m_lastTargetPath = m_TargetPathLineEdit->text();
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
        // Close streams if there are any open
        m_ISOExtractor->CloseStreams();
        m_CommunityPatchExtractor->CloseStreams();
        m_BonusPackExtractor->CloseStreams();

        m_lastInstallationSuccessful = false;
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
    m_StatusLabel->setText("Status: Extracting Bonus Pack");
    
    // Extract all of the content to a subfolder instead
    fs::path finalPath(m_TargetPathLineEdit->text().toStdString());
    finalPath /= m_CurrentInstallData->gameFolderName;

    // Fusion Map Pack should be extracted to Maps folder
    if (m_CurrentBonusPackInstallData == &UGFusionMapPackInstallData)
        finalPath /= "Maps";

    m_BonusPackExtractor->ExtractTo(downloadedFilePath, finalPath.string());
}

void UInstaller::handleCommunityPatchExtractionFinished()
{
    if (m_BonusPackCheckBox->isChecked())
        m_BonusPackDownloader->DownloadFile(*m_CurrentBonusPackInstallData, "");
    else
        AllDone();
}

void UInstaller::handleBonusPackExtractionFinished()
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
    fs::path gameRootPath(m_TargetPathLineEdit->text().toStdString());
    gameRootPath /= m_CurrentInstallData->gameFolderName;

    RenameRootFolders(gameRootPath);
    RemoveUselessFolders();
#ifndef WIN32
    handleLinuxFolderExtractionErrors(gameRootPath);
#endif
    CleanupSystemFolder();

    if (m_CurrentInstallData == &UnrealTournamentInstallData)
    {
        DecompressMapFiles(gameRootPath);
    }
    else
    {
        handleOtherSettings();
    }
}

void UInstaller::handleOtherSettings()
{
    if (m_InstallCommunityPatchCheckBox->isChecked())
    {
        m_CommunityPatchDownloader->DownloadFile(*m_CurrentCommunityPatchInstallData, "");
    }
    else if (m_BonusPackCheckBox->isChecked())
    {
        m_BonusPackDownloader->DownloadFile(*m_CurrentBonusPackInstallData, "");
    }
    else
    {
        AllDone();
    }
}

void UInstaller::DeleteInstallationFolder()
{
    fs::path gameRootPath(m_TargetPathLineEdit->text().toStdString());
    gameRootPath /= m_CurrentInstallData->gameFolderName;

    fs::remove_all(gameRootPath);
}

void UInstaller::AllDone()
{
    m_StatusLabel->setText("Status: Done!");

    m_lastInstallationSuccessful = true;

    fs::path gamePath(m_TargetPathLineEdit->text().toStdString());

    gamePath /= m_CurrentInstallData->gameFolderName;

    QDesktopServices::openUrl(QUrl::fromLocalFile(QString(gamePath.c_str())));

    SetEnableForAllFields(true);

    // The user will have to change the path for the start button to activate again
    m_StartInstallationButton->setEnabled(false);
}

void UInstaller::RenameRootFolders(fs::path gameRootFolderPath)
{
    // Make all the root folders lowercase.
    auto dirIter = fs::directory_iterator(gameRootFolderPath);

    for (auto& dir : dirIter)
    {
        if (dir.is_directory())
        {
            fs::path dirPath = dir.path();
            std::string dirName = dirPath.filename().string();
            fs::path dirParent = dirPath.parent_path();

            std::transform(dirName.begin() + 1, dirName.end(), dirName.begin() + 1, [](unsigned char c) { return std::tolower(c); });

            // Only rename the folder if the folder isn't renamed already
            // Avoids an error in the cases you're trying to "override" an installation
            if (dirName != dirPath.filename().string())
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

void UInstaller::RemoveUselessFolders()
{
    fs::path gamePath(m_TargetPathLineEdit->text().toStdString());
    gamePath /= m_CurrentInstallData->gameFolderName;

    std::vector<std::string> uselessFolders {
        "Directx7",
        "Gamespy",
        "Microsoft",
        "Netgamesusa.com",
        "System400"
    };

    for (auto& folderName : uselessFolders)
    {
        if (fs::exists(gamePath / folderName) && fs::is_directory(gamePath / folderName))
            fs::remove_all(gamePath / folderName);
    }

    if (fs::exists(gamePath / "Setup.exe") && fs::is_regular_file(gamePath / "Setup.exe"))
        fs::remove(gamePath / "Setup.exe");
}

void UInstaller::DecompressMapFiles(fs::path gameRootPath)
{
    fs::path mapsPath = gameRootPath / "Maps";

    auto mapDirIter = fs::directory_iterator(mapsPath);

    m_mapDecompressDialog = new MapDecompressDialog(this, mapsPath);

    if (m_mapDecompressDialog->exec())
    {
        for (auto& entry : mapDirIter)
        {
            if (entry.is_regular_file() && entry.path().extension() == ".uz")
                fs::remove(entry.path());
        }

        handleOtherSettings();
    }
    else
    {
        // Uh.. we failed
        // We will only remove the installation folder HERE, because in other cases
        // there is a possibility of the installation failing because there is already a game being installed there
        DeleteInstallationFolder();
        m_lastInstallationSuccessful = false;
        QMessageBox::critical(this, "Error", "Error occured while trying to extract maps. Installation couldn't complete!\nInstallation folder has been cleaned up", QMessageBox::Ok);
        SetEnableForAllFields(true);
    }
}

/* For some reason, extracted folders under Linux can have strange case-sensitivity translation errors 
 * This function aims to "fix" this.
 */
void UInstaller::handleLinuxFolderExtractionErrors(fs::path gameRootPath)
{
    auto borked_music_folder = gameRootPath / "MusIc";
    auto borked_systemlocalized_folder = gameRootPath / "SystemlocalIzed";
    auto borked_directx7_folder = gameRootPath / "DIrectx7";

    if (fs::exists(borked_music_folder) && fs::is_directory(borked_music_folder))
    {
        // Copy everything within "MusIc" folder to "Music" folder
        fs::copy(borked_music_folder, gameRootPath/"Music", fs::copy_options::recursive);

        // Remove the MusIc folder once the copy is done
        fs::remove_all(borked_music_folder);
    }

    if (fs::exists(borked_systemlocalized_folder) && fs::is_directory(borked_systemlocalized_folder))
    {
        // Simply remove this one, there don't seem to be anything worthwhile in it.
        fs::remove_all(borked_systemlocalized_folder);
    }

    if (fs::exists(borked_directx7_folder) && fs::is_directory(borked_directx7_folder))
    {
        // Ditto.
        fs::remove_all(borked_directx7_folder);
    }
}

void UInstaller::SetEnableForAllFields(bool value)
{
    m_GamePickerComboBox->setEnabled(value);
    m_CommunityPatchPickerComboBox->setEnabled(m_InstallCommunityPatchCheckBox->isChecked());
    m_InstallCommunityPatchCheckBox->setEnabled(value);
    m_BonusPackCheckBox->setEnabled(value);
    m_StartInstallationButton->setEnabled(value);
    m_BrowseFolderButton->setEnabled(value);
}
