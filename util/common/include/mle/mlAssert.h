/** @defgroup MleCore Magic Lantern Runtime Engine Core Utility Library API */

/**
 * @file mlAssert.h
 * @ingroup MleCore
 *
 * This file contains the assertion macros defined
 * for Magic Lantern. It is designed to be shared by both C
 * and C++ programs.
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_ASSERT_H_
#define __MLE_ASSERT_H_


#if defined(MLE_DEBUG)

#if defined(__sgi) || defined(WIN32) || defined (__linux__)
#include <stdio.h>
#include <stdlib.h>
#elif defined(psx)
#include <libsn.h>
void BREAKPOINT ();
#else /* Unknown platform. */
error...undefined platform
#endif

#ifndef MLE_ERROR_OUTPUT_FILE
/**
 *  This is the file on which error information is displayed.
 */
#define MLE_ERROR_OUTPUT_FILE stdout
#endif /* MLE_ERROR_OUTPUT_FILE */

#endif /* MLE_DEBUG */


/**** UTILITY MACROS ****/

#if defined(MLE_DEBUG)

#if defined(__sgi) || defined(WIN32) || defined(__linux__)
/**
 * Assert macro for testing pre-conditions in function implementations
 * - turned on if MLE_DEBUG flag is set.
 * It will write a log entry to MLE_ERROR_OUTPUT_FILE.
 */
#define MLE_ASSERT(cond)                                                      \
    {                                                                         \
        if (! (cond))                                                         \
        {                                                                     \
            fprintf(MLE_ERROR_OUTPUT_FILE,                                    \
              "Assert Failure: " __FILE__ ": %d: " #cond " failed!\n",        \
              __LINE__);                                                      \
            fflush(MLE_ERROR_OUTPUT_FILE);                                    \
            abort();                                                          \
        }                                                                     \
    }
#elif defined(psx)
#define MLE_ASSERT(cond)                                                      \
    {                                                                         \
        if (! (cond))                                                         \
        {                                                                     \
            BREAKPOINT();                                                     \
        }                                                                     \
    }
#else /* Unknown platform. */
error...undefined platform (must define one of __linux__, __sgi, WIN32, psx)
#endif
#else /* ! MLE_DEBUG */

#define MLE_ASSERT(cond)

#endif /* MLE_DEBUG */


#if defined(MLE_DEBUG)

#if defined(__sgi) || defined(WIN32) || defined(__linux__)
/**
 * Assert macro for testing pre-conditions in function implementations
 * - turned off unless MLE_DEBUG flag is set.
 * It will write a log entry to MLE_ERROR_OUTPUT_FILE.
 */
#define MLE_ASSERT_MSG(cond, msg)                                             \
	{                                                                         \
        if (! (cond))                                                         \
		{                                                                     \
            fprintf(MLE_ERROR_OUTPUT_FILE,                                    \
              "Error: " __FILE__ ": %d: " #cond " failed!\n",                 \
              __LINE__);                                                      \
        fprintf(MLE_ERROR_OUTPUT_FILE, msg);                                  \
            fflush(MLE_ERROR_OUTPUT_FILE);                                    \
            abort();                                                          \
		}                                                                     \
    }
#elif defined(psx)
#define MLE_ASSERT_MSG(cond, msg)                                             \
	{                                                                         \
        if (! (cond))                                                         \
		{                                                                     \
            BREAKPOINT();                                                     \
		}                                                                     \
    }
#else
error...undefined platform (must define one of__linux__, __sgi, WIN32, psx)
#endif
#else /* ! MLE_DEBUG */

#define MLE_ASSERT_MSG(cond, msg)

#endif /* MLE_DEBUG */


#if defined(MLE_DEBUG)

#if defined(__sgi) || defined(WIN32) || defined(__linux__)
/**
 * Use the following when calling methods or functions that are
 * not absolutely required to succeed.
 * It will write a log entry to MLE_ERROR_OUTPUT_FILE.
 */
#define MLE_WARN(func)                                                        \
    {                                                                         \
        MlBoolean rval;                                                       \
        if (TRUE != (rval = (func)))                                          \
		{                                                                     \
            fprintf(MLE_ERROR_OUTPUT_FILE,                                    \
              "Warning: " __FILE__ ": %d" ": " #func " failed! rval = %d\n",  \
              __LINE__,(unsigned long) rval);                                 \
            fflush(MLE_ERROR_OUTPUT_FILE);                                    \
        }                                                                     \
    }
#else /* All other platforms. */
    #define MLE_WARN(func) func;
#endif

#else /* ! MLE_DEBUG */

    #define MLE_WARN(func) func;

#endif /* MLE_DEBUG */


#if defined(MLE_DEBUG)

#if defined(__sgi) || defined(WIN32) || defined(__linux__)

/**
 * Use the following when calling methods or functions that MUST
 * succeed or cause the application to exit if they fail.
 * It will write a log entry to MLE_ERROR_OUTPUT_FILE.
 */
#define MLE_ERROR(func)                                                       \
    {                                                                         \
        MlBoolean rval;                                                       \
        if (TRUE != (rval = (func)))                                          \
        {                                                                     \
            fprintf(MLE_ERROR_OUTPUT_FILE,                                    \
              "Error: " __FILE__ ": %d" ": " #func " failed! rval = %d\n",    \
              __LINE__,(unsigned long) rval);                                 \
            fflush(MLE_ERROR_OUTPUT_FILE);                                    \
            abort();                                                          \
        }                                                                     \
    } /* MLE_ERROR */
#else /* All other platforms. */
#define MLE_ERROR(func) func;
#endif

#else /* ! MLE_DEBUG */

    #define MLE_ERROR(func) func;

#endif


#if defined(MLE_DEBUG)


#if defined(__sgi) || defined(WIN32) || defined(__linux__)
/**
 * Use this macro when calling functions and methods that should throw an
 * exception if they don't succeed.
 * It will write a log entry to MLE_ERROR_OUTPUT_FILE.
 */
#define MLE_EXCEPTION(func, exception)                                        \
    {                                                                         \
        MlBoolean rval;                                                       \
        if (TRUE != (rval = (func)))                                          \
        {                                                                     \
            fprintf(MLE_ERROR_OUTPUT_FILE,                                    \
              "Exception: " __FILE__ ": %d" ": " #func " failed! " #exception "(%d) thrown\n", \
               __LINE__,(unsigned long) rval);                                \
            fflush(MLE_ERROR_OUTPUT_FILE);                                    \
            MLE_THROW(exception(rval));                                       \
        }                                                                     \
    } /* MLE_EXCEPTION */
#else /* All other platforms. */
    #define MLE_EXCEPTION(func, exception)                                    \
    {                                                                         \
        MlBoolean rval;                                                       \
        if (TRUE != (rval = func))                                            \
            MLE_THROW(exception(rval));                                       \
    } /* MLE_EXCEPTION */
#endif

#else /* ! MLE_DEBUG */

#define MLE_EXCEPTION(func, exception)                                        \
    {                                                                         \
        MlBoolean rval;                                                       \
        if (TRUE != (rval = func))                                            \
            MLE_THROW(exception(rval));                                       \
    } /* MLE_EXCEPTION */

#endif /* MLE_DEBUG */


#if defined(MLE_DEBUG)

/**
 * Use this macro to validate pointers.  It is a convenience macro
 * for using MLE_ASSERT.
 */
#define MLE_VALIDATE_PTR(ptr) \
    MLE_ASSERT(ptr != 0)

#else /* ! MLE_DEBUG */

#define MLE_VALIDATE_PTR(ptr)

#endif /* MLE_DEBUG */


#ifdef __cplusplus

#if defined(MLE_DEBUG)

/**
 * Use this macro to validate objects. It is a convenience macro
 * for using MLE_ASSERT
 */
#define MLE_VALIDATE(anObject) \
   MLE_ASSERT((anObject)._checkState() == TRUE)

#else

#define MLE_VALIDATE(anObject)

#endif /* MLE_DEBUG */

#endif /* __cplusplus */


/**** COMPATIBILITY MACROS ****/

#if defined(WIN32)

/**
 * A macro for handling exception throwing on different platforms.
 */
#define MLE_THROW(exception)                                                  \
    {                                                                         \
        THROW(new exception);                                                 \
    }

#elif defined (__GNUC__)

#define MLE_THROW(exception)                                                  \
    {                                                                         \
        MLE_ERROR_OUTPUT_STREAM << #exception " thrown at line #"             \
            << __LINE__ << " in " __FILE__ "\n";                              \
        IM_ERROR_OUTPUT_STREAM.flush();                                       \
        exit(-1);                                                             \
    }

#else /* All other platforms. */

#define MLE_THROW(exception) exception

#endif


#endif /* __MLE_ASSERT_H_ */
