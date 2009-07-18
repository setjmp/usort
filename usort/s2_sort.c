/* c 2008 Andrew I. Schein */

/* reference:
 *
 * http://www.stereopsis.com/radix.html
 * (was possibly buggy wrt to prefetching)
 */

#include "s2_sort.h"
#include <string.h> /* __BYTE_ORDER */

#ifndef __BYTE_ORDER
#error __FILE__ ": __BYTE_ORDER is not defined!"
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
#include <stdlib.h>

#include "isort/ufunc/s2_isort.c"

#define _0(v) (unsigned) ((v)         & 0xFF)
#define _1(v) (unsigned) (( ((v) >> 8)  & 0xFF) ^ 0x80)
#define HIST_SIZE 256


/* implements in place u4 radix sort. */

S2_SORT_LKG void s2_sort(signed short *a, const long sz) {
    long j;
    unsigned pos;
    long n, sum0=0 , sum1=0 , tsum=0;
    signed short *reader, *writer, *buf;
    long *b0, *b1;
    if (sz < 32) { return s2_isort(a,sz);}
    buf  = (signed short*) malloc(sz * sizeof(signed short));
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

#undef _0
#undef _1
#undef HIST_SIZE

#else /* endian */
#define QS_(name) s2_## name 
#define QSORT_TY short
#include "../qsort/qsort.c"
#endif
