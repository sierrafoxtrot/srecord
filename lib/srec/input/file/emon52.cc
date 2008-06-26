//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001-2003, 2005-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//


#include <lib/srec/input/file/emon52.h>
#include <lib/srec/record.h>


srec_input_file_emon52::~srec_input_file_emon52()
{
}


srec_input_file_emon52::srec_input_file_emon52(const string &a_file_name) :
    srec_input_file(a_file_name)
{
}


srec_input::pointer
srec_input_file_emon52::create(const string &a_file_name)
{
    return pointer(new srec_input_file_emon52(a_file_name));
}


void
srec_input_file_emon52::skip_white_space()
{
    for (;;)
    {
        int c = peek_char();
        if (c != ' ')
            return;
        get_char();
    }
}


bool
srec_input_file_emon52::read(srec_record &record)
{
    //
    // This format has no execution start address record type, and no
    // magic start character.  So look ahead to see if there is anything
    // more.
    //
    if (peek_char() < 0)
        return false;

    //
    // Looks like there should be a record.  Read it all in.
    //
    int length = get_byte();
    if (length == 0)
        fatal_error("data length of zero is not valid");
    skip_white_space();
    unsigned long address = get_word();
    if (get_char() != ':')
        fatal_error("colon expected");
    checksum_reset();
    unsigned char buffer[256];
    for (int j = 0; j < length; ++j)
    {
        skip_white_space();
        buffer[j] = get_byte();
    }
    skip_white_space();
    int csumX = checksum_get16();
    int csum = get_word();
    if (use_checksums() && csumX != csum)
        fatal_error("checksum mismatch (%04X != %04X)", csumX, csum);
    if (get_char() != '\n')
        fatal_error("end-of-line expected");

    srec_record::type_t type = srec_record::type_data;
    record = srec_record(type, address, buffer, length);
    return true;
}


const char *
srec_input_file_emon52::get_file_format_name()
    const
{
    return "Elektor Monitor (EMON52)";
}
