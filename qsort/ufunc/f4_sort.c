/* the <= and == tests for floats are extremely slow for certain distributions of numbers
exhibited in the test harness.  So we turn off sections of the sorting alg that use this
with QSORT_SKIP_EQUALITY */

#ifndef F4_QSORT
#define F4_QSORT
#define QSORT_TY float
#define QS_(name) f4_## name

#include "../csort.c"
#endif
