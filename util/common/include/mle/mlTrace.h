/**
 * @defgroup MleCore Magic Lantern Core Utility Library API
 * @brief Utility library for Magic Lantern tools and titles.
 *
 * The utility library is a common library that provides components
 * that may be used in both Magic Lantern tools as well and target
 * platform titles.
 */

/**
 * @file mlTrace.h
 * @ingroup MleCore
 *
 * This file contains the trace macros defined
 * for Magic Lantern. It is designed to be shared by both C
 * and C++ programs.
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

#ifndef __MLE_TRACE_H_
#define __MLE_TRACE_H_


#if defined(MLE_DEBUG)

#if defined(WIN32) || defined (__linux__) || defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#else /* Unknown platform. */
error...undefined platform
#endif

// Include Magic Lantern header files
#include "mle/mlMalloc.h"
#include "mle/mlTime.h"

#endif /* MLE_DEBUG */


/**** UTILITY MACROS *****/

#if defined(MLE_DEBUG)

#if defined(WIN32)|| defined(__linux__) || defined(__APPLE__)
/**
 * Trace macro for displaying informative messages.
 * - turned on if MLE_DEBUG flag is set.
 * It will write a log entry to file.
 */
#define MLE_TRACE_INFO(file, msg)                                             \
    {                                                                         \
        char *utc = mlGetUTC();                                               \
        fprintf(file,                                                         \
                "%s [INFO] " __FILE__ ": %d: " #msg "\n",                     \
                utc, __LINE__);                                               \
        fflush(file);                                                         \
        mlFree(utc);                                                          \
    }
#else /* Unknown platform. */
error...undefined platform (must define one of __linux__, __APPLE__, WIN32)
#endif
#else /* ! MLE_DEBUG */

#define MLE_TRACE_INFO(file, msg)

#endif /* MLE_DEBUG */

#if defined(MLE_DEBUG)

#if defined(WIN32) || defined(__linux__) || defined(__APPLE__)
/**
 * Trace macro for displaying warning messages in debug mode.
 * - turned on if MLE_DEBUG flag is set.
 * It will write a log entry to file.
 */
#define MLE_TRACE_WARN(file, msg)                                             \
    {                                                                         \
        char *utc = mlGetUTC();                                               \
        fprintf(file,                                                         \
                "%s [WARN] " __FILE__ ": %d: " #msg "\n",                     \
                utc, __LINE__);                                               \
        fflush(file);                                                         \
        mlFree(utc);                                                          \
    }
#else /* Unknown platform. */
error...undefined platform (must define one of __linux__, __APPLE__, WIN32)
#endif
#else /* ! MLE_DEBUG */

#define MLE_TRACE_WARN(file, msg)

#endif /* MLE_DEBUG */

#if defined(MLE_DEBUG)

#if defined(WIN32)|| defined(__linux__) || defined(__APPLE__)
/**
 * Trace macro for displaying error messages in debug mode.
 * - turned on if MLE_DEBUG flag is set.
 * It will write a log entry to file.
 */
#define MLE_TRACE_ERROR(file, msg)                                             \
    {                                                                          \
        char *utc = mlGetUTC();                                                \
        fprintf(file,                                                          \
                "%s [ERROR] " __FILE__ ": %d: " #msg "\n",                     \
                utc, __LINE__);                                                \
        fflush(file);                                                          \
        mlFree(utc);                                                           \
    }
#else /* Unknown platform. */
error...undefined platform (must define one of __linux__, __APPLE__, WIN32)
#endif
#else /* ! MLE_DEBUG */

#define MLE_TRACE_ERROR(file, msg)

#endif /* MLE_DEBUG */


#endif /* __MLE_TRACE_H_ */

