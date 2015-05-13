#!/bin/sh

fsize=`stat -c %s $1`
echo Total Size: `printf "0x%x" $fsize`
ssize=`printf "%d" $2`
echo Start Size: `printf "0x%x" $ssize`
len=`printf "%d" $3`
echo Len Size: `printf "0x%x" $len`
rsize=`expr $fsize - $ssize`
#echo Remain Size: $rsize
echo

tail -c $rsize $1 > last.bin
echo Strip data before `printf "0x%x" $ssize`.
head -c $len last.bin > $4
echo Get first `printf "0x%x" $len` data.
