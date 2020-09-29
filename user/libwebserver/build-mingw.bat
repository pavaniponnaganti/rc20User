@echo off
setlocal enableextensions
set BUILD_NAME=mingw
set QT_MAKE_SPEC=win32-g++
set VER_QT=5.6
set ARCH=x86
set QTCREATOR_BIN=C:\Qt\Tools\QtCreator\bin
set QT_BIN=C:\Qt\%VER_QT%\mingw49_32\bin
set CMAKE=C:\Program Files (x86)\CMake\bin\cmake.exe
set PROJ_PATH=%cd%
set APP_PATH=%cd%\application
set LIB3P_PATH=%PROJ_PATH%\..\..\lib3p
set BUILD=build-%BUILD_NAME%
set OUT=out
set MINGW_BIN=C:\Qt\Tools\mingw492_32\bin
set GENERATOR_NAME=MinGW Makefiles
set TOOL_CHAIN=toolchain-%BUILD_NAME%.cmake
echo %path%|find /i "%MINGW_BIN%">nul  || set path=%path%;%MINGW_BIN%
echo %path%|find /i "%CMAKE%">nul  || set path=%path%;%CMAKE%

IF EXIST %OUT% rd /s /q %OUT%
mkdir %OUT%

IF EXIST %BUILD% rd /s /q %BUILD%
mkdir %BUILD%
cd %BUILD%

REM Build the CJSON parser
CALL :build libcjson
REM Build the CJSON parser
CALL :build zlib
REM Build the XML parser
CALL :build libxml2
REM Build OpenSSL
CALL :build openssl
REM Build Websocket
CALL :build libwebsockets
cd "%PROJ_PATH%\%BUILD%"

echo QMAKE: Generate makefile for release...
echo on
%QT_BIN%/qmake -makefile -r CONFIG+=release BUILD_TYPE=%BUILD_TYPE% -spec %QT_MAKE_SPEC% "%APP_PATH%/webserver.pro"
@echo off
if %ERRORLEVEL% NEQ 0 (
    echo qmake failed
    exit /b 1
)
mingw32-make all
if %ERRORLEVEL% NEQ 0 (
    echo make all failed
    exit /b 1
)
mingw32-make install
if %ERRORLEVEL% NEQ 0 (
    echo make install failed
    exit /b 1
)

xcopy /Y "webserverapp\release\webserverapp.exe" "%PROJ_PATH%\%OUT%\"
xcopy /Y "webserverlib\release\*.dll" "%PROJ_PATH%\%OUT%\"
xcopy /Y "webserverlib\release\*.a" "%PROJ_PATH%\%OUT%\"
cd "%PROJ_PATH%\%OUT%"
echo "Deploying dependencies...."
%QT_BIN%/windeployqt --no-compiler-runtime --release "%PROJ_PATH%\%OUT%\webserverapp.exe"

cd ..
copy /y "%MINGW_BIN%\*.dll" "%OUT%\" 
copy /y "%APP_PATH%\bin-%BUILD_NAME%\*.dll" "%PROJ_PATH%\%OUT%\"
copy /y "%APP_PATH%\bin-%BUILD_NAME%\*.a" "%PROJ_PATH%\%OUT%\"
IF NOT EXIST "%PROJ_PATH%\%OUT%\mount" mkdir "%PROJ_PATH%\%OUT%\mount"
xcopy /s /y "%PROJ_PATH%\mount" "%PROJ_PATH%\%OUT%\mount"
echo "....DONE."
exit /b 0

:build
echo Start building %~1...
set PROJ_BUILD=%~1
IF NOT EXIST "%PROJ_BUILD%" mkdir "%PROJ_BUILD%"
cd "%PROJ_BUILD%"

"%CMAKE%" -G "%GENERATOR_NAME%" -DCMAKE_BUILD_FILES_DIRECTORY="%PROJ_BUILD%" -DCMAKE_TOOLCHAIN_FILE="%LIB3P_PATH%\%~1\%TOOL_CHAIN%" -DCMAKE_CONFIGURATION_TYPES="Debug;Release" --config Release %LIB3P_PATH%\%~1

if %ERRORLEVEL% NEQ 0 (
    echo cmake %~1 failed
    exit /b 1
)
mingw32-make
if %ERRORLEVEL% NEQ 0 (
    echo make %~1 failed
    exit /b 1
)
xcopy /Y *%~1.a "%APP_PATH%\bin-%BUILD_NAME%\"
xcopy /Y *%~1.lib "%APP_PATH%\bin-%BUILD_NAME%\"
xcopy /Y *%~1.dll "%APP_PATH%\bin-%BUILD_NAME%\"
cd ..\
