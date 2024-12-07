cmake_minimum_required(VERSION 3.16)
project(UInstaller LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Network
)
qt_standard_project_setup()

# Least painful way to build libarchive with zlib support
# Which is necessary if we want to extract binary files correctly
set(ZLIB_ROOT Thirdparty/zlib)
find_package(zlib REQUIRED)

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
