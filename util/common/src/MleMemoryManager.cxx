/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 *  @file MleMemoryManager.cxx
 *  @ingroup MleCore
 *
 *  Implementation of memory manager abstraction.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


// Include system header files.
#include <string.h>
#include <stdlib.h>
#ifdef _WINDOWS
#include <malloc.h>
#endif

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/MleMemoryManager.h"

#define MLE_MAX_ALLOCATION "MleMaxAllocation"


MleMemoryManager *MleMemoryManager::g_GlobalManager = NULL;
static uint_t g_globalAllocationNumber = 0;
static uint_t g_globalMaxAllowedAllocation = 0;


MleMemoryManager *
MleMemoryManager::getManager()
{
	if (g_GlobalManager == NULL)
		g_GlobalManager = new MleMemoryManager();
    return g_GlobalManager;
}


#ifndef PLATFORM_MACOS
void *
MleMemoryManager::operator new(size_t s)
{
//#undef malloc
    return mlMalloc(s);
}


void
MleMemoryManager::operator delete(void *ptr) 
{
//#undef free
    mlFree(ptr);
}

#endif /* PLATFORM_MACOS */


extern "C" {
	void *mlDebugMalloc(size_t);
    void mlDebugFree(void *ptr);
    void *mlDebugRealloc(void *ptr, size_t size);
    
    void *mlDebugMalloc(size_t size)
	{
	    void *data;
	    MleMemoryManager::getManager()->allocate(&data, size);
	    return data;
    }

    void mlDebugFree(void *ptr)
	{
	    MleMemoryManager::getManager()->release(&ptr);
    }

    void *mlDebugRealloc(void *ptr, size_t size)
	{
	    //if (MleMemoryManager::getManager()->resize(&ptr, size) != NULL)
	    if (MleMemoryManager::getManager()->resize(&ptr, size) != MLE_E_FAIL)
		{
	        return ptr;
		}
	    else {
	        return NULL;
		}
    }
} /* "C" */


MleMemoryManager::MleMemoryManager()
{
    if (g_globalMaxAllowedAllocation == 0)
	{
	    char *maxStr = getenv(MLE_MAX_ALLOCATION);
	    g_globalMaxAllowedAllocation = maxStr ? atoi(maxStr) : 0;
    }
    platformInit(m_Cookie);
}


MleMemoryManager::~MleMemoryManager()
{
    platformCleanup(m_Cookie);
}


MlResult  
MleMemoryManager::allocate(void **memory, uint_t size)
{
    MLE_ASSERT(size > 0);
    if (size == 0)
	{
	    return MLE_E_FAIL;
    }

    if (g_globalMaxAllowedAllocation &&
        ++g_globalAllocationNumber > g_globalMaxAllowedAllocation)
	{
	    return MLE_E_FAIL;
    }
    
    void *newMemory = platformAllocate(m_Cookie, size);

    if (newMemory == NULL)
	{
	    return MLE_E_FAIL;
    }
    
    *memory = newMemory;
    
    return MLE_S_OK;
}


MlResult  
MleMemoryManager::resize(void **memory, uint_t newSize)
{
    MLE_ASSERT(newSize > 0);
    MLE_ASSERT(memory != NULL);
    if (newSize == 0 || memory == NULL)
	{
	    return MLE_E_FAIL;
    }

    if (g_globalMaxAllowedAllocation &&
        ++g_globalAllocationNumber > g_globalMaxAllowedAllocation)
	{
	    return MLE_E_FAIL;
    }

    void *newMemory = platformResize(m_Cookie, *memory, newSize);
    
    if (newMemory == NULL)
	{
	    return MLE_E_FAIL;
    }
    
    *memory = newMemory;
    
    return MLE_S_OK;
}


MlResult	
MleMemoryManager::release(void **memory)
{
    MLE_ASSERT(memory != NULL);
    if (memory == NULL)
	{
	    return MLE_E_FAIL;
    }

    if (platformRelease(m_Cookie, *memory) == MLE_E_FAIL)
	{
	    return MLE_E_FAIL;
    }
    
    *memory = NULL;
    
    return MLE_S_OK;
}


MlResult
MleMemoryManager::dupString(const MlChar *source, MlChar **destination)
{
    MLE_ASSERT(source != NULL);
    
    if (MLE_S_OK == allocate((void **) destination, strlen((char *)source) + 1))
	{
	    strcpy((char *)*destination, (char *)source);
	    return MLE_S_OK;
    }
    else
	{
	    return MLE_E_FAIL;
    }
}
