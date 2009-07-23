/* Test harness for csort.c, useful for all C base types up to 64 bits.  
   Uses macros for polymorphism.
   To use, define macro TY (e.g. #define TY float) then #include this file. */

#ifndef CSORT_CMP
#define CSORT_CMP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "datetime/TIME.c"
#include <unistd.h>
#ifndef ISNAN
#define CHECKNAN(x) (x)
#else 
#define CHECKNAN(x) (ISNAN((x)) ? 0 : (x)) 
#endif

enum generator {RAND,BOUNDED,SORTED, REVERSE ,IDENT} ;

long long k,j;
const char* usage="csort-cmp N dist trials\n"
"dist is one of: RAND, BOUNDED , SORTED, REVSORTED, IDENT\n"
"N:              size of the array.\n"
"trials:         how many trials to do.  Necessary for small N.\n";

int parseDist(char* dist_str) {
    if (!strcmp("BOUNDED",dist_str)) 
        return BOUNDED;
    else if (!strcmp("RAND",dist_str)) 
        return RAND;
    else if (!strcmp("SORTED",dist_str))
        return SORTED;
    else if (!strcmp("REVERSE",dist_str))
        return REVERSE;
    else if (!strcmp("IDENT",dist_str))
        return IDENT;
    else fprintf(stderr,"dist argument mismatch.\n%s\n",usage),exit(1);
}

void randomized(TY *x, long long n) {
    x[0] = -1;
    x[1] = 1;
    
    /*
      for (i = 0; i < n; i+=1) {
      x[i] = i;
      } */
}

int compare(const void *a, const void *b) {
    TY A = *(const TY *)a, B = *(const TY *)b;
    if (A > B) return 1;
    if (B < A) return -1;
    return 0;
}

void fill(TY* array1,long n) {
    randomized(array1,n) ; 
}

void cmpWork(const TY *g, const TY *m, long long n ) {
    long long i;
    long j;
    for (i = 1; i < n; i++) {
        if (g[i] != m[i]) {
            for (j = 0; j < n; j++) fprintf(stderr,"%u ", (unsigned) m[j]);
            fprintf(stderr,"\n");
            fprintf (stderr,"cmpWork: %lld x %zd: failure at offset %lld\n", n,
                     sizeof(TY), i);
            exit(1);
        }
    }
}

void checkWork(const char *name, TY *a, long long n ) {
    long long i,j;
    for (i = 1; i < n; i++) {
        if (a[i-1] > a[i]) {
            fprintf (stderr,"%s: %lld x %zd: failure at offset %lld\n", name, n,
                     sizeof(TY), i);
            for (j=0; j < n; j++)
                fprintf(stderr,TY_FMT " ",a[j]);
            fprintf(stderr,"\n"); 
            free(a);
            exit(1);
        }
    }
}

int main (int argc, char **argv)
{
    if (argc < 4) fprintf(stderr,"too few arguments: %d\n%s",argc,usage) , exit(1);
    long i=j=0; long n=strtoul(argv[1],NULL,10);
    long num_trials = strtoul(argv[3],NULL,10);
    double start, end, m_tot=0, g_tot=0;
    TY *array_orig = (TY*) malloc (n * sizeof(TY));
    TY *array_g = (TY*) malloc (n * sizeof(TY));
    TY *array_m = (TY*) malloc (n * sizeof(TY));
    
    if (array_orig == NULL)
        {
            fprintf (stderr,"%d x %zd: no memory\n", argc, sizeof(TY));
            return 1;
        }
    if (getenv("SEED")) srand(time(NULL)); /* default is debugging mode. */
    for (i = 0; i < num_trials; i++) {
        fill(array_orig,n);
        memcpy(array_g,array_orig,n*sizeof(TY));
        memcpy(array_m,array_orig,n*sizeof(TY));
        start = TIME();
        csort (array_g,n,sizeof(TY),&compare);
        end   = TIME();
        if (i) {
            g_tot += end-start;
        }  
        checkWork("GNU",array_g,n);
        
        start = TIME();
        CS(array_m,n);
        end   = TIME();
        if (i) {
            m_tot += end - start;
        }    
        checkWork("schein",array_m,n);
        cmpWork(array_g,array_m,n);
        
        
    }
    g_tot /= (double) (num_trials - 1);
    m_tot /= (double) (num_trials - 1);
    fprintf(stdout,"%10ld\t%5.10f\t%5.10f\t%2.2f\n",n,m_tot,g_tot,
            100*((g_tot/m_tot) - 1));
    free (array_m);
    free (array_g);
    free (array_orig);
    return 0; 
}
#endif





