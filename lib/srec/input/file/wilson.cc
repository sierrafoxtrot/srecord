//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2003, 2005, 2006 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_file_wilson class
//

#pragma implementation "srec_input_file_wilson"

#include <srec/input/file/wilson.h>
#include <srec/record.h>


srec_input_file_wilson::srec_input_file_wilson(const char *filename) :
    srec_input_file(filename),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false)
{
}


srec_input_file_wilson::~srec_input_file_wilson()
{
}


int
srec_input_file_wilson::get_byte()
{
    int n = -1;
    int c = get_char();
    if (c >= 0x40 && c < 0xE0)
	n = (c - 0x40);
    else if (c >= 0xE0)
	n = c;
    else if (c >= 0x3A && c < 0x3E)
    {
	int c2 = get_char();
	if (c2 >= 0x30 && c2 < 0x40)
    	    n = ((c - 0x30) << 4) + (c2 - 0x30);
	else
    	    goto bomb;
    }
    else
    {
	bomb:
	fatal_error("illegal character");
	// NOTREACHED
    }
    checksum_add(n);
    return n;
}

int
srec_input_file_wilson::read_inner(srec_record &record)
{
    int c;
    for (;;)
    {
	c = get_char();
	if (c < 0)
	    return 0;
	if (c == '#' || c == '\'')
	    break;
	if (c == '\n')
	    continue;
	if (!garbage_warning)
	{
	    warning("ignoring garbage lines");
	    garbage_warning = true;
	}
	for (;;)
	{
	    c = get_char();
	    if (c < 0)
	       	return 0;
	    if (c == '\n')
	       	break;
	}
    }
    int tag = c;
    checksum_reset();
    int line_length = get_byte();
    if (line_length < 1)
	fatal_error("line length invalid");
    unsigned char buffer[256];
    for (int j = 0; j < line_length; ++j)
	buffer[j] = get_byte();
    if (use_checksums())
    {
	int n = checksum_get();
	if (n != 0xFF)
	    fatal_error("checksum mismatch (%02X != FF)", n);
    }
    if (get_char() != '\n')
	fatal_error("end-of-line expected");
    --line_length;

    int naddr = 4;
    srec_record::type_t type = srec_record::type_unknown;
    switch (tag)
    {
    case '#':
	// data
	type = srec_record::type_data;
	break;

    case '\'':
	// termination
	type = srec_record::type_start_address;
	break;
    }
    if (line_length < naddr)
    {
	fatal_error
	(
    	    "data length too short (%d < %d) for data type (%x)",
    	    line_length,
    	    naddr,
    	    tag
	);
    }
    record =
	srec_record
	(
    	    type,
    	    srec_record::decode_big_endian(buffer, naddr),
    	    buffer + naddr,
    	    line_length - naddr
	);
    return 1;
}


int
srec_input_file_wilson::read(srec_record &record)
{
    for (;;)
    {
	if (!read_inner(record))
	{
		if (!seen_some_input)
			fatal_error("file contains no data");
		if (!termination_seen)
		{
			warning("no start address record");
			termination_seen = true;
		}
		return 0;
	}
	seen_some_input = true;
	if
	(
		record.get_type() != srec_record::type_start_address
	&&
		termination_seen
	)
	{
		warning("termination record should be last");
		termination_seen = false;
	}
	switch (record.get_type())
	{
	case srec_record::type_unknown:
	case srec_record::type_header:
	case srec_record::type_data_count:
	    fatal_error("record type not recognised");
	    break;

	case srec_record::type_data:
	    if (record.get_length() == 0)
	    {
		    warning("empty data record ignored");
		    continue;
	    }
	    break;

	case srec_record::type_start_address:
	    if (record.get_length() > 0)
	    {
		    warning("data in termination record ignored");
		    record.set_length(0);
	    }
	    if (termination_seen)
		    warning("redundant termination record");
	    termination_seen = true;
	    break;
	}
	break;
    }
    return 1;
}


const char *
srec_input_file_wilson::get_file_format_name()
    const
{
    return "Wilson (anyone know this format's real name?)";
}


const char *
srec_input_file_wilson::mode()
    const
{
    return "rb";
}
