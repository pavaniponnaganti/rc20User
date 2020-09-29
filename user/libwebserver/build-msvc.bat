@echo off
setlocal enableextensions
set BUILD_NAME=msvc
set VER_MSVC=2015
set VER_VS_NAME=Microsoft Visual Studio 14.0
set VER_VS_NUM=14
set QT_MAKE_SPEC=win32-msvc%VER_MSVC%
set VER_QT=5.6
set ARCH=x86
set QTCREATOR_BIN=C:\Qt\Tools\QtCreator\bin
set QT_BIN=C:\Qt\%VER_QT%\msvc%VER_MSVC%\bin
set CMAKE=C:\Program Files (x86)\CMake\bin\cmake.exe
set PROJ_PATH=%cd%
set APP_PATH=%cd%\application
set LIB3P_PATH=%PROJ_PATH%\..\..\lib3p
set BUILD=build-%BUILD_NAME%
set OUT=out
set GENERATOR_NAME=Visual Studio 14 2015
set TOOL_CHAIN=toolchain-%BUILD_NAME%.cmake

REM set required build environements
if not defined DevEnvDir (
	echo VCVARSALL: Setting up %VER_VS_NAME% path and environment variables...
	call "C:\Program Files (x86)\%VER_VS_NAME%\VC\vcvarsall.bat" %ARCH%
	if %ERRORLEVEL% NEQ 0 (
		echo Unable to set the required path and environment variables
		exit /b 1
	)
)
set MSVC_REDIST_DIR="C:\Program Files (x86)\%VER_VS_NAME%\VC\redist\%ARCH%\Microsoft.VC%VER_VS_NUM%0.CRT"

IF EXIST %OUT% rd /s /q %OUT%
mkdir %OUT%

IF EXIST %BUILD% rd /s /q %BUILD%
mkdir %BUILD%
cd %BUILD%

REM Build the CJSON parser
CALL :build libcjson cJSON
REM Build the CJSON parser
REM CALL :build zlib
REM Build the XML parser
REM CALL :build libxml2
REM Build Websocket
REM CALL :build libwebsockets
cd "%PROJ_PATH%\%BUILD%"

echo QMAKE: Generate makefile for release...
echo on
%QT_BIN%/qmake -makefile -r CONFIG+=release BUILD_TYPE=%BUILD_TYPE% -spec %QT_MAKE_SPEC% "%APP_PATH%/Webserver.pro"
@echo off
if %ERRORLEVEL% NEQ 0 (
    echo qmake failed
    exit /b 1
)

echo on
nmake /A
echo off
if %ERRORLEVEL% NEQ 0 (
    echo jom failed
    exit /b 1
)
xcopy /Y "WebServerApp\release\WebServerApp.exe" "%PROJ_PATH%\%OUT%\"
xcopy /Y "webserverlib\release\*.dll" "%PROJ_PATH%\%OUT%\"
xcopy /Y "webserverlib\release\*.a" "%PROJ_PATH%\%OUT%\"

cd "%PROJ_PATH%\%OUT%
%QT_BIN%/windeployqt --no-compiler-runtime --release "%PROJ_BUILD%\%OUT%\WebServerApp.exe"
copy /y %MSVC_REDIST_DIR% "%PROJ_PATH%\%OUT%\" 
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
msbuild %~2.sln
if %ERRORLEVEL% NEQ 0 (
    echo make %~1 failed
    exit /b 1
)
xcopy /Y *%~1.a "%APP_PATH%\bin-%BUILD_NAME%\"
xcopy /Y *%~1.lib "%APP_PATH%\bin-%BUILD_NAME%\"
xcopy /Y *%~1.dll "%APP_PATH%\bin-%BUILD_NAME%\"
xcopy /Y *%~2.a "%APP_PATH%\bin-%BUILD_NAME%\"
xcopy /Y *%~2.lib "%APP_PATH%\bin-%BUILD_NAME%\"
xcopy /Y *%~2.dll "%APP_PATH%\bin-%BUILD_NAME%\"
cd ..\
