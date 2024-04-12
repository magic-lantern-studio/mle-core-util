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

// Include system header files.
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/mlDebug.h"
#include "mle/mlFileio.h"

#define BOGUS_ENVVAR "_BOGUS_"
#define BOGUS_FILE   "._bogus"
#define TEST_FILE    "mldbg.tmp"

TEST(MlDebugTest, CreateDebugManager) {
    // This test is named "CreateDebugManager", and belongs to the "MlDebugTest"
    // test case.

	MleDebugMgrP *mlDebugMgr = NULL;
    mlDebugMgr = mlDebugCreate(const_cast<char *>(BOGUS_ENVVAR),const_cast<char *>(BOGUS_FILE));

    ASSERT_NE(mlDebugMgr, nullptr);

    MlBoolean match;
    match = mlDebugMatch(mlDebugMgr,NULL,"info",1);

    EXPECT_FALSE(match);

    MlBoolean deleted;
    deleted = mlDebugDelete(mlDebugMgr);

    EXPECT_TRUE(deleted);

	mlDebugMgr = NULL;
}

TEST(MlDebugTest, SimpleCategoryConstruction) {
    // This test is named "SimpleCategoryConstruction", and belongs to the "MlDebugTest"
    // test case

	signed long fd;
    if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        FAIL();
    }

    char buf[BUFSIZ];
    strcpy(buf,"info");
    ssize_t nWrite;
    nWrite = mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    MleDebugMgrP *mlDebugMgr = nullptr;
    mlDebugMgr = mlDebugCreate(const_cast<char *>(BOGUS_ENVVAR),const_cast<char *>(TEST_FILE));

    ASSERT_NE(mlDebugMgr, nullptr);

    MlBoolean match;
    match = mlDebugMatch(mlDebugMgr,NULL,"info",0);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched '*.info' with level '0'\n");

    match = mlDebugMatch(mlDebugMgr,NULL,NULL,0);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched '*.*' with level '0'\n");

    MlBoolean deleted;
    deleted = mlDebugDelete(mlDebugMgr);

    EXPECT_TRUE(deleted);

    // Clean up.
    mlDebugMgr = NULL;
    unlink(TEST_FILE);
}

TEST(MlDebugTest, ComponentCategoryConstruction) {
    // This test is named "ComponentCategoryConstruction", and belongs to the "MlDebugTest"
    // test case

	signed long fd;
    if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        FAIL();
    }

    char buf[BUFSIZ];
    strcpy(buf,"Test3.info=5");
    ssize_t nWrite;
    nWrite = mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    MleDebugMgrP *mlDebugMgr = nullptr;
    mlDebugMgr = mlDebugCreate(const_cast<char *>(BOGUS_ENVVAR),const_cast<char *>(TEST_FILE));

    ASSERT_NE(mlDebugMgr, nullptr);

    MlBoolean match;
    match = mlDebugMatch(mlDebugMgr,"Test3","info",1);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched 'Test3.info' with level '1'\n");

    match = mlDebugMatch(mlDebugMgr,NULL,"info",0);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched '*.info' with level '1'\n");

    match = mlDebugMatch(mlDebugMgr,"Test3",NULL,0);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched 'Test3.*' with level '0'\n");

    match = mlDebugMatch(mlDebugMgr,NULL,NULL,0);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched '*.*' with level '0'\n");

    MlBoolean deleted;
    deleted = mlDebugDelete(mlDebugMgr);

    EXPECT_TRUE(deleted);

    // Clean up.
    mlDebugMgr = NULL;
    unlink(TEST_FILE);
}

TEST(MlDebugTest, MultipleComponentCategoryConstruction) {
    // This test is named "MultipleComponentCategoryConstruction", and belongs to the "MlDebugTest"
    // test case

	signed long fd;
    if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        FAIL();
    }

    ssize_t nWrite;
    char buf[BUFSIZ];
    strcpy(buf,"Test1.info=1\n");
    nWrite = mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test2.info=2\n");
    nWrite = mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test3.info=3\n");
    nWrite = mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test1.debug=4\n");
    nWrite = mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test2.debug=5\n");
    nWrite = mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    MleDebugMgrP *mlDebugMgr = nullptr;
    mlDebugMgr = mlDebugCreate(const_cast<char *>(BOGUS_ENVVAR),const_cast<char *>(TEST_FILE));

    ASSERT_NE(mlDebugMgr, nullptr);

    MlBoolean match;
    match = mlDebugMatch(mlDebugMgr,"Test1","info",1);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched 'Test1.info' with level '1'\n");

    match = mlDebugMatch(mlDebugMgr,"Test1","debug",2);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched 'Test1.debug' with level '2'\n");

    match = mlDebugMatch(mlDebugMgr,NULL,"debug",2);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched '*.debug' with level '2'\n");

    match = mlDebugMatch(mlDebugMgr,"Test2",NULL,2);

    EXPECT_TRUE(match);
    fprintf(stdout,"Matched 'Test2.*' with level '2'\n");

    match = mlDebugMatch(mlDebugMgr,NULL,"bogus",5);

    EXPECT_FALSE(match);
    fprintf(stdout,"Unable to match '*.bogus' with level '5'\n");

    MlBoolean deleted;
    deleted = mlDebugDelete(mlDebugMgr);

    EXPECT_TRUE(deleted);

    // Clean up.
    mlDebugMgr = NULL;
    unlink(TEST_FILE);
}

TEST(MlDebugTest, ConvenienceMacros) {
    // This test is named "ConvenienceMacros", and belongs to the "MlDebugTest"
    // test case

	signed long fd;
    if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        FAIL();
    }

    ssize_t nWrite;
    char buf[BUFSIZ];
    strcpy(buf,"Test5.info=5");
    nWrite = mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    MleDebugMgrP *mlDebugMgr = nullptr;
    mlDebugMgr = mlDebugCreate(const_cast<char *>(BOGUS_ENVVAR),const_cast<char *>(TEST_FILE));

    ASSERT_NE(mlDebugMgr, nullptr);
    g_mlDebugMgr = mlDebugMgr;

    //mlDebugDump(g_mlDebugMgr);

    MLE_DEBUG_FULL("Test5","info",3, {
    	SUCCEED();
        fprintf(stdout,"Matched 'Test5.info' with level '3'\n");
    });
    MLE_DEBUG_CAT("info", {
    	SUCCEED();
        fprintf(stdout,"Matched '*.info' with level '0'\n");
    });
    MLE_DEBUG_CATLEVEL("info",3, {
    	SUCCEED();
        fprintf(stdout,"Matched '*.info' with level '3'\n");
    });
    MLE_DEBUG_CMPT("Test5", {
    	SUCCEED();
        fprintf(stdout,"Matched 'Test5.*' with level '0'\n");
    });
    MLE_DEBUG_CMPTLEVEL("Test5",3, {
    	SUCCEED();
        fprintf(stdout,"Matched 'Test5.*' with level '3'\n");
    });
    MLE_DEBUG_FULL("Bogus","bogus",3, {
    	FAIL();
        fprintf(stdout,"Matched 'Bogus.bogus' with level '3'\n");
    });

    MlBoolean deleted;
    deleted = mlDebugDelete(mlDebugMgr);

    EXPECT_TRUE(deleted);

    // Clean up.
    mlDebugMgr = NULL;
    unlink(TEST_FILE);
}
