/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file mlUnique.c
 * @ingroup MleCore
 *
 * This file contains utilities for maintaining a log file.
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <memory.h>

/*
 * mleUnique - return an array identical to the input but with duplicate entries
 * compressed out.
 */

int mlUnique(void *ap, int n, int size, int(*compare)(const void *, const void *))
{
    int i, j;

    for ( i = 0; i < n-1 ; i++ )
	{
		for ( j = i+1 ; j < n ; )
		{
			if ( compare(((char *)ap)+i*size, ((char *)ap)+j*size) == 0 )
			{
#if 0
				int k;
				for ( k = j+1 ; k < n ; k++ )
				{
				   memcpy(((char *)ap)+(k-1)*size, ((char *)ap)+k*size, size);
 				}
#endif
				memcpy(((char *)ap)+j*size, ((char *)ap)+(j+1)*size, (n-(j+1))*size);
				n--;
			} else {
				j++;
			}
		}
    }
    return n;
}

	    
#ifdef UNIT_TEST

//#include <unistd.h>
#include <stdio.h>

int cmp(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}

int in1[] = { 1 };
int out1[] = { 1 };
int in2[] = {1, 2, 3};
int out2[] = {1, 2, 3};
int in3[] = { 1,2,3,1,2,3,1 };
int out3[] = {1,2,3};
int in4[] = {1,1,1,1,1,1,1,1,1,2};
int out4[] = {1,2};
int in5[] = { 1,2,2,3,3,4,4,5,1};
int out5[]= { 1,2,3,4,5};

struct {
    int n0;
    int *a0;
    int n1;
    int *a1;
} testdata[] = {
    { 0, NULL, 0, NULL },
    { 1, in1, 1, out1},
    { 3, in2, 3, out2},
    { 7, in3, 3, out3},
    { 10, in4, 2, out4 },
    { 9, in5, 5, out5 },
    { -1 }
};

main(int argc, char **argv)
{
    int i, j, nn;
    int error;

    for ( i = 0 ; testdata[i].n0 >= 0 ; i++ )
	{
		error = 0;
		nn = mlUnique((void *)testdata[i].a0, testdata[i].n0, sizeof (int), cmp);
		if ( nn != testdata[i].n1 )
		{
			printf("FAIL\n");
			error++;
			continue;
		}
		for ( j=0 ; j<nn ; j++ )
		{
			if ( testdata[i].a0[j] != testdata[i].a1[j] ) 
			{
				printf("FAIL\n");
				error++;
				break;
			}
		}

		if ( error == 0 )
		{
			printf("PASS\n");
		}
    }
}

#endif /* UNITTEST */
