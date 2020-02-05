/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 *  @file MleMemoryManager.h
 *  @ingroup MleCore
 *
 *  @author Mark S. Millard
 *  @date September 19, 2015
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

#ifndef __MLE_MEMORY_MANAGER_H_
#define __MLE_MEMORY_MANAGER_H_


// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/mlMalloc.h"
#include "mle/mlErrno.h"
#include "mle/mlAssert.h"


/**
 * @ingroup MleCore
 * @brief MleMemoryManager is used to provide an abstraction to memory management.
 */
class MleMemoryManager
{

	public:

#ifndef PLATFORM_MACOS
		/**
		 * Custom new operator.
		 */
		void * operator new(size_t s);

		/**
		 * Custom delete operator.
		 */
		void operator delete(void *ptr);
#endif /* PLATFORM_MACOS */

		/**
		 * Constructor.
		 */
		MleMemoryManager();

		/**
		 * Destructor.
		 */
		virtual ~MleMemoryManager();

		/**
		 * Get the memory manager.
		 *
		 * @return A pointer to the singleton manager is returned that is shared application wide.
		 */
		static  MleMemoryManager * getManager();

		/**
		 * Allocates a chunk of memory.
		 * 
		 * @param memory A pointer to a pointer to the newly allocated memory.  Input 
		 * value is unimportant, but will be unchanged if allocation fails.
		 * @param size The amount of new memory to allocate.  Memory manager can allocate
		 * more than requested amount and still return successfully.
		 *
		 * @return A MlResult is returned.
		 * <ul>
		 *	    <li><b>MLE_S_OK</b> is returned if memory is successfully allocated.  
		 *	    <li><b>MLE_E_FAIL</b> is returned otherwise.
		 * </ul>
		 */
		virtual MlResult allocate(void **memory, uint_t size);

		/**
		 * Resizes a chunk of memory.
		 *
		 * @param memory A pointer to a pointer to the allocated memory.  Will be 
		 * unchanged if resize fails.  May change if resize succeeds.
		 * @param newSize The size of new memory chunk.  Memory manager can resize chunk to more
		 * than the requested amount and still return successfully, though
		 * the size will be no larger than the original size of the chunk.
		 *
		 * @return A MlResult is returned.
		 * <ul>
		 *     <li><b>MLE_S_OK</b> is returned if memory is successfully resized. 
		 *	   <li><b>MLE_E_FAIL</b> is returned otherwise.
		 * </ul>
		 */
		virtual MlResult resize(void **memory, uint_t newSize);

		/**
		 * Releases an allocated chunk of memory.
		 *
		 * @param memory A pointer to a pointer to the allocated memory.  Will be 
		 * unchanged if release fails.  Will be set to NULL if release 
		 * succeeds.
		 *
		 * @return A MlResult is returned.
		 * <ul>
		 *     <li><b>MLE_S_OK</b> is returned if the memory is successfully released. 
		 *	   <li><b>MLE_E_FAIL</b> is returned otherwise.
		 * </ul>
		 */
		virtual MlResult release(void **memory);

		/**
		 * Duplicates a string.
		 *
		 * @param source The string to duplicate.
		 * @param destination The duplicate string is returned.
		 *
		 * @return A MlResult is returned.
		 * <ul>
		 *	   <li><b>MLe_S_OK</b> is returned if the string is copied.
		 *	   <li><b>MLE_E_FAIL</b> is returned if it cannot be copied.
		 * </ul>
		 */
		virtual MlResult dupString(const MlChar *source, MlChar **destination);
    
	private:

		//
		// Platform interfaces
		//

		//
		// The global memory manager singleton.
		//
		static MleMemoryManager * g_GlobalManager;
    
		//
		// Initialize/cleanup cookie containing platform specific info.
		//
		static void platformInit(void *&cookie);
		static void platformCleanup(void *&cookie);

		//
		// Platform implementations of allocate, resize and release that
		// return pointers to new memory (much like malloc and realloc)
		// or MLE_S_OK on success.  Return NULL or MLE_E_FAIL on error.
		//
		static void * platformAllocate(void *cookie, uint_t size);
		static void * platformResize(void *cookie, void *memory, uint_t newSize);
		static MlResult platformRelease(void *cookie, void *memory);

		//
		// Platform specific cookie.
		//
		void * m_Cookie;
};


/**
 * @ingroup MleCore
 * @brief MleBlockMemoryManager is a template for block based memory manager.
 *
 * @param blockSize Provides a hint to the memory manager that memory need only be
 * allocated in chunks that are multiples of blockSize.  Manager
 * can use this hint to set up a more efficient private heap for
 * memory management.
 */
template <uint_t blockSize>
	class MleBlockMemoryManager : public MleMemoryManager
{

	public:

		/**
		 * Constructor.
		 *
		 * @param heapSize The size of initial heap for block allocations
		 */
		MleBlockMemoryManager(uint_t heapSize)
		{
		    MleMemoryManager::allocate(m_FreeList, heapSize);

		    // Determine how many blocks will fit in heap.
		    int numBlocks = heapSize / blockSize;
    
		    // Initialize the chained pointers of free list.
		    for (MlInt i = 0; i < numBlocks - 1 ; i++)
			{
			    m_FreeList[i].next = &m_FreeList[i + 1];
			}
    
		    // Terminate the free list.
		    m_FreeList[numBlocks - 1].next = NULL;
		}
		
		/**
		 * Destructor.
		 */
		virtual ~MleBlockMemoryManager()
		{
		    MleMemoryManager::release(m_FreeList);
		}

		/**
		 * Gets the block size of this allocator
		 */
		uint_t getBlockSize() { return blockSize; }

		/**
		 * Allocates a chunk of memory.  See base class description.
		 *
		 * @param memory A pointer to a pointer to the successfully allocated memory is returned.
		 * If allocation is unsuccessful, then NULL will be returned.
		 * @param size The size of the memory chunk to allocate.
		 *
		 * @return A MleResult is returned.
		 * <ul>
		 *     <li><b>MLE_S_OK</b> is returned if the memory is successfully allocated.
		 *     <li><b>MLE_E_FAIL</b> is returned if the memory is not successully allocated.
		 *     NOTE: if size does not fit in blockSize, MLE_E_FAIL returned.
		 * </ul>
		 */ 
		virtual MlResult allocate(void **memory, uint_t size)
		{
	    	MLE_ASSERT(size > 0);

		    if (size == 0 || m_FreeList == NULL || size > blockSize)
			{
			    return MLE_E_FAIL;
			}
    
	    	memory = (void *) m_FreeList;
		    m_FreeList = m_FreeList->next;
    
		    return MLE_S_OK;    
		}

		/**
		 * Resizes a chunk of memory.  See base class description.
		 *
		 * @param memory A pointer to a pointer to the successfully resized memory is returned.
		 * If resizing is unsuccessful, then NULL will be returned.
		 *
		 * @param newize The size of the memory chunk to reallocate.
		 *
		 * @return A MlResult is returned.
		 * <ul>
		 *     <li><b>MLE_S_OK</b> is returned if the memory is successfully allocated.
		 *     <li><b>MLE_E_FAIL</b> is returned if the memory is not successully allocated.
		 *     NOTE: if newSize does not fit in blockSize, MLE_E_FAIL returned.
		 * </ul>
		 */
		virtual MlResult resize(void **memory, uint_t newSize)
		{
		    MLE_ASSERT(memory != NULL);
		    MLE_ASSERT(newSize > 0);

		    if (memory == NULL || newSize == 0 || newSize > blockSize)
			{
			    return MLE_E_FAIL;
			}

		    // Resizing is a NOP for block allocator
		    return MLE_S_OK;
		}

		/**
		 * Releases an allocated chunk of memory.  See base class description.
		 *
		 * @param memory A pointer to a pointer of memory to release.
		 *
		 * @return A MlResult is returned.
		 * <ul>
		 *     <li><b>MLE_S_OK</b> is returned if the memory is successfully released.
		 * </ul>
		 */
		virtual MlResult release(void **memory)
		{
		    MLE_ASSERT(memory != NULL);

		    Block * block = (Block *) memory;
		    block.next = m_FreeList;
		    m_FreeList = block;
    
		    memory = NULL;

		    return MLE_S_OK;
		}

	private:

		/*
		 * Block data structure definition.
		 */
		struct Block {
		    union {
			    Block *next;
			    MlChar data[blockSize];
			};
		};

		/*
		 * The free block list
		 */
		Block *	m_FreeList;
};


#ifdef DEBUG_MEMORY_FAILURE

inline void * operator new(MlInt s)
{
    void *data = NULL;
    MleMemoryManager::getManager()->allocate(&data, s);
    return data;
}

inline void operator delete(void *ptr)
{
    MleMemoryManager::getManager()->release(&ptr);
}


#define mlMalloc 	mlDebugMalloc
#define mlFree		mlDebugFree
#define mlRealloc	mlDebugRealloc


extern "C" {
    void *mlDebugMalloc(size_t size);
    void mlDebugFree(void *ptr);
    void *mlDebugRealloc(void *ptr, size_t size);
}


#endif /* DEBUG_MEMORY_FAILURE */

#endif /* __MLE_MEMORYMANAGER_H_ */
