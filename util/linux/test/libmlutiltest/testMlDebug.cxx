// COPYRTIGH_BEGIN
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
    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,BOGUS_FILE);

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
    mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    MleDebugMgrP *mlDebugMgr = NULL;
    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,TEST_FILE);

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
    mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    MleDebugMgrP *mlDebugMgr = NULL;
    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,TEST_FILE);

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

    char buf[BUFSIZ];
    strcpy(buf,"Test1.info=1\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test2.info=2\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test3.info=3\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test1.debug=4\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test2.debug=5\n");
    mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    MleDebugMgrP *mlDebugMgr = NULL;
    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,TEST_FILE);

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
