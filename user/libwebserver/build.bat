@echo off
setlocal enableextensions
set BASH_BIN=C:\msys64\usr\bin
set BASH=%BASH_BIN%\bash.exe
echo ********************************************************************************
echo This build script requires MSYS2 (https://www.msys2.org/) with the following     
echo packages to run successfully:                                                   
echo   Packages                    Installation                                      
echo   PERL                        pacman -S Perl            
echo   MinGW32                     pacman -S mingw-w64-i686-toolchain                
echo   MinGW64                     pacman -S mingw-w64-x86_64-toolchain              
echo   CMake                       pacman -S mingw-w64-x86_64-cmake                  
echo   Diff                        pacman -S diffutils                               
echo ********************************************************************************
echo Building...
REM Run the build script in MSYS2 command prompt
"%BASH%" -c "./build.sh %1"
if %ERRORLEVEL% NEQ 0 (
    echo Build Failed!
    exit /b 1
)
echo Build Done!
exit /b 0
