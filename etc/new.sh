#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 2006, 2007 Peter Miller
#
# MANIFEST: shell script to sort include lists
#
sortflag=
if test "$1" = "-r"; then
        sortflag=r
        shift
fi
echo $* | tr ' ' '\12' | sort -t. +1n$sortflag -2 +2n$sortflag -3 +3n$sortflag -5 |
while read f
do
        echo ".br"
        echo ".ne 3i"
        echo ".so $f"
done
exit 0
