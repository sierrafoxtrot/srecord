#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 2006, 2007 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
#
# MANIFEST: shell script to generate Makefile fragment for each source file
#
depfile=no-such-file-or-directory
case $# in
2)
        ;;
3)
        depfile="$3";
        ;;
*)
        echo "usage: $0 filename resolved-filename" 1>&2
        exit 1
        ;;
esac
file="$1"
rfn="$2"

TAB=`echo | tr '\n' '\t'`

case $file in

*/*.y)
        root=`basename $file .y`
        stem=`echo $file | sed 's/\.y$//'`
        dir=`dirname $file`

        case $file in
        *)
                numconf="no"
                ;;
        esac

        yy=`echo $stem | sed -e 's|^[^/]*/||' -e 's|[^a-zA-Z0-9]|_|g'`
        echo ""
        echo "${stem}.gen.cc ${stem}.gen.h: $file"
        echo "${TAB}@echo Expect $numconf conflicts:"
        echo "${TAB}\$(YACC) -d $file"
        echo "${TAB}sed -e 's/[yY][yY]/${yy}_/g' y.tab.c > ${stem}.gen.cc"
        echo "${TAB}sed -e 's/[yY][yY]/${yy}_/g' y.tab.h > ${stem}.gen.h"
        echo "${TAB}rm y.tab.c y.tab.h"
        ;;

*/*.cc)
        root=`basename $file .cc`
        stem=`echo $file | sed 's/\.cc$//'`
        dir=`dirname $file`

        dep=
        if [ -r $depfile ]; then
                dep=`sed -e 's_.arch]/__' $depfile`
        fi

        echo ""
        echo "${stem}.o: $file" $dep
        echo "${TAB}\$(CXX) \$(CPPFLAGS) \$(CXXFLAGS) -I. -c $file"
        echo "${TAB}mv ${root}.o ${stem}.o"
        ;;

man/man[0-9]/*.[0-9])
        dir=`dirname $file`
        stem=`echo $file | sed 's|^man/\(.*\)|\1|'`

        dep=
        if [ -r $depfile ]; then
                dep=`sed -e 's_.arch]/__' $depfile`
        fi

        echo ""
        echo "\$(mandir)/$stem: $file" $dep $dir/.mandir
        echo "${TAB}\$(SOELIM) -I. -Ietc -I$dir $file > tmp"
        echo "${TAB}\$(INSTALL_DATA) tmp \$@"
        echo "${TAB}@rm -f tmp"
        ;;

etc/*.man)
        base=`echo $file | sed 's|etc/\(.*\).man$|\1|'`

        dep=
        if [ -r $depfile ]; then
                dep=`sed -e 's_.arch]/__' $depfile`
        fi

        echo ""
        echo "etc/$base.ps: $file" $dep
        echo "${TAB}\$(SOELIM) -I. -Iman/man1 -Iman/man5 -Ietc" \
            "$file | \$(GROFF) -t -man > \$@"

        echo ""
        echo "etc/$base.dvi: $file" $dep
        echo "${TAB}\$(SOELIM) -I. -Iman/man1 -Iman/man5 -Ietc" \
            "$file | \$(GROFF) -Tdvi -t -man > \$@"

        echo ""
        echo "etc/$base.txt: $file" $dep
        echo "${TAB}\$(SOELIM) -I. -Iman/man1 -Iman/man5 -Ietc" \
            "$file | \$(GROFF) -Tascii -t -man > $@"
        ;;

test/*/*.sh)
        root=`basename $file .sh`
        echo ""
        echo "$root: $file all"
        echo "${TAB}\$(SH) $file"
        ;;

*)
        ;;
esac
exit 0
