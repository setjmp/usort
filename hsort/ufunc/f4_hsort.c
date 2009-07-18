/* the <= and == tests for floats are extremely slow for certain distributions of numbers
exhibited in the test harness.  So we turn off sections of the sorting alg that use this
with HSORT_SKIP_EQUALITY */

#ifndef F4_HSORT
#define F4_HSORT
#define HSORT_TY float
#define HS_(name) f4_h## name

#include "../hsort.c"
#endif
