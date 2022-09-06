// COPYRTIGH_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2022 Wizzer Works
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

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/mlLogFile.h"

#define CATEGORY "mleLogTest"
#define NAME     "myLogFile"
#define TAG      "test_c"

TEST(LogFileTest, CreateLogFileDescriptor) {
    // This test is named "CreateLogFileDescriptor", and belongs to the "LogFileTest"
    // test case.

	MleLogFileP *descriptor = NULL;
    descriptor = mlLogFileCreate(CATEGORY, NAME, TAG);

    ASSERT_NE(descriptor, nullptr);
    EXPECT_STREQ(mlLogFileGetCategory(descriptor), CATEGORY);
    EXPECT_STREQ(mlLogFileGetName(descriptor), NAME);
    EXPECT_STREQ(mlLogFileGetTag(descriptor), TAG);

    mlLogFileDelete(descriptor);
}

TEST(LogFileTest, OpenLogFile) {
    // This test is named "OpenLogFile", and belongs to the "LogFileTest"
    // test case.

	MleLogFileP *descriptor = NULL;
    descriptor = mlLogFileCreate(CATEGORY, NAME, TAG);

    ASSERT_NE(descriptor, nullptr);
    EXPECT_STREQ(mlLogFileGetCategory(descriptor), CATEGORY);
    EXPECT_STREQ(mlLogFileGetName(descriptor), NAME);
    EXPECT_STREQ(mlLogFileGetTag(descriptor), TAG);

    mlLogFileSetDirectory(descriptor,".");

    EXPECT_STREQ(mlLogFileGetDirectory(descriptor), ".");

    MlBoolean opened;
    opened = mlLogFileOpen(descriptor);

    ASSERT_TRUE(opened);

    MlBoolean closed;
    closed = mlLogFileClose(descriptor);

    ASSERT_TRUE(closed);

    // Clean up.
    unlink(mlLogFileGetCategory(descriptor));
    mlLogFileDelete(descriptor);
}

TEST(LogFileTest, PrintfLogFile) {
    // This test is named "PrintfLogFile", and belongs to the "LogFileTest"
    // test case.

	MleLogFileP *descriptor = NULL;
    descriptor = mlLogFileCreate(CATEGORY, NAME, TAG);

    ASSERT_NE(descriptor, nullptr);
    EXPECT_STREQ(mlLogFileGetCategory(descriptor), CATEGORY);
    EXPECT_STREQ(mlLogFileGetName(descriptor), NAME);
    EXPECT_STREQ(mlLogFileGetTag(descriptor), TAG);

    mlLogFileSetDirectory(descriptor,".");

    EXPECT_STREQ(mlLogFileGetDirectory(descriptor), ".");

    MlBoolean opened;
    opened = mlLogFileOpen(descriptor);

    ASSERT_TRUE(opened);

    mlLogFilePrintf(descriptor,"\tTesting mleLogFilePrintf() ...\n");
    for (int i = 0; i < 10; i++)
        mlLogFilePrintf(descriptor,"\tTest %d\n",i);

    MlBoolean closed;
    closed = mlLogFileClose(descriptor);

    ASSERT_TRUE(closed);

    // Clean up.
    unlink(mlLogFileGetCategory(descriptor));
    mlLogFileDelete(descriptor);
}





