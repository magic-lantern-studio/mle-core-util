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

/**
 * Create a log file descriptor.
 *
 * @param category The name of a category for the logged item.
 * @param name The name of the item to log.
 * @param tag Additional tagging information for the logged item (i.e. timestamp).
 *
 * @return A pointer to a strucutre containing the logging descriptor is returned.
 */
MleLogFileP *mlLogFileCreate(const char *category,const char *name,const char *tag);

/**
 * Delete a log file descriptor.
 *
 * @param A pointer to the log file descriptor to delete.
 */
void mlLogFileDelete(MleLogFileP *logFile);

/**
 * Open a log file.
 *
 * @param A pointer to the descriptor for the log file.
 *
 * @return If the log file is successfully opened, then <b>TRUE</b is returned.
 * Otherwise, <b>FALSE</b> will be returned.
 */
MlBoolean mlLogFileOpen(MleLogFileP *logFile);

/**
 * Close a log file.
 *
 * @param logFile A pointer to a descriptor for the log file to close.
 *
 * @return If the log file is successfully opened, then <b>TRUE</b is returned.
 * Otherwise, <b>FALSE</b> will be returned.
 */
MlBoolean mlLogFileClose(MleLogFileP *logFile);

/**
 * Set the directory location for the log file.
 *
 * @param logFile A pointer to the descriptor for the log file.
 * @param dir The directory location to set.
 */
void mlLogFileSetDirectory(MleLogFileP *logFile,const char *dir);

/**
 * Set the tag on the log file.
 *
 * @param logFile A pointer to the log file descriptor.
 * @param tag The tag to set.
 */
void mlLogFileSetTag(MleLogFileP *logFile,const char *tag);

/**
 * Print an item to the log file.
 *
 * @param logFile A pointer to the log file descriptor.
 * @param format The item that will be printed to the log file.
 *
 */
void mlLogFilePrintf(MleLogFileP *logFile,const char *format, ...);

/**
 * Dump the contents of memory in HEX.
 *
 * @param ptr A pointer to the location in memory to dump.
 * @param cnt The number of bytes to dump.
 */
void mlLogFileHexDump(const char *ptr,const int cnt);

#ifdef __cplusplus
}
#endif


#endif /* __MLE_LOGFILE_H_ */
