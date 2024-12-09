cmake_minimum_required(VERSION 3.27)
project(UInstaller LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Network
)
qt_standard_project_setup()

# zlib here is necessary if we want libarchive to extract binary files correctly
# Time to pray to whatever gods you want to pray that 
# 1. You get libarchive to recognize zlib, AND
# 2. You don't tear off your hair trying to figure out why zlib cannot find its own damn zconf.h file. AND
# 3. Even if it does (you lucky fella), hope that it will stick this time

if(WIN32)
    set(ZLIB_ROOT Thirdparty/zlib)
    find_package(zlib REQUIRED)
else()
    find_package(zlib)
    if(!ZLIB_FOUND)
        include(PkgConfig)
        pkg_search_module(ZLIB REQUIRED zlib)
    endif()
endif()

include_directories(UInstaller Thirdparty/libarchive/libarchive Thirdparty/zlib "${CMAKE_CURRENT_BINARY_DIR}/zlib_build")
add_subdirectory(Thirdparty/libarchive)

set(PROJECT_SOURCES
    main.cpp
    UInstaller.h
    UInstaller.cpp
    InstallData.h
    FileDownloader.cpp
    FileDownloader.h
    FileExtractor.cpp
    FileExtractor.h
)

if(WIN32)
    qt_add_executable(UInstaller ${PROJECT_SOURCES} UInstaller.rc)
else()
    qt_add_executable(UInstaller ${PROJECT_SOURCES})
endif()

set_target_properties(UInstaller
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(UInstaller
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
    PRIVATE
        archive
)
