/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlExpandFilename.c
 * @ingroup MleCore
 *
 * UNIX-like routine to expand file names containing tilde notation and
 * environment variables, plus strftime(3) notation
 *
 * Notations that are supported include
 *    1) Leading tildes.  
 *        A) If not followed by a user name, refers to current
 *           user's HOME directory.
 *        B) If followed by a valid user name, refers to the
 *           home directory of that user.
 *        C) If followed by an invalid user name, no replacement
 *           is done.
 *    2) Embedded environment variables.
 *        A) $WORD notation.  WORD may be alphanumeric with
 *           any characters in the constant SPECIALS below (only
 *           an underscore for now).
 *        B) ${WORD} notation.  WORD may contain anything.
 *        C) If WORD is not an environment variable, it is replaced
 *           by an empty string, unless...
 *          D) Undefined environment variables that are in the Known
 *             table below have the corresponding values used.
 *      3) Percent notation.
 *          A) if %n is used, a counting integer is substituted.
 *          B) Other % keys are passed to strftime
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#include <strings.h>
#include <pwd.h>
#endif /* ! WIN32 */
#include <time.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlConfig.h"
#include "mle/mlExpandFilename.h"


static struct {
    char *envvar;
    char *value;
} Known[] = {
    { "MLE_HOME", MleDefaultHomePath },
    { NULL }
};


/*
 * Various constants that might be found in pathnames.
 */

#define TILDE    '~'
#define SLASH    '/'
#define DOLLAR   '$'
#define NIL      '\0'
#define PERCENT  '%'
#define SPECIALS "_"
#define LBRACE   '{'
#define RBRACE   '}'

static char *getHomeOfMe(void), *getHomeOfName(char*);
//extern char *getenv();


/*
 * VStr describes a variable length dynamic string.  This is not
 * a highly efficient implementation.
 */

typedef struct {
    int allocked;
    int used;
    char *str;
} VStr;

static int mle_filename_index = 0;
static void appendStr(VStr *vstr,char *str);
static void appendChar(VStr *vstr,char ch);


void
mlFilenameIndexSet(int x)
{
    mle_filename_index = x;
}


void
mlFilenameIndexIncrement()
{
    mle_filename_index++;
}


/*
 * char *mlFilenameExpand(char *)
 * ------------------------------
 *
 * The result is newly malloc'ed storage; it's up to the caller to
 * mleFree() this.
 */

char *
mlFilenameExpand(const char *inName)
{
    char *userName;
    char *tildeName;
    char *curPtr, *newPtr;
    char *varName, *val;
    VStr *vstr;

    if (inName == NULL) {
        return strdup("");
    }
    
    /*
     * Quick check for no work -- start with tilde or contain a $? 
     */

    if (*inName != TILDE &&
        strchr(inName, DOLLAR) == NULL &&
        strchr(inName, PERCENT) == NULL ) {
        return strdup(inName);
    }

    /*
     * If the first character is a tilde, process it.  Note that embedded
     * tildes are not allowed, as per C-shell semantics. 
     */

    vstr = (VStr *) mlCalloc(1, sizeof *vstr);
    curPtr = (char *)inName;
    if (*curPtr == TILDE) {
        curPtr++;
        if (*curPtr == NIL || *curPtr == SLASH) {
            tildeName = getHomeOfMe();
        } else {
            if ((newPtr = strchr(inName, SLASH)) == NULL) {
                newPtr = curPtr + strlen(curPtr);
            }
            userName = (char *) mlMalloc(newPtr - curPtr + 1);
            strncpy(userName, curPtr, newPtr - curPtr);
            userName[newPtr - curPtr] = NIL;
            tildeName = getHomeOfName(userName);
            mlFree(userName);
            curPtr = newPtr;
        }
        appendStr(vstr, tildeName);
    }

    /*
     * curPtr now points to the current scan point in the remaining input.
     * Check for environment variables. 
     */

    while (*curPtr != NIL) {
        char        *savePtr;
        if (*curPtr == DOLLAR) {
            savePtr = curPtr;
            newPtr = varName = (char *) mlMalloc(strlen(curPtr));
            curPtr++;
            if (*curPtr == LBRACE) {
                curPtr++;
                while (*curPtr != NIL && *curPtr != RBRACE) {
                    *newPtr++ = *curPtr++;
                }
                if (*curPtr != NIL) {
                    curPtr++;
                }
            } else {
                while (*curPtr != NIL && (isalnum(*curPtr) ||
                       strchr(SPECIALS, *curPtr) != NULL)) {
                    *newPtr++ = *curPtr++;
                }
            }

            *newPtr = NIL;
            if ((val = getenv(varName)) == NULL) {
                int i;

                /*
                 * If the undefined environment variable is in the Known table
                 * substitute the default value.
                 */

                for ( i=0 ; Known[i].envvar!=NULL ; i++ ) {
                    if ( strcmp(Known[i].envvar, varName) == 0 ) {
                        appendStr(vstr, Known[i].value);
                        break;
                    }
                }
            } else {
                appendStr(vstr, val);
            }

            mlFree(varName);

        } else if ( *curPtr == PERCENT ) {
            char fmt[3];
            char buf[32];
            time_t clock;

            fmt[0] = *curPtr++;
            fmt[1] = *curPtr++;
            fmt[2] = 0;
            if ( fmt[1] == 'n' ) {
                sprintf(buf,"%d",mle_filename_index);
            } else {
                clock = time(0);
                strftime(buf,32,fmt,localtime(&clock));
            }
            appendStr(vstr, buf);

        } else {
            appendChar(vstr, *curPtr);
            curPtr++;
        }
    }

    appendChar(vstr, NIL);
    newPtr = vstr->str;
    mlFree(vstr);
    return newPtr;
}

#ifdef WIN32

/*
 * XXX - may want to add password features in getHomeOfMe() and getHomeOfName()
 *       to match UNIX versions below.
 */

static char *
getHomeOfMe()
{
    char *home;
 
    if ((home = getenv("HOME")) != NULL) {
    return strdup(home);
    }
    home = (char *) mlMalloc(2);
    home[0] = TILDE;
    home[1] = NIL;
    return home;
}

static char *
getHomeOfName(char *name)
{
    char *home;

    home = (char *) mlMalloc(1 + strlen(name) + 1);
    home[0] = TILDE;
    strcpy(home + 1, name);
    return home;
}

#else /* ! WIN32 */

static char *
getHomeOfMe()
{
    char           *home;
    struct passwd  *pw;

    if ((home = getenv("HOME")) != NULL) {
        return strdup(home);
    }
    if ((pw = getpwuid(getuid())) != NULL) {
        return strdup(pw->pw_dir);
    }
    home = (char *) mlMalloc(2);
    home[0] = TILDE;
    home[1] = NIL;
    return home;
}


static char *
getHomeOfName(char *name)
{
    struct passwd  *pw;
    char           *home;

    if ((pw = getpwnam(name)) != NULL) {
        return strdup(pw->pw_dir);
    }
    home = (char *) mlMalloc(1 + strlen(name) + 1);
    home[0] = TILDE;
    strcpy(home + 1, name);
    return home;
}

#endif /* WIN32 */


/*
 * Variable length string routines
 */

static void appendStr(VStr *vstr, char *str)
{
    while (*str != NIL) {
        appendChar(vstr, *str);
        str++;
    }
}


#define ALLOC_INC 64

static void appendChar(VStr *vstr, char ch)
{
    if (vstr->used+1 >= vstr->allocked) {
        vstr->allocked += ALLOC_INC;
        if (vstr->str==NULL) {
            vstr->str = (char *)mlMalloc(vstr->allocked);
        } else {
            vstr->str = (char *)mlRealloc(vstr->str, vstr->allocked);
        }
    }
    vstr->str[vstr->used++] = ch;
    vstr->str[vstr->used] = NIL;
}


#ifdef UNIT_TEST

main ()
{
    char buf[1024];
    while (gets(buf) != NULL) {
        puts(mlFilenameExpand(buf));
    }
}

#endif /* UNIT_TEST */
