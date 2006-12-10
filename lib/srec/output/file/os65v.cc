//
//	srecord - manipulate eprom load files
//	Copyright (C) 2002, 2006 Peter Miller;
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
// MANIFEST: functions to impliment the srec_output_file_os65v class
//


#include <lib/srec/output/file/os65v.h>
#include <lib/srec/record.h>


srec_output_file_os65v::~srec_output_file_os65v()
{
    if (!data_only_flag && !seen_start_address)
	put_string(".00FD/00\r");
}


srec_output_file_os65v::srec_output_file_os65v(const char *filename) :
    srec_output_file(filename),
    address(0),
    state(0),
    seen_start_address(false)
{
}


void
srec_output_file_os65v::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
	// All header data is discarded
	break;

    case srec_record::type_data:
	if (seen_start_address)
	    fatal_error("more data following start address (bug)");
	if (address != record.get_address() || state == 0)
	{
	    address = record.get_address();
	    put_stringf(".%04lX/", address);
	    state = '/';
	}
	for (int j = 0; j < record.get_length(); ++j)
	{
	    unsigned char n = record.get_data(j);
	    if (address == 0x00FD && n == 0)
		fatal_error("you may not write 00 to 00FD in this format");
	    put_byte(n);
	    put_char('\r');
	    ++address;
	}
	break;

    case srec_record::type_data_count:
	// ignore
	break;

    case srec_record::type_start_address:
	if (!data_only_flag)
	{
	    if (address != record.get_address() || state == 0)
	    {
		address = record.get_address();
		put_stringf(".%04lX", address);
		state = '.';
	    }
	    put_char('G');
	}
	seen_start_address = true;
	break;

    case srec_record::type_unknown:
	fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_os65v::line_length_set(int)
{
    // Irrelevant.  Ignore.
}


void
srec_output_file_os65v::address_length_set(int)
{
    // Irrelevant.  Ignore.
}


int
srec_output_file_os65v::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srec_record::max_data_length;
}


const char *
srec_output_file_os65v::mode(void)
    const
{
    return "wb";
}
