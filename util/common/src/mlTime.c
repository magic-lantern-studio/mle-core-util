/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlTime.c
 * @ingroup MleCore
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2024-2025 Wizzer Works
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
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlTime.h"



char *mlGetUTC()
{
    char *retValue = NULL;
  
    time_t t = time(NULL);
    assert(t != ((time_t)-1)); // error handling

    struct tm tm;
    memset(&tm, 0, sizeof(tm));
#if defined(_WINDOWS)
	assert(gmtime_s(&tm, &t) == 0);
#else
    assert(gmtime_r(&t, &tm) != NULL); // error handling
#endif /* ! _WINDOWS */

    retValue = mlMalloc(72);
    assert(retValue != NULL); // error handling

#if defined(_WINDOWS)
	sprintf_s(retValue, 72, "%04d-%02d-%02dT%02d:%02d:%02d",
		1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec);
#else
    sprintf(retValue, "%04d-%02d-%02dT%02d:%02d:%02d",
            1900+tm.tm_year, tm.tm_mon+1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
#endif /* ! _WINDOWS */

    return retValue;
}
