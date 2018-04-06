/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlErrno.h
 * @ingroup MleCore
 *
 * This file contains utilities for keeping track of
 * Magic Lantern errors and return results.
 * It is designed to be shared by both C and C++ programs.
 *
 * @author Mark S. Millard
 * @date May 5, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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

#ifndef __MLE_ERRNO_H_
#define __MLE_ERRNO_H_

#if defined(__linux__)
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type definition for routines returning error values.
 */
typedef unsigned short MlErr;

/**
 * Type definition for routines returning values.
 */
typedef unsigned short MlResult;


/**
 * Global variable for holding the last error.
 */
extern unsigned short g_mlErrno;


/**
 * Set the errno value.
 *
 * @param The errno value to set. It should be an unsigned short.
 */
#define mlSetErrno(value) (g_mlErrno = value)

/**
 * Unset the errno value.
 *
 * @return The errno value is returned. It will be an unsigned short.
 */
#define mlGetErrno() (g_mlErrno)


/* Errno values. */

/**
 * Errno value for no error. Zero is no error.
 */
#define MlNoErr 0

/**
 * Errno value for success, or Ok.
 */
#define MLE_S_OK MlNoErr

/**
 * Errno value for failure, or Fail.
 */
#define MLE_E_FAIL ~MLE_S_OK

/*
 * Errno values 1 - 511 are reserved for the Runtime Player Library.
 */
#define MLE_E_PLAYERLIB_BASE             1                       /**< Base errno value for Runtime Player Library. */
#define MLE_E_EVMGR_FAILEDDISPATCH       MLE_E_PLAYERLIB_BASE+1  /**< Event Manager dispatch failed. */
#define MLE_E_EVMGR_DISABLEDDISPATCH     MLE_E_PLAYERLIB_BASE+2  /**< Event Manager dispatch disabled. */
#define MLE_E_EVMGR_FAILEDCALLBACK       MLE_E_PLAYERLIB_BASE+3  /**< Event Manager callback failed. */


/*
 * Errno vlaues 512 - 1023 are reserved for the Digital Workprint Library.
 */
#define MLE_E_DWPLIB_BASE                512                     /**< Base errno for Digital Workprint Libary. */

/*
 * Errno values 1024 - 1535 are reserved for the Digital Playprint Library.
 */
#define MLE_E_DPPLIB_BASE                1024                    /**< Base errno for Digital Playprint Libary. */

/*
 * Errno values 1536 - 2047 are reserved for the Foundation Library.
 */
#define MLE_E_UTILLIB_BASE               1536                    /**< Base errno for Core Foundation Libary. */
#define MLE_E_UTILLIB_BADFORMAT          MLE_E_UTILLIB_BASE+1

/*
 * Errno values 2048 - 2559 are reserved for the Mastering Library.
 */
#define MLE_E_MASTERLIB_BASE             2048                    /**< Base errno for Mastering Libary. */
#define MLE_E_MASTERLIB_BADTBLINDEX      MLE_E_MASTERLIB_BASE+0  /**< Mastering Library bad table index. */
#define MLE_E_MASTERLIB_NOACTORCLASS     MLE_E_MASTERLIB_BASE+1  /**< Mastering Library no actor class. */
#define MLE_E_MASTERLIB_NODELEGATECLASS  MLE_E_MASTERLIB_BASE+2  /**< Mastering Library no role class. */
#define MLE_E_MASTERLIB_NOFORUM          MLE_E_MASTERLIB_BASE+3  /**< Mastering Library no set. */

/*
 * Errno values 2560 - 3071 are reserved for the Authoring Toolkit Library.
 */
#define MLE_E_ATKLIB_BASE                2560                    /**< Base errno for Authroing Toolkit. */
#define MLE_E_ATKLIB_CONNECTION_LOST     MLE_E_ATKLIB_BASE+0     /**< Authoring Toolkit connection lost. */

/*
 * Errno values 3072 - 3583 are reserved for the Math Library.
 */
#define MLE_E_MATHLIB_BASE               3072                    /**< Base errno for Math Libary. */

/*
 * Errno values 3584 - 4095 are reserved for the Behavior Library.
 */
#define MLE_E_BEHAVIORLIB_BASE           3584                    /**< Base errno for Behavior Libary. */

/*
 * Errno values 4096 - 4607 are reserved for Web I/O.
 */
#define MLE_E_WEBIO_BASE                 4096                    /**< Base errno for Web IO. */
#define MLE_E_WEBIO_NOT_READY            MLE_E_WEBIO_BASE+0      /**< Web IO not ready. */
#define MLE_E_WEBIO_ERROR                MLE_E_WEBIO_BASE+1      /**< Web IO general error. */
#define MLE_E_WEBIO_FAILED_OPEN          MLE_E_WEBIO_BASE+2      /**< Web IO failed to open. */
#define MLE_E_WEBIO_FAILED_WRITE         MLE_E_WEBIO_BASE+3      /**< Web IO failed to write. */
#define MLE_E_WEBIO_FAILED_READ          MLE_E_WEBIO_BASE+4      /**< Web IO failed to read*/
#define MLE_E_WEBIO_BAD_SOCKET           MLE_E_WEBIO_BASE+5      /**< Web IO bad socket. */
#define MLE_E_WEBIO_BAD_STATE            MLE_E_WEBIO_BASE+6      /**< Web IO bad state. */
#define MLE_E_WEBIO_FAILED_READ_HEADERS  MLE_E_WEBIO_BASE+7      /**< Web IO failed to read headers. */
#define MLE_E_WEBIO_FAILED_READ_DATA     MLE_E_WEBIO_BASE+8      /**< Web IO failed to read data. */

/*
 * Errno values 4608 - 5119 are reserved for Roles
 */
#define MLE_E_ROLE_BASE              4608                        /**< Base errno for Roles. */
#define MLE_E_ROLE_NO_ROOT           MLE_E_ROLE_BASE+1           /**< No root. */
#define MLE_E_ROLE_NO_MEDIA          MLE_E_ROLE_BASE+2           /**< No media. */
#define MLE_E_ROLE_MEDIA_LOAD_FAILED MLE_E_ROLE_BASE+3           /**< Media failed to load. */

/*
 * Errno values 5120 - 5631 are reserved for Actors.
 */
#define MLE_E_ACTOR_BASE                 5120                    /**< Base errno for Actors. */
#define MLE_E_ACTOR_NO_PROP_DATASET      MLE_E_ACTOR_BASE + 1    /**< No property data set. */

/*
 * Errno values 10240 - 11263 are reserved for Mastering Tools.
 */
#define MLE_E_MASTERTOOLS_BASE           10240                   /**< Base errno for Mastering Tools. */

/*
 * Errno values 11264 - 12287 are reserved for Authoring Tools.
 */
#define MLE_E_AUTHORTOOLS_BASE           11264                   /**< Base errno for Authoring Tools. */



#ifdef __cplusplus
} /* end of 'extern "C"' */
#endif


#endif /* __MLE_ERRNO_H_ */
