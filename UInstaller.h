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
#include "MapDecompressDialog.h"


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
    void CleanupSystemFolder(); // Clear the system folder from translation files
    void RemoveUselessFolders(); // Nuke the folders that somehow escaped from the file skippery.

    // UT ISO comes with the map files compressed.
    // This function will call uz decompress on all the maps.
    void DecompressMapFiles(fs::path rootFolderPath);

    void handleOtherSettings();

    void PopulatePatchPicker();

    void DeleteInstallationFolder();

    void AllDone();

    QGroupBox* m_GamePickerGroupBox;
    QGroupBox* m_GameSettingsGroupBox;

    QComboBox* m_GamePickerComboBox;
    QComboBox* m_CommunityPatchPickerComboBox;

    QProgressBar* m_DownloadProgressBar;

    QLabel* m_StatusLabel;
#ifndef WIN32
    QLabel* m_wineRequiredLabel;
#endif

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

    MapDecompressDialog* m_mapDecompressDialog;

    bool m_lastInstallationSuccessful = false;
    QString m_lastTargetPath = "";

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
    void handleBonusPackExtractionFinished();

    void handleLinuxFolderExtractionErrors(fs::path gameRootPath);
};
