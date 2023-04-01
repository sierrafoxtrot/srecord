#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2023 Sean Alling
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="-Round_Nearest"
. test_prelude.sh

srec_cat -generate 0 8 -constant 0 -offset - 1 -Round_Nearest 0 \
    2> test.out

if test $? -ne 1; then fail; fi

cat > test.ok << 'fubar'
srec_cat: -Round_Nearest value must not be 0
fubar
if test $? -ne 0; then no_result; fi

diff -u test.ok test.out
if test $? -ne 0; then fail; fi
