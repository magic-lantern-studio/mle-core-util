/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 *  @file MleLinuxPath.h
 *  @ingroup MleCore
 *
 *  This file defines the Linux interface of the cross platform path abstraction
 *  for file names and Uniform Resource Locators (URLs)
 *  It was ported from the Auteur Portability Toolkit from Wizzer Works.
 *
 *  @author Mark S. Millard
 *  @date September 19, 2015
 *  @version 0.1
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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

#ifndef __MLE_LINUXPATH_H__
#define __MLE_LINUXPATH_H__

// Include Magic Lantern header files.
#include "mle/MlePath.h"


/**
 * @ingroup MleCore
 * @brief MleLinuxPath is a Linux implementation of a the abstract class, MlePath.
 */
class MleLinuxPath : public MlePath
{

    public:

        /**
         * Default constructor.
         */
        MleLinuxPath();

        /**
         * Constructor.
         *
         * @param path The path to initialize the object with.
         * @param isPlatformPath Whether the path argument represents
         * a platform specific or cross platform path.
         */
        MleLinuxPath(const MlChar *path, bool isPlatformPath = true);

        /**
         * Destructor.
         */
        ~MleLinuxPath();

        //
        // Base class methods
        //

        /**
         * Set path in platform specific format.
         *
         * @param platformPath The path in platform specific format 
         * 
         * @return An AptResult is returned.
         *        MLE_S_OK is returned if the path is successfully set.  
         *        MLE_E_UTILLIB_BADFORMAT is returned if the platformPath parameter can't
         *        be parsed for this platform.
         */
        virtual MlResult setPlatformPath(const MlChar *platformPath);

        /**
         * Get path in platform specific format
         *
         * @return A constant character string is returned.
         * The path in platform specific format is returned.  This is a 
         * pointer to internal data structures, and so should
         * not be changed or held on to for long.
         */
        virtual const MlChar * getPlatformPath() const;

        /**
         * Set path in cross platform format.  Cross platform format
         * is similar to unix style format, with the following modifications
         *
         * All absolute paths start with at least one '/'.  On Win32, other 
         * drives are reffered to via ' //x/' where x is the drive letter.  
         * On the Mac, volumes are referenced via ' //volume/'.  
         * 
         * Relative paths may begin with a single './' to reference the 
         * current working directory, or a sequence of '../' to reference
         * parent directories.  After the './' or '../'s, the path must
         * consist of printable ascii path components (excluding the
         * '/', '\', ':', '*', and '?' characters and must not contain
         * solely '.' or '..') separated by '/' characters.  Relative
         * paths are stored in relative form, and are not resolved against
         * the current working directory when the path is set or retrieved.
         * It is the responsibility of the user to resolve a relative path
         * to an absolute path (possibly using AptPath utilities to do so).
         *
         * @param path
         *        The path in cross platform format
         * 
         * @return An AptResult is returned.
         *        MLE_S_OK  is returned if the path is successfully set.  
         *        MLE_E_UTILLIB_BADFORMAT  is returned if the path parameter can't be parsed
         */
        virtual MlResult setPath(const MlChar *path);

        /**
         * Get path in cross platform format.
         *
         * @return A constant character string is returned.
         * The path in cross platform format.  This is a 
         * pointer to internal data structures, and so should
         * not be changed or held on to for long.
         */
        virtual const MlChar * getPath() const;

        /**
         * Gets the filename portion of the path
         *
         * @return A constant character string is returned.
         * The filename (portion after last '/' in cross 
         * platform format).  This is a pointer to internal 
         * data structures, and so should not be changed or 
         * held on to for long.
         */
        virtual const MlChar * getFilename() const;

        /**
         * Get URL in cross platform format.
         *
         * @return A constant character string is returned.
         * The url in cross platform format.  This is a 
         * pointer to internal data structures, and so should
         * not be changed or held on to for long.
         */
        virtual const MlChar * getURL() const;

        /**
         * Determine whether the path is an absolute path or not.
         *
         * @return TRUE is returned if the path is an absolute one.
         * Otherwise, FALSE will be returned.
         */
        virtual MlBoolean isAbsolutePath();
    
    protected:

        //
        // Platform specific and cross platform path storage.
        //

        /** A pointer to the platform path. */
        MlChar * m_PlatformPath;
        /** The path. */
        MlChar * m_Path;
        /** The Universal Resource Locator. */
        MlChar * m_URL;
};

#endif /* __MLE_LINUXPATH_H_ */
