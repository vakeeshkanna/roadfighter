First checkout the code using mercurial.

On the command prompt
Go to root of **C:\**

**hg clone https://roadfighter.googlecode.com/hg roadfighter**

this command will get the source for Roadfighter.

Open the file cmakelists.txt with a text editor

set path to these variables

**DIRECTX9**
SET ( DIRECTX9 "D:/apps/DirectX\_Mar09" )

**ROOT\_DIR** // if your source code is in C:\roadfighter then ROOT\_DIR will be
**SET ( ROOT\_DIR "C:/roadfighter" )**

Go to **C:\Roadfighter**

Run **C:\apps\cmake 2.6\ bin\cmake-gui.exe**

this will generate all the necessary make files.

Set these two location

**Where is the source code?**

**Where to build the binaries?**

Then hit configure, and then hit configure.

This will generate the Visual Studio Project files.

Go to **C:\Roadfighter\build** directory

To build Roadfighter in **Debug mode** run
**devenv ALL\_BUILD.vcproj /build**

To build Roadfighter in Release mode run
**devenv ALL\_BUILD.vcproj /build “release”**

This will start the build on your machine. Once its done copying folders images, maps and sounds from resources to

**C:\Roadfighter\build\bin** directory

In order to run the game in debug mode go to directory

**C:\Roadfighter\build\bin\Debug** and run **roadfighter.exe**

In order to run the game in release mode go to directory

**C:\Roadfighter\build\bin\Release** and run **roadfighter.exe**

To create the installer, open **C:\Roadfighter\CPackConfig.cmake**. Set the value for **ROOT\_DIR** variable

**SET ( ROOT\_DIR "C:\Roadfighter" )**
Run **C:\apps\cmake 2.6\ bin\cpack.exe**

This will generate the installer for you in the **C:\Roadfighter** directory, make sure you run cpack after creating the Release build of Roadfighter.