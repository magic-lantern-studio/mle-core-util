/** @defgroup MleCore Magic Lantern Core Library API */
/**
 *  @file mlGetopt.h
 *  @ingroup MleCore
 *
 *  Magic Lantern Core - Win32 specific definitions.
 *
 *  @author Mark S. Millard
 *  @date July 10, 2003
 *  @version 0.13
 */

// COPYRIGHT BEGIN
// COPYRIGHT END

#ifndef __MLE_CORE_GETOPT_H_
#define __MLE_CORE_GETOPT_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int optind;
extern TCHAR *optarg;

int getopt(int argc, TCHAR *argv[], TCHAR *optstring);

#ifdef __cplusplus
} /* end of 'extern "C"' */
#endif


#endif /* __MLE_CORE_GETOPT_H_ */
