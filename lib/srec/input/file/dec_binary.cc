//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001-2003, 2005-2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_input_file_dec_binary class
//


#include <lib/srec/input/file/dec_binary.h>
#include <lib/srec/record.h>


srec_input_file_dec_binary::srec_input_file_dec_binary(
        const string &a_file_name) :
    srec_input_file(a_file_name),
    current_pos(0),
    current_length(0),
    current_address(0)
{
}


srec_input_file_dec_binary::~srec_input_file_dec_binary()
{
}


bool
srec_input_file_dec_binary::skip_nul()
{
    for (;;)
    {
        int c = peek_char();
        if (c < 0)
            return false;
        if (c)
            return true;
        get_char();
    }
}


int
srec_input_file_dec_binary::get_byte()
{
    int c = get_char();
    if (c < 0)
        fatal_error("premature end-of-file");
    checksum_add(c);
    return c;
}


int
srec_input_file_dec_binary::get_word()
{
    // Little endian
    unsigned char n1 = get_byte();
    unsigned char n2 = get_byte();
    unsigned short n = (n1 | (n2 << 8));
    return n;
}


int
srec_input_file_dec_binary::read(srec_record &record)
{
    //
    // The records in the file are much larger than SRecord usually
    // handles, so we keep track of where we are within the current
    // record.  When we run out, we go on to the next one.
    //
    if (current_pos >= current_length)
    {
        if (current_length)
        {
            // Verify the checksum at end of the record.
            get_byte();
            if (use_checksums() && checksum_get())
            {
                fatal_error("checksum mismatch (%02X)", checksum_get());
            }
        }
        current_pos = 0;
        current_length = 0;
        current_address = 0;

        //
        // You are allowed to have NUL characters between records.
        // Presumably this is used to get recordss onto even
        // byte boundardies and/or whole block boundaries.
        //
        if (!skip_nul())
            return 0;

        checksum_reset();
        int tag = get_word();
        if (tag != 1)
            fatal_error("record type %d unknown", tag);
        int length = get_word();
        if (length < 6)
            fatal_error("record length (%d) invalid", length);
        length -= 6;
        current_address = get_word();
        if (length == 0)
        {
            get_byte();
            if (use_checksums() && checksum_get())
            {
                fatal_error("checksum mismatch (%02X)", checksum_get());
            }
            srec_record::type_t type = srec_record::type_start_address;
            record = srec_record(type, current_address, 0, 0);
            seek_to_end();
            return 1;
        }
        current_length = length;
    }

    //
    // Read as many bytes as possible into the buffer.
    //
    unsigned char buffer[srec_record::max_data_length];
    int nbytes = srec_record::max_data_length;
    if (current_pos + nbytes > current_length)
        nbytes = current_length - current_pos;
    for (int j = 0; j < nbytes; ++j)
        buffer[j] = get_byte();

    //
    // Create a data record and return.
    //
    srec_record::type_t type = srec_record::type_data;
    record = srec_record(type, current_address, buffer, nbytes);
    current_address += nbytes;
    current_pos += nbytes;
    return 1;
}


const char *
srec_input_file_dec_binary::mode()
    const
{
    return "rb";
}


const char *
srec_input_file_dec_binary::get_file_format_name()
    const
{
    return "DEC Binary (XXDP)";
}
