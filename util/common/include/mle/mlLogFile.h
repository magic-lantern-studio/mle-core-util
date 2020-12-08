/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlLogFile.h
 * @ingroup MleCore
 *
 * This file contains the definition of the Magic Lantern
 * logging utilities.
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

#ifndef __MLE_LOGFILE_H_
#define __MLE_LOGFILE_H_


/* Include system header files. */
#include <stdio.h>
#include <sys/types.h>

/* Include Magic Lantern header files. */
#include "mle/mlTypes.h"


/**
 * A private data structure for MleLogFile.
 */
typedef struct _MleLogFileP
{
    MlBoolean   m_isOpen;    /**< Flag indicating whether log is open. */
    char       *m_name;      /**< Name of log item. */
    char       *m_tag;       /**< Tag for log item. */
    char       *m_directory; /**< Directory where log file is created/opened. */
    char       *m_category;  /**< Category/class of log item. */
    int         m_fd;        /**< File descriptor for log file. */
#if defined(WIN32)
    int         m_pid;       /**< Process id which is logging file. */
#else
    pid_t       m_pid;       /**< Process id which is logging file. */
#endif /* WIN32 */
} MleLogFileP;


#ifdef __cplusplus
extern "C" {
#endif

#define mlLogFileGetName(logFile) (logFile->m_name)
#define mlLogFileGetTag(logFile) (logFile->m_tag)
#define mlLogFileGetDirectory(logFile) (logFile->m_directory)
#define mlLogFileGetCategory(logFile) (logFile->m_category)
#define mlLogFileGetFd(logFile) (logFile->m_fd)
#define mlLogFileGetPid(logFile) (logFile->m_pid)


void _mlLogFileInit(MleLogFileP *,const char *,const char *,const char *);
void _mlLogFileDeinit(MleLogFileP *);

MleLogFileP *mlLogFileCreate(const char *,const char *,const char *);
void mlLogFileDelete(MleLogFileP *);
MlBoolean mlLogFileOpen(MleLogFileP *);
MlBoolean mlLogFileClose(MleLogFileP *);
void mlLogFileSetDirectory(MleLogFileP *,const char *);
void mlLogFileSetTag(MleLogFileP *,const char *);
void mlLogFilePrintf(MleLogFileP *,const char *, ...);
void mlLogFileHexDump(const char *,const int);

#ifdef __cplusplus
}
#endif


#endif /* __MLE_LOGFILE_H_ */
