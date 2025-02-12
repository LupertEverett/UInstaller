#pragma once

#include <string>
#include <vector>
#include <cstdint>

// TODO: Less-platform-dependent community patches download links?
// TODO: Any kind of integrity checking???

struct InstallData {
	std::string gameName;
	std::string gameFolderName;
	std::string downloadURL;
	std::string fileName;
	uintmax_t fileSize; // In Bytes
	// std::string sha1sum;
};

std::vector<std::string> getPatchesList(const std::string& gameName);
InstallData* getPatchData(const std::string& patchName);

static InstallData UnrealGoldInstallData = {
	.gameName = "Unreal Gold",
	.gameFolderName = "UnrealGold",
	.downloadURL = "https://archive.org/download/totallyunreal/UNREAL_GOLD.ISO",
	.fileName = "UNREAL_GOLD.ISO",
	.fileSize = 676734976 // 645 MB
};

static InstallData UnrealTournamentInstallData = {
	.gameName = "Unreal Tournament",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://archive.org/download/ut-goty/UT_GOTY_CD1.iso",
	.fileName = "UT_GOTY_CD1.iso",
	.fileSize = 649633792 // 620 MB
};

static InstallData UTBonusPack4InstallData = {
	.gameName = "UTBonusPack4",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://unreal-archive-files.eu-central-1.linodeobjects.com/patches-updates/Unreal%20Tournament/Bonus%20Packs/utbonuspack4-zip.zip",
	.fileName = "utbonuspack4-zip.zip",
	.fileSize = 14017081  // 13 MB
};

static InstallData UGFusionMapPackInstallData = {
	.gameName = "UGoldFusionMapPack",
	.gameFolderName = "UnrealGold",
	.downloadURL = "https://unreal-archive-files.eu-central-1.linodeobjects.com/Unreal/MapPacks/F/3/d/ebaf0d/fusionmappack.zip",
	.fileName = "fusionmappack.zip",
	.fileSize = 2809153  // 2.7 MB
};

static InstallData UG227kInstallData_Win_x86 = {
	.gameName = "UG227k_12",
	.gameFolderName = "UnrealGold",
	.downloadURL = "https://github.com/OldUnreal/Unreal-testing/releases/download/v227k_12/OldUnreal-UnrealPatch227k-Windows.zip",
	.fileName = "OldUnreal-UnrealPatch227k-Windows.zip",
	.fileSize = 127703260  // 123 MB
};

static InstallData UT469dInstallData_Win_x86 = {
	.gameName = "UT469d",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-Windows.zip",
	.fileName = "OldUnreal-UTPatch469d-Windows.zip",
	.fileSize = 75626393 // 73 MB
};

static InstallData UT469eRC7InstallData_Win_x86 = {
	.gameName = "UT469e_RC7",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469e-rc7/OldUnreal-UTPatch469e-Windows-x86.zip",
	.fileName = "OldUnreal-UTPatch469e-Windows-x86.zip",
	.fileSize = 106047233 // 101.1 MB
};

static InstallData UG227kInstallData_Lin_x86 = {
	.gameName = "UG227k_12",
	.gameFolderName = "UnrealGold",
	.downloadURL = "https://github.com/OldUnreal/Unreal-testing/releases/download/v227k_12/OldUnreal-UnrealPatch227k-Linux.tar.bz2",
	.fileName = "OldUnreal-UnrealPatch227k-Linux.tar.bz2",
	.fileSize = 126346305 // 121 MB
};

static InstallData UT469dInstallData_Lin_x86 = {
	.gameName = "UT469d",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-Linux-x86.tar.bz2",
	.fileName = "OldUnreal-UTPatch469d-Linux-x86.tar.bz2",
	.fileSize = 74050863 // 70.6 MB
};

static InstallData UT469eRC7InstallData_Lin_x86 = {
	.gameName = "UT469e_RC7",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469e-rc7/OldUnreal-UTPatch469e-Linux-x86.tar.bz2",
	.fileName = "OldUnreal-UTPatch469e-Linux-x86.tar.bz2",
	.fileSize = 74844494 // 71.4 MB
};

static InstallData UT469dInstallData_Lin_x64 = {
	.gameName = "UT469d",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-Linux-amd64.tar.bz2",
	.fileName = "OldUnreal-UTPatch469d-Linux-amd64.tar.bz2",
	.fileSize = 73973312 // 71 MB
};

static InstallData UT469eRC7InstallData_Lin_x64 = {
	.gameName = "UT469e_RC7",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469e-rc7/OldUnreal-UTPatch469e-Linux-amd64.tar.bz2",
	.fileName = "OldUnreal-UTPatch469e-Linux-amd64.tar.bz2",
	.fileSize = 75259066 // 71.8 MB
};

// Skip these files that has an exact name match
static const std::vector<std::string> skipFiles = {
	"Autorun.inf",
	"AUTORUN.INF",
	"DATA.TAG",
	"data1.cab",
	"lang.dat",
	"layout.bin",
	"msvcrt.dll",
	"os.dat",
	"SETUP.EXE",
	"setup.exe",
	"SETUP.INI",
	"setup.ins",
	"setup.lid",
	"_INST32I.EX_",
	"_isauto.exe",
	"_ISDEL.EXE",
	"_SETUP.DLL",
	"_sys1.cab",
	"_user1.cab",
	"AUTOPLAY.EXE",
	"UNINSTAL.EXE",

	"Directx7/",
	"Directx7"
	"GameSpy/",
	"Microsoft/",
	"System400",
	"System400/",

	"DIRECTX7/",
	"SYSTEMLOCALIZED",
	"SYSTEMLOCALIZED/"
};
