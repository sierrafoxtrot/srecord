#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2009, 2011, 2012 Peter Miller
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

TEST_SUBJECT="fletcher-32"
. test_prelude.sh

cat > test.in << 'fubar'
S00600004844521B
S1230000C36C09725AD4A01E201DA1A055152E6E7C8B730F5D797537921D012417C3A6B7AC
S12300201D8B87A0D025963300A265DF089B0C220CD5BFAB3CB6BB4A2C889FBB9682399641
S12300403607BA01F47D2B80B7C4B995FA7C50DD8AD002DA16F69C39483D2BE7938B1ADA5C
S1230060AB1673CA8723FC9AE7FD214AB5A0BBC8FCCB86DF45EC40688BF7C3BD631A61181A
S1230080262B761798329E52F62798C4E3B033EBF20049DA6DC1C028861F33AEA1A1FA04A9
S12300A0C91186FE09CF99FF79E85717202D77A0F948361DB2D33BDA87F4E964B67ADFCA6C
S12300C002E6C5C480A5AF4B57BBF65E8A631058910B091E0739D9C6B6F4CC8F44DB72FA9F
S12300E077B9F7E9FFEE7FF109A8395B23A6B58E6E4836BA387322D7A1B5598BF453106102
S123010000A5E8E16A268A50C22E140084CCE01A6F568A3B2A52005B94294F7DA03814C619
S12301202600EB506596D2F6CDE22453A09892171AB028418E3D688BF60FC97E5256E35D6B
S123014006F7173DF1A3E7CE83E65057F6337240E6E6823FCD22570BBF75E041C83CD9A95D
S12301609F8ABF6C2A43ABA782113A7D7213CAA89B1A71807A4A8AE58F67D722DE2EC3117F
S123018038E77849D0F2598840C925A1FC83E6359338E71F964640CBDC8E99D3A8EA094BFB
S12301A0551F6387019D1B0725A0A2DC1F52CF7303CE41D2E98FE61F4918F42B27E0D30567
S12301C037D940A14676CBED2D301AFD06D7796B8F25EBF23B98792AD11DB93A11B9A8180F
S12301E04C83494EDFD91C6B97D965F7435A79BB1E0E082AD334987953CAA727F9DB175419
S1230200999A6AB50E7104B6AA908AD9EDEC088C2E988511A805E9E8E69FAE402275C46A33
S12302205110F8711CB4BF0ED457D16CA6D85D958282BD5AD61DD5577E4601D2B79F2DBC6B
S1230240B501F0E59E1624DAFDA6F9AF9ACBE605B7D8BA423901697AAF6272A78F76D745CF
S12302607CDEE99E1F96802852D1B1CB6C39405B334D972B546B90F11A0D5370A12C27B14C
S123028003F1FBD72BA9D650D2ADEA6B2DB8B1BF9BDC881703B91DE4784FE3B5F30DD870F7
S12302A0F70FA98E3A590EFACA9AEB28DE7CA9AD93DB2349E4D356FC67BA390D17702AD070
S12302C013B0B8B298405C0164BED85BADF3E92FE6076DA8B8CAF427BE3C967EEFB77043A5
S12302E0D78D926EBD4D016378D264A16A5874BBEEAE16B679CE67C481D4B1C8BB04C51D9F
S12303003DF6F1A3C74EC6B8631CDF828842D8A4C5CF579C5FBBF55E6DBC01C2559B779979
S123032007BF6BCC0DE4B1DBE48866FAF2B963EED88F0EB10E7B39A574B24FCB6A0EB6C8B4
S123034058735035228CD32F35534BBF007B341619507F71C21F3DEE97197DBA5746443BDA
S123036073C17D37B6A6C4818AAF14F3ABBD2A84278159DADEE2CF916AB0608929B38AC373
S12303808E246943447D9BC17FD85F5D5CA6CB0219216572154450CA96B64F6CB551B8D1E2
S12303A04C226E5F247F42231258CDB69F482686CD8BFEBD8666DB7DCE61C85430D462B2BC
S12303C0E5628A6657551CFD92164373DFE773A0FA917664E52EDA95FC90F8B8C125024D8E
S12303E07A576ECB34DE1B9A6B8600390FCBC3992180BAA770B58CB263D9BC7C5EB734E5C1
S5030020DC
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
00001000: 76 F8 02 84                                      #vx..
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fletcher-32-b-e 0x1000 -crop 0x1000 0x1004 \
    -o test.out -hexdump
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
