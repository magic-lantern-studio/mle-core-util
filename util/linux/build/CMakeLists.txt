# COPYRIGHT_BEGIN
#
# The MIT License (MIT)
#
# Copyright (c) 2025 Wizzer Works
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#  For information concerning this header file, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#  More information concerning Wizzer Works may be found at
#
#      http://www.wizzerworks.com
#
# COPYRIGHT_END

# Stop if cmake version is below 3.0
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Project name and enable C++ support
project(mlert VERSION 1.0.0 LANGUAGES C CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify where to find CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Specify Debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

# Turn on verbose mode.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Fail early if one of the required packages cannot be found
find_package(PkgConfig REQUIRED)

# Check for Magic Lantern environment variables
if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()
if (DEFINED ENV{MLE_WORKPRINTS})
  message(STATUS "MLE_WORKPRINTS=" $ENV{MLE_WORKPRINTS})
else()
  message(STATUS "MLE_WORKPRINTS NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern/include/workprints")
  message(STATUS "Using " $ENV{MLE_WORKPRINTS})
endif()

# Specify the library

# Specify the include directories for the header files
include_directories(
  include
  ../../common/include
  ../../linux/include
  $ENV{MLE_ROOT}/include)

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_NOT_UTIL_DLL)

# Specify the shared library
add_library(
  mlutilShared SHARED
    ../../common/src/mlDebug.c
    ../../common/src/MleDsoLoader.cxx
    ../../common/src/MleMemoryManager.cxx
    ../../common/src/mlErrno.c
    ../../common/src/MleTemplate.cxx
    ../../common/src/mlExpandFilename.c
    ../../common/src/mlLogFile.c
    ../../common/src/mlUnique.c
    ../../common/src/mlItoa.c
    ../../common/src/mlTime.c
    ../src/MleLinuxMemoryManager.cxx
    ../src/MleLinuxPath.cxx)

# Specify the static library
add_library(
  mlutilStatic STATIC
    ../../common/src/mlDebug.c
    ../../common/src/MleDsoLoader.cxx
    ../../common/src/MleMemoryManager.cxx
    ../../common/src/mlErrno.c
    ../../common/src/MleTemplate.cxx
    ../../common/src/mlExpandFilename.c
    ../../common/src/mlLogFile.c
    ../../common/src/mlUnique.c
    ../../common/src/mlItoa.c
    ../../common/src/mlTime.c
    ../src/MleLinuxMemoryManager.cxx
    ../src/MleLinuxPath.cxx)

  # Specify the shared library properties
  set_target_properties(mlutilShared PROPERTIES
    OUTPUT_NAME mlutil
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

  target_compile_options(mlutilShared
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(mlutilShared
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG>
      $<$<CONFIG:Release>:>)

  # Specify the static library properties
  set_target_properties(mlutilStatic PROPERTIES
    OUTPUT_NAME mlutil
    VERSION ${PROJECT_VERSION})

  target_compile_options(mlutilStatic
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(mlutilStatic
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG>
      $<$<CONFIG:Release>:>)

  # Install the libraries
  install(
    TARGETS
      mlutilShared
      mlutilStatic
    DESTINATION
      lib
   )

  # Install the header files
  install(
    FILES
      ../../common/include/mle/mlArray.h
      ../../common/include/mle/mlAssert.h
      ../../common/include/mle/mlConfig.h
      ../../common/include/mle/mlDebug.h
      ../../common/include/mle/MleDsoLoader.h
      ../../common/include/mle/mle.h
      ../../common/include/mle/MleMemoryManager.h
      ../../common/include/mle/MlePath.h
      ../../common/include/mle/mlErrno.h
      ../../common/include/mle/MleTemplate.h
      ../../common/include/mle/MleUtil.h
      ../../common/include/mle/mlExpandFilename.h
      ../../common/include/mle/mlFileio.h
      ../../common/include/mle/mlLogFile.h
      ../../common/include/mle/mlMacros.h
      ../../common/include/mle/mlMalloc.h
      ../../common/include/mle/mlToken.h
      ../../common/include/mle/mlTrace.h
      ../../common/include/mle/mlTypes.h
      ../../common/include/mle/mlUnique.h
      ../../common/include/mle/mlItoa.h
      ../../common/include/mle/mlTime.h
      ../../linux/include/mle/mlPlatformDefs.h
      ../../linux/include/mle/MleLinuxPath.h
    DESTINATION
      include/mle
  )

  # Uninstall libraries and header files
  add_custom_target("uninstall" COMMENT "Uninstall installed files")
  add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
  )
