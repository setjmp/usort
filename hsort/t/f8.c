#include <math.h>
#define ISNAN(x) isnan((x)) 
#define TY double
#define TY_FMT "%lf"
#include "../ufunc/f8_hsort.c"
#define HS f8_hsort
#include "hsort-cmp.c"
