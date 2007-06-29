dnl
dnl     srecord - manipulate eprom load files
dnl     Copyright (C) 1998, 1999, 2002, 2006, 2007 Peter Miller
dnl
dnl     This program is free software; you can redistribute it and/or modify
dnl     it under the terms of the GNU General Public License as published by
dnl     the Free Software Foundation; either version 2 of the License, or
dnl     (at your option) any later version.
dnl
dnl     This program is distributed in the hope that it will be useful,
dnl     but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl     GNU General Public License for more details.
dnl
dnl     You should have received a copy of the GNU General Public
dnl     License along with this program; if not, write to the Free
dnl     Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl     Boston, MA 02110-1301 USA.
dnl
dnl MANIFEST: how to configure srecord, input to the GNU autoconf program
dnl
AC_INIT(install-sh)
AC_CONFIG_HEADER(lib/config.h)
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_YACC
AC_PROG_RANLIB
AC_CHECK_PROGS(GROFF, groff roff)
AC_CHECK_PROGS(SOELIM, roffpp gsoelim soelim)
AC_CHECK_FUNCS(snprintf vsnprintf)

#
# If we are using the GNU C++ compiler, add the -fno-rtti command line
# option.  This fixes the ugly GCC 3.x "undefined symbols" bug on Cygwin
# and MacOsX.
#
if test "$ac_cv_cxx_compiler_gnu" = "yes"
then
    CXXFLAGS="$CXXFLAGS -fno-rtti"
fi

dnl
dnl     Test to find a Bourne shell which understands functions
dnl
AC_MSG_CHECKING([for a Bourne shell which understands functions])
if test "z$SH" = "z"; then
    if test -f /bin/sh5; then
        SH=/bin/sh5
    else
        SH=/bin/sh
    fi
fi
AC_SUBST(SH)
AC_DEFINE_UNQUOTED(CONF_SHELL, ["$SH"],
[Set this to be the absolute path of a Bourne shell
which understands functions.])
AC_MSG_RESULT($SH)

dnl
dnl     Test to see if this operating system has the ZFS sparse file support.
dnl
AC_MSG_CHECKING([for sparse file lseek support])
AC_TRY_COMPILE([#include <unistd.h>], [fpathconf(0, _PC_MIN_HOLE_SIZE);
lseek(0, 0L, SEEK_DATA); lseek(0, 0L, SEEK_HOLE); ],
AC_DEFINE(HAVE_SPARSE_LSEEK, 1, [Define this symbol if your
operating system has support for sparse file seeking.])
AC_MSG_RESULT(yes),AC_MSG_RESULT(no))dnl

dnl
dnl     If the soelim program exists, and understands the -I option,
dnl     arrange for the Makefile to install the .ps files from the
dnl     documentation source files.
dnl
if test -n "$SOELIM"
then
  if $SOELIM -I. /dev/null > /dev/null 2>&1
  then
    : nothing
  else
    AC_MSG_RESULT([])
    AC_MSG_RESULT([     The $SOELIM program does not understand the -I])
    AC_MSG_RESULT([     option.  There is a patch available for GNU Groff])
    AC_MSG_RESULT([     which makes this work correctly.  See the BUILDING])
    AC_MSG_RESULT([     file for details.])
    GROFF=
  fi
fi
if test -n "$GROFF"
then
  HAVE_GROFF=yes
 else
  HAVE_GROFF=no
  AC_MSG_RESULT([])
  AC_MSG_RESULT([       The SRecord documentation set and manual pages will])
  AC_MSG_RESULT([       be formatted and installed if you first install])
  AC_MSG_RESULT([       GNU Groff and then re-run this ./configure script.])
  AC_MSG_RESULT([       Remember to use \`\`rm config.cache'' before you do.])
  AC_MSG_RESULT([])
fi
AC_SUBST(HAVE_GROFF)

AH_TOP([/*
 *      srecord - manipulate eprom load files
 *      Copyright (C) 1998-2002, 2006, 2007 Peter Miller
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 *      02110-1301 USA.
 *
 * MANIFEST: template lib/config.h file
 */

#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H
])

AH_BOTTOM([
/*
 * There is more to do, but we need to insulate it from config.status,
 * because it screws up the #undef lines.  They are all implications of
 * the above information, so there is no need for you to edit the file,
 * if you are configuring Aegis manually.
 */
#include <lib/config.messy.h>

#endif /* INCLUDE_CONFIG_H */])

AC_OUTPUT(Makefile)
