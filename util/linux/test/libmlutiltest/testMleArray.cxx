// COPYRTIGH_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2022 - 2025 Wizzer Works
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

// Include system header files.
#include <string.h>
#include <sstream>

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlArray.h"

TEST(MleArrayTest, Empty) {
	MleArray<int> array;
	EXPECT_EQ(0, array.size());
}

TEST(MleArrayTest, SizedConstructor) {
	MleArray<int> array(5);
	ASSERT_EQ(5, array.size());
	for (int i = 0; i < 5; i++)
		array[i] = i * 10;
	EXPECT_EQ(0, array[0]);
	EXPECT_EQ(40, array[4]);
}

TEST(MleArrayTest, ZeroSizedConstructor) {
	MleArray<int> array(0);
	EXPECT_EQ(0, array.size());
}

TEST(MleArrayTest, PointerConstructorTakesOwnership) {
	// The memory must come from mlMalloc(). The array frees it.
	int *data = (int *) mlMalloc(3 * sizeof(int));
	data[0] = 1; data[1] = 2; data[2] = 3;
	MleArray<int> array(3, data);
	ASSERT_EQ(3, array.size());
	EXPECT_EQ(2, array[1]);
	EXPECT_EQ(data + 2, array + 2);
}

TEST(MleArrayTest, CopyConstructorCopiesElements) {
	MleArray<int> source(3);
	source[0] = 1; source[1] = 2; source[2] = 3;

	MleArray<int> copy(source);
	ASSERT_EQ(3, copy.size());
	EXPECT_NE(source + 0, copy + 0);
	copy[0] = 99;
	EXPECT_EQ(1, source[0]);
	EXPECT_EQ(99, copy[0]);
}

TEST(MleArrayTest, CopyOfEmptyArray) {
	MleArray<int> source;
	MleArray<int> copy(source);
	EXPECT_EQ(0, copy.size());
}

TEST(MleArrayTest, AssignmentCopiesElements) {
	// The arrays must not share memory, or both would free it.
	MleArray<int> source(3);
	source[0] = 1; source[1] = 2; source[2] = 3;

	MleArray<int> target(10);
	target = source;
	ASSERT_EQ(3, target.size());
	EXPECT_NE(source + 0, target + 0);
	target[1] = 99;
	EXPECT_EQ(2, source[1]);
	EXPECT_EQ(99, target[1]);
}

TEST(MleArrayTest, AssignmentFromEmpty) {
	MleArray<int> source;
	MleArray<int> target(4);
	target = source;
	EXPECT_EQ(0, target.size());
}

TEST(MleArrayTest, SelfAssignmentKeepsElements) {
	MleArray<int> array(2);
	array[0] = 5; array[1] = 6;
	MleArray<int> &same = array;
	array = same;
	ASSERT_EQ(2, array.size());
	EXPECT_EQ(5, array[0]);
	EXPECT_EQ(6, array[1]);
}

TEST(MleArrayTest, ResizeKeepsElements) {
	MleArray<int> array(2);
	array[0] = 7; array[1] = 8;
	ASSERT_TRUE(array.resize(100) != NULL);
	EXPECT_EQ(100, array.size());
	EXPECT_EQ(7, array[0]);
	EXPECT_EQ(8, array[1]);
}

TEST(MleArrayTest, IndexAtSizeGrowsTheArray) {
	// Indexing the first element past the end must make room for it.
	MleArray<int> array(2);
	array[0] = 1; array[1] = 2;
	array[2] = 3;
	EXPECT_EQ(3, array.size());
	EXPECT_EQ(3, array[2]);
	EXPECT_EQ(1, array[0]);
}

TEST(MleArrayTest, Print) {
	MleArray<int> array(3);
	array[0] = 1; array[1] = 2; array[2] = 3;
	std::ostringstream out;
	out << array;
	EXPECT_EQ("1 2 3 ", out.str());
}
