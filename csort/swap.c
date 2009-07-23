/* c 2008 Andrew I. Schein */

#ifndef AS_SWAP
#define AS_SWAP
#include <string.h>
#include <stdint.h> /* C99 defs for int_fastN_t */
#include <stdio.h>
#include <stdlib.h>
#define SWAPCPY(x,y,sz) {memcpy((buf.c),(x),(sz)); memcpy((x),(y),(sz)); memcpy((y),(buf.c),(sz));}

#define SWAPASS(ty) {*((ty*)buf.c)=*((ty*)x); *((ty*)x)=*((ty*)y); *((ty*)y)=*((ty*)buf.c);}
#define SWAPINT32 (buf.d=*(int_fast32_t*)x, \
                   *((int_fast32_t*)x)=*((int_fast32_t*)y), *((int_fast32_t*)y)= buf.d)
#define SWAPINT64 (buf.f=*(int_fast64_t*)x, \
                   *((int_fast64_t*)x)=*((int_fast64_t*)y), *((int_fast64_t*)y)= buf.f)
#define SWAP_MIN(x,y)  ((x) < (y) ? (x) : (y))

    
void *swap(void *_x,void *_y,size_t sz) {
    union swap_buf_t { char c[sizeof(int_fast64_t)] ; int_fast32_t d ; int_fast64_t f; } buf; 
    size_t n; 
    char *x=_x, *y=_y;
    switch (sz) {
    case             0 : return NULL;
    case             sizeof(char)         :  SWAPASS(char);           return _x;
    case             sizeof(short)        :  SWAPASS(short);          return _x;
    case             sizeof(int32_t)      :  SWAPASS(int32_t);        return _x;
    case             sizeof(int_fast64_t) :  SWAPASS(int_fast64_t);   return _x;
    default : 
        do {
            n = SWAP_MIN(sz,sizeof(int_fast64_t));
            if (n == sizeof(int_fast64_t))
                (SWAPINT64);
            else 
                SWAPCPY(x,y,n);
            sz -= n;
            x  += n;
            y  += n;
        } while (sz > 0);
        return _x;
    }   
    fprintf(stderr,"error in %s\n",__func__); 
    exit(1);
}

#undef SWAPCPY
#undef SWAP_MIN
#undef SWAP_ASS
#endif
