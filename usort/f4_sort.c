/* c 2009 Andrew I. Schein */

/* reference:
 *
 * http://www.stereopsis.com/radix.html
 * (was possibly buggy wrt to prefetching)
 */

/* f4 must not use strict aliasing */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "f4_sort.h"
#include "../qsort/ufunc/f4_qsort.c"

#define _0(v) ( (v)         & 0x7FF)
#define _1(v) (((v)  >> 11) & 0x7FF)
#define _2(v) ( (v)  >> 22)
#define F4_SORT_HIST_SIZE 2048

static inline unsigned f4_sort_FloatFlip(unsigned f) {
    unsigned mask       =  -(f >> 31) | 0x80000000 ;
    return                  (f ^ mask);
} 

static inline unsigned f4_sort_IFloatFlip(unsigned f) {
    unsigned mask       =  ((f >> 31) - 1) | 0x80000000;
    return                  (f ^ mask);
}

F4_SORT_LKG void f4_sort(float *a, const long sz) {
    long j;
    unsigned long pos;
    long n, sum0=0 , sum1=0 , sum2=0 , tsum=0;
    unsigned *reader, *writer, *buf1=(unsigned*)a, *buf2;
    unsigned long b0[F4_SORT_HIST_SIZE*3], *b1, *b2;
    if (sz < 256) { f4_qsort(a,sz); return; } 
    buf2 = (unsigned* ) malloc(sz * sizeof(unsigned));
    
    b1 = b0 + F4_SORT_HIST_SIZE;
    b2 = b1 + F4_SORT_HIST_SIZE;
    memset(b0,0,F4_SORT_HIST_SIZE*3*sizeof(unsigned long));
        
    //fprintf(stderr,"starting: %f g%f\n", a[0], a[1]);
    for (n=0; n < sz; n++) {
        
        //fprintf(stdout,"flip test: %u\t%u\t%u\n", (signed) blob.u, (signed)f4_sort_FloatFlip(blob.u), 
        //        (signed) f4_sort_IFloatFlip(f4_sort_FloatFlip(blob.u)));
        //assert(blob.u == (unsigned)f4_sort_IFloatFlip(f4_sort_FloatFlip(blob.u)));
        
        buf1[n] = f4_sort_FloatFlip(a[n]);
        //fprintf(stderr,"a[%ld] as u: %u as f: %f\n", n, blob.u, blob.f);
        //fprintf(stderr,"_ res: %u %u %u\n", _0(blob.u), _1(blob.u), _2(blob.u));
        b0[_0(buf1[n])]++; 
        b1[_1(buf1[n])]++; 
        b2[_2(buf1[n])]++; 
    }
    
    //fprintf(stderr,"a[0]: %f a[1]: %f\n", a[0], a[1]);
    //fprintf(stderr,"here is b0: %lu %lu\n",b0[0],b0[1]);
    //fprintf(stderr,"here is b1: %lu %lu\n",b1[0],b1[1]);
    //fprintf(stderr,"here is b2: %lu %lu\n",b2[0],b2[1]);
    
    for (j = 0; j < F4_SORT_HIST_SIZE; j++) { 
        tsum  = b0[j] + sum0;
        b0[j] = sum0 - 1;
        sum0  = tsum;
        tsum  = b1[j] + sum1;
        b1[j] = sum1 - 1;
        sum1  = tsum;
        tsum  = b2[j] + sum2;
        //fprintf(stderr,"j: %ld b2[j]: %ld\n", j , sum2 - 1);
        b2[j] = sum2 - 1;
        sum2  = tsum;
    }   
    
    //fprintf(stderr,"after hist here is b2: %ld %ld\n",b2[0],b2[1]);
    writer = buf2;  /* byte 0 */
    reader = buf1;
    for (n=0; n < sz; n++) {
        pos = _0(reader[n]);
        writer[++b0[pos]] = reader[n];
    }
    
    writer = buf1;    /* byte 1 */
    reader = buf2;
    for (n=0; n < sz; n++) {
        pos = _1(reader[n]); 
        writer[++b1[pos]] = reader[n];
    }
    //fprintf(stderr,"hist1: %f %f\n",writer[0],writer[1]);
    //fprintf(stderr,"here is b2: %lu %lu\n",b2[0],b2[1]);
    
    writer = buf2;  /* byte 2 */
    reader = buf1;
    for (n=0; n < sz; n++) {
        pos = _2(reader[n]);
        writer[++b2[pos]] = f4_sort_IFloatFlip(reader[n]);
    }
    memcpy(a,writer,sz * sizeof(float));
    free(buf2);  
    
}

#undef F4_SORT_HIST_SIZE
#undef _0
#undef _1
#undef _2
