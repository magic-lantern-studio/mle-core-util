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


# Stop if cmake version is below 3.10
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and enable C++ support
project(libmlutil-test VERSION 1.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for Magic Lantern environment variables
if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()

# Fail early if one of the required packages cannot be found. The tests are
# run against libmlutil, which must already be installed in $MLE_ROOT.
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_library(MLUTIL_LIBRARY
  NAMES mlutild mlutil
  HINTS $ENV{MLE_ROOT}/lib
  NO_DEFAULT_PATH)
if (NOT MLUTIL_LIBRARY)
  message(FATAL_ERROR
    "Could not find libmlutil in $ENV{MLE_ROOT}/lib. "
    "Install mle-core-util into MLE_ROOT first.")
endif()
message(STATUS "Using ${MLUTIL_LIBRARY}")

# Specify the compiler macro definitions
add_compile_definitions(
  MLE_NOT_UTIL_DLL
  MLE_DEBUG)

# Specify the include directories for the header files
include_directories(
  include
  ../include
  ../../common/include
  $ENV{MLE_ROOT}/include)

# The unit test library, libmlutiltest. Like the autotools build, it is not
# linked against Google Test or libmlutil; the symbols are resolved by
# exampleProgram, which loads it.
add_library(mlutiltest SHARED
  libmlutiltest/libmlutiltest.cxx
  libmlutiltest/testMlDebug.cxx
  libmlutiltest/testLogFile.cxx
  libmlutiltest/testMlTrace.cxx
  libmlutiltest/testMleArray.cxx
  libmlutiltest/testMleDsoLoader.cxx
  libmlutiltest/testMlUnique.cxx)
target_include_directories(mlutiltest PRIVATE
  $<TARGET_PROPERTY:GTest::gtest,INTERFACE_INCLUDE_DIRECTORIES>)

# The unit test runner.
add_executable(exampleProgram exampleProgram/exampleProgram.cxx)
target_link_libraries(exampleProgram
  mlutiltest
  ${MLUTIL_LIBRARY}
  GTest::gtest
  ${CMAKE_DL_LIBS}
  Threads::Threads)

# Run the unit tests with ctest.
enable_testing()
add_test(NAME libmlutil-test COMMAND exampleProgram)
