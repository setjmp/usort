/* c 2008 Andrew I. Schein.  All rights reserved.
   
   A fast general-purpose heap sort implementation.
   
   Caller defines a few macros:
   Required: HSORT_TY, HSORT_LT
   Recommended: 
   1. #define HS_(name) ty_##name, e.g. HS_(sort) -> u4_hsort
   To do: 
   1. handle keys within structs.
   2. define optional mode using comparison operators wrt a single CMP a la python, ocaml.
   
*/

#ifndef HSORT
#define HSORT
#if !defined LIBBING_AS_HSORT
#  define HSORT_LKG static inline
#else
#  define HSORT_LKG 
#endif

#ifndef HSORT_TY
#  error "hsort.c imported without HSORT_TY definition."
#endif


/* can redefine with type_ */
#ifndef HS_
#  define HS_(name) HS_##name
#endif

/* Comparisons... default to arithmatic */
#ifndef HSORT_EQ
#  define HSORT_EQ(a,b) (*(a) == *(b))
#endif
#ifndef HSORT_LT
#  define HSORT_LT(a,b) (*(a) < *(b))
#endif
#ifndef HSORT_LE
#  define HSORT_LE(a,b) (*(a) <= *(b))
#endif

#define HSORT_MIN(a,b) ((a) < (b) ? (a) : (b))

static inline void HS_(SWAP)(HSORT_TY *a, HSORT_TY *b) {
    /* made a function since arguments tend to be incremented by caller */
    HSORT_TY swap = *a;
    *a            = *b;
    *b            = swap;
}

static inline void HS_(siftdown)(HSORT_TY *a, const long start, const long end) {
    long child, root = start;
    while (root * 2 + 1 <= end) {
        child = root * 2 + 1;
        if ((child + 1) <= end && HSORT_LT(&a[child],&a[child+1])) ++child;           
        if (HSORT_LT(&a[root],&a[child])) HS_(SWAP)(&a[root],&a[child]), root=child; 
        else return;      
    }
}

static inline void HS_(heapify)(HSORT_TY *a, const long count) {
    long start = (count-2) / 2; 
    while ( start >= 0 ) HS_(siftdown)(a,start--,count-1);
}

static inline void HS_(sort)(HSORT_TY *a, const long count) {
    long end;
    HS_(heapify)(a, count);
    end = count - 1;
    while (end > 0) {
        HS_(SWAP)(&a[end], &a[0]);
        HS_(siftdown)(a, 0, end-1);
        --end;   
    }
}

#undef HS_
#undef HSORT_MIN
#undef HSORT_LKG 
#undef HSORT_LT
#undef HSORT_LE
#undef HSORT_EQ
#undef HSORT_TY
#endif
