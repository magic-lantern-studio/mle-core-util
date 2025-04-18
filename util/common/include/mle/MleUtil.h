/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file MleUtil.h
 * @ingroup MleCore
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
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

#ifndef __MLE_UTIL_H_
#define __MLE_UTIL_H_

/* Precaution to avoid an error easily made by the application programmer. */
#ifdef MLE_UTIL_API
#error Leave the internal MLE_UTIL_API define alone.
#endif /* MLE_UTIL_API */

/*
  On MSWindows platforms, one of these defines must always be set when
  building application programs:

   - "MLE_UTIL_DLL", when the application programmer is using the
     library in the form of a dynamic link library (DLL)

   - "MLE_NOT_UTIL_DLL", when the application programmer is using the
     library in the form of a static object library (LIB)

  Note that either MLE_UTIL_DLL or MLE_NOT_UTIL_DLL _must_ be defined by
  the application programmer on MSWindows platforms, or else the
  #error statement will hit. Set up one or the other of these two
  defines in your compiler environment according to how the library
  was built -- as a DLL (use "MLE_UTIL_DLL") or as a LIB (use
  "MLE_NOT_UTIL_DLL").

  (Setting up defines for the compiler is typically done by either
  adding something like "/DMLE_UTIL_DLL" to the compiler's argument
  line (for command-line build processes), or by adding the define to
  the list of preprocessor symbols in your IDE GUI (in the MSVC IDE,
  this is done from the "Project"->"Settings" menu, choose the "C/C++"
  tab, then "Preprocessor" from the dropdown box and add the
  appropriate define)).

  It is extremely important that the application programmer uses the
  correct define, as using "MLE_NOT_UTIL_DLL" when "MLE_UTIL_DLL" is
  correct is likely to cause mysterious crashes.
 */

// Magic Lantern Digital Workprint export rules.
# ifdef MLE_INTERNAL
#  ifdef MLE_MAKE_DLL
#   define MLE_UTIL_API __declspec(dllexport)
#  endif /* MLE_MAKE_DLL */
# else /* !MLE_INTERNAL */
#  ifdef MLE_UTIL_DLL
#   define MLE_UTIL_API __declspec(dllimport)
#  else /* !MLE_UTIL_DLL */
#   ifndef MLE_NOT_UTIL_DLL
#    error Define either MLE_UTIL_DLL or MLE_NOT_UTIL_DLL as appropriate for your linkage! See mle/MleUtil.h for further instructions.
#   endif /* MLE_NOT_UTIL_DLL */
#  endif /* !MLE_UTIL_DLL */
# endif /* !MLE_INTERNAL */

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef MLE_UTIL_API
# define MLE_UTIL_API
#endif /* !MLE_UTIL_API */

#endif /* __MLE_UTIL_H_ */
