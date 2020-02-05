/** @defgroup MleCore Magic Lantern Core Library API */

/**
 * @file mle.h
 * @ingroup MleCore
 *
 * Magic Lantern Core Library API.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */


// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
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
//  For information concerning this header fiel, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_H_
#define __MLE_H_

//
// mlPlatformDefs.h defines the PLATFORM_<foobar> name for each
// platform should there arise a need for platform specific code
// in the common tree, which there won't.
//
#include "mle/mlPlatformDefs.h"

//
// mlPlatformTypes.h should be defined in each platforms
// include/mle directory.  It includes type information
// necessary to bring that platform up to the common level.
//
//#include "mle/mlPlatformTypes.h"

//
// mlCommonDefs.h defines a number of nice macros and other definitions
// useful for code across all platforms.
//
//#include "mle/mlCommonDefs.h"

//
// mlCommonTypes.h defines a number of common types.
//
//#include "mle/mlCommonTypes.h"

// Magic Lantern Core export rules
#ifdef _MLE_CORE_API
#  define MLE_CORE_API MLE_CORE_EXPORT
#  define MLE_CORE_MIN_API MLE_CORE_MIN_EXPORT
#else
#  define MLE_CORE_API MLE_CORE_IMPORT
#  define MLE_CORE_MIN_API MLE_CORE_MIN_IMPORT
#endif

//
// The MLE_ASSERT macro replacement for assert().
//
#include "mle/mlAssert.h"

//
// The MLE_TRACE macro/function.
//
//#include "mle/mlTrace.h"

//#define DEBUG_MEMORY_FAILURE 1

//
// The memory override calls.
//
#ifdef DEBUG_MEMORY_FAILURE

#include "mle/mlMemoryManager.h"

#endif /* DEBUG_MEMORY_FAILURE */

#endif /* __MLE_H_ */


