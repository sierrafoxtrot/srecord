//
//	srecord - manipulate eprom load files
//	Copyright (C) 2003 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_file_vmem class
//

#pragma implementation "srec_input_file_vmem"

#include <cctype>
#include <srec/input/file/vmem.h>
#include <srec/record.h>


srec_input_file_vmem::~srec_input_file_vmem()
{
}


srec_input_file_vmem::srec_input_file_vmem(const char *filename) :
    srec_input_file(filename),
    garbage_warning(false),
    seen_some_input(false),
    address(0)
{
}


int
srec_input_file_vmem::read(srec_record &record)
{
    if (!seen_some_input)
    {
	for (;;)
	{
	    int c = get_char();
	    if (c < 0)
	    {
		fatal_error("file contains no data");
		return 0;
	    }
	    if (c == '@')
	    {
		seen_some_input = true;
		get_char_undo(c);
		break;
	    }
	    if (!garbage_warning)
	    {
		warning("ignoring garbage lines");
		garbage_warning = true;
	    }
	}
    }

    for (;;)
    {
	int c = peek_char();
	if (c < 0)
	    return 0;
	if (c == '@')
	{
	    get_char();

	    // collect address
	    address = 0;
	    for (;;)
	    {
		address = (address << 4) + get_nibble();
		c = peek_char();
		if (c < 0 || !isxdigit((unsigned char)c))
		    break;
	    }
	    continue;
	}
	if (isspace((unsigned char)c))
	{
	    get_char();
	    continue;
	}

	// collect value
	unsigned char value[5];
	size_t nbytes = 0;
	while (nbytes < sizeof(value))
	{
	    value[nbytes++] = get_byte();
	    c = peek_char();
	    if (c < 0 || !isxdigit((unsigned char)c))
		break;
	}
	switch (nbytes)
	{
	default:
	    fatal_error("value has too many bytes (%d)", nbytes);

	case 1:
	case 2:
	case 4:
	    record =
		srec_record
		(
		    srec_record::type_data,
		    address * nbytes,
		    value,
		    nbytes
		);
	    break;
	}

	// This is not a byte address, it's a chunk address.
	++address;
	return 1;
    }
}


const char *
srec_input_file_vmem::get_file_format_name()
    const
{
    return "Verilog VMEM";
}
