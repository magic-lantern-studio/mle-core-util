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
#include <stdlib.h>
#include <string>
#if defined(__GLIBC__)
#include <malloc.h>
#endif

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/mlExpandFilename.h"
#include "mle/mlMalloc.h"

namespace {

// Expand a name, and free the result.
std::string expand(const char *name) {
	char *result = mlFilenameExpand(name);
	EXPECT_TRUE(result != NULL);
	std::string s = result ? result : "";
	mlFree(result);
	return s;
}

} // namespace

TEST(MlExpandFilenameTest, NameWithNothingToExpandIsCopied) {
	EXPECT_EQ("plain/file.txt", expand("plain/file.txt"));
	EXPECT_EQ("", expand(""));
}

TEST(MlExpandFilenameTest, NullNameIsEmpty) {
	EXPECT_EQ("", expand(NULL));
}

TEST(MlExpandFilenameTest, EnvironmentVariableIsExpanded) {
	setenv("MLUTILTEST_DIR", "/some/dir", 1);
	EXPECT_EQ("/some/dir/file", expand("$MLUTILTEST_DIR/file"));
	EXPECT_EQ("/some/dir/file", expand("${MLUTILTEST_DIR}/file"));
	unsetenv("MLUTILTEST_DIR");
}

TEST(MlExpandFilenameTest, TildeIsTheHomeDirectory) {
	setenv("HOME", "/home/mltest", 1);
	EXPECT_EQ("/home/mltest", expand("~"));
	EXPECT_EQ("/home/mltest/file", expand("~/file"));
}

TEST(MlExpandFilenameTest, TildeOfAnUnknownUserIsKept) {
	EXPECT_EQ("~nosuchuseranywhere/file", expand("~nosuchuseranywhere/file"));
}

TEST(MlExpandFilenameTest, ExpansionDoesNotLeak) {
#if defined(__GLIBC__) && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 33))
	setenv("HOME", "/home/mltest", 1);
	setenv("MLUTILTEST_DIR", "/some/dir", 1);
	const char *names[] = { "plain", "~", "~/file", "$MLUTILTEST_DIR/file" };
	// (An unknown user is left out: getpwnam() keeps memory of its own.)
	// Warm up, so that memory the library keeps for good is not counted.
	for (const char *name : names) expand(name);
	size_t before = mallinfo2().uordblks;
	for (int i = 0; i < 200; i++)
		for (const char *name : names) expand(name);
	EXPECT_EQ(before, mallinfo2().uordblks);
#else
	GTEST_SKIP() << "No heap statistics.";
#endif
}
