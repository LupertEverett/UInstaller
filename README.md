# UInstaller
## Alternative installer program for Unreal Gold and Unreal Tournament (1999)

UInstaller will allow you to install Unreal Gold and Unreal Tournament onto your computer.

It uses:

* Epic Games approved ISO files on archive.org to download the games.
* OldUnreal's community patches for both games for countless amount of bugfixes and improvements.

There is also an option to download Bonus Pack 4 when installing Unreal Tournament.

![UInstaller Screenshot](https://i.imgur.com/MzCZRxd.png)

## Usage Instructions

1. Download the 7z file appropriate for your platform
2. Extract everything and run the UInstaller executable
3. Select the game you want to download from the top section
4. Pick the optional components (OldUnreal patches/Bonus Pack 4) to install
5. Select a folder from the "..." menu to install the game. (Note: The game will be extracted to a subfolder!)
6. Click on "Start", then wait for the installation to complete
7. Enjoy!

(Note: Downloaded files are kept in the cache folder of where UInstaller is ran)

## Troubleshooting

### Missing libbz2.so.1.0 on Fedora Linux

Making a symbolic link to the """missing""" library fixes the issue:

    ln -s /usr/lib64/libbz2.so.1.0.8 /usr/lib64/libbz2.so.1.0

## Build Instructions

### Windows

You'll need the following installed:

* Visual Studio 2022
* Qt6 (Core, Gui, Widgets and Network components)
* Qt Visual Studio Tools
* CMake

Once everything is installed, it is as simple as cloning this repo, opening the resulting folder as a CMake project in VS and compiling everything from there.

### Linux

You'll need CMake installed, as well as the following libraries, including their development packages:

* zlib
* Qt6

(libarchive is included in the Thirdparty folder)

Once you have the dependencies, the build process is as simple as following these commands in order:

    git clone https://github.com/LupertEverett/UInstaller
    cd UInstaller && mkdir build && cd build
    cmake ..
    make
    
The resulting UInstaller executable will be in the build folder.

## Components used

* Qt6: https://www.qt.io/
* libarchive: https://github.com/libarchive/libarchive
* zlib (so that libarchive extracts binary files correctly): https://github.com/madler/zlib
* UZ standalone .uz (de)compression library: https://github.com/SeriousBuggie/UZ/

## License

License info for this program, as well as the components it uses are available in the Licenses folder

* [License for UInstaller](/Licenses/LICENSE-UINSTALLER)
* [License for libarchive](/Licenses/LICENSE-LIBARCHIVE)
* [License for Qt](/Licenses/LICENSE-QT)
* [License for zlib](/Licenses/LICENSE-ZLIB)
* [License for UZ](/Licenses/LICENSE-UZ)
