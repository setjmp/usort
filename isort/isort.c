/* c 2008 Andrew I. Schein.  All rights reserved.
   A fast general-purpose insertion sort implemention.
*/

#include "../common/defs.c" /* comparisons


#if !defined LIBBING_AS_ISORT
#  define ISORT_LKG static inline
#else
#  define ISORT_LKG 
#endif


#ifndef IS_
#  define IS_(name) IS_##name
#endif

#ifndef ISORT_SWAP
/* swap is implemented as local macro to avoid pollution of name space */
#define ISORT_SWAP(y) { swap = *(y); *(y) = *((y)-1) ; *((y)-1) = swap; } 
#endif

#include <stdlib.h>

static inline void IS_(isort)(ISORT_TY* a, const long long n) {    
    ISORT_TY *x=a+1,*y=NULL,swap;
    for (;x < a + n; x++) {
        for ( y=x; y>a && ISORT_LT(y,(y-1)); y-- )
            ISORT_SWAP(y);
    }
}

#undef ISORT_LT
#undef ISORT_TY
#undef ISORT_SWAP
#undef IS_

