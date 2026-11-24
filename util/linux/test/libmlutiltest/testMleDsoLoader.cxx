// COPYRTIGH_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2022 - 2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <string.h>
#include <sstream>

// Include Google Test header files.
#include "gtest/gtest.h"

// Include system header files.
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/MleDsoLoader.h"

namespace {

// Load a class in a process of its own, so that a crash is a failure of this
// test only, and so is a hang.
void loadAndLeave(const std::string &name, const char *prefix) {
	alarm(30);
	MleDSOLoader loader;
	loader.loadClass(name.c_str(), prefix);
	exit(0);
}

} // namespace

TEST(MleDsoLoaderTest, ClassThatDoesNotExistIsNotLoaded) {
	MleDSOLoader loader;
	EXPECT_NE(0, loader.loadClass("NoSuchClassAnywhere", "MleNoSuch"));
	EXPECT_NE(0, loader.loadClass("NoSuchClassAnywhere"));
}

// The names of the file and the entry point of a class are built in buffers of
// 256 bytes, so a class name that is longer than that must be refused and not
// overwrite the stack.
TEST(MleDsoLoaderTest, LongClassNamesAreRefused) {
	for (size_t length : { 100, 249, 250, 252, 253, 256, 300, 5000 }) {
		std::string name(length, 'k');
		EXPECT_EXIT(loadAndLeave(name, NULL), ::testing::ExitedWithCode(0), "") << length;
		EXPECT_EXIT(loadAndLeave(name, "MleDwp"), ::testing::ExitedWithCode(0), "") << length;
	}
}

TEST(MleDsoLoaderTest, LongClassNameIsNotLoaded) {
	MleDSOLoader loader;
	EXPECT_NE(0, loader.loadClass(std::string(1000, 'k').c_str(), "MleDwp"));
}

TEST(MleDsoLoaderTest, LongFileNameAndDirectoryAreNotOpened) {
	static char dir[] = "./";
	static char *paths[] = { dir, NULL };
	EXPECT_TRUE(MleDSOLoader::loadFile("NoSuchLibrary.so", paths) == NULL);

	for (size_t length : { 250, 254, 255, 256, 300, 5000 }) {
		std::string file(length, 'f');
		EXPECT_TRUE(MleDSOLoader::loadFile(file.c_str(), paths) == NULL) << length;
	}
	std::string longDir(300, 'd');
	char *longPaths[] = { const_cast<char *>(longDir.c_str()), dir, NULL };
	EXPECT_TRUE(MleDSOLoader::loadFile("NoSuchLibrary.so", longPaths) == NULL);
}

TEST(MleDsoLoaderTest, ClassNamesThatFitAreStillLookedFor) {
	// The longest class name that fits in a file name is 252 characters
	// (the name, then ".so"), and in a file name with "lib" in front, 249.
	for (size_t length : { 10, 100, 245, 249 }) {
		MleDSOLoader loader;
		EXPECT_NE(0, loader.loadClass(std::string(length, 'k').c_str())) << length;
	}
}
