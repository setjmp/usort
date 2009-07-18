/* c 2008 Andrew I. Schein.  All rights reserved.
   
   A fast general-purpose sorting smplemention based on Bentley and McIlroy, 1993's discussion
   of quicksort.
   
   Caller defines a few macros:
   Required: QSORT_TY, QSORT_LT
   Recommended: 
   1. #define QS_(name) ty_##name, e.g. QS_(sort) -> u4_qsort
   To do: 
   1. handle keys within structs.
   2. define optional mode using comparison operators wrt a single CMP a la python, ocaml.
   
   This implementation achieves a speedup compared to GLIBC 2.7 qsort by using the following
   strategies:

   1. Static definitions of comparions ensure inlining of this operation.  This is accomplished
   through C macros.
   2. Switch to insertion sort (GLIBC does this much later in the recursion tree than my impl.)
   3. Smart treatment of duplicate pivots.  Useful for arrays containing very low entropy elements.
   
   In addition, this implementation differs from GLIBC 2.7 qsort in the use of recursion.
   GLIB 2.7 manually manages a stack in order to bypass the more conventional decision of using 
   the C runtime stack.  My implementation allows the C runtime to manage the
   recursion stack for the non-tail first recursion call of quicksort, but performs the second
   recursion in tail fashion by performing a goto. 

   Something this implementation shares with the GLIBC implementation is recursing on the smaller 
   partition first.  This ensures a O(N*log(N)) bound on memory usage. Another commonality is the
   use of a smart median selection procedure the, "Tukey Ninther" method.
   
   The speedup compared to GLIBC is documented through a test harness located in the same 
   directory as this file.

*/

#ifndef QSORT
#define QSORT
#if !defined LIBBING_AS_QSORT
#  define QSORT_LKG static inline
#else
#  define QSORT_LKG 
#endif

#ifndef QSORT_TY
#  error "qsort.c imported without QSORT_TY definition."
#endif

/* can redefine with type_ */
#ifndef QS_
#  define QS_(name) QS_##name
#endif

#include "swap.c" 
/* smaller than this value => insertion sort */
#define QSORT_ISORT_SWITCH 32
#define QSORT_NINTHER_SWITCH 64

/* Comparisons... default to arithmatic */
#ifndef QSORT_EQ
#  define QSORT_EQ(a,b) (*(a) == *(b))
#endif
#ifndef QSORT_LT
#  define QSORT_LT(a,b) (*(a) < *(b))
#endif
#ifndef QSORT_LE
#  define QSORT_LE(a,b) (*(a) <= *(b))
#endif

#define QSORT_MIN(a,b) ((a) < (b) ? (a) : (b))

/* implements median of 3 "the ninther." Argumments are addresses. */
#define QSORT_NINTHER(a,b,c)                                            \
    (QSORT_LT((a),(b)) ? (QSORT_LT((b),(c)) ? (b) :                     \
                          QSORT_LT((a),(c)) ? (c) : (a))                \
     : (QSORT_LT((c),(b)) ? (b) : QSORT_LT((c),(a)) ? (c) : (a)))


static inline void QS_(SWAP)(QSORT_TY *a, QSORT_TY *b) {
    /* made a function since arguments tend to be incremented by caller */
    QSORT_TY swap = *a;
    *a            = *b;
    *b            = swap;
}

static inline void QS_(isort)(QSORT_TY* a, const long long len) {    
    QSORT_TY *x=a+1,*y;
    for (;x < a + len;x++)
        for ( y=x; y>a && QSORT_LT(y,(y-1)); y-- )
            QS_(SWAP)(y,y-1);
}

static inline void QS_(sort)(QSORT_TY *x, const long long orig_n) {
    long long n = orig_n,s;
    long long ty_sz; /* ,l,h; */ 
    QSORT_TY *p0,*pm,*p1;
    QSORT_TY *a,*b,*c,*d; /* ,*t; */ /* indices within array */
    QSORT_TY pivot;
 ssort_start:
    if (n < 0) fprintf(stderr,"sort error: n < 0: %lld\n",n),exit(1);
    if (n <= QSORT_ISORT_SWITCH) return QS_(isort)(x,n);
    s=(n>>3), ty_sz=sizeof(QSORT_TY);
    p0=x;pm=x+(n>>1);p1=x+n-1; /* pivot candidates 0,1 from calculus, m for median */
    if (n >= QSORT_NINTHER_SWITCH) {
        p0    = QSORT_NINTHER(p0    , p0+s, p0+2*s);
        pm    = QSORT_NINTHER(pm-s  , pm  , pm+s);
        p1    = QSORT_NINTHER(p1-2*s, p1-s, p1);    
    } 
    pm    = QSORT_NINTHER(p0,pm,p1); /* now pm contains the pivot */
    pivot = *pm;
    a     = b = x;
    c     = d = x + (n-1);
    for (;;) { 
        while (b <= c && QSORT_LE(b, &pivot)) { 
            if (QSORT_EQ(b,&pivot)) QS_(SWAP)(a++,b);  /* repeated pivots treated separately */
            b++; 
        }  
        while (c >= b && QSORT_LE(&pivot,c)) { 
            if (QSORT_EQ(c,&pivot)) QS_(SWAP)(d--,c);  
            c--; 
        }
        if (b > c) break;
        QS_(SWAP)(b++,c--);
    }
    s = QSORT_MIN(a-x,b-a); /* repeat pivot movement */
    //for (l=0,h=(b-x)-s; s ; s--) QS_(SWAP)(&x[l++],&x[h++]); 
    swap(x , b - s     , s * sizeof(QSORT_TY));
    s = QSORT_MIN(d-c, (x + n - 1) - d);
    //for (l=b-x,h=n-s;s;s--) QS_(SWAP)(&x[l++],&x[h++]);
    swap(b, x + (n - s), s * sizeof(QSORT_TY));
    if ((b-a) < n-(d-c)) {  /* recurse on smaller first to bound memory usage. */
        if ((b-a) > 1) QS_(sort)(x, (b-a));
        if ((n-(d-c)) > 1) { /* avoid procedure call on second recursion. */
            x = x+n-(d-c);
            n = d-c;
            goto ssort_start;
        }
    }
    else {
        if ((d-c) > 1) QS_(sort)(x + n-(d-c), d-c);
        if ((b - a) > 1) {
            n = (b-a); 
            goto ssort_start; /* avoid procedure call on second recursion. */
        }
    }
}

#undef QS_
#undef QSORT_MIN
#undef QSORT_LKG 
#undef QSORT_LT
#undef QSORT_LE
#undef QSORT_EQ
#undef QSORT_TY
#undef QSORT_SWITCH
#undef QSORT_NINTHER
#endif
