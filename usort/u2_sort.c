/* c 2008 Andrew I. Schein */

/* reference:
 *
 * http://www.stereopsis.com/radix.html
 * (was possibly buggy wrt to prefetching)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "u2_sort.h"

#include "isort/ufunc/u2_isort.c"

#define _0(v) ((v)         & 0xFF)
#define _1(v) (((v) >> 8)  & 0xFF)
#define HIST_SIZE 256

/* implements in place u4 radix sort. */

U2_SORT_LKG void u2_sort(unsigned short* a, const long sz) {
    long j;
    unsigned pos;
    long n, sum0=0 , sum1=0 , tsum=0;
    unsigned short *reader, *writer, *buf;
    long *b0, *b1;
    if (sz < 32) { return u2_isort(a,sz);}
    buf = (unsigned short*) malloc(sz * sizeof(unsigned short));
    b0   = calloc(HIST_SIZE * 2, sizeof(long));
    b1   = b0 + HIST_SIZE;
    
    for (n=0; n < sz; n++) {
        b0[_0(a[n])]++; 
        b1[_1(a[n])]++; 
    }
    for (j = 0; j < HIST_SIZE; j++) { 
        tsum  = b0[j] + sum0;
        b0[j] = sum0 - 1;
        sum0  = tsum;
        
        tsum  = b1[j] + sum1;
        b1[j] = sum1 - 1;
        sum1  = tsum;
    }   
    writer = buf;
    reader = a;
    for (n=0; n < sz; n++) {
        pos = _0(reader[n]);
        writer[++b0[pos]] = reader[n];
    }
    writer = a;
    reader = buf;
    for (n=0; n < sz; n++) {
        pos = _1(reader[n]); 
        writer[++b1[pos]] = reader[n];
    }
    free(buf);  
    free(b0);
}

#undef REFRESH
#undef _0
#undef _1
#undef _2
#undef HIST_SIZE
