include defs.mk

.PHONY : clean all cmp sample 

all :
	cd t/ ; make all

clean : 
	cd t/ ; make clean
	rm -f code_sample.tar
	rm -f code_sample.tar.gz

cmp   : all
	cd t ; ./cmp-all.sh

sample : 
	rm -f code_sample.*
	echo "*.back*\n.tar_ignore\n*3way*\n*.hg*" > .tar_ignore
	tar -hX .tar_ignore -cf code_sample.tar *
	gzip code_sample.tar
	rm -f .tar_ignore
