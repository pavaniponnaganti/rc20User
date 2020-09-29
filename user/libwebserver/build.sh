#!/bin/bash
build_name=mingw
build_os=`/usr/bin/uname`
clean=0
platform_x64=0
build_dependencies=0
build_config=release
qt_ver=5.6
qt_make_spec=win32-g++

function print_help
{
cat << EOF
Shell script for building Webserver test application.

Usage: $0 [options]

Options:
   -x   Build for 64-bit platform, otherwise 32-bit\
   -l   Build all dependencies. Dependencies are install to system
   -c   Clean before build
   -d   Debug build
   -h   Print this usage information

Example:  
  # Build using release config
  $ $0
  # Clean and build all dependecies
  $ $0 -lc
  # Clean output before building
  $ $0 -c
  # Build debug
  $ $0 -d
  # Print usage information.
  $ $0 -h
EOF
}
#-------------------------------------------------------
# Configure using CMAKE and then build
#-------------------------------------------------------
function build-cmake
{    
    intermediate_path=$build_path/$1
    source_path=$lib3p_path/$1
    mkdir -p "$install_path"
    mkdir -p "$intermediate_path"
    cd "${intermediate_path}"    
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Configuring $1, this may take few minutes..." | tee -a $log
    echo "$cmake" -G "$generator" -DCMAKE_TOOLCHAIN_FILE="$source_path/toolchain-${build_name}.cmake" -DCMAKE_CONFIGURATION_TYPES="Debug;Release" --config $build_config -DCMAKE_INSTALL_PREFIX="$install_path" ${@:2} "$source_path"| tee -a $log
    "$cmake" -G "$generator" -DCMAKE_TOOLCHAIN_FILE="$source_path/toolchain-${build_name}.cmake" -DCMAKE_CONFIGURATION_TYPES="Debug;Release" --config $build_config -DCMAKE_INSTALL_PREFIX="$install_path" ${@:2} "$source_path"| tee -a $log
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
      echo "ERROR: Failed to configure the library $1!" | tee -a $log
      exit -1
    fi
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Configuring $1... Done!" | tee -a $log
    if [ $clean -eq 1 ]; then
        echo "`date "+%Y/%m/%d %H:%M:%S"`] Cleaning $1..." | tee -a $log
        $makebin clean | tee -a $log
        echo "`date "+%Y/%m/%d %H:%M:%S"`] Cleaning $1... Done!" | tee -a $log
    fi
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Building $1, this may take few minutes..." | tee -a $log
    $makebin | tee -a $log
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
      echo "ERROR: Failed to build the library $1!" | tee -a $log
      exit -1
    fi
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Building $1... Done!" | tee -a $log
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Installing $1, this may take few minutes..." | tee -a $log
    $makebin install | tee -a $log
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
      echo "ERROR: Failed to install the library $1!" | tee -a $log
      exit -1
    fi    
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Installing $1... Done!" | tee -a $log
}
#-------------------------------------------------------
# Configure using "Configuration" script and then build
#-------------------------------------------------------
function build-config
{
    source_path=$lib3p_path/$1    
    intermediate_path=$build_path/$1    
    if [ $1 == "openssl" ]; then
        # No need to install the OpenSSL manpages
        install_method=install_sw
        build_method=build_libs
        compiler_str=mingw
    else
        install_method=install
        build_method=all
        compiler_str=
    fi     
    mkdir -p "$intermediate_path"
    cd $source_path     
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Configuring $1, this may take few minutes..." | tee -a $log
    echo ./$2 $compiler_str --prefix=$install_path ${@:3} | tee -a $log
    ./$2 $compiler_str --prefix=$install_path ${@:3} | tee -a $log
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
      echo "ERROR: Failed to configure the library $1!" | tee -a $log
      exit -1
    fi
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Configuring $1... Done!" | tee -a $log
    if [ $clean -eq 1 ]; then
        echo "`date "+%Y/%m/%d %H:%M:%S"`] Cleaning $1..." | tee -a $log
        $makebin clean | tee -a $log
        echo "`date "+%Y/%m/%d %H:%M:%S"`] Cleaning $1... Done!" | tee -a $log
    fi        
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Building $1, this may take few minutes..." | tee -a $log
    $makebin | tee -a $log    
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
      echo "ERROR: Failed to build the library $1!" | tee -a $log
      exit -1
    fi    
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Building $1... Done!" | tee -a $log
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Installing $1, this may take few minutes..." | tee -a $log
    $makebin $install_method | tee -a $log    
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
      echo "ERROR: Failed to install the library $1!" | tee -a $log
      exit -1
    fi   
    echo "[`date "+%Y/%m/%d %H:%M:%S"`] Installing $1... Done!" | tee -a $log
}
#--------------------------------
# Build all libwebserver dependencies
#--------------------------------
function build-all-dependencies
{
    build-cmake libcjson
    build-cmake zlib-1.2.11
    build-config openssl Configure no-shared no-asm no-hw no-engine no-dso no-dynamic-engine --openssldir="$install_path" -DOPENSSL_USE_IPV6=0 -DOPENSSL_NO_ENGINE
    build-config libxml2-2.9.10 configure --host=$triplet_host --with-zlib=$install_path --disable-shared --enable-shared=no --enable-static=yes --disable-dependency-tracking --without-python LIBS=-lws2_32        
    build-cmake libwebsockets -DLWS_WITHOUT_TESTAPPS=ON -DLWS_WITHOUT_TEST_SERVER=ON -DLWS_WITHOUT_TEST_SERVER_EXTPOLL=ON -DLWS_WITHOUT_TEST_PING=ON -DLWS_WITHOUT_TEST_CLIENT=ON
}
#--------------------------------
# Entry point (a.k.a the main function)
#--------------------------------

# parse script parameters
while getopts "cxldh" opt
do
    case "${opt}" in   
    c)  clean=1
        ;;
	x)  platform_x64=1
        echo "WARNING: 64-bit build is currently not implemented!"
        ;;
	l)  build_dependencies=1
        ;;	
    d)  build_config=debug
        ;;
    h)  print_help
        exit 0;;    
    *) echo "ERROR: $0 doesn't expect any argument!"
        print_help
        exit -1;;
    esac
done

# setup based from platform
if [ "${build_os:0:6}" == "CYGWIN" ] || [ "${build_os:0:5}" == "MINGW" ]|| [ "${build_os:0:4}" == "MSYS" ]; then # Windows build requires cygwin or msys
    echo "Building in "${build_os}" using MINGW compiler tools..."
    build_name=mingw
    generator="MSYS Makefiles"                
    if [ "${build_os:0:6}" == "CYGWIN" ]; then
        webserver_path=`pwd`
        cd $webserver_path/../..
        rc20_path=`pwd`
        compiler_path=c:/mingw/bin
        cmake_path="c:/Program Files (x86)/CMake/bin" # unable to mount the program files to fstab
        webserver_path=c:${webserver_path:11}
        rc20_path=c:${rc20_path:11}
    else         
        # NOTE: The following needs to be mounted
        # Dir                                                       Mount
        # <install path>/msys64/mingw32			                    /mingw32 ntfs binary 0 0
        # <install path>/msys64/mingw64			                    /mingw64 ntfs binary 0 0
        # <install path>/rc20-user			                        /rc20 ntfs binary 0 0
        # <install path>/rc20-user/user/libwebserver		        /nws ntfs binary 0 0
		mount --all
        webserver_path=/nws
        rc20_path=/rc20        
		if [ $platform_x64 -ne 0 ]; then
			mingw_path=/mingw64
		else
			mingw_path=/mingw32
		fi
        compiler_path=$mingw_path/bin
        cmake_path=/mingw64/bin
        # deploy all dependecies to this path
        install_path=$mingw_path		
        # check mounted directories
        if [ ! -d "$rc20_path" ]; then
            echo "ERROR: RC20 $rc20_path is not mounted!"
            exit -1
        fi
        if [ ! -d "$webserver_path" ]; then
            echo "ERROR: Webserver $webserver_path is not mounted!"
            exit -1
        fi
        if [ ! -d "$compiler_path" ]; then
            echo "ERROR: Compiler $compiler_path is not mounted!"
            exit -1
        fi
    fi
	# determine the target host
	if [ $platform_x64 -ne 0 ]; then
		triplet_host=x86_64-w64-mingw32
	else
		triplet_host=i686-w64-mingw32
	fi
    # Setup compiler tools
    cc_bin=$compiler_path/gcc.exe
    cxx_bin=$compiler_path/g++.exe    
    makebin=$compiler_path/mingw32-make.exe            
    cmake=$cmake_path/cmake.exe
    perl=/usr/bin/perl    
    export PATH=$compiler_path:/usr/bin:$PATH
    export CC=$cc_bin
    export CXX=$cxx_bin
    export MAKE=$makebin 
    export RANLIB=$compiler_path/ranlib
    # only support QT MINGW 32-bit. If you have 64-bit then revised this script to support
    qt_bin=/c/Qt/$qt_ver/mingw49_32/bin
elif [ "${BUILD_OS}" == "Darwin" ]; then #MAC, IOS is cross-build
    echo "ERROR: ${build_os} is not implemented!"
    exit -1
elif [ "${BUILD_OS}" == "Linux" ]; then #Linux, Android is cross-build
    echo "ERROR: ${build_os} is not implemented!"
    exit -1
else
    echo "ERROR: ${build_os} is not supported!"
    exit -1
fi

# Check required tools
if [ -z "$cmake" ]; then
    echo "ERROR: cmake is not specified."
    exit -1
fi
if [ ! -f "$cmake" ]; then
    echo "ERROR: $cmake doesn't exists! Please install the required CMAKE."
    exit -1
fi
if [ -z "$makebin" ]; then
    echo "ERROR: makebin is not specified."
    exit -1
fi
if [ ! -f "$makebin" ]; then
    echo "ERROR: ${makebin} doesn't exists! Please install the appropriate build tools"
    exit -1
fi
if [ ! -f "$perl" ]; then
    echo "ERROR: $perl doesn't exists! Please install PERL to build OpenSSL"
    exit -1
fi

# set the required directories
lib3p_path=$rc20_path/lib3p
build_path=$webserver_path/build-$build_name
log=$build_path/output.log

# clear log
if [ ! -f "$log" ]; then
    rm -f $log
fi

# clean-up if required
if [ $clean -eq 1 ]; then
    echo "Cleaning output directories..."
    rm -rf $build_path 
fi

rm -rf "$webserver_path/out"

# go back to libwebserver directory
cd $webserver_path

# make sure the output directory exists
/usr/bin/mkdir -p "${build_path}"

# start building all required dependencies if required
if [ $build_dependencies -ne 0 ]; then
	build-all-dependencies
fi

mkdir -p "$build_path/webserver"
cd "$build_path/webserver"

# Build the QT Webserver project
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Configuring QT project webserver.pro, this may take few minutes..." | tee -a $log
echo $qt_bin/qmake -makefile -r CONFIG+=$build_config -spec $qt_make_spec "$webserver_path/app/webserver.pro" | tee -a $log
$qt_bin/qmake -makefile -r CONFIG+=$build_config -spec $qt_make_spec "$webserver_path/app/webserver.pro" | tee -a $log
if [ ${PIPESTATUS[0]} -ne 0 ]; then
  echo "ERROR: Failed to configure webserver.pro!" | tee -a $log
  exit -1
fi 
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Configuring QT project webserver.pro... Done!" | tee -a $log
if [ $clean -eq 1 ]; then
    $makebin clean | tee -a $log
fi
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Building QT project webserver.pro, this may take few minutes..." | tee -a $log
$makebin all | tee -a $log
if [ ${PIPESTATUS[0]} -ne 0 ]; then
  echo "ERROR: Failed to build webserver.pro!" | tee -a $log
  exit -1
fi 
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Building QT project webserver.pro... Done!" | tee -a $log
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Installing QT project webserver.pro, this may take few minutes..." | tee -a $log
$makebin install | tee -a $log
if [ ${PIPESTATUS[0]} -ne 0 ]; then
  echo "ERROR: Failed to install webserver.pro!" | tee -a $log
  exit -1
fi 
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Installing QT project webserver.pro... Done!" | tee -a $log

# deploy webserver to out folder
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Deploying Webserver and dependencies to $webserver_path/out, this may take few minutes..." | tee -a $log
mkdir -p "$webserver_path/out"
cp -f "$build_path/webserver/console/$build_config/webserver.exe" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/libcrypto-1_1.dll" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/libgcc_s_dw2-1.dll" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/libiconv-2.dll" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/liblzma-5.dll" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/libssl-1_1.dll" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/libwebsockets.dll" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/libwinpthread-1.dll" "$webserver_path/out" | tee -a $log
cp -f "$compiler_path/zlib1.dll" "$webserver_path/out" | tee -a $log
cp -rf "$webserver_path/mount" "$webserver_path/out"
cd "$webserver_path/out"
$qt_bin/windeployqt --no-compiler-runtime --$build_config "$webserver_path/out/webserver.exe"
echo "[`date "+%Y/%m/%d %H:%M:%S"`] Webserver deployment to $webserver_path/out... Done!" | tee -a $log

echo "[`date "+%Y/%m/%d %H:%M:%S"`] All build jobs are done!" | tee -a $log
cd "$webserver_path"
exit 0