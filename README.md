# UInstaller
## Alternative installer program for Unreal Gold and Unreal Tournament (1999)

UInstaller will allow you to install Unreal Gold and Unreal Tournament onto your computer.

It uses:

* Epic Games approved ISO files on archive.org to download the games.
* OldUnreal's community patches for both games for countless amount of bugfixes and improvements.

There is also an option to download Bonus Pack 4 when installing Unreal Tournament.

## Usage Instructions

1. Download the 7z file appropriate for your platform
2. Extract everything and run the UInstaller executable
3. Select the game you want to download from the top section
4. Pick the optional components (OldUnreal patches/Bonus Pack 4) to install
5. Select a folder from the "..." menu to install the game. (Note: The game will be extracted to a subfolder!)
6. Click on "Start", then wait for the installation to complete
7. Enjoy!

(Note: Downloaded files are kept in the cache folder of where UInstaller is ran)

## Build Instructions

### Windows

You'll need the following installed:

* Visual Studio 2022
* Qt6 (Core, Gui, Widgets and Network components)
* Qt Visual Studio Tools

Once everything is installed, it is as simple as opening this folder as a CMake project in VS and compiling everything from there.

### Linux

Coming soon!

## Components used

* Qt6
* libarchive
* zlib (so that libarchive extracts binary files correctly)

## License

License info for this program, as well as the components it uses are available in the Licenses folder

* [License for UInstaller](/Licenses/LICENSE-UINSTALLER)
* [License for libarchive](/Licenses/LICENSE-LIBARCHIVE)
* [License for Qt](/Licenses/LICENSE-QT)
* [License for zlib](/Licenses/LICENSE-ZLIB)