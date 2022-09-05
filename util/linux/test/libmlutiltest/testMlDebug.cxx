// COPYRTIGH_BEGIN
// COPYRIGHT_END

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/mlDebug.h"

#define BOGUS_ENVVAR "_BOGUS_"
#define BOGUS_FILE   "._bogus"
#define TEST_FILE    "mldbg.tmp"

TEST(MlDebugTest, CreateDebugManager) {
    // This test is named "CreateDebugManager", and belongs to the "MlDebugTest"
    // test case.

	MleDebugMgrP *mlDebugMgr = NULL;
    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,BOGUS_FILE);

    EXPECT_TRUE(mlDebugMgr != NULL);

    MlBoolean match;
    match = mlDebugMatch(mlDebugMgr,NULL,"info",1);

    EXPECT_FALSE(match);

    MlBoolean deleted;
    deleted = mlDebugDelete(mlDebugMgr);

    EXPECT_TRUE(deleted);

	mlDebugMgr = NULL;
}
