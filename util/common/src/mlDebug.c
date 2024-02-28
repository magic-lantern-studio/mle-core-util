/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlDebug.c
 * @ingroup MleCore
 *
 * This file contains the implementation of the Magic Lantern
 * debugging class.
 *
 * @author Mark S. Millard
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2024 Wizzer Works
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

/* include system header files */
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Include Magic Lantern header files */
#include "mle/mlDebug.h"
#include "mle/mlAssert.h"
#include "mle/mlMalloc.h"
#include "mle/mlFileio.h"

#include "mle/mlToken.h"

/* Declare global debug manager. */
MLE_UTIL_API MleDebugMgrP *g_mlDebugMgr;

/*
 * Routine to facilitate syntax parsing of token strings.
 */
static void _appendToToken(MleDBTokenDataP *tokenData,char ch)
{
    if (tokenData->_tokenUsed >= tokenData->_tokenAllocated-1) {
        if (tokenData->_tokenAllocated == 0) {
            tokenData->_tokenText = mlMalloc(MLE_TOKEN_BUFSIZ);
            tokenData->_tokenAllocated = MLE_TOKEN_BUFSIZ;
        } else {
            char *tmp;

            tmp = mlMalloc(tokenData->_tokenAllocated + MLE_TOKEN_BUFSIZ);
            memcpy(tmp, tokenData->_tokenText, tokenData->_tokenAllocated);
            mlFree(tokenData->_tokenText);
            tokenData->_tokenText = tmp;
            tokenData->_tokenAllocated += MLE_TOKEN_BUFSIZ;
        }
    }
    tokenData->_tokenText[tokenData->_tokenUsed++] = ch;
    tokenData->_tokenText[tokenData->_tokenUsed] = 0;
}


/*
 * Routine for parsing strings and creating tokens.
 */
static unsigned short _parseToken(MleDBTokenDataP *tokenData,char **cpp)
{
    MLE_VALIDATE_PTR(tokenData);
    MLE_VALIDATE_PTR(cpp);

    tokenData->_tokenUsed = 0;
    while (isspace(**cpp)) {
        if (**cpp == '\n') {
            *cpp += 1;
            return DELIM;
        }
        *cpp += 1;
    }
    if (**cpp == 0) {
        return ATEOF;
    }
    if (isdigit(**cpp)) {
        while (isdigit(**cpp)) {
            _appendToToken(tokenData,**cpp);
            *cpp += 1;
        }
        return NUMERIC;
    }
    if (isalpha(**cpp) || strchr("_$", **cpp) !=NULL) {
        while (**cpp && (isalnum(**cpp) || strchr("_$-+", **cpp) != NULL)) {
            _appendToToken(tokenData,**cpp);
            *cpp += 1;
        }
        return WORD;
    }
    if (**cpp == '.') {
        *cpp += 1;
        return DOT;
    }
    if (**cpp == ';' || **cpp == ',') {
        *cpp += 1;
        return DELIM;
    }
    if (**cpp == '=') {
        *cpp += 1;
        return EQUAL;
    }
    if (**cpp == '*') {
        _appendToToken(tokenData,**cpp);
        *cpp += 1;
        return STAR;
    }
    return BOGUS;
}


/*
 * Routine for extracting text of current token type.
 */
static char *_takeTokenText(MleDBTokenDataP *tokenData)
{
    /* declare local variables */
    char *txt;

    MLE_VALIDATE_PTR(tokenData);

    if (tokenData->_tokenText != NULL) {
#if defined(WN32)
        txt = _strdup(tokenData->_tokenText);
#else
        txt = strdup(tokenData->_tokenText);
#endif
        tokenData->_tokenUsed = 0;
        tokenData->_tokenText[0] = 0;
        return txt;
    }
    return NULL;
}

/*
 * Routine for setting a component in the debug manager.
 */
static void _setComponent(MleDebugMgrP *manager,char *str)
{
    MLE_VALIDATE_PTR(manager);

    if (manager->curComponent != NULL) {
        mlFree(manager->curComponent);
    }
    manager->curComponent = str;
}

/*
 * Routine for setting a category in the debug manager.
 */
static void _setCategory(MleDebugMgrP *manager,char *str)
{
    MLE_VALIDATE_PTR(manager);

    if (manager->curCategory != NULL) {
       mlFree(manager->curCategory);
    }
    manager->curCategory = str;
}

/*
 * Routine for setting the level in the debug manager.
 */
static void _setLevel(MleDebugMgrP *manager,unsigned long level)
{
    MLE_VALIDATE_PTR(manager);

    manager->curLevel = level;
}

/*
 * Add the debug manager definition to the internal debug component and category
 * dictionaries.
 */
static void _add(MleDebugMgrP *manager)
{
    /* Declare local variables. */
    char *starString = "*";
    _dbgQuark componentQuark,categoryQuark;
    MleDebugComponentP *component;
    MleDebugCategoryP *category;
    
    MLE_VALIDATE_PTR(manager);

    /* Quark-ify the current component and category strings. */
    if (! manager->curComponent) {
        manager->curComponent = mlMalloc(strlen(starString)+1);
        strcpy(manager->curComponent,starString);
    }
    if (! manager->curCategory) {
        manager->curCategory = mlMalloc(strlen(starString)+1);
        strcpy(manager->curCategory,starString);
    }
    componentQuark = _dbgStringToQuark(manager->curComponent);
    categoryQuark = _dbgStringToQuark(manager->curCategory);

    /* Check to see if component already exists. */
    component = _dbgDictFind(manager->components,componentQuark);
    if (! component) {
        /* Create a new component. */
        component = (MleDebugComponentP *)mlMalloc(sizeof(MleDebugComponentP));
        MLE_VALIDATE_PTR(component);

        component->categories = _dbgDictCreate();
        MLE_VALIDATE_PTR(component->categories);

        _dbgDictSet(manager->components,componentQuark,(void *)component);
        component->flags = 0;
    }

    /* Check to see if category already exists. */
    category = _dbgDictFind(component->categories,categoryQuark);
    if (! category) {
        /* Create a new category. */
        category = (MleDebugCategoryP *)mlMalloc(sizeof(MleDebugCategoryP));
        MLE_VALIDATE_PTR(category);

        _dbgDictSet(component->categories,categoryQuark,(void *)category);
        category->flags = 0;
    }

    category->level = manager->curLevel;
    MLE_DEBUG_SET_MATCH(category->flags);
    MLE_DEBUG_SET_MATCH(component->flags);
}

/*
 * Parse a string into the two-level debug structure.
 */
static void _parseString(MleDebugMgrP *manager,char *str)
{
    /* Declare local variables. */
    static MleDBTokenDataP tokenData = {0,NULL,0,0};

    char *cp;
    unsigned short token;
    signed long error, done;

    MLE_VALIDATE_PTR(manager);
    MLE_VALIDATE_PTR(str);

    cp = str;
    error = 0;
    done = 0;
    tokenData._state = 0;

    while (!done && !error) {
        token = _parseToken(&tokenData,&cp);
        if (token == ATEOF) {
            done = 1;
            token = DELIM;
        }

        switch (tokenData._state) {
        case 0:
            _setComponent(manager,NULL);
            _setCategory(manager,NULL);
            _setLevel(manager,0);
            if (token == WORD || token == STAR) {
                tokenData._state = 1;
            } else if (token != ATEOF) {
                error = 1;
            }
            break;
        case 1:
            if (token == DOT) {
                _setComponent(manager,_takeTokenText(&tokenData));
                tokenData._state = 2;
            } else if (token == EQUAL) {
                _setComponent(manager,NULL);
                _setCategory(manager,_takeTokenText(&tokenData));
                tokenData._state = 4;
            } else if (token == DELIM) {
                _setComponent(manager,NULL);
                _setCategory(manager,_takeTokenText(&tokenData));
                _setLevel(manager,0);
                _add(manager);
                tokenData._state = 0;
            } else {
                error = 1;
            }
            break;
        case 2:
            if (token==WORD || token == STAR) {
                _setCategory(manager,_takeTokenText(&tokenData));
                tokenData._state = 3;
            } else if (token == EQUAL) {
                _setCategory(manager,NULL);
                tokenData._state = 4;
            } else if (token == DELIM) {
                _setCategory(manager,NULL);
                _setLevel(manager,0);
                _add(manager);
                tokenData._state = 0;
            } else {
                error = 1;
            }
            break;
        case 3:
            if (token == EQUAL) {
                tokenData._state = 4;
            } else if (token == NUMERIC) {
                _setLevel(manager,atoi(tokenData._tokenText));
                _add(manager);
                tokenData._state = 5;
            } else if (token == DELIM) {
                _setLevel(manager,0);
                _add(manager);
                tokenData._state = 0;
            } else {
                error = 1;
            }
            break;
        case 4:
            if (token == NUMERIC) {
                _setLevel(manager,atoi(tokenData._tokenText));
                _add(manager);
                tokenData._state = 5;
            } else if (token == DELIM) {
                _setLevel(manager,0);
                _add(manager);
                tokenData._state = 0;
            } else {
                error = 1;
            }
            break;
        case 5:
            if (token == DELIM) {
                tokenData._state = 0;
            } else {
                error = 1;
            }
            break;
        }
    }
}

/*
 * Parse a file's contents into its two-level debug structure.
 */
static void _parseFile(MleDebugMgrP *manager,const char *file)
{
    /* Declare local variables. */
    signed long fd,rc;
    char *buf;
    struct stat statbuf;

    MLE_VALIDATE_PTR(manager);
    MLE_VALIDATE_PTR(file);

	if ((fd = mlOpen(file,O_RDONLY)) < 0 ) {
        return;
    }

    if (fstat(fd,&statbuf) < 0) {
        mlClose(fd);
        return;
    }

    buf = mlMalloc(statbuf.st_size+1);
    if (buf == 0) {
        mlClose(fd);
        return;
    }

    rc = mlRead(fd,buf,(int)statbuf.st_size);
    if (rc > 0) {
        buf[rc] = 0;
        _parseString(manager,buf);
    }

    mlFree(buf);
    mlClose(fd);
}

/*
 * Initialize the debug manager.
 */
MlBoolean _mlDBInit(
    MleDebugMgrP *manager,
    const char *envVar,
    const char *homeFile)
{
    /* Declare local variables. */
    char *env;
    MlBoolean retValue = TRUE;

    MLE_VALIDATE_PTR(manager);
    MLE_VALIDATE_PTR(envVar);
    MLE_VALIDATE_PTR(homeFile);

    /* Initialize Magic Lantern Debug Manager state variables. */
    manager->flags = 0;
    manager->curComponent = NULL;
    manager->curCategory = NULL;
    manager->components = _dbgDictCreate();
    MLE_VALIDATE_PTR(manager->components);

    /* Check debugger environment variable or home directory file. */
    if ((env = getenv(envVar)) != NULL) {
        if (env[0] == '/') {
            /* It's a path name. */
            _parseFile(manager,env);
            MLE_DEBUG_SET_MATCH(manager->flags);
        } else {
            /* It's a parseable string. */
            _parseString(manager,env);
            MLE_DEBUG_SET_MATCH(manager->flags);
        }
    } else if (mlAccess(homeFile,R_OK) == 0) {
        _parseFile(manager,homeFile);
        MLE_DEBUG_SET_MATCH(manager->flags);
    } else if ((env = getenv("HOME"))!= NULL) {
        char *file = mlMalloc(strlen(env)+1+strlen(homeFile)+1);
        MLE_VALIDATE_PTR(file);

        sprintf(file,"%s/%s",env,homeFile);
        if (mlAccess(file,R_OK) == 0) {
            _parseFile(manager,file);
            MLE_DEBUG_SET_MATCH(manager->flags);
        }

        mlFree(file);
    } else
        retValue = FALSE;

    return(retValue);
}

/*
 * Deinitialize the debug manager.
 */
void _mlDBDeinit(MleDebugMgrP *manager)
{
    /* Declare local variables. */
    signed short i,j;
    MleDebugCategoryP  *cat;
    MleDebugComponentP *dcp;
    _dbgDictEntry *dcpEntry,*catEntry;

    MLE_VALIDATE_PTR(manager);

    for (i = 0; i < _dbgDICTTABLESIZE; i++) {
        dcpEntry = manager->components->entry[i];
        while (dcpEntry) {
            dcp = (MleDebugComponentP *)dcpEntry->ptr;
            for (j = 0; j < _dbgDICTTABLESIZE; j++) {
                catEntry = dcp->categories->entry[j];
                while (catEntry) {
                    cat = (MleDebugCategoryP *)catEntry->ptr;
                    mlFree(cat);
                    catEntry = catEntry->next;
                }
			} /* For each category in the hash. */
            _dbgDictDestroy(dcp->categories);
            mlFree(dcp);
            dcpEntry = dcpEntry->next;
        }
    } /* For each component in the hash. */

    _dbgDictDestroy(manager->components);
}

/*
 * Create a new debug manager.
 */
MleDebugMgrP *mlDebugCreate(const char *envVar, const char *homeFile)
{
    /* Declare local variables. */
    MleDebugMgrP *manager;

    MLE_VALIDATE_PTR(envVar);
    MLE_VALIDATE_PTR(homeFile);

    /* Create a new Magic Lantern Debug Manager. */
    manager = (MleDebugMgrP *)mlMalloc(sizeof(MleDebugMgrP));
    if (manager)
        _mlDBInit(manager,envVar,homeFile);

    return(manager);
}

/*
 * Delete the specified debug manager.
 *
 */
MlBoolean mlDebugDelete(MleDebugMgrP *manager)
{
    MLE_VALIDATE_PTR(manager);

    _mlDBDeinit(manager);
    mlFree(manager);

    return(TRUE);
}

/*
 * Match the component in the specified dictonary.
 */
static MlBoolean _mlDBMatchComponent(
    _dbgDict dictionary,
    const char *component,
    _dbgDict *categories)
{
    /* Declare local variables. */
    MleDebugComponentP *dcp;
    _dbgQuark componentQuark;
    MlBoolean status = FALSE;

    MLE_VALIDATE_PTR(dictionary);

    if (! component) {
        /* By default, matching component is "*". */
        *categories = NULL;
        status = TRUE;
    } else {
        /* Find the matching component. */
        componentQuark = _dbgStringToQuark(component);
        dcp = _dbgDictFind(dictionary,componentQuark);
        if (dcp) {
            if (MLE_DEBUG_MATCH(dcp->flags)) {
                *categories = dcp->categories;
                status = TRUE;
	        }
        }
    }

    if (status == FALSE)
        *categories = NULL;
    return(status);
}

/*
 * Match the category in the specified dictionary.
 */
static MlBoolean _mlDBMatchCategory(
    _dbgDict dictionary,
    const char *category,
    unsigned long level)
{
    /* Declare local variables. */
    signed short i;
    _dbgDictEntry *catEntry;
    MleDebugCategoryP *cat;
    _dbgQuark categoryQuark;
    MlBoolean status = FALSE;

    MLE_VALIDATE_PTR(dictionary);

    if (! category) {
        /* By default, matching category is "*". */
        for (i = 0; (i < _dbgDICTTABLESIZE) && (! status); i++) {
            catEntry = dictionary->entry[i];
            while ((catEntry)  && (! status)) {
                cat = (MleDebugCategoryP *)catEntry->ptr;
                if (MLE_DEBUG_MATCH(cat->flags)) {
                    if (cat->level >= level) {
                        status = TRUE;
		    }
                }
                catEntry = catEntry->next;
            }
        }
    } else {
        /* Find the matching category. */
        categoryQuark = _dbgStringToQuark(category);
        cat = _dbgDictFind(dictionary,categoryQuark);
        if (cat) {
            if (MLE_DEBUG_MATCH(cat->flags)) {
                if (cat->level >= level) {
                    status = TRUE;
                }
            }
        }
    }

    return(status);
}

/*
 * Match the component, category and level in the specified debug manager.
 */
MlBoolean mlDebugMatch(
    MleDebugMgrP *manager,
    const char *component,
    const char *category,
    signed long level)
{
    /* Declare local variables. */
    signed short i;
    _dbgDict categories;
    _dbgDictEntry *dcpEntry;
    MleDebugComponentP *dcp;
    MlBoolean status = FALSE;

    MLE_VALIDATE_PTR(manager);

    if (_mlDBMatchComponent(manager->components,component,&categories)) {
        /* Valid match. */
        if (! categories) {
            /* Match "all" components. */
            for (i = 0; (i < _dbgDICTTABLESIZE) && (! status); i++) {
                dcpEntry = manager->components->entry[i];
                while ((dcpEntry)  && (! status)) {
                    dcp = (MleDebugComponentP *)dcpEntry->ptr;
                    if (MLE_DEBUG_MATCH(dcp->flags)) {
                        status = _mlDBMatchCategory(dcp->categories,category,level);
                    }
                    dcpEntry = dcpEntry->next;
                }
            }
        } else {
            /* Match specific component. */
            status = _mlDBMatchCategory(categories,category,level);
		}
    }

    return(status);
}


signed long mlDebugGetLevel(MleDebugMgrP *manager,const char *str)
{
    /* Declare local variables. */
    MleDebugCategoryP  *cat;
    MleDebugComponentP *dcp;
    _dbgDictEntry *dcpEntry;
    _dbgQuark strQuark;
    unsigned long lev = -1;
    signed short i;

    MLE_VALIDATE_PTR(manager);
    MLE_VALIDATE_PTR(str);

    /* Quark-ify the string. */
    strQuark = _dbgStringToQuark(str);

    for (i = 0; i < _dbgDICTTABLESIZE; i++) {
        dcpEntry = manager->components->entry[i];
        while (dcpEntry) {
            dcp = (MleDebugComponentP *)dcpEntry->ptr;

            if (MLE_DEBUG_MATCH(dcp->flags)) {
                cat = _dbgDictFind(dcp->categories,strQuark);
                if (cat) {
                    if (MLE_DEBUG_MATCH(cat->flags)) {
                        lev = cat->level > lev ? cat->level : lev;
                    }
                }
	        }

            dcpEntry = dcpEntry->next;
        }
    } /* For each component in the hash. */

    return lev;
}

/*
 * Dump the contents of the debug manager.
 */
void mlDebugDump(MleDebugMgrP *manager)
{
    /* Declare local variables. */
    signed short i,j;
    const char *dcpName,*catName;
    MleDebugCategoryP  *cat;
    MleDebugComponentP *dcp;
    _dbgDictEntry *dcpEntry,*catEntry;

    MLE_VALIDATE_PTR(manager);

    printf("COMPONENTS\n");
    for (i = 0; i < _dbgDICTTABLESIZE; i++) {
        dcpEntry = manager->components->entry[i];
        while (dcpEntry) {
            dcp = (MleDebugComponentP *)dcpEntry->ptr;
            dcpName = _dbgQuarkToString(dcpEntry->key);
            printf("\t%s\n",dcpName);
            printf("\tCATEGORIES\n");
            for (j = 0; j < _dbgDICTTABLESIZE; j++) {
                catEntry = dcp->categories->entry[j];
                while (catEntry) {
                    cat = (MleDebugCategoryP *)catEntry->ptr;
                    catName = _dbgQuarkToString(catEntry->key);
                    printf("\t\t%s = %ld\n",catName,cat->level);
                    catEntry = catEntry->next;
                }
	    } /* For each category in hash. */
            dcpEntry = dcpEntry->next;
        }
    } /* For each component in hash. */
}

#ifdef __QUARK_SRC__

#define _dbgQTABLESIZE 199

/* Hash list entry. */
typedef struct _dbgQuarkEntry
{
    char      *string;
    _dbgQuark  quark;
    int        ref;

    struct _dbgQuarkEntry *next;
} _dbgQuarkEntry;

/* Define the NULL quark. */
static _dbgQuarkEntry _dbgQ = { "",0,0,NULL };

/* Define the quark hash table (with one predefine, the NULL quark. */
static _dbgQuarkEntry *_dbgQTable[_dbgQTABLESIZE] = { &_dbgQ };

static _dbgQuark hash(const char *s)
{
    _dbgQuark q = 0;

    /* TODO get a better hash function */
    /* XXX this is lame, especially with a 256 table size */
    do {
        /* Rotate q left. */
        q = (q << 8) | (q >> 24);

		/* Add character. */
        q |= *s;
    } while ( *s++);

    /* Reduce to the size of the table and return. */
    return (q % _dbgQTABLESIZE);
}
                
_dbgQuark _dbgStringToQuark(const char *s)
{
    _dbgQuark q;
    _dbgQuarkEntry **qe;

    /* Get hashed value. */
    q = hash(s);

    /* Get initial entry pointer from the quark table. */
    qe = &_dbgQTable[q];

    /* Traverse to a blank spot and update hash number. */
    while ( *qe )
    {
        /* Check for a match. */
        if ( !strcmp(s,(*qe)->string) )
            return (*qe)->quark;

        q = (*qe)->quark + _dbgQTABLESIZE;
        qe = &(*qe)->next;
    }

    /* Put a new entry in the list. */
    *qe = (_dbgQuarkEntry *)mlMalloc(sizeof(_dbgQuarkEntry));
#if defined(WIN32)
    (*qe)->string = _strdup(s);      /* XXX this also does a malloc */
#else
    (*qe)->string = strdup(s);      /* XXX this also does a malloc */
#endif
    (*qe)->quark = q;
    (*qe)->ref = 0;
    (*qe)->next = NULL;

    /* Return the new quark. */
    return q;
}


const char *_dbgQuarkToString(const _dbgQuark q)
{
    _dbgQuarkEntry *qe;

    /* Find the right hash list. */
    qe = _dbgQTable[q % _dbgQTABLESIZE];

    /* Look through the list for this quark. */
    while ( qe )
    {
        if ( qe->quark == q )
            return qe->string;
                
        qe = qe->next;
    }

    /* No match. */
    return NULL;
}


void _dbgRefQuark(const _dbgQuark q)
{
    _dbgQuarkEntry *qe;

    /* Find the right hash list. */
    qe = _dbgQTable[q % _dbgQTABLESIZE];

    /* Look through the list for this quark. */
    while ( qe )
    {
        if ( qe->quark == q )
        {
            qe->ref++;
            return;
        }
                
        qe = qe->next;
    }
}


void _dbgUnrefQuark(const _dbgQuark q)
{
    _dbgQuarkEntry **qe;

    /* Find the right hash list. */
    qe = &_dbgQTable[q % _dbgQTABLESIZE];

    /* Find the right entry if it exists. */
    while ( *qe )
    {
        if ( (*qe)->quark == q )
        {
            _dbgQuarkEntry *old = *qe;

            /* Skip over this entry. */
            *qe = (*qe)->next;

            /* Free resources. */
            mlFree(old->string);
            mlFree(old);

			return;
        }

        qe = &(*qe)->next;
    }
}

#endif /* __QUARK_SRC__*/


#ifdef __DICT_SRC__

#include <stdio.h>
#ifdef WIN32
#include <memory.h>
#endif /* WIN32 */


_dbgDict _dbgDictCreate(void)
{
    _dbgDict dict;

    /* Allocate space for dictionary. */
    dict = (_dbgDict)mlMalloc(sizeof(struct __dbgDict));

    /* Zero out entry tables. */
    memset ( dict, 0, sizeof(*dict) );      /* MAD */

    return dict;
}
                

void _dbgDictDestroy(_dbgDict dict)
{
    /* Deallocate dictionary space. */
    mlFree(dict);
}


_dbgDictEntry *_dbgDictSet(_dbgDict dict,_dbgQuark key,void *ptr)
{
    _dbgDictEntry **entry;

    /* Find list of entries. */
    entry = &dict->entry[key % _dbgDICTTABLESIZE];

	/* Linear search through the list. */
    while ( *entry )
    {
        /* If found, set the current entry. */
        if ( (*entry)->key == key )
        {
            (*entry)->ptr = ptr;
            return *entry;
        }

        entry = &(*entry)->next;
    }

    /* If not found, create a new entry. */
    *entry = (_dbgDictEntry *)mlMalloc(sizeof(_dbgDictEntry));
    (*entry)->key = key;
    (*entry)->ptr = ptr;

    (*entry)->next = NULL; /* MAD */

    return *entry;
}


void _dbgDictRemove(_dbgDict dict,_dbgQuark key)
{
    _dbgDictEntry **entry;

    /* Find list of entries. */
    entry = &dict->entry[key % _dbgDICTTABLESIZE];

    /* Linear search through the list. */
    while ( *entry )
    {
        /* If found, remove the entry. */
        if ( (*entry)->key == key )
        {
            _dbgDictEntry *old = *entry;

			*entry = (*entry)->next;
            mlFree(old);

            return;
        }

        entry = &(*entry)->next;
    }

    /* Not found if this line is reached. */
}


void *_dbgDictFind(_dbgDict dict,_dbgQuark key)
{
    _dbgDictEntry *entry;

    /* Find list of entries. */
    entry = dict->entry[key % _dbgDICTTABLESIZE];

    /* Linear search through the list. */
    while ( entry )
    {
        /* Return the pointer if found. */
        if ( entry->key == key )
            return entry->ptr;
                
        entry = entry->next;
    }

    return NULL;
}

_dbgDictEntry *_dbgDictFindEntry(_dbgDict dict,_dbgQuark key)
{
    _dbgDictEntry *entry;

    /* Find list of entries. */
    entry = dict->entry[key % _dbgDICTTABLESIZE];

    /* Linear search through the list. */
    while ( entry )
    {
        /* Return the pointer if found. */
        if ( entry->key == key )
             return entry;
                
        entry = entry->next;
    }

    return NULL;
}

#endif /* __DICT_SRC__ */


#ifdef UNIT_TEST


#define BOGUS_ENVVAR "_BOGUS_"
#define BOGUS_FILE   "._bogus"
#define TEST_FILE    "mldbg.tmp"


main (int argc,char *argv[])
{
    /* Declare local variables. */
    char buf[BUFSIZ];
    MleDebugMgrP *mlDebugMgr = NULL;
    signed long fd;

    /* Initialize the test. */
    fprintf(stdout,"Testing Magic Lantern Debug Module ...\n");

    /* Test 1 -- test for empty construction/destruction. */
    fprintf(stdout,"Running Test 1 ...\n");

    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,BOGUS_FILE);
    MLE_VALIDATE_PTR(mlDebugMgr);

    //mlDBDump(mlDebugMgr);
    
	if (mlDebugMatch(mlDebugMgr,NULL,"info",1))
        fprintf(stdout,"Matched '*.info' with level '1'\n");

    MLE_WARN(mlDebugDelete(mlDebugMgr));
	mlDebugMgr = NULL;

    /* Test 2 -- test for simple category construction. */
    fprintf(stdout,"Running Test 2 ...\n");

	if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        exit(-1);
    }

    strcpy(buf,"info");
    mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,TEST_FILE);
    MLE_VALIDATE_PTR(mlDebugMgr);

    //mlDBDump(mlDebugMgr);
    
	if (mlDebugMatch(mlDebugMgr,NULL,"info",0))
        fprintf(stdout,"Matched '*.info' with level '0'\n");
    if (mlDebugMatch(mlDebugMgr,NULL,NULL,0))
        fprintf(stdout,"Matched '*.*' with level '0'\n");

    MLE_WARN(mlDebugDelete(mlDebugMgr));
	mlDebugMgr = NULL;

    unlink(TEST_FILE);

    /* Test 3 -- test for component/category construction. */
    fprintf(stdout,"Running Test 3 ...\n");

	if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        exit(-1);
    }
    strcpy(buf,"Test3.info=5");
    mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,TEST_FILE);
    MLE_VALIDATE_PTR(mlDebugMgr);

    //mlDBDump(mlDebugMgr);
    
	if (mlDebugMatch(mlDebugMgr,"Test3","info",1))
        fprintf(stdout,"Matched 'Test3.info' with level '1'\n");
    if (mlDebugMatch(mlDebugMgr,NULL,"info",0))
        fprintf(stdout,"Matched '*.info' with level '1'\n");
    if (mlDebugMatch(mlDebugMgr,"Test3",NULL,0))
        fprintf(stdout,"Matched 'Test3.*' with level '1'\n");
    if (mlDebugMatch(mlDebugMgr,NULL,NULL,0))
        fprintf(stdout,"Matched '*.*' with level '1'\n");

    MLE_WARN(mlDebugDelete(mlDebugMgr));
	mlDebugMgr = NULL;

    unlink(TEST_FILE);

    /* Test 4 -- test for multiple component/category construction. */
    fprintf(stdout,"Running Test 4 ...\n");

	if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        exit(-1);
    }
    strcpy(buf,"Test1.info=1\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test2.info=2\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test3.info=3\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test1.debug=4\n");
    mlWrite(fd,buf,strlen(buf));
    strcpy(buf,"Test2.debug=5\n");
    mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,TEST_FILE);
    MLE_VALIDATE_PTR(mlDebugMgr);

    //mlDBDump(mlDebugMgr);

    if (mlDebugMatch(mlDebugMgr,"Test1","info",1))
        fprintf(stdout,"Matched 'Test1.info' with level '1'\n");
    if (mlDebugMatch(mlDebugMgr,"Test1","debug",2))
        fprintf(stdout,"Matched 'Test1.debug' with level '2'\n");
    if (mlDebugMatch(mlDebugMgr,NULL,"debug",2))
        fprintf(stdout,"Matched '*.debug' with level '2'\n");
    if (mlDebugMatch(mlDebugMgr,"Test2",NULL,2))
        fprintf(stdout,"Matched 'Test2.*' with level '2'\n");
    if (! mlDebugMatch(mlDebugMgr,NULL,"bogus",5))
        fprintf(stdout,"Unable to match '*.bogus' with level '5'\n");

    MLE_WARN(mlDebugDelete(mlDebugMgr));
	mlDebugMgr = NULL;

    unlink(TEST_FILE);

    /* Test 5 -- test convenience macros. */
    fprintf(stdout,"Running Test 5 ...\n");

	if ((fd = mlCreate(TEST_FILE,S_IREAD | S_IWRITE)) < 0 ) {
        exit(-1);
    }

    strcpy(buf,"Test5.info=5\n");
    mlWrite(fd,buf,strlen(buf));
    mlClose(fd);

    g_mlDebugMgr = mlDebugCreate(BOGUS_ENVVAR,TEST_FILE);
    MLE_VALIDATE_PTR(g_mlDebugMgr);

    //mlDBDump(g_mlDebugMgr);

    MLE_DEBUG_FULL("Test5","info",3, {
        fprintf(stdout,"Matched 'Test5.info' with level '3'\n");
    });
    MLE_DEBUG_CAT("info", {
        fprintf(stdout,"Matched '*.info' with level '0'\n");
    });
    MLE_DEBUG_CATLEVEL("info",3, {
        fprintf(stdout,"Matched '*.info' with level '3'\n");
    });
    MLE_DEBUG_CMPT("Test5", {
        fprintf(stdout,"Matched 'Test5.*' with level '0'\n");
    });
    MLE_DEBUG_CMPTLEVEL("Test5",3, {
        fprintf(stdout,"Matched 'Test5.*' with level '3'\n");
    });
    MLE_DEBUG_FULL("Bogus","bogus",3, {
        fprintf(stdout,"Matched 'Bogus.bogus' with level '3'\n");
    });

    MLE_WARN(mlDebugDelete(g_mlDebugMgr));
	g_mlDebugMgr = NULL;

    unlink(TEST_FILE);

    /* Complete the test. */
    fprintf(stdout,"Magic Lantern Debug Test Complete.\n");
}


#endif /* UNIT_TEST */
