include ../defs.mk

APPS=s2 s8
SRC=$(patsubst %,%.c,$(wildcard $(APPS)))

all : $(APPS)

s2  : isort-cmp.c  ../isort.c  $(SRC)
	gcc $(G) $(W) $(I) $(O) -o s2 s2.c 
s8  : isort-cmp.c  ../isort.c  $(SRC)
	gcc $(G) $(W) $(I) $(O) -o s8 s8.c 

clean :
	rm -f $(APPS) csort-cmp 
