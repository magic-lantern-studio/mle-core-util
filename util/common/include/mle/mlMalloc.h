/** @defgroup MleCore Magic Lantern Runtime Engine Core Utility Library API */

/**
 * @file mlMalloc.h
 * @ingroup MleCore
 *
 * This file includes wrappers for the Magic Lantern memory
 * allocation utilities. It is designed to be shared by
 * both C and C++ programs.
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
//  For information concerning this header files, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_MALLOC_H_
#define __MLE_MALLOC_H_

/* No Magic Lantern headers needed. */

/**
 * To replace the Magic Lantern memory allocation utilities, simply write
 * routines which correspond to mlMalloc(), mlFree(), mlRealloc() and
 * mlCalloc(). Then compile dependent code with the MLE_MALLOC compilation
 * macro turned on.
 */

#ifdef MLE_MALLOC

/**
 * MlePhysadr is used to force long-word alignment (a useful thing for
 * RISC-based systems).
 */
typedef struct _MlPhysadr { int r[1]; } *MlPhysadr;

void       mlFree(void *);
MlPhysadr *mlMalloc(unsigned int);
MlPhysadr *mlRealloc(void *, unsigned int);
MlPhysadr *mlCalloc(unsigned int, unsigned int);

#else /* ! MLE_MALLOC */

/* Include system header files. */
#include <stdlib.h>
#ifdef WIN32
#include <memory.h>
#endif /* WIN32 */

typedef void *MlPhysadr;

#define mlFree    free
#define mlMalloc  malloc
#define mlRealloc realloc
#define mlCalloc  calloc

#endif /* MLE_MALLOC */


#endif /* __MLE_MALLOC_H_ */
