/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlFileio.h
 * @ingroup MleCore
 *
 * This file includes wrappers for the Magic Lantern file
 * IO utilities. It is designed to be shared by
 * both C and C++ programs.
 *
 * @author Mark S. Millard
 * @date September 19, 2015
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

#ifndef __MLE_FILEIO_H_
#define __MLE_FILEIO_H_


/* Include Magic Lantern headers. */
#include "mlTypes.h"


/*** L E V E L  1   F I L E   I / O ***/

#if defined(__sgi) || defined(__linux__)

/* Include system header files. */
#include <unistd.h>

#define mlOpen     open
#define mlClose    close
#define mlRead     read
#define mlWrite    write
#define mlSeek     lseek
#define mlAccess   access
#define mlCreate   creat

#endif /* __sgi */


#ifdef WIN32

/* Include system header files. */
#include <io.h>
#include <ctype.h>

#define mlOpen     _open
#define mlClose    _close
#define mlRead     _read
#define mlWrite    _write
#define mlSeek     _lseek
#define mlAccess   _access
#define mlCreate   _creat


/* Define permission modes for open(). */
#define O_RDONLY   _O_RDONLY
#define O_WRONLY   _O_WRONLY
#define O_RDWR     _O_RDWR
#define O_APPEND   _O_APPEND
#define O_CREATE   _O_CREATE
#define O_TRUNC    _O_TRUNC
#define O_EXCL     _O_EXCL

/*  Note: "origin" modes for _lseek() are the same as IRIX "whence" modes. */

/* Define access modes for access(). */
#define R_OK       0x04
#define W_OK       0x02
#define F_OK       0x00

/* Define permission modes for creat(). */
#define S_IREAD    _S_IREAD
#define S_IWRITE   _S_IWRITE

#endif /* WIN32 */


/*** L E V E L  2   F I L E   I / O ***/

#if defined(__sgi) || defined(WIN32) || defined(psx) || defined(__linux__)

/* Include syste header files. */
#include <stdio.h>

#define mlFOpen    fopen
#define mlFClose   fclose
#define mlFRead    fread
#define mlFWrite   fwrite
#define mlFSeek    fseek
#define mlFFlush   fflush
#define mlFTell    ftell
#define mlUnlink   unlink

#if defined(psx)
#define FILE int
int fread (void *ptr, size_t size, size_t nitems, FILE *stream);
int mlerite (const void *ptr, size_t size, size_t nitems, FILE *stream);
FILE *fopen (const char *filename, const char *type);
int fclose (FILE *stream);
int fflush (FILE *stream);
int fseek (FILE *stream, long offset, int whence);
long ftell (FILE *stream);
int unlink(char *path);
#endif /* psx */

#endif /* __sgi */


/*** B Y T E   O R D E R I N G  ***/

/*
 * Definitions for byte order,
 * according to byte significance from low address to high.
 */
#if !defined(__linux__)
#define LITTLE_ENDIAN   1234    /**< least-significant byte first */
#define BIG_ENDIAN      4321    /**< most-significant byte first */

#ifdef WIN32
#define BYTE_ORDER LITTLE_ENDIAN
#else
#define BYTE_ORDER BIG_ENDIAN
#endif
#else
#include <endian.h>
#endif


/*
 * Macros for network/external number representation conversion.
 */
#if BYTE_ORDER == BIG_ENDIAN
#define mlBEToLELong(x)        (x)
#define mlBEToLEInt(x)         (x)
#define mlBEToLEShort(x)       (x)
#define mlLEtoBELong(x)        (x)
#define mlLEtoBEInt(x)         (x)
#define mlLEtoBEShort(x)       (x)
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#if defined(__linux__)
#include <byteswap.h>
#define mlBEToLELong(x)			__bswap_64(x)
#define mlBEToLEInt(x)			__bswap_32(x)
#define mlBEToLEShort(x)		__bswap_16(x)
#else
static uintd_t
mlBEToLELong(uintd_t x)
{
    x = ((x << 8) & 0xFF00FF00FF00FF00ULL ) | ((x >> 8) & 0x00FF00FF00FF00FFULL );
    x = ((x << 16) & 0xFFFF0000FFFF0000ULL ) | ((x >> 16) & 0x0000FFFF0000FFFFULL );
    return (x << 32) | (x >> 32);
}

static uint_t
mlBEToLEInt(uint_t x)
{
    //x = ((x << 8) & 0xFF00FF00 ) | ((x >> 8) & 0xFF00FF );
    //return (x << 16) | (x >> 16);
    return ((x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24));
}

static ushort_t
mlBEToLEShort(ushort_t x)
{
   return (x << 8) | (x >> 8);
}
#endif
#define mlLEToBELong(x)        mlBEToLELong(x)
#define mlLEToBEInt(x)         mlBEToLEInt(x)
#define mlLEToBEShort(x)       mlBEToLEShort(x)
#endif


#endif /* __MLE_FILEIO_H_ */
