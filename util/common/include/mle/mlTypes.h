/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlTypes.h
 * @ingroup MleCore
 *
 * This file contains the data types and macro constants defined
 * for Magic Lantern. It is designed to be shared by both C
 * and C++ programs.
 *
 * @author Mark S. Millard
 * @date May 1, 2015
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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

#ifndef __MLE_TYPES_H_
#define __MLE_TYPES_H_


/* Required system header files. */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#ifdef __cplusplus
/* #include <iostream.h> */
#endif


/*** A R C H I T E C T U R E    D E P E N D A N T ***/
/* Build basic architecture dependant types here. */

/* Unsigned types. */
typedef unsigned char       ubyte_t;     /**< 8-bit unsigned type. */
typedef unsigned char       uchar_t;     /**< 8-bit unsigned type. */
typedef unsigned short      ushort_t;    /**< 16-bit unsigned type. */
#ifdef __sgi
typedef __uint32_t          uint_t;      /**< 32-bit unsigned type. */
typedef __uint64_t          uintd_t;     /**< 64-bit unsigned type. */
#endif
#ifdef WIN32
typedef unsigned __int32    uint_t;      /**< 32-bit unsigned type. */
typedef unsigned __int64    uintd_t;     /**< 64-bit unsigned type. */
#endif
#ifdef __linux__
typedef u_int32_t           uint_t;      /**< 32-bit unsigned type. */
typedef u_int64_t           uintd_t;     /**< 64-bit unsigned type. */
#endif

/* Signed types. */
typedef signed char         byte_t;      /**< 8-bit signed type. */
typedef signed char         char_t;
typedef signed short        short_t;     /**< 16-bit signed type. */
#ifdef __sgi
typedef __int32_t           int_t;       /**< 32-bit signed type. */
typedef __int64_t           intd_t;      /**< 64-bit signed type. */
#endif
#ifdef WIN32
typedef signed __int32      int_t;       /**< 32-bit signed type. */
typedef signed __int64      intd_t;      /**< 64-bit signed type. */
#endif
#ifdef __linux__
typedef int32_t             int_t;      /**< 32-bit unsigned type. */
typedef int64_t             intd_t;     /**< 64-bit unsigned type. */
#endif

/* Floating point types. */
#if defined(LINUX)
/* The ix86 FPUs evaluate all values in the 80 bit floating-point format
   which is also available for the user as `long double'.  Therefore we
   define:  */
typedef long double         float_t;     /**< single precision floating-point type. */
typedef long double         double_t;    /**< double precision floating-point type. */
#else
typedef float               float_t;     /**< single precision floating-point type. */
typedef double              double_t;    /**< double precision floating-point type. */
#endif


#define ML_UBYTE_MIN        0            /**< Minimal 8-bit unsigned value. */
#define ML_UBYTE_MAX        UCHAR_MAX    /**< Maximum 8-bit unsigned value. */
#define ML_UINT16_MIN       0            /**< Minimal 16-bit unsigned value. */
#define ML_UINT16_MAX       USHRT_MAX    /**< Maximum 16-bit unsigned value. */
#define ML_UINT32_MIN       0            /**< Minimal 32-bit unsigned value. */
#define ML_UINT32_MAX       ULONG_MAX    /**< Maximum 32-bit unsigned value. */
#define ML_UINT64_MIN       0            /**< Minimal 64-bit unsigned value. */
#define ML_UINT64_MAX       ULONG_MAX    /**< Maximum 64-bit unsigned value. */

#define ML_SBYTE_MIN        SCHAR_MIN    /**< Minimal 8-bit signed value. */
#define ML_SBYTE_MAX        SCHAR_MAX    /**< Maximum 8-bit signed value. */
#define ML_SINT16_MIN       SHRT_MIN     /**< Minimal 16-bit signed value. */
#define ML_SINT16_MAX       SHRT_MAX     /**< Maximum 16-bit unsigned value. */
#define ML_SINT32_MIN       LONG_MIN     /**< Minimal 32-bit signed value. */
#define ML_SINT32_MAX       LONG_MAX     /**< Maximum 32-bit unsigned value. */
#define ML_SINT64_MIN       LONG_MIN     /**< Minimal 64-bit signed value. */
#define ML_SINT64_MAX       LONG_MAX     /**< Maximum 64-bit unsigned value. */

#define ML_FLOAT_MIN        FLT_MIN      /**< Minimal single precision floating-point value. */
#define ML_FLOAT_MAX        FLT_MAX      /**< Maximum single precision floating-point value. */
#define ML_DOUBLE_MIN       DBL_MIN      /**< Minimal double precision floating-point value. */
#define ML_DOUBLE_MAX       DBL_MAX      /**< Maximum double precision floating-point value. */


#ifndef NULL
/**
 * Magic Lantern Macro Constant NULL
 */
#define NULL    0
#endif

#ifndef FALSE
/**
 * Magic Lantern Macro Constant FALSE
 *
 * This can be the value of a MlBoolean typed variable.
 */
#define FALSE   0
#define False   0
#endif

#ifndef TRUE
/**
 * Magic Lantern Macro Constant TRUE
 *
 * This can be the value of a MlBoolean typed variable.
 */
#define TRUE    1
#define True    1
#endif


/* Set the size of a long so that Magic Lantern can set types correctly. */
#ifdef __sgi
#if (_MIPS_SZLONG == 64)
// 64-bit "native" integers
#define _ML_SZLONG 64
#else /* _MIPS_SZLONG */
// 32-bit "native" integers
#define _ML_SZLONG 32
#endif /* _MIPS_SZLONG */
#endif /* __sgi */

#ifdef __linux__
#if (UINT_MAX >= 0xffffffffffffffff)
// 64-bit "native" integers
#define _ML_SZLONG 64
#else /* UINT_MAX */
// 32-bit "native" integers
#define _ML_SZLONG 32
#endif
#endif /* __linux __ */

#ifdef WIN32
// 32-bit "native" integers
#define _ML_SZLONG 32
#endif /* WIN32 */


/*** A R C H I T E C T U R E    I N D E P E N D A N T ***/

/* Define internal types here. */


/* Unsigned types. */
typedef ubyte_t         MlByte;    /**< Magic Lantern byte data type. */
typedef uchar_t         MlUChar;   /**< Magic Lantern unsigned char data type. */
typedef ushort_t        MlUShort;  /**< Magic Lantern unsigned short data type. */
typedef uint_t          MlUInt;    /**< Magic Lantern unsigned integer data type. */
#if (_ML_SZLONG == 64)
typedef uintd_t         MlULong;
#else /* _ML_SZLONG */
typedef uintd_t         MlULong;   /**< Magic Lantern unsigned long data type. */
#endif /* _ML_SZLONG */

/* Signed types. */
typedef char_t          MlChar;    /**< Magic Lantern signed char data type. */
typedef short_t         MlShort;   /**< Magic Lantern signed short data type. */
typedef int_t           MlInt;     /**< Magic Lantern signed integer data type. */
#if (_ML_SZLONG == 64)
typedef intd_t          MlLong;
#else /* _ML_SZLONG */
typedef intd_t          MlLong;    /**< Magic Lantern signed long data type. */
#endif /* _ML_SZLONG */

/* Floating point types. */
typedef float_t         MlFloat;   /**< Magic Lantern floating-point precision data type. */
typedef double_t        MlDouble;  /**< Magic Lantern double precision data type. */

/* Floating-point vector types. */
typedef MlFloat         MlFloatVec3[3];  /**< Magic Lantern 3 element vector. */
typedef MlFloat         MlFloatVec4[4];  /**< Magic Lantern 4 element vector. */

/* Convenience names for floating-point types. */
typedef MlFloat         MlVec3[3];  /**< Magic Lantern 3 element vector. */
typedef MlFloat         MlVec4[4];  /**< Magic Lantern 4 element vector. */

/* Floating-point 4x4 matrix type. */
typedef MlFloat         MlMatrix[4][4];  /**< Magic Lantern 4x4 matrix. */

/* Floating-point quaternion type. */
typedef MlFloat         MlQuat[4];       /**< Magic Lantern quaternion. */

/* Orientation is a quaternion, but hide the fact via this typedef. */
typedef MlQuat          MlOrient;        /**< Magic Lantern orientation. */


/* Float vs. fixed point arithmetic. */
/* #define ML_USE_FIXED */

/* Fixed point manipulation constants. */
#ifdef ML_USE_FIXED
#if (_ML_SZLONG == 64)
/* If long == 64 bits, then store "floats" as 32.32 */
#define ML_HALF_LONG_FLOAT    4294967296.0
#define ML_HALF_LONG_SHIFT    32
#else /* _ML_SZLONG */
/* If long == 32 bits, then store "floats" as 16.16 */
#define ML_HALF_LONG_FLOAT    65536.0
#define ML_HALF_LONG_SHIFT    16
#endif /* _ML_SZLONG */
#endif /* ML_USE_FLOATS */


/*
 *  Boolean value type
 *
 *  Many systems define 'BOOL' and 'BOOLEAN' as an int (e.g.
 *  (Microsoft Windows 3.1)
 *
 *  To improve the semantic consistency of our code we use this
 *  enumeration for all boolean values that do not interface to
 *  system dependent libraries.
 */

/* ML_FALSE, ML_NO and ML_NOTOK represent the binary state 0 */
#define ML_FALSE   0    /**< Magic Lantern FALSE macro - binary state 0. */
#define ML_NO      0    /**< Magic Lantern NO macro - binary state 0. */
#define ML_NOTOK   0    /**< Magic Lantern NOTOK macro - binary state 0. */
/* ML_TRUE, ML_YES and ML_OK represent the binary state 1 */
#define ML_TRUE    1    /**< Magic Lantern TRUE macro - binary state 1. */
#define ML_YES     1    /**< Magic Lantern YES macro - binary state 1. */
#define ML_OK      1    /**< Magic Lantern OK macro - binary state 1. */

/**
 * Magic Lantern Data Type MleBoolean
 *
 * MlBoolean is used to designate a Magic Lantern
 * boolean flag. MlBoolean can be either TRUE
 * or FALSE
 */
typedef unsigned short MlBoolean;


/**
 *  This is the file on which debugging information is displayed.
 */
#define ML_DEBUG_OUTPUT_FILE stdout

/**
 *  This is the file on which error information is displayed.
 */
#define ML_ERROR_OUTPUT_FILE stdout


#ifdef __cplusplus

/* Include C++ specific constants here. */

/**
 *  This is the ostream on which debugging information is displayed.
 */
#define ML_DEBUG_OUTPUT_STREAM cout

/**
 *  This is the ostream on which error information is displayed.
 */
#define ML_ERROR_OUTPUT_STREAM cout

#endif /* __cplusplus */


/* Define EXTERN macro. */
#if !defined(EXTERN)
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif
#endif


#endif /* _MLE_TYPES_H_ */
