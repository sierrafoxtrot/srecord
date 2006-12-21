#!/bin/sh
#
#	srecord - manipulate eprom load files
#	Copyright (C) 1998, 2002, 2006 Peter Miller
#
#	This program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
#
# MANIFEST: shell script to generate Makefile file
#
clean_files="core y.tab.c y.tab.h y.output .bin .bindir lib/libsrecord.a"

#
# list of progams to be linked and installed
#
progs=

#
# Emit the rules to build a directory, and all the ones above it.
#
recursive_mkdir()
{
	src_dir="$1"
	dst_dir="$2"
	flavor="${3-datadir}"
	while :
	do
		dirvar=`echo $src_dir | sed 's|[^a-zA-Z0-9]|_|g'`
		dotdot1=`dirname $src_dir`
		dotdot2=`dirname $dst_dir`
		if eval "test \${${dirvar}_${flavor}-no} != yes" ; then
			echo ""
			if test "$dotdot1" != "." -a "$dotdot2" != "." ; then
				echo "$src_dir/.${flavor}: $dotdot1/.${flavor}"
			else
				echo "$src_dir/.${flavor}:"
			fi
			echo "	-\$(INSTALL) -m 0755 -d $dst_dir"
			echo "	@-test -d $dst_dir && touch \$@"
			echo "	@sleep 1"
			eval "${dirvar}_${flavor}=yes"
			clean_files="$clean_files $src_dir/.${flavor}"
		fi
		src_dir=$dotdot1
		dst_dir=$dotdot2
		if test "$src_dir" = "." -o "$dst_dir" = "." ; then break; fi
	done
}

remember_prog()
{
	if eval "test \"\${prog_${1}-no}\" != yes"
	then
		progs="$progs $1"
		eval "prog_${1}=yes"
	fi
}

for file in $*
do
	case $file in

	lib/*.y)
		stem=`echo $file | sed 's/\.y$//'`
		lib_files="$lib_files ${stem}.gen.o"
		clean_files="$clean_files ${stem}.gen.cc ${stem}.gen.h \
${stem}.gen.o"
		;;

	lib/*.cc)
		stem=`echo $file | sed 's/\.cc$//'`
		lib_files="$lib_files ${stem}.o"
		clean_files="$clean_files ${stem}.o"
		;;

	prog/*.y)
		prog=`echo $file |sed 's|^[^/]*/||;s|/.*||'`
		stem=`echo $file | sed 's/\.y$//'`
		eval "${prog}_files=\"\$${prog}_files ${stem}.gen.o\""
		clean_files="$clean_files ${stem}.gen.cc ${stem}.gen.h \
${stem}.gen.o"
		remember_prog $prog
		;;

	prog/*.cc)
		prog=`echo $file |sed 's|^[^/]*/||;s|/.*||'`
		stem=`echo $file | sed 's/\.cc$//'`
		eval "${prog}_files=\"\$${prog}_files ${stem}.o\""
		clean_files="$clean_files ${stem}.o"
		remember_prog $prog
		;;

	test/*/*)
		root=`basename $file .sh`
		test_files="$test_files ${root}"
		;;

	man/man[0-9]/*.[0-9])
		stem=`echo $file | sed 's|^man/||'`
		file2="\$(mandir)/$stem"
		man_files="$man_files $file2"

		src=`dirname $file`
		dst=`dirname "$file2"`
		recursive_mkdir $src $dst mandir
		;;

	etc/*.man)
		stem=`echo $file | sed 's|.man$||'`
		clean_files="$clean_files ${stem}.ps ${stem}.pdf ${stem}.dvi" \
			"${stem}.txt"
		;;

	*)
		;;
	esac
done

all=
install_bin=
for prog in $progs
do
	echo ""
	eval "echo \"${prog}_obj =\" \${${prog}_files}"

	echo ""
	echo "bin/${prog}: \$(${prog}_obj) lib/libsrecord.a .bin"
	echo "	\$(CXX) \$(LDFLAGS) -o \$@ \$(${prog}_obj) lib/libsrecord.a \$(LIBS)"

	all="${all} bin/${prog}"

	echo ""
	echo "\$(bindir)/${prog}: bin/${prog} .bindir"
	echo "	\$(INSTALL_PROGRAM) bin/${prog} \$@"

	install_bin="${install_bin} \$(bindir)/${prog}"
done

echo ""
echo "#"
echo "# The real default target"
echo "#"
echo 'all: bin-all doc-all'
echo ''
echo 'bin-all:' ${all}
echo ''
echo 'doc-all: doc-${HAVE_GROFF}'
echo ''
echo 'doc-yes: etc/reference.ps'
echo ''
echo 'doc-no:'

echo ""
echo "lib_obj =" $lib_files
echo ""
echo "lib/libsrecord.a: \$(lib_obj)"
echo "	rm -f \$@"
echo "	\$(AR) qc \$@ \$(lib_obj)"
echo "	\$(RANLIB) \$@"

echo ""
echo ".bin:"
echo "	-mkdir bin"
echo "	-chmod 0755 bin"
echo "	@-test -d bin && touch \$@"
echo "	@sleep 1"

echo ""
echo ".bindir:"
echo "	-\$(INSTALL) -m 0755 -d \$(bindir)"
echo "	@-test -d \$(bindir) && touch \$@"
echo "	@sleep 1"

echo ""
echo "sure:" $test_files
echo "	@echo Passed All Tests"

echo ""
echo "clean-obj:"
echo $clean_files | tr ' ' '\12' | gawk '{
	if (pos > 0 && pos + length($1) > 71) { printf("\n"); pos = 0; }
	if (pos == 0) { printf "	rm -f"; pos = 13; }
	printf " %s", $1
	pos += 1 + length($1);
}
END { if (pos) printf "\n"; }'

echo ""
echo "clean: clean-obj"
echo $all | tr ' ' '\12' | gawk '{
	if (pos > 0 && pos + length($1) > 71) { printf("\n"); pos = 0; }
	if (pos == 0) { printf "	rm -f"; pos = 13; }
	printf " %s", $1
	pos += 1 + length($1);
}
END { if (pos) printf "\n"; }'

echo ""
echo "distclean: clean"
echo "	rm -f Makefile lib/config.h"
echo "	rm -f config.status config.cache config.log"

echo ""
echo "install-bin:" ${install_bin}

echo ''
echo 'install-man: install-man-${HAVE_GROFF}'
echo ''
echo 'install-man-yes:' $man_files
echo ''
echo 'install-man-no:'

echo ""
echo "install: install-bin install-man"

exit 0
