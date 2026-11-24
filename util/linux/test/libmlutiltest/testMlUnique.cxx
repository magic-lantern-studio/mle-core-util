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
#include <vector>

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/mlUnique.h"

namespace {

int compareInts(const void *a, const void *b) {
	return *(const int *)a - *(const int *)b;
}

// Compress the duplicates out of the list, and give the result.
std::vector<int> unique(std::vector<int> list) {
	int n = mlUnique(list.empty() ? NULL : &list[0], (int)list.size(), sizeof(int), compareInts);
	list.resize(n);
	return list;
}

} // namespace

TEST(MlUniqueTest, ListsWithoutDuplicatesAreNotChanged) {
	EXPECT_EQ(std::vector<int>(), unique({}));
	EXPECT_EQ(std::vector<int>({ 1 }), unique({ 1 }));
	EXPECT_EQ(std::vector<int>({ 1, 2, 3 }), unique({ 1, 2, 3 }));
}

TEST(MlUniqueTest, FirstOfEachValueIsKeptInOrder) {
	EXPECT_EQ(std::vector<int>({ 1, 2, 3 }), unique({ 1, 2, 3, 1, 2, 3, 1 }));
	EXPECT_EQ(std::vector<int>({ 1, 2 }), unique({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 }));
	EXPECT_EQ(std::vector<int>({ 1, 2, 3, 4, 5 }), unique({ 1, 2, 2, 3, 3, 4, 4, 5, 1 }));
	EXPECT_EQ(std::vector<int>({ 7 }), unique({ 7, 7 }));
	EXPECT_EQ(std::vector<int>({ 3, 1, 2 }), unique({ 3, 1, 3, 2, 1 }));
}

TEST(MlUniqueTest, ManyElementsWithRunsOfDuplicates) {
	// The duplicates are removed by moving the rest of the list down over
	// memory that overlaps, in runs of every length.
	std::vector<int> in, expected;
	for (int value = 0; value < 300; value++) {
		expected.push_back(value);
		for (int copies = 0; copies <= value % 7; copies++)
			in.push_back(value);
	}
	EXPECT_EQ(expected, unique(in));
}

TEST(MlUniqueTest, ElementsOfAnySize) {
	struct Pair { int key; char pad[13]; };
	Pair list[] = { { 1, {} }, { 2, {} }, { 1, {} }, { 3, {} }, { 2, {} } };
	int n = mlUnique(list, 5, sizeof(Pair), [](const void *a, const void *b) {
		return ((const Pair *)a)->key - ((const Pair *)b)->key; });
	ASSERT_EQ(3, n);
	EXPECT_EQ(1, list[0].key);
	EXPECT_EQ(2, list[1].key);
	EXPECT_EQ(3, list[2].key);
}
