/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlArray.h
 * @ingroup MleCore
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Wizzer Works
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

#ifndef __MLE_ARRAY_H_
#define __MLE_ARRAY_H_


// Include standard header files.
#if ! defined(__linux__)
#include <new.h> // So we can call new on a preallocated buffer.
#ifdef WIN32
#include <memory.h>
#endif /* WIN32 */
#else
#include <string.h>
#include <new>
#endif
#include <ostream>

// Include Magic Lantern header files.
#include <mle/mlMalloc.h>

//using namespace std;

/**
 * @brief MleArray is a template for managing an array of objects.
 */
template <class T> class MleArray
{

  // Declare member variables.

  private:
    
	// The number of elements in the array.
    int m_numElements;
    // The array of elements.
    T * m_data;
    

  // Declare member functions.

  public:
    
	/**
	 * @brief Default constructor.
	 *
	 * The array is initialized to be empty.
     */
    MleArray()
    {
        m_numElements = 0;
        m_data = NULL;
    }
    

	/**
	 * @brief A constructor that is used to specify the initial number of elements
	 * in the array.
	 *
	 * @param num The number of elements to reserve for in the array.
	 */
    MleArray(int num)
    {
        m_numElements = num;
        m_data = new T[num];
    }

    
	/**
	 * @brief A constructor that is used to specify the initial contents
	 * of the array.
	 *
	 * @param num The number of elements in the array.
	 * @param ptr A pointer to the elements to place in the array.
	 */
    MleArray(int num, T * ptr)
    {
        m_numElements = num;
        m_data = ptr;
    }
    

	/**
	 * @brief The copy constructor.
	 *
	 * @param a A reference to another array of elements from which to
	 * copy.
	 */
#if 0
    MleArray(MleArray& a)
    {
        m_numElements = a.m_numElements;
        m_data = new T[m_numElements];
        memcpy ( m_data, a.m_data, m_numElements * sizeof(T) );
    }
#endif

	/**
	 * @brief The copy constructor.
	 *
	 * @param a A reference to another array of elements from which to
	 * copy.
	 */
    MleArray(const MleArray& a)
    {
        m_numElements = a.m_numElements;
        m_data = new T[m_numElements];
        memcpy ( m_data, a.m_data, m_numElements * sizeof(T) );
    }


	/**
	 * @brief Destructor.
	 */
    ~MleArray()
    {
    	if (m_data != NULL) delete m_data;
    }

	
	/**
	 * @brief Index operator.
	 *
	 * @return The element located at the specifed location in the
	 * array is returned.
	 */
    T& operator[] (int i)
    {
        if (i > size()) {
            resize( i+1 );
        }
        return ( ((T *)m_data)[i] );
    }
    

	/**
	 * @brief Index operator.
	 *
	 * @return The element located at the specified offset in the
	 * array is returned.
	 */
    T* operator+ (int i)
    {
        return ( ((T *)m_data)+i );
    }

    
    /**
     * @brief Redirection operator.
     *
     * @return The contents of the array are redirected to the output stream.
     */
    friend std::ostream& operator<< (std::ostream& os, const MleArray<T>& a)
    {
        a.print(os);
        return os;
    }


	/**
	 * @brief Get the size of the array.
	 *
	 * @return The number of elements in the array is returned.
	 */
    int size() const
    {
        return(m_numElements);
    }


	/**
	 * @brief Resize the array.
	 *
	 * @param num The new size of the array.
	 *
	 * @return A pointer to the resized array is returned.
	 */
    T* resize(const int num)
    {
        int oldSize = size();

        // Make the data larger, without calling constructor.
        m_data = (T *) mlRealloc( m_data, num * sizeof(T) );
        m_numElements = (NULL == m_data) ? 0 : num;
        if (0 == m_numElements) {
            return NULL;
        }

        // Run constructor on data[oldSize...num-1].
        new (&m_data[oldSize]) T[m_numElements-oldSize];

        return(m_data);
    }

    /**
     * @brief Print the array contents to the output stream.
     *
     * @param os A reference to the stream to redirect the output to.
     */
    void print(std::ostream& os) const
    {
        for (int i = 0 ; i < m_numElements; i++)
        {
            os << ((T *)m_data)[i];
            os << " ";
        }
    }
};

#endif /* __MLE_ARRAY_H_ */
