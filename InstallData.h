#pragma once

#include <string>
#include <vector>

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

#ifdef WIN32
static InstallData UG227kInstallData = {
	.gameName = "UG227k_12",
	.gameFolderName = "UnrealGold",
	.downloadURL = "https://github.com/OldUnreal/Unreal-testing/releases/download/v227k_12/OldUnreal-UnrealPatch227k-Windows.zip",
	.fileName = "OldUnreal-UnrealPatch227k-Windows.zip",
	.fileSize = 127703260  // 123 MB
};

static InstallData UT469dInstallData = {
	.gameName = "UT469d",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-Windows.zip",
	.fileName = "OldUnreal-UTPatch469d-Windows.zip",
	.fileSize = 75626393 // 73 MB
};
#else
static InstallData UG227kInstallData = {
	.gameName = "UG227k_12",
	.gameFolderName = "UnrealGold",
	.downloadURL = "https://github.com/OldUnreal/Unreal-testing/releases/download/v227k_12/OldUnreal-UnrealPatch227k-Linux.tar.bz2",
	.fileName = "OldUnreal-UnrealPatch227k-Linux.tar.bz2",
	.fileSize = 126346305 // 121 MB
};

// amd64 builds will also have the x86 builds included, and doesn't differ much in file size
// So let's just use em instead
static InstallData UT469dInstallData = {
	.gameName = "UT469d",
	.gameFolderName = "UnrealTournamentGOTY",
	.downloadURL = "https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-Linux-amd64.tar.bz2",
	.fileName = "OldUnreal-UTPatch469d-Linux-amd64.tar.bz2",
	.fileSize = 73973312 // 71 MB
};
#endif

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
