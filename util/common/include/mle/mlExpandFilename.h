/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlExpandFilename.h
 * @ingroup MleCore
 *
 * This file contains function definitions for expanding a
 * UNIX-like filename pathcontaining tilde notation and
 * environment variables, plus strftime(3) notation
 *
 * Notations that are supported include
 * <ol>
 * <li>Leading tildes</li>
 * <ol>
 * <li>If not followed by a user name, refers to current
 *     user's HOME directory.</li>
 * <li>If followed by a valid user name, refers to the
 *     home directory of that user.</li>
 * <li>If followed by an invalid user name, no replacement
 *     is done.</li>
 * </ol>
 * <li>Embedded environment variables.</li>
 * <ol>
 * <li>$WORD notation.  WORD may be alphanumeric with
 *     any characters in the constant SPECIALS below (only
 *     an underscore for now).</li>
 * <li>${WORD} notation.  WORD may contain anything.</li>
 * <li>If WORD is not an environment variable, it is replaced
 *     by an empty string, unless...</li>
 * <li>Undefined environment variables that are in the Known
 *     table below have the corresponding values used.</li>
 * </ol>
 * <li>Percent notation.</li>
 * <ol>
 * <li>if %n is used, a counting integer is substituted.</li>
 * <li>Other % keys are passed to strftime.</li>
 * </ol>
 * </ol>
 *
 * @author Mark S. Millard
 * @date July 10, 2003
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

#ifndef __MLE_EXPANDFILENAME_H_
#define __MLE_EXPANDFILENAME_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set the filename index.
 *
 * @param The index to set.
 */
void mlFilenameIndexSet(int);

/**
 * Increment the filename index.
 */
void mlFilenameIndexIncrement(void);


/**
 * Expand the specified filename.
 *
 * @param name The file name to expand.
 *
 * @return The result is newly malloc'ed storage; it's up to the caller to
 * mleFree() this.
 */
char *mlFilenameExpand(const char *name);

#ifdef __cplusplus
} /* end of 'extern "C"' */
#endif


#endif /* __MLE_EXPANDFILENAME_H_ */
