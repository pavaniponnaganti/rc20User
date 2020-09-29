This readme file explains how to build the Demo Webserver Application.
The project has the following components:
	Webserver.exe
		+- LibWebserver
			+- LibWebsockets (https://libwebsockets.org/)
				+- OpenSSL (https://www.openssl.org/)
			+- LibCJSON (https://github.com/DaveGamble/cJSON)
			+- LibXML2 (http://xmlsoft.org/)
				+- LibZLIB (Optional, if not found will exclude by LibXML2 configuration) (https://zlib.net/)
				+- LibIconV (Not built by this script. Optional, if not found will exclude by LibXML2 configuration) (https://www.gnu.org/software/libiconv/)
Requirements:
	* MSYS2
	* MinGW32/64 Toolchain
	* CMAKE
	* QT MinGW32/64 Build Toolkits

1. MSYS2 Setup
MSYS2 offer Linux like environment on top of Windows. It contains GNU utilities (bash, sh, mkdir, grep, etc..) for building an applications. MSYS2 is synonymous to or has the same functions as CYGWIN.
	1.1. Installing MSYS2
		1.1.1 Follow MSYS2 installation procedures from https://www.msys2.org/. Select MSYS2 64-bit.
		1.1.2. Do not forget to update the base packages and package DB (this is important to update the base-devel package).
			$ pacman -Syu
		1.1.3. Install the required MSYS2 packages:
			a. MinGW32                     
				$ pacman -S mingw-w64-i686-toolchain 
			b. MinGW64                     
				$ pacman -S mingw-w64-x86_64-toolchain (This is required when building for x86-64 platform but is not yet implemented in the script)
			c. CMake
				$ pacman -S mingw-w64-x86_64-cmake
			d. Diff
				$ pacman -S diffutils
		1.1.4. Check PERL version. OpenSSL required PERL with version greater or equal 5.10
			$ perl -v
			a. If PERL is not found or less than the required version, install the latest (NOTE: make sure you are using the PERL that comes in the MSYS2 repo, otherwise the path in the openssl configuration may be be parsed incorrectly. Execute 'which perl' and make sure you are running PERL from '/usr/bin'):
				$ pacman -S Perl
	1.2. Setup requires mount points. Open "<install path>/msys64/etc/fstab" and add the following lines, replace <install path> with correct path:
		<install path>/msys64/mingw32						/mingw32 ntfs binary 0 0
		<install path>/msys64/mingw64						/mingw64 ntfs binary 0 0
		<install path>/rc20-user/						/rc20 ntfs binary 0 0
		<install path>/rc20-user/user/libwebserver		/nws ntfs binary 0 0
	1.3. Open MSYS2 shell and confirm if the paths are mounted correctly. If not, try to command "mount --all". If still not mounting, check if the paths are correct. Make sure you are using the correct file system, in the mount instructions earlier "ntfs" is used as file system. Replace this with your correct file system.
	
2. QT Creator. I will not explain how to install QT as I assumed you already have QT installed. The menus and settings described in this section are based from QT Creator 4.0.1. If your QT is not the same, just search for the proper dialog windows and settings.
	2.1. Open QT Creator, in the main menu select [Tools]->[Options] to open the "Options" dialog window.
	2.2. In the Item List on the right, please select [Build & Run].
	2.3. In the "Build & Run" panel select [Compilers].
	2.4. In the "Compiler" tab, click [Add] button and select MINGW. Set the following properties:
		Name: MSYS2 MinGW32 G++
		Compiler Path: <install path>\msys64\mingw32\bin\g++.exe
    2.5. Click [Apply] button
	2.6. In the "Debuggers" tab, click [Add] button. Set the following properties:
		Name: MSYS2 MinGW32 GDB
		Path: <install path>\msys64\mingw32\bin\gdb.exe
    2.7. Click [Apply] button
	2.8. In the "CMake" tab, click [Add] button. Set the following properties:
		Name: MSYS2 MinGW32 CMake
		Path: <install path>\msys64\mingw64\bin\cmake.exe
    2.9. Click [Apply] button
	2.10. In the "Kits" tab, click [Add] button. Set the following properties:
		Name: Desktop Qt %{Qt:Version} MSYS2 MinGW 32bit
		Device Type: Desktop
		Device: Local PC
		Compiler: MSYS2 MinGW32 G++
		Debugger: MSYS2 MinGW32 GDB
		QT Version: QT <your version> MinGW 32bit
		QT mkspec: win32-g++
		CMakeTool: MSYS2 MinGW32 CMake
		CMake Generator: MSYS Makefiles
	2.11. Click [Apply] button, then [OK]
	
3. QT Project.
	3.1. In QT Creator open the "<install_path>\rc20-user\user\libwebserver\app\webserver.pro". 
	3.2. When asked for build kits, select "Desktop Qt %{Qt:Version} MSYS2 MinGW 32bit".
	3.3. The project has the following structure:
		Webserver.pro (Main Project)
			+- Webserver.pri 			(Path declaration)
			+- Console.pro				(Console application)
				+- LibWebserver.pri		(Project settings to use libwebserver)
			+- LibWebserver.pro			(Libwebserver implementation)
	3.4. Click the [Projects] button on the right side of the QT creator.
	3.5. If there are multiple build kits for this project, select "Desktop Qt %{Qt:Version} MSYS2 MinGW 32bit"
	3.6. In the Build steps, add "make install" by:
		3.6.1. Click [Add Build Step] and select "Make". Set "Make arguments" to "install".
	3.7. Open "<install_path>\rc20-user\user\libwebserver\app\webserver.pri" file and make sure NWS_DEP_LIB_PATH and NWS_DEP_INC_PATH are correct. 
		
4. Build depedencies. You need to build the dependencies first before you can debug the project in QT creator.
	4.1. Open command prompt and go to: <install path>\rc20-user\user\libwebserver
	4.2. Execute the build batch file.
		<install path>\rc20-user\user\libwebserver>\build.bat -lc
	4.3. The application is deployed to <install path>\rc20-user\user\libwebserver>\out. Call webserver.exe if you want to execute. Please note default port for this demo is 80. If 80 is blocked then use another port. Use webserver -h to see all options.
	
5. You can now run the project in QT creator.
	5.1. In the main menu, select [Build]->[Run QMake] TIP: Always run QMake whenever you changed any build settings. This is to make sure you have the latest makefile.
	5.2. In the main menu, select [Build]->[Rebuild All]

Notes:
1. About the build scripts:
	build.bat - execute the build.sh script in MSYS2 bash shell
	build.sh - actual build script
2. If you are facing problems in downloading and updating MSYS2, there is a copy in my shared folder: S:/_Public_/ChrisF/MSYS2	
3. If you prefer CYGWIN, then please use CYGWIN but you need to change the script to match the CYGWIN environment.
4. About building in Windows Command Prompt instead of MSYS/CYGWIN shell. I experienced many errors in doing this and to save time I decided not to continue with this build further. I don't see any issue in building using MSYS/CYGWIN shell as it is basically the same as building with Windows command prompt.
5. You don't need to build the dependencies all the time. One-time build is OK as the dependencies may not change. All dependencies are installed in <installed path>/msys64/<mingw32 or mingw64>
