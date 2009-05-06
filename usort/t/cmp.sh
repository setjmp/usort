#!/usr/bin/zsh -e

echo "TESTING APP: " ${app:="f8"}
APP=$app

i=(8 128  1024 10000 100000 1000000 10000000 100000000 )
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
echo "N               usort (secs)    GLIBC qsort (secs)    x-fold speedup"
c=1

while [ $c -lt $sz ] ; do
    ./${APP} ${i[$c]} RAND ${t[$c]}  
    c=$(( $c + 1 ))
done;
