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
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
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
