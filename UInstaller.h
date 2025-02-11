#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QProgressBar>
#include <QtGui/QDesktopServices>
#include <QtCore/QThread>
#include <QtCore/QProcess>

#include "InstallData.h"
#include "FileDownloader.h"
#include "FileExtractor.h"


class UInstaller : public QWidget
{
    Q_OBJECT

public:
    UInstaller(QWidget *parent = nullptr);
    ~UInstaller();

private:
    void prepareLayout();
    void setupConnections();

    void SetEnableForAllFields(bool value);

    void ExtractISOFile(std::string& downloadedISOPath);

    void PerformPostExtraction();
    static void RenameRootFolders(fs::path rootFolderPath); // Make all the root folders (Maps, System, etc.) Capital Case
    void CleanupSystemFolder(); // Clean the system folder from translation files

    // UT ISO comes with the map files compressed.
    // This function will call ucc decompress on all the maps.
    void DecompressMapFiles(fs::path rootFolderPath);

    void handleOtherSettings();

    void PopulatePatchPicker();

    void AllDone();

    QGroupBox* m_GamePickerGroupBox;
    QGroupBox* m_GameSettingsGroupBox;

    QComboBox* m_GamePickerComboBox;
    QComboBox* m_CommunityPatchPickerComboBox;

    QProgressBar* m_DownloadProgressBar;

    QLabel* m_StatusLabel;

    // Setting checkboxes
    QCheckBox* m_InstallCommunityPatchCheckBox;
    QCheckBox* m_BonusPackCheckBox;

    QLineEdit* m_TargetPathLineEdit;
    QPushButton* m_BrowseFolderButton;
    
    QPushButton* m_StartInstallationButton;

    // Layouts
    QVBoxLayout* main_layout;
    QHBoxLayout* bottom_button_layout;
    QVBoxLayout* game_picker_group_layout;
    QHBoxLayout* patch_picker_group_layout;
    QVBoxLayout* game_settings_group_layout;

    InstallData* m_CurrentInstallData;
    InstallData* m_CurrentCommunityPatchInstallData;
    InstallData* m_CurrentBonusPackInstallData;

    FileDownloader* m_fileDownloader;
    FileDownloader* m_CommunityPatchDownloader;
    FileDownloader* m_BonusPackDownloader;

    FileExtractor* m_ISOExtractor;
    FileExtractor* m_CommunityPatchExtractor;
    FileExtractor* m_BonusPackExtractor;

private slots:
    void handleGamePickerSelectionChanged();
    void handleCommunityPatchPickerSelectionChanged();
    void handleCommunityPatchCheckboxToggled(bool toggle);

    void handleTargetPathLineEditChanged();

    void handleBrowseButtonPressed();
    void handleStartButtonPressed();

    void handleDownloadProgress(qint64 downloaded, qint64 total);
    void handleDownloadFinished(std::string downloadedFilePath);

    void handleCommunityPatchDownloadProgress(qint64 downloaded, qint64 total);
    void handleCommunityPatchDownloadFinished(std::string downloadedFilePath);
    void handleBonusPackDownloadFinished(std::string downloadedFilePath);

    void handleISOExtractionFinished();
    void handleCommunityPatchExtractionFinished();
    void handleBP4ExtractionFinished();

    void handleLinuxFolderExtractionErrors(fs::path gameRootPath);
};
