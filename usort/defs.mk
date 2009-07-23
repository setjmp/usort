I= -I/usr/include -I./ -I../ -I../../ -I../../../ -I../../../../   
W=-Wall
#O=-fmudflap -lmudflap
O=-O5
G=-g
L=-lm
LD_LIBRARY_PATH=/usr/lib:/lib:
CC=/usr/bin/colorgcc -std=c99 -fgnu89-inline
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

%.o : %.cc
	$(CC) $(I) $(W) $(O) -o $@ -c $<


#notes: c89 has no isfinite.  c99 inline is not supported.