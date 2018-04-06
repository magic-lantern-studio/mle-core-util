/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlToken.h
 * @ingroup MleCore
 *
 * This file contains the type definitions to support the
 * Magic Lantern debugging utilities.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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


#ifndef __MLE_TOKEN_H_
#define __MLE_TOKEN_H_


/* Include Magic Lantern header files. */

typedef struct _MleDBTokenDataP {
    signed long _state;          /* state variable for token parsing */
    char   *_tokenText;          /* string content of current token  */
    signed long _tokenUsed;      /* number of bytes used for token string */
    signed long _tokenAllocated; /* number of bytes available for token string */
} MleDBTokenDataP;


/* Define tokens for parsing "component.category" string */
enum MleDBTokenTypeP {
    WORD, DOT, STAR, EQUAL, NUMERIC, DELIM, ATEOF, BOGUS
};

#define MLE_TOKEN_BUFSIZ    40


#endif /* __MLE_TOKEN_H_ */
