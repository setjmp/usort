#include <math.h>
#define ISNAN(x) !isfinite((x))
#define TY double
#define TY_FMT "%lf"
#include "../ufunc/f8_qsort.c"
#define QS f8_qsort
#include "qsort-cmp.c"
