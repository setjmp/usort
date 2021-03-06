/* c 2008 Andrew I. Schein */

/* reference:
 *
 * http://www.stereopsis.com/radix.html
 * (was possibly buggy wrt to prefetching)
 */

#include "s4_sort.h"
#include <string.h> /* __BYTE_ORDER */

#ifndef __BYTE_ORDER
#error __FILE__ ": __BYTE_ORDER is not defined!"
#endif
#if __BYTE_ORDER == __LITTLE_ENDIAN

#include <stdlib.h>
#define CSORT_TY int
#define CS_(name) s4_c##name
#include "../csort/csort.c"

#define _0(v) ((unsigned)(v)         & 0x7FF)
#define _1(v) (((unsigned)(v) >> 11) & 0x7FF)
#define _2(v) (((unsigned)(v) >> 22) ^ 0x200)
#define HIST_SIZE 2048

/* implements in place s4 radix sort. */

S4_SORT_LKG void s4_sort(int *a, const long sz) {
    long j;
    int pos;
    long n, sum0=0 , sum1=0 , sum2=0 , tsum=0;
    int *reader, *writer, *buf = (int*) malloc(sz * sizeof(int));
    size_t *b0, *b1, *b2;
    if (sz < 256) { free(buf); return s4_csort(a,sz); }
    b0   = malloc(HIST_SIZE * 3 * sizeof(size_t));
    b1   = b0 + HIST_SIZE;
    b2   = b1 + HIST_SIZE;
    
    memset(b0,0,3 * HIST_SIZE * sizeof(size_t));
    for (n=0; n < sz; n++) {
        b0[_0(a[n])]++; 
        b1[_1(a[n])]++; 
        b2[_2(a[n])]++; 
    }
    for (j = 0; j < HIST_SIZE; j++) { 
        tsum  = b0[j] + sum0;
        b0[j] = sum0 - 1;
        sum0  = tsum;
        
        tsum  = b1[j] + sum1;
        b1[j] = sum1 - 1;
        sum1  = tsum;
        
        tsum  = b2[j] + sum2;
        b2[j] = sum2 - 1;
        sum2  = tsum;
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
    writer = buf;
    reader = a;
    for (n=0; n < sz; n++) {
        pos = _2(reader[n]);
        writer[++b2[pos]] = reader[n];
    }
    memcpy(a,buf,sz * sizeof(int));
    free(buf);  
    free(b0);
}


#undef _0
#undef _1
#undef _2
#undef HIST_SIZE
#else
/* endian */
#define CS_(name) s4_## name 
#define CSORT_TY int
#include "../csort/csort.c"
#endif
