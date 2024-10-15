/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlMacros.h
 * @ingroup MleCore
 *
 * This file includes useful parameterized macros.
 * It is designed to be shared by both C and C++ programs.
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


#ifndef __MLE_MACROS_H_
#define __MLE_MACROS_H_

/**
 * A macro for determining the minimum of two values.
 *
 * @param x The first value for comparison.
 * @param y The second value for comparison.
 *
 * @return The maximum of the two specified values
 * is selected.
 */
#define MLE_MIN(x,y) ((x) < (y) ? (x) : (y))

/**
 * A macro for determining the maximum of two values.
 *
 * @param x The first value for comparison.
 * @param y The second value for comparison.
 *
 * @return The maximum of the two specified values
 * is selected.
 */
#define MLE_MAX(x,y) ((x) > (y) ? (x) : (y))

/**
 * A macro for determining if the value is odd.
 *
 * @param x The value to test.
 *
 * @return TRUE will be returned if the value is odd.
 * Otherwise, FALSE will be returned.
 */
#define MLE_ODD(x) (x & 0x0001)

/**
 * A macro for determining if the value is even.
 *
 * @param x The value to test.
 *
 * @return TRUE will be returned if the value is even.
 * Otherwise, FALSE will be returned.
 */
#define MLE_EVEN(x) (! (x & 0x0001))

/**
 * A macro for obtaining the absolute of the specified value.
 *
 * @param val The value to determine its absolute.
 *
 * @return The absolute of the specified value is returned.
 */
#define MLE_ABS(val) ((val >= 0) ? (val) : (-(val)))

/**
 * A macro for rounding a floating-point value.
 *
 * @param fnum The floating-point number to round off.
 *
 * @return An integer representation of the floating-point value
 * is returned.
 */
#define MLE_ROUND(fnum) ((int) ((fnum) < 0.0 ? (fnum) - 0.5 : (fnum) + 0.5))


/**
 * A macro for determining the minimum of two values.
 *
 * @param x The first value for comparison.
 * @param y The second value for comparison.
 *
 * @return The maximum of the two specified values
 * is selected.
 */
#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif /* MIN */

/**
 * A macro for determining the maximum of two values.
 *
 * @param x The first value for comparison.
 * @param y The second value for comparison.
 *
 * @return The maximum of the two specified values
 * is selected.
 */
#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif /* MAX */

/**
 * A macro for determining if a value occurs in the range of
 * two values.
 *
 * @param a The value to test.
 * @param x The lower-bound value in the range for comparison.
 * @param y The higherbound value in the range for comparison.
 *
 * @return If the specified value occurs in the specified range
 * then TRUE will be returned. Otherwise, FALSE will be returned.
 */
#ifndef RANGE
#define	RANGE(a, b1, b2)        \
						        \
        ((b1)<(b2)?				\
	     ((a)<(b1)?				\
		  (b1):				    \
		  ((a)>(b2)?			\
		       (b2):(a))):		\
	     ((a)<(b2)?				\
		  (b2):				    \
		  ((a)>(b1)?			\
		       (b1):(a))))
#endif /* RANGE */

#ifndef PI
/**
 * A macro for the Magic Lantern constant, PI
 */
#define PI 3.14159265358979323846
#endif /* PI */

#if 0
#ifndef offsetof
/**
 * A macro providing a convenient way to offset into a
 * strucure or class.
 */
#define offsetof(type, member) (&(((type*)(0))->member))
#endif
#endif /* 0 */

#endif /* __MLE_MACROS_H_ */
