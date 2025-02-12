#include "InstallData.h"

InstallData* getPatchData(const std::string& patchName)
{
    #if WIN32
    if (patchName == "227k_12")
        return &UG227kInstallData_Win_x86;
    else if (patchName == "469d 32-Bit")
        return &UT469dInstallData_Win_x86;
    else if (patchName == "469e RC7 32-Bit")
        return &UT469eRC7InstallData_Win_x86;
    #else
    if (patchName == "227k_12")
        return &UG227kInstallData_Lin_x86;
    else if (patchName == "469d 32-Bit")
        return &UT469dInstallData_Lin_x86;
    else if (patchName == "469d 64-Bit")
        return &UT469dInstallData_Lin_x64;
    else if (patchName == "469e RC7 32-Bit")
        return &UT469eRC7InstallData_Lin_x86;
    else if (patchName == "469e RC7 64-Bit")
        return &UT469eRC7InstallData_Lin_x64;
    #endif
    return nullptr;
}

std::vector<std::string> getPatchesList(const std::string& gameName)
{
    std::vector<std::string> result;

    #if WIN32
    if (gameName == "Unreal Gold")
    {
        result.push_back("227k_12");
    }
    else if (gameName == "Unreal Tournament")
    {
        result.push_back("469d 32-Bit");
        result.push_back("469e RC7 32-Bit");
    }
    #else
    if (gameName == "Unreal Gold")
    {
        result.push_back("227k_12");
    }
    else if (gameName == "Unreal Tournament")
    {
        result.push_back("469d 32-Bit");
        result.push_back("469d 64-Bit");

        result.push_back("469e RC7 32-Bit");
        result.push_back("469e RC7 64-Bit");
    }
    #endif

    return result;
}
