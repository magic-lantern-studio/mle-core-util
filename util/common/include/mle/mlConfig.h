/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlConfig.h
 * @ingroup MleCore
 *
 * This file contains constants concerning the installation of
 * Magic Lantern in a customer machine.
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

#ifndef __MLE_CONFIG_H_
#define __MLE_CONFIG_H_


#if defined(__linux__) || defined(__APPLE__)
#define MleDefaultHomePath "/usr/WizzerWorks/MagicLantern"
#else
#if defined(WIN32)
#define MleDefaultHomePath "C:/Program Files/Wizzer Works/MagicLantern"
#else
Unknown Platform Configuration!!!
#endif
#endif

/*
 * Template Files
 */

#define MLE_TITLE_CPP_TEMPLATE        "$MLE_ROOT/lib/TitleCppCode.tmpl"    /**< Title template for C++ code generation. */
#define MLE_TITLE_JAVA_TEMPLATE       "$MLE_ROOT/lib/TitleJavaCode.tmpl"   /**< Title template for Java code generation. */
#define MLE_TITLE_MAKEFILE_TEMPLATE   "$MLE_ROOT/lib/TitleMakefile.tmpl"   /**< Title template for Makefile generation. */
#define MLE_TITLE_ANT_TEMPLATE        "$MLE_ROOT/lib/TitleAnt.tmpl"        /**< Title template for Ant generation. */
#define MLE_TOC_CPP_GLOBALS_TEMPLATE  "$MLE_ROOT/lib/TOCCppGlobals.tmpl"   /**< Template for C++ TOC globals. */
#define MLE_TOC_JAVA_GLOBALS_TEMPLATE "$MLE_ROOT/lib/TOCJavaGlobals.tmpl"  /**< Template for Java TOC globals. */

/*
 * Miscellaneous executables
 */

#if defined(__linux__) || defined(__APPLE__)
#define MLE_PLAYER_PATH \
	(getenv("MLE_PLAYER") ? getenv("MLE_PLAYER") : "$MLE_ROOT/bin/mleplay")
#else
#if defined(WIN32)
#define MLE_PLAYER_PATH \
	(getenv("MLE_PLAYER") ? getenv("MLE_PLAYER") : "C:/Program Files/Wizzer Works/MagicLantern/bin/mleplay")
#endif
#endif

/*
 * Directories...
 */

#define MLE_ACTOR_DIR "$MLE_HOME/actors" /**< Magic Lantern actor directory. */
#define MLE_LIB_DIR "$MLE_HOME/lib"      /**< Magic Lantern library directory. */

/*
 * These are the recommended filename extensions for code generation.
 */

#define CPP_CODE_FILENAME_EXT   ".cxx"   /**< Suffix for C++ source file generation. */
#define CPP_HEADER_FILENAME_EXT ".h"     /**< Suffix for C++ header file generation. */
#define JAVA_CODE_FILENAME_EXT  ".java"  /**< Suffix for Java source file generation. */


#endif /* __MLE_CONFIG_H_ */
