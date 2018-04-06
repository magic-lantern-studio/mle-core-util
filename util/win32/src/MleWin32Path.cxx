/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 *  @file MleWin32Path.cxx
 *  @ingroup MleCore
 *
 *  This file implemtns the Win32 interface of the cross platform path abstraction
 *  for file names and Uniform Resource Locators (URLs)
 *  It was ported from the Auteur Portability Toolkit from Wizzer Works.
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

// Include Windows header filesn
#include <windows.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/MleWin32Path.h"
#include "mle/MleMemoryManager.h"

// MlePath implementation.

MlePath *
MlePath::create()
{
    return new MleWin32Path();
}

MlePath *
MlePath::create(const MlChar *path)
{
    // Create a new path based on the cross platform path
    return new MleWin32Path(path, false);
}


static MleWin32Path g_globalSystemPath;


const MlePath &	
MlePath::getSystemPath()
{
    if ('\0' == g_globalSystemPath.getPath()[0])
	{
	    // g_globalSystemPath is empty, so lets try to
	    // fetch it.
	    MlChar buffer[MAX_PATH];
	    UINT size;
    
	    size = GetSystemDirectory((char *)buffer, MAX_PATH);
	    g_globalSystemPath.setPlatformPath(buffer);
    }

    return g_globalSystemPath;
}

// MleWin32Path implementation.

MleWin32Path::MleWin32Path()
{
    MleMemoryManager::getManager()->allocate((void**)&m_Path, 1);
    MleMemoryManager::getManager()->allocate((void**)&m_PlatformPath, 1);
    MleMemoryManager::getManager()->allocate((void**)&m_URL, 1);
    *m_Path = '\0';
    *m_PlatformPath = '\0';
    *m_URL = '\0';
}

MleWin32Path::MleWin32Path(const MlChar *path, bool isPlatformPath) 
{
    MLE_ASSERT(path != NULL);

    MleMemoryManager::getManager()->allocate((void**)&m_Path, 1);
    MleMemoryManager::getManager()->allocate((void**)&m_PlatformPath, 1);
    MleMemoryManager::getManager()->allocate((void**)&m_URL, 1);
    *m_Path = '\0';
    *m_PlatformPath = '\0';
    *m_URL = '\0';

    if (isPlatformPath)
	{
	    setPlatformPath(path);
    }
    else
	{
	    setPath(path);
    }

}

MleWin32Path::~MleWin32Path()
{
    MleMemoryManager::getManager()->release((void**)&m_Path);
    MleMemoryManager::getManager()->release((void**)&m_PlatformPath);
    MleMemoryManager::getManager()->release((void**)&m_URL);
}


MlResult	
MleWin32Path::setPlatformPath(const MlChar *platformPath)
{
    MlBoolean done = FALSE;
    MlUInt	pos = 0, newPos = 0;
    MlChar	newPath[MAX_PATH];
    MlChar	firstLetter;

    enum State {
	    START,
	    FIRSTLETTER,
	    DRIVE,
	    PATHCOMPONENT,
	    END,
	    FAULT
    } state = START;

    while (!done)
	{
	    MlChar next = platformPath[pos];
	    switch(state)
		{
	      case START:
	        if (isalpha(next))
			{
		        firstLetter = next;
		        state = FIRSTLETTER;
		        pos++;
			}
	        else
			{
		        state = PATHCOMPONENT;
		        // Don't increment pos.
			}
	        break;
	      case FIRSTLETTER:
	        if (':' == next)
			{
		        newPath[newPos++] = '/';
		        newPath[newPos++] = '/';
		        newPath[newPos++] = firstLetter;
		        state = DRIVE;
		        pos++;
			}
	        else
			{
		        newPath[newPos++] = firstLetter;
		        state = PATHCOMPONENT;
		        //pos++;
				// Don't increment pos.
			}
	        break;
	      case DRIVE:
	        if ('\\' == next)
			{
		        newPath[newPos++] = '/';
		        state = PATHCOMPONENT;
		        pos++;
			}
	        else
			{
		        state = FAULT;
			}
	        break;
	      case PATHCOMPONENT:
	        switch(next)
			{
	          case '\0':
	         	state = END;
		        break;
	          case '\\':
		        newPath[newPos++] = '/';
		        pos++;
		        break;
	          default:
		        newPath[newPos++] = next;
		        pos++;
		        break;
			}
	        break;
	      case END:
	        newPath[newPos++] = '\0';
	        done = TRUE;
	        break;
    	  case FAULT:
	        return MLE_E_UTILLIB_BADFORMAT;
	        break;
	      default:
	        MLE_ASSERT(FALSE);
		}
    }

    // We made it through the state machine, so the 
    // platform specific path was parsed.

    MleMemoryManager::getManager()->resize((void **)&m_Path, strlen((char *)newPath) + 1);
    strcpy((char *)m_Path, (char *)newPath);
    MleMemoryManager::getManager()->resize((void **)&m_PlatformPath, strlen((char *)platformPath) + 1);
    strcpy((char *)m_PlatformPath, (char *)platformPath);

    return MLE_S_OK;
}


const MlChar *
MleWin32Path::getPlatformPath() const
{
    return m_PlatformPath;
}


MlResult	
MleWin32Path::setPath(const MlChar *path)
{
    MlBoolean done = FALSE;
    MlUInt	pos = 0, newPos = 0;
    MlChar	newPath[MAX_PATH];

    enum State {
	    START,
	    FIRSTSLASH,
	    SECONDSLASH,
    	FIRSTPATHSLASH,
	    PATHCOMPONENT,
	    END,
	    FAULT,
    } state = START;

    while (! done)
	{  
	    MlChar next = path[pos];
	    switch (state)
		{
	      case START:
	        switch(next)
			{
	          case '/':
		        state = FIRSTSLASH;
		        pos++;
		        break;
	          default:
		        state = PATHCOMPONENT;
		        // don't increment pos
		        break;
			}
	        break;
	      case FIRSTSLASH:
	        switch (next)
			{
	          case '/':
		        state = SECONDSLASH;
		        pos++;
		        break;
	          default:
		        newPath[newPos++] = '\\';
		        state = PATHCOMPONENT;
		        // don't increment pos
		        break;
			}
	        break;
          case SECONDSLASH:
	        if (isalpha(next))
			{
		        newPath[newPos++] = next;
		        newPath[newPos++] = ':';
		        state = FIRSTPATHSLASH;
		        pos++;
			}
	        else
			{
		        state = FAULT;
			}
	        break;
	      case FIRSTPATHSLASH:
	        switch(next)
			{
	          case '/':
		        newPath[newPos++] = next;
		        state = PATHCOMPONENT;
		        pos++;
		        break;
	          default:
		        state = FAULT;
		        break;
			}
	        break;
	      case PATHCOMPONENT:
	        switch(next)
			{
	          case '\0':
		        state = END;
		        break;
	          case '/':
		        newPath[newPos++] = '\\';
		        pos++;
		        break;
	          default:
		        newPath[newPos++] = next;
		        pos++;
		        break;
			}
	        break;
	      case END:
	        newPath[newPos++] = '\0';
	        done = TRUE;
	        break;
	      case FAULT:
	        return MLE_E_UTILLIB_BADFORMAT;
	        break;
	      default:
	        MLE_ASSERT(FALSE);
		}
    }

    // We made it through the state machine, so the 
    // cross platform path was parsed.

    MleMemoryManager::getManager()->resize((void**) &m_Path, strlen((char *)path) + 1);
    strcpy((char *)m_Path, (char *)path);
    MleMemoryManager::getManager()->resize((void**) &m_PlatformPath, strlen((char *)newPath) + 1);
    strcpy((char *)m_PlatformPath, (char *)newPath);

    return MLE_S_OK;
}


const MlChar *    
MleWin32Path::getPath() const
{
    return m_Path;
}


const MlChar *
MleWin32Path::getURL() const
{
    return m_URL;
}


const MlChar *    
MleWin32Path::getFilename() const
{
    MlChar *filename = NULL;

    if (m_Path != NULL && (filename = (MlChar *)strrchr((char *)m_Path, '/')) != NULL)
	{
	    return ++filename;
    }
    else
	{
	    return m_Path; // May be NULL, but definitely has no '/'s in it.
    }
}


MlBoolean
MleWin32Path::isAbsolutePath()
{
	MlBoolean isAbsolute = FALSE;

	if (m_Path != NULL)
	{
		if (*m_Path == '/')
			isAbsolute = TRUE;
	}

	return isAbsolute;
}
