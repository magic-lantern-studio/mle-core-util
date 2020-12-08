/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file MleDsoLoader.h
 * @ingroup MleCore
 *
 * This file contains a generic interface for loading dynamic
 * loadable modules such as DSOs and DLLs.
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

#ifndef __MLE_DSOLOADER_H_
#define __MLE_DSOLOADER_H_

// Include Magic Lantern header files.
#include "mle/mle.h"
#include "mle/mlTypes.h"
#include "mle/MleUtil.h"

/**
 * The DSO entry structure.
 */
struct MLE_UTIL_API MleDSOEntry
{
	char *filename;     /**< The name of the DSO. */
	void *handle;       /**< A handle to the loaded DSO. */
	MleDSOEntry *next;  /**< A pointer to the next DSO entry. */
};

/**
 * @brief This class is used to load modules such as DSOs on the Linux
 * platform and DLLs on the Windows platforms.
 */
class MLE_UTIL_API MleDSOLoader
{
  public:

    /**
	 * Default constructor.
	 */
    MleDSOLoader(void);

	/**
	 * A constructor that specifies initialization data.
	 *
	 * @param initData A pointer to initialization data to register with
	 * the DSO.
	 */
	MleDSOLoader(void *initData);

	/**
	 * The destructor.
	 */
    ~MleDSOLoader();
    
    /**
	 * @brief Directory management.
	 *
     * setSearchList() sets an array of directories to search for
     * DSOs for files without complete path specifications.  If
     * this is not called or NULL is passed, the internal default
     * path will be used.  The argument is an array of strings that
     * are directories (each should end with a '/'), terminated with
     * a NULL pointer.  The constructed paths are passed through
     * mleExpandFilename(), so environment variables and '~' notation
     * is allowed.
	 *
	 * @param A pointer to an array of directory paths.
	 */
    void setSearchList(char **pathlist);
    
    /**
	 * @brief Load a Magic Lantern class and initialize it.
	 *
     * loadClass() tries to find an initClass() method, loading
     * a new DSO if necessary, and runs it to initialize the new
     * class.  Nonzero is returned if this could not be done.  The
     * arguments are the classname and an optional system prefix.
	 *
	 * @param classname A pointer to a character string representing the
	 * name of the class to load.
	 * @param prefix A pointer to a character string identifying the
	 * system prefix.
	 */
    int loadClass(const char *classname,const char *prefix = NULL);

    
    /**
	 * @brief Load a dynamically loadable module.
	 *
     * loadFile() opens a new DSO and adds it to the list of currently
     * loaded DSOs.  Future calls to loadClass() will search through
     * previously opened DSOs before opening new ones.  It returns a
     * handle to the loaded DSO, or NULL if unsuccessful.
	 *
	 * @param filename A pointer to a character string representing the
	 * name of the DSO to load.
	 * @param pathlist A pointer to an array of directory paths to search.
	 *
	 * @return A handle to the loaded DSO is returned. NULL will be returned
	 * if the call is unsuccessful.
	 */
    static void *loadFile(const char *filename,char **pathlist = NULL);

  protected:

	/** Initialization data for the DSOs. */
	void *m_initData;

    /** Pointer to linked list of open DSOs. */
    static MleDSOEntry *m_dsoList;

  private:
    
    // Pointer to current directory priority array.
    char **m_pathlist;
    
    // Get the initClass function pointer.
	void (*findInitClass(const char *type,void *handle,const char *prefix))
		(void);

	// Get the initModule function pointer.
	void (*findInitModule(void *handle))(void *initData);

};


#endif /* __MLE_DSOLOADER_H_ */
