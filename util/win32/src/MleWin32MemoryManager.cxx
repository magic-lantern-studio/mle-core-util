/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 *  @file MleWin32MemoryManager.cxx
 *  @ingroup MleCore
 *
 *  Win32 implementation of platform specific memory manager code.
 *
 *  @author Mark S. Millard
 *  @date July 10, 2003
 *  @version 0.13
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

// Include system header files.
#include <stdlib.h>
#include <malloc.h>

// Include Magic Lantern header files.
#include "mle/MleMemoryManager.h"


void
MleMemoryManager::platformInit(void *&cookie)
{
    cookie = NULL;
    // Nothing to do for win32.
}


void
MleMemoryManager::platformCleanup(void *&cookie)
{
    // Nothing to do for win32.
}


void *
MleMemoryManager::platformAllocate(void * /* cookie */, MlUInt size)
{
//#undef mlMalloc
    return mlMalloc(size);
}


void *
MleMemoryManager::platformResize(void * /* cookie */, void *memory, MlUInt newSize)
{
//#undef mlRealloc
    return mlRealloc(memory, newSize);
}


MlResult
MleMemoryManager::platformRelease(void * /* cookie */, void *memory)
{
//#undef mlFree
    mlFree(memory);

    return MLE_S_OK;
}
