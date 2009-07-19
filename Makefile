.PHONY : timing-binaries clean

timing-binaries :
	cd usort ; make 
	cd qsort ; make 
	cd hsort ; make 
	cd isort ; make 

clean :
	cd usort ; make clean
	cd qsort ; make clean
	cd hsort ; make clean
	cd isort ; make clean
