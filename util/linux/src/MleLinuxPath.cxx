/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 *  @file MleLinuxPath.cxx
 *  @ingroup MleCore
 *
 *  This file implements the Linux interface of the cross platform path abstraction
 *  for file names and Uniform Resource Locators (URLs)
 *  It was ported from the Auteur Portability Toolkit from Wizzer Works.
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

// Include system header files.
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/MleLinuxPath.h"
#include "mle/MleMemoryManager.h"

// The following value is derived from Windows platform. On Linux this may be limiting.
#define MAX_PATH 0x00000104

// MlePath implementation.

MlePath *
MlePath::create()
{
    return new MleLinuxPath();
}

MlePath *
MlePath::create(const MlChar *path)
{
    // Create a new path based on the cross platform path
    return new MleLinuxPath(path, false);
}


#if 0
static MleLinuxPath g_globalSystemPath;


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
#endif /* 0 */

// MleLinuxPath implementation.

MleLinuxPath::MleLinuxPath()
{
    MleMemoryManager::getManager()->allocate((void**)&m_Path, 1);
    MleMemoryManager::getManager()->allocate((void**)&m_PlatformPath, 1);
    MleMemoryManager::getManager()->allocate((void**)&m_URL, 1);
    *m_Path = '\0';
    *m_PlatformPath = '\0';
    *m_URL = '\0';
}

MleLinuxPath::MleLinuxPath(const MlChar *path, bool isPlatformPath) 
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

MleLinuxPath::~MleLinuxPath()
{
    MleMemoryManager::getManager()->release((void**)&m_Path);
    MleMemoryManager::getManager()->release((void**)&m_PlatformPath);
    MleMemoryManager::getManager()->release((void**)&m_URL);
}


MlResult    
MleLinuxPath::setPlatformPath(const MlChar *platformPath)
{
    MlBoolean done = FALSE;
    MlUInt    pos = 0;
    MlChar    newPath[MAX_PATH];

    while (! done)
    {
        MlChar next = platformPath[pos];
        newPath[pos] = next;
        if (next == '\0') done = TRUE;
        else pos++;
    }

    MleMemoryManager::getManager()->resize((void **)&m_Path, strlen((char *)newPath) + 1);
    strcpy((char *)m_Path, (char *)newPath);
    MleMemoryManager::getManager()->resize((void **)&m_PlatformPath, strlen((char *)platformPath) + 1);
    strcpy((char *)m_PlatformPath, (char *)platformPath);

    return MLE_S_OK;
}


const MlChar *
MleLinuxPath::getPlatformPath() const
{
    return m_PlatformPath;
}


MlResult    
MleLinuxPath::setPath(const MlChar *path)
{
    MlBoolean done = FALSE;
    MlUInt    pos = 0;
    MlChar    newPath[MAX_PATH];

    while (! done)
    {  
        MlChar next = path[pos];
        newPath[pos] = next;
        if (next == '\0') done = TRUE;
        else pos++;
    }

    MleMemoryManager::getManager()->resize((void**) &m_Path, strlen((char *)path) + 1);
    strcpy((char *)m_Path, (char *)path);
    MleMemoryManager::getManager()->resize((void**) &m_PlatformPath, strlen((char *)newPath) + 1);
    strcpy((char *)m_PlatformPath, (char *)newPath);

    return MLE_S_OK;
}


const MlChar *    
MleLinuxPath::getPath() const
{
    return m_Path;
}


const MlChar *
MleLinuxPath::getURL() const
{
    return m_URL;
}


const MlChar *    
MleLinuxPath::getFilename() const
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
MleLinuxPath::isAbsolutePath()
{
    MlBoolean isAbsolute = FALSE;

    if (m_Path != NULL)
    {
        if (*m_Path == '/')
            isAbsolute = TRUE;
    }

    return isAbsolute;
}
