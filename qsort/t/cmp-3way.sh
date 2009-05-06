#!/usr/bin/zsh -e

i=(10 100 1000 10000 100000 1000000 10000000 100000000 )
t[0]=10000000
t[1]=1000000
t[2]=100000
t[3]=10000
t[4]=1000
t[5]=100
t[6]=10
t[7]=3
t[8]=1
sz=${#i[@]}
c=1
echo "Sort Comparison of unsigned integers on N numbers generating according to:"
echo "RANDOM:    random integers"
echo "BOUNDED:   random() % N"
echo "SORTED:    sorted integers 1...N"
echo "REVERSED:  sorted integers N...1"
echo "IDENT:     all values of the array are \"1\""

echo "\nRANDOM"
echo "N               mysort (secs)   GLIBC (secs)    %impr   CB (secs)    CB_%improv"
c=1
while [ $c -lt $sz ] ; do
    ./qsort-3way-cmp ${i[$c]} RAND ${t[$c]}  
    c=$(( $c + 1 ))
done;
echo "BOUNDED"
c=1
while [ $c -lt $sz ] ; do
    ./qsort-3way-cmp ${i[$c]} BOUNDED ${t[$c]}  
    c=$(( $c + 1 ))
done;
echo "SORTED"
c=1
while [ $c -lt $sz ] ; do
    ./qsort-3way-cmp ${i[$c]} SORTED ${t[$c]}  
    c=$(( $c + 1 ))
done;
echo "REVERSED"
c=1
while [ $c -lt $sz ] ; do
    ./qsort-3way-cmp ${i[$c]} REVERSE ${t[$c]}  
    c=$(( $c + 1 ))
done;
echo "IDENT"
c=1
while [ $c -lt $sz ] ; do
    ./qsort-3way-cmp ${i[$c]} IDENT ${t[$c]}  
    c=$(( $c + 1 ))
done;
