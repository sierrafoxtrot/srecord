//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000, 2002, 2003, 2006 Peter Miller
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
// MANIFEST: functions to impliment the srec_input_file_ti_tagged class
//


#include <cctype>
#include <lib/srec/input/file/ti_tagged.h>
#include <lib/srec/record.h>


srec_input_file_ti_tagged::srec_input_file_ti_tagged(const char *filename) :
    srec_input_file(filename),
    address(0),
    csum(0)
{
}


srec_input_file_ti_tagged::~srec_input_file_ti_tagged()
{
}


int
srec_input_file_ti_tagged::get_char()
{
    int c = inherited::get_char();
    if (c < 0 || c == '\n')
	csum = 0;
    else
	csum += c;
    return c;
}


int
srec_input_file_ti_tagged::read(srec_record &record)
{
    for (;;)
    {
	unsigned char data[2];
	int n;
	int c = get_char();
	switch (c)
	{
	default:
	    fatal_error
	    (
		(isprint(c) ? "unknown tag ``%c''" : "unknown tag (%02X)"),
		c
	    );

	case -1:
	    return 0;

	case '*':
	    data[0] = get_byte();
	    record = srec_record(srec_record::type_data, address, data, 1);
	    ++address;
	    return 1;

	case ':':
	    while (get_char() >= 0)
		;
	    return 0;

	case '7':
	    n = (-csum) & 0xFFFF;
	    if (n != get_word())
		fatal_error("checksum mismatch (%04X)", n);
	    break;

	case '8':
	    // ignored;
	    get_word();
	    break;

	case '9':
	    address = get_word();
	    break;

	case 'B':
	    data[0] = get_byte();
	    data[1] = get_byte();
	    record = srec_record(srec_record::type_data, address, data, 2);
	    address += 2;
	    return 1;

	case 'F':
	    if (get_char() != '\n')
		fatal_error("end of line expected");
	    break;

	case 'K':
	    {
		n = get_word();
		if (n < 5)
		{
		    bad_desc:
		    fatal_error("broken description");
		}
		n -= 5;
		int max = 250;
		unsigned char *buffer = new unsigned char [max];
		for (int j = 0; j < n; ++j)
		{
		    c = get_char();
		    if (c < 0 || c == '\n')
			    goto bad_desc;
		    if (j < max)
			    buffer[j] = c;
		}
		if (n > max)
		    n = max;
		record = srec_record(srec_record::type_header, 0, buffer, n);
		delete buffer;
	    }
	    return 1;
	}
    }
}


const char *
srec_input_file_ti_tagged::get_file_format_name()
    const
{
    return "Texas Instruments Tagged (SDSMAC)";
}
