//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003, 2006 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_file_needham class
//


#include <cctype>

#include <srec/input/file/needham.h>
#include <srec/record.h>


srec_input_file_needham::~srec_input_file_needham()
{
}


srec_input_file_needham::srec_input_file_needham(const char *filename) :
    srec_input_file(filename),
    seen_some_input(false),
    address(0)
{
}


int
srec_input_file_needham::read(srec_record &record)
{
    for (;;)
    {
	int c = peek_char();
	if (c < 0)
	{
	    if (!seen_some_input)
		fatal_error("file contains no data");
	    return 0;
	}
	if (isxdigit(c))
	{
	    unsigned char c = get_byte();
	    record =
		srec_record
		(
	    	    srec_record::type_data,
	    	    address,
	    	    &c,
	    	    1
		);
	    seen_some_input = true;
	    ++address;
	    return 1;
	}
	c = get_char();
	switch (c)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
	case '\f':
	    break;

	case '\32':
	    // ignore ^Z characters
	    break;

	default:
	    fatal_error("illegal character");

	case '$':
	    if (get_char() != 'A')
		fatal_error("unknown command");
	    address = 0;
	    int n = 0;
	    for (n = 0; n < 4 && peek_char() != ','; ++n)
	    {
		int b = get_byte();
		address = (address << 8) + b;
	    }
	    if (n < 2)
		fatal_error("short address");
	    if (get_char() != ',')
		fatal_error("comma expected");
	    break;
	}
    }
}


const char *
srec_input_file_needham::get_file_format_name()
    const
{
    return "Needham";
}
