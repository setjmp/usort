/* c 2008 Andrew I. Schein */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "s1_sort.h"
#define S1_HIST_SIZE 256
#define S1_HIST_MIN  -128
#define S1_HIST_MAX  127

#include "isort/ufunc/s1_isort.c"

/* implements in place u1 bucket sort. */

S1_SORT_LKG void s1_sort(char* restrict a, const long sz) {
    long j;
    long n;
    char *writer=a; 
    long b0[S1_HIST_SIZE];
    if (sz < 32) { return s1_isort(a,sz);}
    memset(b0,0,S1_HIST_SIZE * sizeof(long));
    for (n=0; n < sz; n++) {
        b0[(unsigned char)a[n]]++; 
    }
    
    for (j = S1_HIST_MIN; j <= S1_HIST_MAX; j++) { 
        while (b0[(unsigned char)j]-- > 0) { 
            *writer = j; 
            writer++; 
        }
    }
}

#undef REFRESH
#undef S1_HIST_SIZE
