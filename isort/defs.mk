I=-L/usr/lib/gcc/i486-linux-gnu/4.2/ -I../  -I/usr/include -I../../ 
W=-Wall
#O=-fmudflap -lmudflap
O=-O9 -g
CC=/usr/bin/colorgcc
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

%.o : %.c
	$(CC) $(I) $(W) $(O) -o $@ -c $<
