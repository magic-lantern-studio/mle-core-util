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
#include "mle/mlTrace.h"
#include "mle/mlFileio.h"


TEST(MlTraceTest, OutputInformativeMsg) {
    MLE_TRACE_INFO(stdout, Testing info trace);
}

TEST(MlTraceTest, OutputWarningMsg) {
    MLE_TRACE_WARN(stdout, Testing warning trace);
}

TEST(MlTraceTest, OutputErrorMsg) {
    MLE_TRACE_ERROR(stderr, Testing error trace);
}


static FILE *traceFd;

TEST(MlTraceTest, OutputInformativeMsgToFile) {
	traceFd = mlFOpen("MlTraceTest.log", "w+");
    MLE_TRACE_INFO(traceFd, Testing info trace to file);
    MLE_TRACE_WARN(traceFd, Testing warning trace to file);
    MLE_TRACE_ERROR(traceFd, Testing error trace to file);
    mlFClose(traceFd);
}
