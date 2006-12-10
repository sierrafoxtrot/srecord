//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2003, 2006 Peter Miller;
//	All rights reserved.
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_output_file_ascii_hex class
//


#include <srec/output/file/ascii_hex.h>
#include <srec/record.h>


srec_output_file_ascii_hex::srec_output_file_ascii_hex(const char *filename) :
    srec_output_file(filename),
    address(0),
    column(0),
    pref_block_size(16),
    address_length(2)
{
}


srec_output_file_ascii_hex::~srec_output_file_ascii_hex()
{
    // check for termination record
}


void
srec_output_file_ascii_hex::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
	// All header data is discarded
	put_char(2);
	column = 1;
	break;

    case srec_record::type_data:
	if (address != record.get_address())
	{
	    if (column + 4 > pref_block_size)
	    {
		put_char('\n');
		column = 0;
	    }
	    else if (column)
		put_char(' ');
	    address = record.get_address();
	    int width = 2;
	    if (address >= 0x1000000)
		width = 4;
	    if (address >= 0x10000)
		width = 3;
	    if (width < address_length)
		width = address_length;
	    // important not to disturb the checksum, don't use put_byte
	    put_stringf("$A%0*lX,\n", width * 2, address);
	    column = 0;
	}
	for (int j = 0; j < record.get_length(); ++j)
	{
	    if (column)
	       	put_char(' ');
	    put_byte(record.get_data(j));
	    ++address;
	    ++column;
	    if (column >= pref_block_size)
	    {
	       	put_char('\n');
	       	column = 0;
	    }
	}
	break;

    case srec_record::type_data_count:
	// ignore
	break;

    case srec_record::type_start_address:
	if (column)
	    put_char(' ');
	put_char(3);
	put_char('\n');
	column = 0;

	if (!data_only_flag)
	{
	    //
	    // According to the documentation we emit the checksum after
	    // the ETX, which should mean that it is ignored (?!?)
	    //
	    put_stringf("$S%4.4X,\n", checksum_get16());
	}
	break;

    case srec_record::type_unknown:
	fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_ascii_hex::line_length_set(int linlen)
{
    int n = (linlen + 1) / 3;
    if (n < 1)
	n = 1;
    if (n > srec_record::max_data_length)
	n = srec_record::max_data_length;
    pref_block_size = n;
}


void
srec_output_file_ascii_hex::address_length_set(int n)
{
    if (n < 2)
	n = 2;
    if (n > 4)
	n = 4;
    address_length = n;
}


int
srec_output_file_ascii_hex::preferred_block_size_get()
    const
{
    return pref_block_size;
}
