/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlDsoLoader.cpp
 * @ingroup MleCore
 *
 * This file contains the implementation of the Magic Lantern
 * DSO Loader class.
 *
 * @author Mark S. Millard
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


// Include system header files.
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#if defined(__linux__)
#include <dlfcn.h>
#endif /* __linux__ */
#if defined(WIN32)
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "psapi.h"
#endif /* WIN32 */

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/mlMalloc.h"
#include "mle/mlExpandFilename.h"
#include "mle/MleDsoLoader.h"


// The default path for searching for DSOs.
static char *dso_path[] = {
	(char *)"",
	(char *)"./",
#if defined(__linux__)
	(char *)"$MLE_ROOT/lib/",
#endif /* __linux__ */
#if defined(WIN32)
	(char *)"C:/Program Files/WizzerWorks/MagicLantern/lib/",
#endif /* WIN32 */
	NULL
};


// Declare the static head of the DSO list.
MleDSOEntry *MleDSOLoader::m_dsoList = NULL;


void (*MleDSOLoader::findInitClass(const char *classname,void *handle,const char *prefix))(void)
{
	MLE_ASSERT(classname);

	char dso_func[256];
	void (*initClass)(void);	// initClass function pointer

	initClass = NULL;

	// Assemble the initClass entry name.

	// Try with prefix first.
	if ( prefix )
	{
#if defined(__linux__)
		sprintf(dso_func,"_ZN%zu%s%s9initClassEv",
			(strlen(classname) + strlen(prefix)),prefix,classname);
		initClass = (void (*)(void))dlsym(handle,dso_func);
#endif /* __linux__ */
#if defined(WIN32)
		typedef VOID (*INITCLASSFUNC)(VOID); 

		INITCLASSFUNC procAddress;
		sprintf(dso_func,"?initClass@%s%s@@SAXXZ",prefix,classname);
		procAddress = (INITCLASSFUNC) GetProcAddress((HINSTANCE)handle, dso_func);

		initClass = procAddress;
#endif /* WIN32 */
	}

	// If that doesn't work, try without the prefix.
	if ( initClass == NULL )
	{
#if defined(__linux__)
		sprintf(dso_func,"_ZN%zu%s9initClassEv",
			strlen(classname),classname);
		initClass = (void (*)(void))dlsym(handle,dso_func);
#endif /* __linux__ */
#if defined(WIN32)
		typedef VOID (*INITCLASSFUNC)(VOID); 

		INITCLASSFUNC procAddress;
		sprintf(dso_func,"?initClass@%s@@SAXXZ",classname);
		procAddress = (INITCLASSFUNC) GetProcAddress((HINSTANCE)handle, dso_func);

		initClass = procAddress;
#endif /* WIN32 */
	}

	// Return the function pointer.
	return initClass;
}


void (*MleDSOLoader::findInitModule(void *handle))(void *)
{
	MLE_ASSERT(handle);

	char dso_func[256];
	void (*initModule)(void *);	// initModule function pointer

	initModule = NULL;

	// Assemble the initModule entry name.

#if defined(__linux__)
    // Todo - need to fix for Unix platform.
#if 0
	char modulename[10];
	modulename[0] = NULL;
	sprintf(dso_func,"initModule__%d%sSFv",
		strlen(modulename),modulename);
	initModule = (void (*)(void))dlsym(handle,dso_func);
#endif /* 0 */
#endif /* __linux__ */
#if defined(WIN32)
	typedef VOID (*INITMODULEFUNC)(VOID *); 

	INITMODULEFUNC procAddress;
	sprintf(dso_func,"?initModule@@YAXPAX@Z");
	procAddress = (INITMODULEFUNC) GetProcAddress((HINSTANCE)handle, dso_func);

	initModule = procAddress;
#endif /* WIN32 */

	// Return the function pointer.
	return initModule;
}

#if defined(WIN32)
#if _MSC_VER >= 1300    // for VC 7.0
  // from ATL 7.0 sources
  #ifndef _delayimp_h
  extern "C" IMAGE_DOS_HEADER __ImageBase;
  #endif
#endif

HMODULE GetCurrentModule()
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

  // Here's a trick that will get you the handle of the module
  // you're running in without any a-priori knowledge:
  // http://www.dotnet247.com/247reference/msgs/13/65259.aspx

  MEMORY_BASIC_INFORMATION mbi;
  static int dummy;
  VirtualQuery( &dummy, &mbi, sizeof(mbi) );

  return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else    // VC 7.0

  // from ATL 7.0 sources

  return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}

void PrintModules(DWORD processID)
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    // Print the process identifier.
    printf("\nProcess ID: %u\n", processID);

    // Get a list of all the modules in this process.
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                           FALSE, processID);
    if (NULL == hProcess)
        return;

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            char szModName[MAX_PATH];

            // Get the full path to the module's file.
            if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
                                    sizeof(szModName)))
            {
                // Print the module name and handle value.
                printf("\t%s (0x%08X)\n", szModName, hMods[i]);
            }
        }
    }

    CloseHandle(hProcess);
}

int GetProcessModules(HMODULE *hMods, unsigned int size)
{
    HANDLE hProcess;
    DWORD cbNeeded = 0;
	int processID;

	// Get the current process id.
	processID = _getpid();

	// Open the process.
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                           FALSE, processID);
    if (NULL == hProcess)
        return 0;

	// Retrieve the modules.
    EnumProcessModules(hProcess, hMods, size, &cbNeeded);

	// Close the process.
    CloseHandle(hProcess);

	return cbNeeded;
}
#endif /* WIN32 */


MleDSOLoader::MleDSOLoader(void)
{
	// Initialize variables.
	m_dsoList = NULL;
	m_initData = NULL;

	// Start the path with the static path defined here.
	m_pathlist = dso_path;
}


MleDSOLoader::MleDSOLoader(void *initData)
{
	// Initialize variables.
	m_dsoList = NULL;
	m_initData = initData;

	// Start the path with the static path defined here.
	m_pathlist = dso_path;
}


MleDSOLoader::~MleDSOLoader()
{
	// Do nothing extra.
}


void
MleDSOLoader::setSearchList(char **list)
{
	if ( list )
		m_pathlist = list;
	else
		m_pathlist = dso_path;
}


void *
MleDSOLoader::loadFile(const char *filename,char **pathlist)
{
	MLE_ASSERT(filename);

	// Check if this file has been previously loaded.
	MleDSOEntry *dsoEntry = m_dsoList;
	while ( dsoEntry )
	{
		// If we find a matching DSO file, just return the handle.
		if ( !strcmp(dsoEntry->filename,filename) )
			return dsoEntry->handle;

		dsoEntry = dsoEntry->next;
	}

	// If no pathlist specified, use the default.
	if ( pathlist == NULL )
		pathlist = dso_path;

	void *handle = NULL; // DSO handle
	int ipath = 0;       // path index
	char dso_file[256];  // DSO filename

	// Look in a whole bunch of paths for the DSO file.
	while ( handle == NULL && pathlist[ipath] )
	{
		// Construct the DSO filename, type.so
		sprintf(dso_file,"%s%s",pathlist[ipath],filename);

		// Expand environment variables, etc.
		char *expand = mlFilenameExpand(dso_file);

		// Open the DSO.
#ifdef WIN32
		handle = LoadLibrary(expand);
#endif /* WIN32 */
#if defined(__linux__)
		handle = dlopen(expand,RTLD_LAZY);
#endif /* __linux__ */

		// Release the expanded filename.
		mlFree(expand);

		// Check for errors.
		if ( handle == NULL )
		{
#if defined(__linux__)
			char *buf = dlerror();
			if ( strstr(buf,"map soname") == NULL ||
                 getenv("MLE_DSO_VERBOSE") ) {
                 printf("MleDsoLoader: %s.\n",buf);
                 fflush(stdout);
            }
#endif /* __linux__ */
		}

		ipath++;
	}

	if ( handle )
	{
		// Add this DSO to the list.
		MleDSOEntry *dsoEntry = new MleDSOEntry;
		dsoEntry->filename =
			strcpy(new char[strlen(filename) + 1],filename);
		dsoEntry->handle = handle;
		dsoEntry->next = m_dsoList;
		m_dsoList = dsoEntry;

		if ( getenv("MLE_DSO_VERBOSE") )
			printf("MleDsoLoader: loaded DSO %s.\n",filename);
	}

	return handle;
}


int
MleDSOLoader::loadClass(const char *classname,const char *prefix)
{
	MLE_ASSERT(classname);

	void *handle = NULL;	 // DSO handle.
	void (*initClass)(void);

	// First try looking in previously loaded DSOs.
	MleDSOEntry *dsoEntry = m_dsoList;
	while ( dsoEntry )
	{
		initClass = findInitClass(classname,dsoEntry->handle,prefix);
		if ( initClass )
		{
			// Found it.
			if ( getenv("MLE_DSO_VERBOSE") )
				printf("%s class loaded from previously loaded DSO %s.\n",classname,dsoEntry->filename);

			// Call the initClass().
			(*initClass)();

			return 0;
		}

		dsoEntry = dsoEntry->next;
	}

#if defined(WIN32)
	// Next, attempt to find the class in all known modules.
	HMODULE hMods[1024];
	int cbNeeded = GetProcessModules(hMods, sizeof(hMods));
	for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
    {
		//PrintModules(_getpid());

        initClass = findInitClass(classname,hMods[i],prefix);
		if (initClass != NULL)
		{
			// Call the initClass().
		    (*initClass)();

		    return 0;
		}
	}
#endif /* WIN32 */

	// Else attempt to load a file base on the class name.
	char dso_file[256];
#if defined(__linux__)
	sprintf(dso_file,"%s.so",classname);
#endif /* __linux__ */
#if defined(WIN32)
#ifdef MLE_DEBUG
	sprintf(dso_file,"%sd.dll",classname);
#else
	sprintf(dso_file,"%s.dll",classname);
#endif /* ! MLE_DEBUG */
#endif /* WIN32 */
	handle = loadFile(dso_file,m_pathlist);

#if defined(__linux__)
    if (handle == NULL) {
        // Try to load with Unix 'lib' prefix.
        sprintf(dso_file,"lib%s.so",classname);
        handle = loadFile(dso_file,m_pathlist);
    }
#endif /* __linux__ */

	// If we haven't found it yet, hope it's in the current image.
	if ( handle == NULL )
#if defined(WIN32)
	    // Attempt to use the current module.
	    handle = GetCurrentModule();
#endif /* WIN32 */
#if defined(__linux__)
	    handle = dlopen(NULL,RTLD_NOW);
#endif /* __linux__ */

	// Initialize the DSO.
	if ((handle != NULL) && (m_initData != NULL))
	{
		// Find and run the initModule().
        void (*initModule)(void *);
		initModule = findInitModule(handle);
		if (initModule != NULL)
			(* initModule)(m_initData);
	}

	// Find and run the initClass().
	if (handle != NULL)
	{
		initClass = findInitClass(classname,handle,prefix);
		if ( initClass == NULL )
			return 1;

		(*initClass)();
	} else
		return 1;

	return 0;
}
