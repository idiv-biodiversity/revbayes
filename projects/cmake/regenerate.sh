#!/bin/sh
HERE=$(pwd)
echo $HERE

#################
# command line options
# set default values
boost="true"
mavericks="false"
win="false"


# parse command line arguments
while echo $1 | grep ^- > /dev/null; do
# intercept help while parsing "-key value" pairs
if [ "$1" = "--help" ] || [ "$1" = "-h" ]
then
echo '
The minimum steps to build RevBayes after running this script is:
cmake .
make

Command line options are:
-h | -help                  : print this help and exit.
-boost      <true|false>    : true (re)compiles boost libs, false dont. Defaults to true.
-mavericks  <true|false>    : set to true if you are building on a OS X - Mavericks system. Defaults to false.
-win        <true|false>    : set to true if you are building on a Windows system. Defaults to false.
'
exit
fi

# parse pairs
eval $( echo $1 | sed 's/-//g' | tr -d '\012')=$2
shift
shift
done


#################
# build boost libraries separately

if [ "$boost" = "true" ]
then
echo 'Building boost libraries'
echo 'you can turn this of with argument "-boost false"'

cd ../../boost_1_55_0
rm ./project-config.jam*  # clean up from previous runs
./bootstrap.sh --with-libraries=system,filesystem,regex,thread,date_time,program_options,math,iostreams,serialization,context,signals

if [ "$mavericks" = "true" ]
then
./b2 toolset=clang cxxflags="-stdlib=libstdc++" linkflags="-stdlib=libstdc++ -lpthread"
else
./b2 linkflags="-lpthread"
fi

else
echo 'not building boost libraries'
fi



#################
# generate cmake configuration
cd "$HERE"
cd ../../src

echo 'cmake_minimum_required(VERSION 2.6)
project(RevBayes)

# Consider:
# -Wno-sign-compare
# -D_GLIBCXX_DEBUG

# Default compiler flags
#if (WIN32)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static -msse -msse2 -msse3")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg -static")
#else ()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3 -stdlib=libstdc++")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg")
#endif ()

' > "$HERE/CMakeLists.txt"

if [ "$mavericks" = "true" ]
then
echo '
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3 -stdlib=libstdc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg")
'  >> "$HERE/CMakeLists.txt"

elif [ "$win" = "true" ]
then
echo '
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static -msse -msse2 -msse3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg -static")
'  >> "$HERE/CMakeLists.txt"
else
echo '
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3 -lpthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg")
'  >> "$HERE/CMakeLists.txt"
fi

echo '
# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src)



SET(BOOST_ROOT ../../boost_1_55_0)
SET(Boost_USE_STATIC_RUNTIME true)
#find_package(Boost 1.55.0 COMPONENTS filesystem regex signals context system thread date_time program_options iostreams serialization math_c99 math_c99f math_tr1f math_tr1l REQUIRED)
find_package(Boost
1.55.0
COMPONENTS filesystem
regex
program_options
system
thread
context
signals
date_time
iostreams
serialization REQUIRED)
MESSAGE("Boost information:")
MESSAGE("  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIR}")
MESSAGE("  Boost_LIBRARIES: ${Boost_LIBRARIES}")
MESSAGE("  Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
#INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

######## optional bio++ ###############

SET(WITH_BPP True) 

#here is a useful function:
MACRO(IMPROVED_FIND_LIBRARY OUTPUT_LIBS lib_name include_to_find)
  #start:
  FIND_PATH(${lib_name}_INCLUDE_DIR ${include_to_find})

  IF(${${lib_name}_INCLUDE_DIR} MATCHES ${lib_name}_INCLUDE_DIR-NOTFOUND)
    MESSAGE("${lib_name} not found.")
    UNSET(WITH_BPP)
  ELSE(${${lib_name}_INCLUDE_DIR} MATCHES ${lib_name}_INCLUDE_DIR-NOTFOUND)
    SET(${lib_name}_NAMES ${lib_name} ${lib_name}lib ${lib_name}dll)
    FIND_LIBRARY(${lib_name}_LIBRARY NAMES ${${lib_name}_NAMES} PATH_SUFFIXES lib${LIB_SUFFIX})
    MESSAGE("-- Library ${lib_name} found here:")
    MESSAGE("   includes : ${${lib_name}_INCLUDE_DIR}")
    MESSAGE("   libraries: ${${lib_name}_LIBRARY}")
    LINK_DIRECTORIES(${${lib_name}_LIBRARY})
  ENDIF(${${lib_name}_INCLUDE_DIR} MATCHES ${lib_name}_INCLUDE_DIR-NOTFOUND)
  
  #add the dependency:
  IF (WITH_BPP)
    INCLUDE_DIRECTORIES(${${lib_name}_INCLUDE_DIR})
    SET(${OUTPUT_LIBS} ${${OUTPUT_LIBS}} ${${lib_name}_LIBRARY})
  ENDIF (WITH_BPP)
  
ENDMACRO(IMPROVED_FIND_LIBRARY)

#Find the Bio++ libraries:
IMPROVED_FIND_LIBRARY(BPP_LIBS bpp-core Bpp/Clonable.h)
IMPROVED_FIND_LIBRARY(BPP_LIBS bpp-seq Bpp/Seq/Alphabet/Alphabet.h)
IMPROVED_FIND_LIBRARY(BPP_LIBS bpp-phyl Bpp/Phyl/Tree.h)

# TODO Split these up based on sub-package dependency
INCLUDE_DIRECTORIES(' >> "$HERE/CMakeLists.txt"
find ui libs core revlanguage -type d | grep -v "svn" | sed 's|^|    ${PROJECT_SOURCE_DIR}/|g' >> "$HERE/CMakeLists.txt"
echo ' ${Boost_INCLUDE_DIR} )

IF (WITH_BPP)
add_definitions(-DWITH_BPP)
ENDIF (WITH_BPP)

####

# Split into much smaller libraries
add_subdirectory(ui)
add_subdirectory(libs)
add_subdirectory(core)
add_subdirectory(revlanguage)

############# executables #################
# basic rev-bayes binary
add_executable(rb ${PROJECT_SOURCE_DIR}/revlanguage/main.cpp)
target_link_libraries(rb rb-parser rb-core libs ${Boost_LIBRARIES})

# extended rev-bayes binary
add_executable(rb-extended ${PROJECT_SOURCE_DIR}/ui/main.cpp)

IF (WITH_BPP)
target_link_libraries(rb ${BPP_LIBS})
target_link_libraries(rb-extended ${BPP_LIBS})
ENDIF(WITH_BPP)


' >> "$HERE/CMakeLists.txt"

if [ "$win" = "true" ]
then
echo '
target_link_libraries(rb-extended rb-ui rb-parser rb-core libs ${Boost_LIBRARIES})
'  >> "$HERE/CMakeLists.txt"
else
echo '
target_link_libraries(rb-extended rb-ui rb-parser rb-core libs pthread ${Boost_LIBRARIES})
'  >> "$HERE/CMakeLists.txt"
fi


echo '


# utility for generating help html files.
add_executable(help-html-generator ${PROJECT_SOURCE_DIR}/ui/utils/HelpHtmlGenerator.cpp)
target_link_libraries(help-html-generator rb-parser rb-core libs ${Boost_LIBRARIES})


' >> $HERE/CMakeLists.txt

echo


########################
### ui

if [ ! -d "$HERE/ui" ]; then
mkdir "$HERE/ui"
fi
echo 'set(UI_FILES' > "$HERE/ui/CMakeLists.txt"
find ui | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/ui/CMakeLists.txt"
echo ')
add_library(rb-ui ${UI_FILES})'  >> "$HERE/ui/CMakeLists.txt"

########################
### libs

if [ ! -d "$HERE/libs" ]; then
mkdir "$HERE/libs"
fi
echo 'set(LIBS_FILES' > "$HERE/libs/CMakeLists.txt"
find libs | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/libs/CMakeLists.txt"
echo ')
add_library(libs ${LIBS_FILES})'  >> "$HERE/libs/CMakeLists.txt"

########################
### core

if [ ! -d "$HERE/core" ]; then
mkdir "$HERE/core"
fi
echo 'set(CORE_FILES' > "$HERE/core/CMakeLists.txt"
find core | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/core/CMakeLists.txt"
echo ')

IF (NOT WITH_BPP)
list(REMOVE_ITEM CORE_FILES ' >> "$HERE/core/CMakeLists.txt"
find core | grep "Bpp" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/core/CMakeLists.txt"
echo ')
ENDIF (NOT WITH_BPP)

add_library(rb-core ${CORE_FILES})'  >> "$HERE/core/CMakeLists.txt"


##########################
### revlanguage

if [ ! -d "$HERE/revlanguage" ]; then
mkdir "$HERE/revlanguage"
fi
echo 'set(PARSER_FILES' > "$HERE/revlanguage/CMakeLists.txt"
find revlanguage | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/revlanguage/CMakeLists.txt"
echo ')

IF (NOT WITH_BPP)
list(REMOVE_ITEM PARSER_FILES ' >> "$HERE/revlanguage/CMakeLists.txt"
find revlanguage | grep "Bpp" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/revlanguage/CMakeLists.txt"
echo ')
ENDIF (NOT WITH_BPP)

add_library(rb-parser ${PARSER_FILES})'  >> "$HERE/revlanguage/CMakeLists.txt"

