/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlDebug.h
 * @ingroup MleCore
 *
 * This file contains the definition of the Magic Lantern
 * debugging macros.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

#ifndef __MLE_DEBUG_H_
#define __MLE_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

// Include standard header files.
#if defined(__linux__)
#elif defined(WIN32)
#elif defined(psx)
/* Temporary hack to add breakpoints to PSX target under PSYQ environment. */
#include <psx/libsn.h>
#define BREAKPOINT() PSYQpause() /* Interrupt platform, drop into debugger. */
#define POLLHOST() pollhost()    /* Target nub checks host from time to time. */
#else
error...undefined platform
#endif

/* Include Magic Lantern header files. */
#include "mle/mlTypes.h"
#include "mle/MleUtil.h"

/* NOTE: This duplicates the source found under the player's  */
/*       source tree.  This was an intentional decision made  */
/*       to alienate changes made to the player's source to   */
/*       facilitate the rehearsal player and debugging tools. */

#define __QUARK_SRC__
#ifdef __QUARK_SRC__

/* Define internal "quark-ification" utilities and references. */
typedef int _dbgQuark;

EXTERN _dbgQuark _dbgStringToQuark(const char *);
EXTERN const char *_dbgQuarkToString(const _dbgQuark);

EXTERN void _dbgRefQuark(const _dbgQuark);
EXTERN void _dbgUnrefQuark(const _dbgQuark);

#endif /* __QUARK_SRC__ */

#define __DICT_SRC__
#ifdef __DICT_SRC__

/* Define a default size for the dictionary hash table. */
#ifndef _dbgDICTTABLESIZE
#define _dbgDICTTABLESIZE 15
#endif

/**
 * This structure is a dictionary entry used for internal debug support.
 */
typedef struct __dbgDictEntry
{
    _dbgQuark key;  /**< A token key for the entry. */
    void *ptr;      /**< A pointer to the entry. */

    struct __dbgDictEntry *next;  /**< A pointer to the next entry. */
} _dbgDictEntry;


/**
 * This structure is a dictionary used for internal debug support.
 */
typedef struct __dbgDict
{
    _dbgDictEntry *entry[_dbgDICTTABLESIZE];  /**< A list of entries. */
} *_dbgDict;


EXTERN _dbgDict _dbgDictCreate(void);
EXTERN void _dbgDictDestroy(_dbgDict);

EXTERN _dbgDictEntry *_dbgDictSet(_dbgDict dict,_dbgQuark key,void *ptr);
EXTERN void _dbgDictRemove(_dbgDict dict,_dbgQuark key);
EXTERN void *_dbgDictFind(_dbgDict dict,_dbgQuark key);
EXTERN _dbgDictEntry *_dbgDictFindEntry(_dbgDict,_dbgQuark key);

#endif /* __DICT_SRC__ */


/**
 * This structure encapsulates a Debug Category.
 */
typedef struct _MleDebugCategoryP
{
    unsigned long flags;  /**< Category flags. */
    unsigned long level;  /**< Debug level. */
} MleDebugCategoryP;


/**
 * This structure encapsulates a Debug Component.
 */
typedef struct _MleDebugComponentP
{
    unsigned long flags;  /**< Component flags. */
    _dbgDict categories;  /**< Categories dictionary. */
} MleDebugComponentP;


/**
 * This structure encapsulates the Debug Manager.
 */
typedef struct _MleDebugMgrP
{
    unsigned long flags;    /**< Manager flags. */
    char     *curComponent; /**< Current component used for creating dictionary. */
    char     *curCategory;  /**< Current category used for creating dictionary . */
    unsigned long curLevel; /**< Current debug level used for creating dictionary. */
    _dbgDict components;    /**< Components dictionary. */
} MleDebugMgrP;


/* Define "flags" for above structures. */
#define MLE_DBMATCH_MASK     0x00000001
#define MLE_DBMATCH_UNSET    0x00000000
#define MLE_DBMATCH_SET      0x00000001

#define MLE_DEBUG_MATCH(flags) \
    ((flags & MLE_DBMATCH_MASK) == MLE_DBMATCH_SET)
#define MLE_DEBUG_SET_MATCH(flags) \
    flags = ((flags & (! MLE_DBMATCH_MASK)) | MLE_DBMATCH_SET)
#define MLE_DEBUG_UNSET_MATCH(flags) \
    flags = ((flags & (! MLE_DBMATCH_MASK)) | MLE_DBMATCH_UNSET)


/* Define default Magic Lantern debug environment variable and file. */
#define MLE_DEBUG_ENVVAR "MLE_DEBUG"
#define MLE_DEBUG_FILE   ".mle.debug"


/* Function prototypes. */
EXTERN MlBoolean _mlDBInit(MleDebugMgrP *,const char *,const char *);
EXTERN void _mlDBDeinit(MleDebugMgrP *);

EXTERN MLE_UTIL_API MleDebugMgrP *mlDebugCreate(char *,char *);
EXTERN MLE_UTIL_API MlBoolean mlDebugDelete(MleDebugMgrP *);
EXTERN MLE_UTIL_API MlBoolean mlDebugMatch(MleDebugMgrP *,const char *,const char *,signed long);
EXTERN MLE_UTIL_API signed long mlDebugGetLevel(MleDebugMgrP *,const char *);
EXTERN MLE_UTIL_API void mlDebugDump(MleDebugMgrP *);


#if defined(MLE_DEBUG)

extern MLE_UTIL_API MleDebugMgrP *g_mlDebugMgr;


/* Useful macros. */

/**
 * @brief Debug using full component, category and level specifications.
 *
 * @param component The debug component to compare against.
 * @param category The debug category to compare against.
 * @param level The debug level to compare against.
 * @param The block of code to execute if all three specifications match.
 */
#define MLE_DEBUG_FULL(component,category,level,block) \
    if ( !g_mlDebugMgr ) g_mlDebugMgr = mlDebugCreate(MLE_DEBUG_ENVVAR,MLE_DEBUG_FILE); \
    if (MLE_DEBUG_MATCH(g_mlDebugMgr->flags)) \
        { if (mlDebugMatch(g_mlDebugMgr,component,category,level)) block }

/**
 * @brief Debug using just the category specification.
 *
 * @param category The debug category to compare against.
 * @param The block of code to execute if the category specification matches.
 */
#define MLE_DEBUG_CAT(category,block) \
    if ( !g_mlDebugMgr ) g_mlDebugMgr = mlDebugCreate(MLE_DEBUG_ENVVAR,MLE_DEBUG_FILE); \
    if (MLE_DEBUG_MATCH(g_mlDebugMgr->flags)) \
        { if (mlDebugMatch(g_mlDebugMgr,NULL,category,0)) block }

/**
 * @brief Debug using the category and level specifications.
 *
 * @param category The debug category to compare against.
 * @param level The debug level to compare against.
 * @param The block of code to execute if the category and level specification match.
 */
#define MLE_DEBUG_CATLEVEL(category,level,block) \
    if ( !g_mlDebugMgr ) g_mlDebugMgr = mlDebugCreate(MLE_DEBUG_ENVVAR,MLE_DEBUG_FILE); \
    if (MLE_DEBUG_MATCH(g_mlDebugMgr->flags)) \
        { if (mlDebugMatch(g_mlDebugMgr,NULL,category,level)) block }

/**
 * @brief Debug using just the component specification.
 *
 * @param category The debug component to compare against.
 * @param The block of code to execute if the component specification matches.
 */
#define MLE_DEBUG_CMPT(component,block) \
    if ( !g_mlDebugMgr ) g_mlDebugMgr = mlDebugCreate(MLE_DEBUG_ENVVAR,MLE_DEBUG_FILE); \
    if (MLE_DEBUG_MATCH(g_mlDebugMgr->flags)) \
        { if (mlDebugMatch(g_mlDebugMgr,component,NULL,0)) block }

/**
 * @brief Debug using the component and level specifications.
 *
 * @param category The debug component to compare against.
 * @param level The debug level to compare against.
 * @param The block of code to execute if the component and level specifications match.
 */
#define MLE_DEBUG_CMPTLEVEL(component,level,block) \
    if ( !g_mlDebugMgr ) g_mlDebugMgr = mlDebugCreate(MLE_DEBUG_ENVVAR,MLE_DEBUG_FILE); \
    if (MLE_DEBUG_MATCH(g_mlDebugMgr->flags)) \
        { if (mlDebugMatch(g_mlDebugMgr,component,NULL,level)) block }

#else

/**
 * @brief Debug using full component, category and level specifications.
 *
 * @param component The debug component to compare against.
 * @param category The debug category to compare against.
 * @param level The debug level to compare against.
 * @param The block of code to execute if all three specifications match.
 */
#define MLE_DEBUG_FULL(component,category,level,block)

/**
 * @brief Debug using just the category specification.
 *
 * @param category The debug category to compare against.
 * @param The block of code to execute if the category specification matches.
 */
#define MLE_DEBUG_CAT(category,block)

/**
 * @brief Debug using the category and level specifications.
 *
 * @param category The debug category to compare against.
 * @param level The debug level to compare against.
 * @param The block of code to execute if the category and level specification match.
 */
#define MLE_DEBUG_CATLEVEL(category,level,block)

/**
 * @brief Debug using just the component specification.
 *
 * @param category The debug component to compare against.
 * @param The block of code to execute if the component specification matches.
 */
#define MLE_DEBUG_CMPT(component,block)

/**
 * @brief Debug using the component and level specifications.
 *
 * @param category The debug component to compare against.
 * @param level The debug level to compare against.
 * @param The block of code to execute if the component and level specifications match.
 */
#define MLE_DEBUG_CMPTLEVEL(component,level,block)

#define MLE_DEBUG_DECLARE()

#endif /* MLE_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* __MLE_DEBUG_H_ */
