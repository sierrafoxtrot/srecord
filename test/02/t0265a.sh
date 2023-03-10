#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2013 Peter Miller
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

TEST_SUBJECT="input efinix"
. test_prelude.sh

cat > test.in1 << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S123000046420A30420A44300A37300A37440A33370A41310A37420A30390A34300A463888
S12300200A35300A45330A42380A41440A43350A46390A44450A42450A43420A41380A4627
S1230040320A30310A43330A38300A38310A45420A34310A41330A42390A45410A43450A6B
S123006035360A33310A35370A33430A38460A39320A36320A36370A42320A35430A313557
S12300800A46350A34410A46310A38320A42320A32320A45330A43460A41340A42420A4407
S12300A0410A45380A35350A31320A43330A39310A45300A42380A39460A43380A43340A03
S5030006F6
fubar
if test $? -ne 0; then no_result; fi

#create intermediate file, actual efinix format
srec_cat test.in1 -moto -out test.in2 -bin
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1230000FB0BD0707D37A17B0940F850E3B8ADC5F9DEBECBA8F201C38081EB41A3B9EACE29
S12300205631573C8F926267B25C15F54AF182B222E3CFA4BBDAE85512C391E0B89FC8C4BE
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in2 -efinix -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
