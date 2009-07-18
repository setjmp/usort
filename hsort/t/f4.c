#include <math.h>
#define ISNAN(x)  isnan((x))
#define TY float
#define TY_FMT "%f"
#include "../ufunc/f4_hsort.c"
#define HS f4_hsort
#include "hsort-cmp.c"
