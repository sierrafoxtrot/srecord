//
//	srecord - manipulate eprom load files
//	Copyright (C) 2004 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_file_aomf class
//

#pragma implementation "srec_input_file_aomf"

#include <srec/input/file/aomf.h>
#include <srec/record.h>


srec_input_file_aomf::~srec_input_file_aomf()
{
    if (current_buffer)
	delete [] current_buffer;
}


srec_input_file_aomf::srec_input_file_aomf(const char *filename) :
    srec_input_file(filename),
    current_buffer(0),
    current_length(0),
    current_maximum(0),
    current_pos(0),
    current_address(0),
    state(expecting_header)
{
}


int
srec_input_file_aomf::get_byte()
{
    int c = get_char();
    if (c < 0)
	fatal_error("premature end-of-file");
    checksum_add(c);
    return c;
}


int
srec_input_file_aomf::get_word()
{
    // Little endian
    unsigned char n1 = get_byte();
    unsigned char n2 = get_byte();
    unsigned short n = (n1 | (n2 << 8));
    return n;
}


int
srec_input_file_aomf::slurp()
{
    current_pos = 0;
    current_length = 0;
    if (peek_char() < 0)
	return -1;
    checksum_reset();
    int type = get_byte();
    size_t length = get_word();
    if (length == 0)
	fatal_error("invalid record length");
    --length; // includes checksum byte
    if (length > current_maximum)
    {
	if (current_buffer)
	    delete [] current_buffer;
	while (current_maximum < length)
	    current_maximum = current_maximum * 2 + 64;
	current_buffer = new unsigned char [current_maximum];
    }
    current_length = length;
    for (size_t j = 0; j < length; ++j)
	current_buffer[j] = get_byte();
    get_byte();
    if (checksum_get() != 0)
	fatal_error("checksum mismatch");
    return type;
}


int
srec_input_file_aomf::read(srec_record &record)
{
    for (;;)
    {
	switch (state)
	{
	case expecting_header:
	    if (slurp() != 0x02)
		fatal_error("Module Header Record expected");
	    state = expecting_data;
	    if (current_length > 0)
	    {
		unsigned nbytes = current_buffer[0];
		// should be exactly (current_length-3)
		if (nbytes > current_length - 1)
		    nbytes = current_length - 1;
		record =
		    srec_record
		    (
			srec_record::type_header,
			0, // address
			current_buffer + 1,
			nbytes
		    );
	    }
	    else
		record = srec_record(srec_record::type_header, 0, 0, 0);
	    current_length = 0;
	    return 1;

	case expecting_eof:
	    if (slurp() >= 0)
		fatal_error("end-of-file expected");
	    return 0;

	case expecting_data:
	    if (current_pos < current_length)
	    {
		size_t nbytes = current_length - current_pos;
		if (nbytes > srec_record::max_data_length)
		    nbytes = srec_record::max_data_length;
		record =
		    srec_record
		    (
			srec_record::type_data,
			current_address,
			current_buffer + current_pos,
			nbytes
		    );
		current_pos += nbytes;
		current_address += nbytes;
		return 1;
	    }
	    switch (slurp())
	    {
	    case 0x02:
		fatal_error("too many Module Header Records");

	    case 0x04:
		state = expecting_eof;
		record = srec_record(srec_record::type_start_address, 0, 0, 0);
		return 1;

	    case 0x06:
		if (current_length < 3)
		    fatal_error("malformed Content Record");
		current_address = 
		    (
			// strictly speaking, this byte should be ignored
			((unsigned long)current_buffer[0] << 16)
		    |
			// length is little-endian
			((unsigned long)current_buffer[2] << 8)
		    |
			((unsigned long)current_buffer[1])
		    );
		current_pos = 3;
		break;

	    case 0x01:
	    case 0x0E:
	    case 0x12:
	    case 0x16:
	    case 0x18:
		// Ignore these
		current_length = 0;
		break;

	    default:
		// Nothing else should be in an AOMF file,
		// even hough it may be valid in an OMF file.
		fatal_error("unexpected record type");
	    }
	    break;
	}
    }
}


const char *
srec_input_file_aomf::mode()
    const
{
    return "rb";
}


const char *
srec_input_file_aomf::get_file_format_name()
    const
{
    return "Intel Absolute Object Module Format (AOMF)";
}
