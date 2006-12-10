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
// MANIFEST: functions to impliment the srec_input_file_tektronix_extended class
//


#include <lib/srec/input/file/tektronix_extended.h>
#include <lib/srec/record.h>


srec_input_file_tektronix_extended::srec_input_file_tektronix_extended(
	const char *filename) :
    srec_input_file(filename),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false)
{
}


srec_input_file_tektronix_extended::~srec_input_file_tektronix_extended()
{
}


int
srec_input_file_tektronix_extended::read_inner(srec_record &record)
{
    for (;;)
    {
	int c = get_char();
	if (c < 0)
	    return 0;
	if (c == '%')
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
    int length = get_byte();
    int tag = get_nibble();
    int csum = ((length >> 4) & 15) + (length & 15) + tag;
    if (length < 2)
	fatal_error("line length invalid");
    int csumX = get_byte();

    int addr_len = get_nibble();
    csum += addr_len;
    if (addr_len < 1 || addr_len > 8 || length < addr_len + 1)
	fatal_error("address length invalid");
    unsigned long address = 0;
    --length;
    while (addr_len > 0)
    {
	int n = get_nibble();
	csum += n;
	address = address * 16 + n;
	--addr_len;
	--length;
    }
    if (length & 1)
	fatal_error("data length invalid");
    length >>= 1;

    unsigned char buffer[124];
    for (int j = 0; j < length; ++j)
    {
	int n = get_byte();
	buffer[j] = n;
	csum += ((n >> 4) & 15) + (n & 15);
    }
    if (csumX != csum)
	fatal_error("checksum mismatch (%02X)", csum);
    if (get_char() != '\n')
	fatal_error("end-of-line expected");

    srec_record::type_t type = srec_record::type_unknown;
    switch (tag)
    {
    default:
	fatal_error("unknown tag (%X)", tag);

    case 6:
	// data
	type = srec_record::type_data;
	break;

    case 8:
	// termination
	type = srec_record::type_start_address;
	break;
    }
    record = srec_record(type, address, buffer, length);
    return 1;
}


int
srec_input_file_tektronix_extended::read(srec_record &record)
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
	    ++data_record_count;
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
srec_input_file_tektronix_extended::get_file_format_name()
    const
{
    return "Tektronix Extended";
}
