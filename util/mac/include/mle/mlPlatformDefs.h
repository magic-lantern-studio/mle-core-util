/** @defgroup MleCore Magic Lantern Core Library API */
/**
 *  @file mlPlatformDefs.h
 *  @ingroup MleCore
 *
 *  Magic Lantern Core - Win32 specific definitions.
 *
 *  @author Mark S. Millard
 *  @date July 10, 2003
 *  @version 0.13
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2012  Wizzer Works
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

#ifndef __MLE_CORE_PLATFORMDEFS_H_
#define __MLE_CORE_PLATFORMDEFS_H_


// The Macintosh platform definition macro.
//
#define PLATFORM_MAC

// Macintosh uses ANSI C/C++
#define MLE_CORE_ANSI_C

// The definition of export and import macros.
#define MLE_CORE_EXPORT
#define MLE_CORE_IMPORT

// The minimal import export macros.
#define MLE_CORE_MIN_EXPORT
#define MLE_CORE_MIN_IMPORT


#endif /* __MLE_CORE_PLATFORMDEFS_H_ */
