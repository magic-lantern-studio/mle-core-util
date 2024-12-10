/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlTime.h
 * @ingroup MleCore
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2024 Wizzer Works
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


#ifndef __MLE_TIME_H_
#define __MLE_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif


// Include system header files
#include <time.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/MleUtil.h"


/**
 * @brief Get the Universal Time Code.
 *
 * @return A pointer to a character string is returned using the
 * %04d-%02d-%02dT%02d:%02d:%02d format. The string is dynamically allocated
 * and is 72 bytes long. The user of this routine is responsible for managing
 * the allocated memory.
 */
EXTERN MLE_UTIL_API char *mlGetUTC();


#ifdef __cplusplus
}
#endif

#endif /* __MLE_TIME_H__ */
