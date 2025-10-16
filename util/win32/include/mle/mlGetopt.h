/** @defgroup MleCore Magic Lantern Core Library API */
/**
 *  @file mlGetopt.h
 *  @ingroup MleCore
 *
 *  Magic Lantern Core - Win32 specific definitions.
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_CORE_GETOPT_H_
#define __MLE_CORE_GETOPT_H_

// Include Magic Lantern header files.
#include "mle/MleUtil.h"

#ifdef __cplusplus
extern "C" {
#endif

MLE_UTIL_API extern int optind;
MLE_UTIL_API extern TCHAR * optarg;

// @brief Get the next option in the argument list.
// 
// The getopt() function parses the command-line arguments. Its
// arguments argc and argv are the argument count and array as passed
// from the calling function on program invocation. An element of argv
// that starts with '-' (and is not exactly "-" or "--") is an option
// element. The characters of this element (aside from the initial
// '-') are option characters. If getopt() is called repeatedly, it
// returns successively each of the option characters from each of
// the option elements.
//
// @param argc The number of arguments in the argument list.
// @param argv A pointer to the list of arguments to parse.
// @param optstring A pointer to a character string defining the legitimate options.
//
// @return If parsing is successful, then the next character will be returned
// as an integer value.
MLE_UTIL_API int getopt(int argc, TCHAR *argv[], TCHAR *optstring);

#ifdef __cplusplus
} /* end of 'extern "C"' */
#endif


#endif /* __MLE_CORE_GETOPT_H_ */
