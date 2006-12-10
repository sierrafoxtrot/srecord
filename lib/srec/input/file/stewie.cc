//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2006 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_file_stewie class
//


#include <lib/srec/input/file/stewie.h>
#include <lib/srec/record.h>


srec_input_file_stewie::srec_input_file_stewie(const char *filename) :
    srec_input_file(filename),
    data_count(0),
    garbage_warning(false),
    seen_some_input(false),
    header_seen(false),
    termination_seen(false)
{
}


srec_input_file_stewie::~srec_input_file_stewie()
{
}


int
srec_input_file_stewie::get_byte()
{
    int n = get_char();
    if (n < 0)
	fatal_error("premature end-of-file");
    checksum_add(n);
    return n;
}


int
srec_input_file_stewie::read_inner(srec_record &record)
{
    if (termination_seen)
	return 0;
    int c = get_char();
    if (c < 0)
	return 0;
    if (c != 'S')
    {
	fatal_error("'S' expected");
	return 0;
    }
    int tag = get_nibble();
    switch (tag)
    {
    case 0:
	// Header records are not like, Motorola hex.
	// The header record is literally "S003"
	if (get_char() != '0' || get_char() != '3')
	    fatal_error("format error");
	record = srec_record(srec_record::type_header, 0, 0, 0);
	return 1;
    
    case 7:
    case 8:
    case 9:
	record = srec_record(srec_record::type_start_address, 0, 0, 0);
	return 1;
    }
    checksum_reset();
    int line_length = get_byte();
    if (line_length < 1)
	fatal_error("record length invalid");
    unsigned char buffer[256];
    for (int j = 0; j < line_length; ++j)
	buffer[j] = get_byte();
    if (use_checksums())
    {
	int n = checksum_get();
	if (n != 0xFF)
	    fatal_error("checksum mismatch (%02X != FF)", n);
    }
    --line_length;

    int naddr = 2;
    srec_record::type_t type = srec_record::type_unknown;
    switch (tag)
    {
    case 1:
	// data
	type = srec_record::type_data;
	break;

    case 2:
	// data
	type = srec_record::type_data;
	naddr = 3;
	break;

    case 3:
	// data
	type = srec_record::type_data;
	naddr = 4;
	break;

    case 5:
	// data count
	type = srec_record::type_data_count;
	//
        // Just in case some smarty-pants uses the Green Hills trick, we
        // cope with address size crap the same as Motorola S-Record.
	//
	if (line_length >= 2 && line_length <= 4)
	    naddr = line_length;
	break;

    case 6:
	// data count
	type = srec_record::type_data_count;
	//
        // Just in case some smarty-pants uses the Green Hills trick, we
        // cope with address size crap the same as Motorola S-Record.
	//
	naddr = 3;
	if (line_length == 4)
	    naddr = line_length;
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
srec_input_file_stewie::read(srec_record &record)
{
    for (;;)
    {
	if (!read_inner(record))
	{
	    if (!seen_some_input && garbage_warning)
		fatal_error("file contains no data");
	    if (!header_seen)
	    {
		warning("no header record");
		header_seen = true;
	    }
	    if (data_count <= 0)
		warning("file contains no data");
	    if (!termination_seen)
	    {
		warning("no start_address record");
		termination_seen = true;
	    }
	    return 0;
	}
	seen_some_input = true;
	if (record.get_type() != srec_record::type_header && !header_seen)
	{
	    warning("no header record");
	    header_seen = true;
	}
	switch (record.get_type())
	{
	case srec_record::type_unknown:
	    fatal_error("record type not recognised");
	    break;

	case srec_record::type_header:
	    if (header_seen)
		warning("redundant header record");
	    if (record.get_address())
	    {
		warning("address in header record ignored");
		record.set_address(0);
	    }
	    header_seen = true;
	    break;

	case srec_record::type_data:
	    ++data_count;
	    if (record.get_length() == 0)
	    {
		warning("empty data record ignored");
		continue;
	    }
	    break;

	case srec_record::type_data_count:
	    {
		srec_record::address_t addr = record.get_address();
		srec_record::address_t mask = 0xFFFF;
		while (addr > mask)
		    mask = ~(~mask << 8);
		mask &= data_count;
		if (addr != mask)
		{
		    fatal_error
		    (
			"data record count mismatch (file %ld, read %ld)",
			addr,
			mask
		    );
		}
	    }
	    continue;

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
srec_input_file_stewie::get_file_format_name()
    const
{
    return "mobile phone signed binary (SBN)";
}


const char *
srec_input_file_stewie::mode()
    const
{   
    return "rb";
}
