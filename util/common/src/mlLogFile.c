/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlLogFile.c
 * @ingroup MleCore
 *
 * This file contains utilities for maintaining a log file.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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

/* Include system header files. */
#include <stdio.h>
#include <stdarg.h>
#ifdef WIN32
#include <string.h>
#include <process.h>
#else
#include <string.h>
#include <strings.h>
#endif /* WIN32 */
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

/* Include Magic Lantern header files. */
#include "mle/mlLogFile.h"
#include "mle/mlMalloc.h"
#include "mle/mlFileio.h"
#include "mle/mlAssert.h"


/* Declare static variables. */
#if defined(__linux__) || defined(__APPLE__)
//static char *g_directory = "/usr/tmp";
const char *defaultDirectory = "/opt/MagicLantern/log";
#else
const char *defaultDirectory = "C:/WINDOWS/Temp";
#endif


void
_mlLogFileInit(
    MleLogFileP *logFile,
    const char *category,
    const char *name,
    const char *tag)
{
    MLE_ASSERT(logFile != NULL);
    MLE_ASSERT(category != NULL);
    MLE_ASSERT(name != NULL);
    MLE_ASSERT(tag != NULL);

    logFile->m_isOpen = FALSE;
#ifdef WIN32
    logFile->m_directory = (char *)_strdup(defaultDirectory);
    logFile->m_category = (char *)_strdup(category);
    logFile->m_name = (char *)_strdup(name);
    if (tag != NULL)
        logFile->m_tag = (char *)_strdup(tag);
    else
        logFile->m_tag = (char *)_strdup("??");
	logFile->m_pid = _getpid();
#else
    logFile->m_directory = (char *)strdup(defaultDirectory);
    logFile->m_category = (char *)strdup(category);
    logFile->m_name = (char *)strdup(name);
    if (tag != NULL)
        logFile->m_tag = (char *)strdup(tag);
    else
        logFile->m_tag = (char *)strdup("??");
    logFile->m_pid = getpid();
#endif /* WIN32 */
}


void
_mlLogFileDeinit(MleLogFileP *logFile)
{
    MLE_ASSERT(logFile != NULL);

    mlLogFileClose(logFile);
    if (logFile->m_directory) {
        mlFree(logFile->m_directory);
        logFile->m_directory = NULL;
    }
    if (logFile->m_category) {
        mlFree(logFile->m_category);
        logFile->m_category = NULL;
    }
    if (logFile->m_name) {
        mlFree(logFile->m_name);
        logFile->m_name = NULL;
    }
    if (logFile->m_tag) {
        mlFree(logFile->m_tag);
        logFile->m_tag = NULL;
    }
}


MleLogFileP
*mlLogFileCreate(
    const char *category,
    const char *name,
    const char *tag)
{
    /* Declare local variables. */
    MleLogFileP *logFile;

    MLE_ASSERT(category != NULL);
    MLE_ASSERT(name != NULL);
    MLE_ASSERT(tag != NULL);

    logFile = (MleLogFileP *)mlMalloc(sizeof(MleLogFileP));
    if (logFile)
        _mlLogFileInit(logFile,category,name,tag);

    return(logFile);
}


void
mlLogFileDelete(MleLogFileP *logFile)
{
    MLE_ASSERT(logFile != NULL);

    _mlLogFileDeinit(logFile);
}


void
mlLogFileSetDirectory(MleLogFileP *logFile,const char *dir)
{
    MLE_ASSERT(logFile != NULL);
    MLE_ASSERT(dir != NULL);

    if (logFile->m_directory)
        mlFree(logFile->m_directory);
#ifdef WIN32
    logFile->m_directory = (char *)_strdup(dir);
#else
    logFile->m_directory = (char *)strdup(dir);
#endif
}


void
mlLogFileSetTag(MleLogFileP *logFile,const char *tag)
{
    MLE_ASSERT(logFile != NULL);
    MLE_ASSERT(tag != NULL);

    if (logFile->m_tag != NULL)
	mlFree(logFile->m_tag);
#ifdef WIN32
    logFile->m_tag = (char *)_strdup(tag);
#else
    logFile->m_tag = (char *)strdup(tag);
#endif
}


MlBoolean
mlLogFileOpen(MleLogFileP *logFile)
{
    /* Declare local variables. */
    char *fileName;
#ifdef WIN32
    int mode = _umask(0);
#else
    mode_t mode = umask(0);
#endif /* WIN32 */
    MlBoolean retValue = TRUE;

    MLE_ASSERT(logFile != NULL);

    /* Check if already open. */
    if (! logFile->m_isOpen) {

        /* Allocate space for file name ... */
        fileName = (char *)mlMalloc(
            strlen(logFile->m_directory)+1+strlen(logFile->m_category)+1);

        if (fileName) {
            /* ... and build it. */
            sprintf(fileName,"%s/%s",logFile->m_directory,logFile->m_category);

            /* Open file. */
            logFile->m_fd = mlOpen(fileName,O_WRONLY|O_APPEND|O_CREAT,0666);
            if (logFile->m_fd >= 0)
                logFile->m_isOpen = TRUE;
            else
                return(FALSE);
        } else
            return(FALSE);
    }

#ifdef WIN32
    _umask(mode);
#else
    umask(mode);
#endif /* WIN32 */
    return(retValue);
}


MlBoolean
mlLogFileClose(MleLogFileP *logFile)
{
    /* Declare local variables. */
    MlBoolean retValue = TRUE;

    MLE_ASSERT(logFile != NULL);

    /* Check if already open. */
    if (logFile->m_isOpen) {
        mlClose(logFile->m_fd);
        logFile->m_isOpen = FALSE;
    } else
        retValue = FALSE;

    return(retValue);
}


static char *monthNames[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

void
mlLogFilePrintf(MleLogFileP *logFile,const char *format, ...)
{
    /* Declare local variables. */
    va_list args;
    char buf[BUFSIZ],*nlp;
    time_t clock;
    struct tm *tm;

    MLE_ASSERT(logFile != NULL);
    MLE_ASSERT(format != NULL);

    /* Check if already open. */
    if (! logFile->m_isOpen) return;

    /* Build output string. */
    clock = time((time_t *)0);
    tm = localtime(&clock);
    sprintf(buf,"%s %02d %2d:%02d:%02d %s %s[%05d]: ",
        monthNames[tm->tm_mon],tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec,
        logFile->m_tag,logFile->m_name,logFile->m_pid);
    va_start(args,format);
    vsprintf(buf+strlen(buf),format,args);
    va_end(args);
    while ((nlp = strchr(buf,'\n')) != NULL ) {
        *nlp = ' ';
    }
    while ((nlp = strchr(buf,'\r')) != NULL ) {
        *nlp = ' ';
    }
    strcat(buf,"\n");
    mlWrite(logFile->m_fd,buf,strlen(buf));
}


void
mlLogFileHexDump(const char *ptr,const int cnt)
{
    /* Declare local variables. */
    char ascii[17];
    char hex[16*3+1+1];
    int i,j;

    MLE_ASSERT(ptr != NULL);
    MLE_ASSERT(cnt > 0);

    printf("------  %d bytes at 0x%p\n",cnt,ptr);
    for (i = 0;i < cnt;i += 16) {
        for (j = 0;j < 16;j++) {
            ascii[j] = ' ';
        }
        ascii[16] = 0;
        for (j = 0;j < 16;j++) {
            if (i+j >= cnt) {
                break;
            }
            sprintf(hex+(j*3)+j/8, "%02x ", ptr[i+j]);
            if (j == 7) {
                strcat(hex, " ");
            }
            if (isprint(ptr[i+j])) {
                ascii[j] = ptr[i+j];
            } else {
                ascii[j] = '.';
            }
        }
        printf("%06d  %-*.*s  %s\n",i,(int)sizeof(hex),(int)sizeof(hex),hex,ascii);
    }
}


#ifdef UNIT_TEST

#define CATEGORY "mleLogTest"
#define TAG      "test_c"


main(int argc,char *argv[])
{
    /* Declare local variables */
    int i;
    MleLogFileP *logFile;

    /* Initialize the test. */
    fprintf(stdout,"Testing Magic Lantern Log Module ...\n");

    /* Do the test. */
    logFile = mlLogFileCreate(CATEGORY,argv[0],TAG);
    MLE_ASSERT(logFile != NULL);
    mlLogFileSetDirectory(logFile,".");
    mlLogFileOpen(logFile);
    mlLogFilePrintf(logFile,"\tTesting mleLogFilePrintf() ...\n");
    for (i = 0; i < 10; i++)
        mlLogFilePrintf(logFile,"\tTest %d\n",i);
    mlLogFileClose(logFile);
    mlLogFileDelete(logFile);

    /* Complete the test. */
    fprintf(stdout,"Magic Lantern Log Test Complete.\n");
}

#endif /* UNIT_TEST */
