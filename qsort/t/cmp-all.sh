#!/bin/sh -e

apps="u1 s1 u2 s2 u4 s4 f4 f8 s8 u8"

for app in $apps ; do
    export app
    ./cmp.sh
done

