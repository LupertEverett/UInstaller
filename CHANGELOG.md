# UInstaller Changelog:

## Version 2:
* UInstaller now uses uz on both Windows and Linux to extract maps when installing Unreal Tournament.
* Map decompression process now happens in a separate dialog window, which fixes the "app looking frozen" issue.
  * When the decompression succeeds, it will close down on its own, to continue with the rest of the installation.
  * When the decompression fails, it will notify the user, and make them close the dialog by themselves instead.
  * Also removes the installation folder when decompression fails.
* Disable the Start button once the installation is successful, until the user picks up another path.
* Fixed the awkward "Cannot find file" error once UInstaller installs a game then the user asks to install another, despite the file existing.
* One can now install different versions of the 227/469 community patches.
  * The selectable patches are limited to 227k_12 and later (when they exist) for Unreal Gold and 469d and later for Unreal Tournament.
  * The main reason behind this is that they are the patches that have zipped versions.
* Added installation support for UT 469e_RC7.
* Added ability to install the Fusion map pack for Unreal Gold.

## Version 1.1:
* When installing UT, decompress the maps using ucc. (Note that the app might look frozen while that happens. Just let it get the work done.)

## Version 1:
* Initial Release.
