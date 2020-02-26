/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlItoa.h
 * @ingroup MleCore
 *
 * @author Mark S. Millard
 * @date November 11, 2017
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Wizzer Works
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

#ifndef __MLE_ITOA_H_
#define __MLE_ITOA_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Convert integer to string.
 *
 * Converts an integer value to a null-terminated string using the
 * specified base and stores the result in the array given by str parameter.
 *
 * If base is 10 and value is negative, the resulting string is preceded
 * with a minus sign (-). With any other base, value is always considered
 * unsigned.
 *
 * str should be an array long enough to contain any possible value:
 * (sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms
 * and 33 in 32-bits platforms.
 *
 * @param value Value to be converted to a string.
 * @param str Array in memory where to store the resulting null-terminated
 * string.
 * @param base Numerical base used to represent the value as a string,
 * between 2 and 36, where 10 means decimal base, 16 hexadecimal,
 * 8 octal, and 2 binary.
 *
 * @return A pointer to the resulting null-terminated string,
 * same as parameter str.
 */
char* mlItoa(int value, char* result, int base);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MLE_ITOA_H_ */
