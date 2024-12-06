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
