c 2008,2009,2021 Andrew I. Schein
See LICENSE file.

# Update for 2021

This repo disappeared from the internet when Bitbucket turned off its mercurial service.  I located a
copy off of a backup and am reposting a converted git repository.

The code was never "production-ready", but it contains several interesting ideas and so I am
sharing for pedagogical purposes.  Personally, I find that on my current version of GCC the code
does not compile as is.  Also, the last person who got it to compile on MacOS discovered that the
code did not give correct answers.  No doubt this is related to very specific CPU architecture or
compiler flag options I used.

So, please use with caution.  But enjoy!


# Usort


This directory contains a set of C99 language sorting functions. 
There are two sets of uses:

1. General purpose comparison-based sorting that is faster than glibc qsort.  
The file csort.c contains an introsort implementation (introsort is a variation of 
quicksort), with comparison operators defined as macros for maximal inlining.

2. Numeric type-specific sort routines.
The directory usort/usort/* contains a set of type-specific files.  For example
f4_sort.c contains a 4 byte float sort. The usort files use a combination of 
strategies including bucket sort, radix sort, insertion sort, and intro sort.  

NOTE: In the case of radix sort a compile time check for  little endian is performed, 
and an introsort is used for big endian hardware.  In hindsight... this may not be necessary
however I don't have a big endian machine to test it on.


To use the code, you may either:

1. #include the appropriate function file.  This approach will give maximum inlining, but
you may have to study the usort macros to ensure there is no collision in your other code.
2. Compile a static or shared library with the files that interest you.  

At a minimum for use of the comparison sort, you are expected to include it in a file that has

#define CSORT_TY
and
#define CS_
set up. For simple examples employing the csort.c, see the csort/ufunc/ directory.

For the usorts, the speedups range from 3x for 8 byte numbers to incredable 250x for 1 byte 
integers (which can be sorted using bucket sort).

I test this code on both 32 and 64 bit intel architectures. More recently, my tests
have been focussed on 64 bit due to available hardware.
Below are complete comparisons of the usorts against the GNU lib C qsort straw
man.  Timings were on my TravelMate 8210, a 32 bit intel machine with
Intel(R) Core2 T7200 @ 2.00GHz and 2 gigs of RAM..

```
<result of tests pasted below>
Univeral Sort Functions (usort or ufunc sorters) are fast sorting 
algorithms specicialized for each of the basic C numeric types
Comparison of ufunc sorters against GLIBC qsort baseline
u1 - unsigned char
s1 - signed char
u2 - unsigned short
s2 - signed short
u4 - unsigned int
s4 - signed int
f4 - 4 byte float
u8 - unsigned long long
s8 - signed long long
f8 - 8 byte float (double).
TESTING APP:  u1
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013044    0.0000022476          1.72
       128      0.0000062864    0.0000709979          11.29
      1024      0.0000144427    0.0007754159          53.69
     10000      0.0000603504    0.0096700000          160.23
    100000      0.0005544242    0.1160923939          209.39
   1000000      0.0055322222    1.3435174444          242.85
  10000000      0.0555820000    15.2838325000          274.98
TESTING APP:  s1
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013236    0.0000022590          1.71
       128      0.0000068531    0.0000718443          10.48
      1024      0.0000161011    0.0007739059          48.07
     10000      0.0000992372    0.0097043173          97.79
    100000      0.0009392525    0.1159364647          123.43
   1000000      0.0093115555    1.3405086666          143.96
  10000000      0.0932485000    15.2905350000          163.98
TESTING APP:  u2
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013062    0.0000022378          1.71
       128      0.0000059180    0.0000712432          12.04
      1024      0.0000283789    0.0007730834          27.24
     10000      0.0002274915    0.0098890911          43.47
    100000      0.0029452828    0.1219671414          41.41
   1000000      0.0308931111    1.4411857778          46.65
  10000000      0.3222000001    16.3975955000          50.89
TESTING APP:  s2
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013016    0.0000022453          1.72
       128      0.0000060273    0.0000709950          11.78
      1024      0.0000357520    0.0007743004          21.66
     10000      0.0003068939    0.0099215435          32.33
    100000      0.0032695051    0.1223466768          37.42
   1000000      0.0272346667    1.4397441111          52.86
  10000000      0.2881805000    16.3111720000          56.60
TESTING APP:  u4
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013842    0.0000019156          1.38
       128      0.0000145125    0.0000400612          2.76
      1024      0.0000841047    0.0003913318          4.65
     10000      0.0005056587    0.0047337638          9.36
    100000      0.0052576566    0.0565568182          10.76
   1000000      0.0739924445    0.6631011111          8.96
  10000000      0.9818515000    7.6159215000          7.76
TESTING APP:  s4
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013934    0.0000019018          1.36
       128      0.0000148855    0.0000399955          2.69
      1024      0.0000885389    0.0003940953          4.45
     10000      0.0005386577    0.0047699309          8.86
    100000      0.0056020303    0.0568531717          10.15
   1000000      0.0724470000    0.6657777778          9.19
  10000000      1.0124520000    7.6765220000          7.58
TESTING APP:  f4
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013674    0.0000019814          1.45
       128      0.0000189171    0.0000443792          2.35
      1024      0.0001437735    0.0004537536          3.16
     10000      0.0010609349    0.0054732893          5.16
    100000      0.0103490303    0.0657784848          6.36
   1000000      0.1114643333    0.7678498889          6.89
  10000000      1.1413060001    8.8897940000          7.79
TESTING APP:  u8
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000014774    0.0000019841          1.34
       128      0.0000180451    0.0000440726          2.44
      1024      0.0002008706    0.0004311888          2.15
     10000      0.0020998599    0.0050863193          2.42
    100000      0.0190793838    0.0621567071          3.26
   1000000      0.2457824444    0.7381691111          3.00
  10000000      2.8299570000    8.5275200000          3.01
TESTING APP:  s8
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000014621    0.0000019779          1.35
       128      0.0000186448    0.0000437078          2.34
      1024      0.0002022761    0.0004321395          2.14
     10000      0.0012153534    0.0051192613          4.21
    100000      0.0122118182    0.0616136869          5.05
   1000000      0.1933817778    0.7349713333          3.80
  10000000      2.3513790000    8.5338990000          3.63
TESTING APP:  f8
N               usort (secs)    GLIBC qsort (secs)    x-fold speedup
         8      0.0000013375    0.0000019670          1.47
       128      0.0000154629    0.0000450866          2.92
      1024      0.0001696976    0.0004562643          2.69
     10000      0.0013682102    0.0055419099          4.05
    100000      0.0136390101    0.0677483838          4.97
   1000000      0.1982658889    0.7983115555          4.03
  10000000      2.3578315001    9.2898030000          3.94
```
