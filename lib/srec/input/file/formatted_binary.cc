//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006, 2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_input_file_formatted_binary class
//


#include <lib/srec/input/file/formatted_binary.h>
#include <lib/srec/record.h>


srec_input_file_formatted_binary::~srec_input_file_formatted_binary()
{
}


srec_input_file_formatted_binary::srec_input_file_formatted_binary(
        const string &a_file_name) :
    srec_input_file(a_file_name),
    header_seen(false),
    upper_bound(0),
    address(0),
    trailer_seen(false),
    check_sum(0)
{
}


int
srec_input_file_formatted_binary::read(srec_record &record)
{
    if (!header_seen)
    {
        // Skip leading NULs
        int c;
        for (;;)
        {
            c = get_char();
            if (c)
                break;
        }
        if (c != 0x08)
        {
            format_error:
            fatal_error("format error");
            // NOTREACHED
        }
        c = get_char();
        if (c != 0x1C)
            goto format_error;
        c = get_char();
        int len = 4;
        if (c == 0x2A)
        {
            c = get_char();
            if (c != 0x49)
                goto format_error;
        }
        else if (c == 0x3E)
        {
            c = get_char();
            if (c != 0x6B)
                goto format_error;
            len = 8;
        }
        else
            goto format_error;
        c = get_char();
        if (c != 0x08)
            goto format_error;
        c = get_char();
        if (c != 0x00)
            goto format_error;
        upper_bound = 0;
        while (len > 0)
        {
            c = get_char();
            if (c < 0 || c >= 16)
                goto format_error;
            upper_bound = (upper_bound << 4) + c;
            --len;
        }
        c = get_char();
        if (c != 0xFF)
            goto format_error;
        header_seen = true;
        address = 0;
    }
    if (address >= upper_bound)
    {
        if (!trailer_seen)
        {
            if (get_char() != 0)
                goto format_error;
            if (get_char() != 0)
                goto format_error;
            int c1 = get_char();
            if (c1 < 0)
                goto format_error;
            int c2 = get_char();
            if (c2 < 0)
                goto format_error;
            int x = (c1 << 8) + c2;
            if ((check_sum & 0xFFFF) != (x & 0xFFFF))
            {
                fatal_error
                (
                    "checksum mismatch (%04X != %04X)",
                    (check_sum & 0xFFFF),
                    (x & 0xFFFF)
                );
            }
            for (;;)
            {
                // skip trailing NULs
                int c = get_char();
                if (c < 0)
                    break;
                if (c != 0)
                    goto format_error;
            }
            trailer_seen = true;
        }
        return 0;
    }

    long datalen = upper_bound - address;
    if (datalen > srec_record::max_data_length)
        datalen = srec_record::max_data_length;
    unsigned char data[srec_record::max_data_length];
    for (long j = 0; j < datalen; ++j)
    {
        int c = get_char();
        if (c < 0)
            goto format_error;
        data[j] = c;
        check_sum += c;
    }
    record = srec_record(srec_record::type_data, address, data, datalen);
    address += datalen;
    return 1;
}


const char *
srec_input_file_formatted_binary::get_file_format_name()
    const
{
    return "Formatted Binary";
}


const char *
srec_input_file_formatted_binary::mode()
    const
{
    return "rb";
}
