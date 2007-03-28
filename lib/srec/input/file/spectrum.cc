//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006, 2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_input_file_spectrum class
//


#include <cctype>

#include <lib/srec/input/file/spectrum.h>
#include <lib/srec/record.h>


srec_input_file_spectrum::~srec_input_file_spectrum()
{
}


srec_input_file_spectrum::srec_input_file_spectrum(const string &a_file_name) :
    srec_input_file(a_file_name),
    header_seen(false),
    trailer_seen(false),
    file_contains_data(false)
{
}


int
srec_input_file_spectrum::get_decimal()
{
    int c;
    for (;;)
    {
        c = get_char();
        if (c < 0)
        {
            format_error:
            fatal_error("decimal number expected");
        }
        if (isspace((unsigned char)c))
            continue;
        if (!isdigit((unsigned char)c))
            goto format_error;
        break;
    }
    int result = c - '0';
    for (;;)
    {
        c = get_char();
        if (c < 0)
            break;
        if (!isdigit((unsigned char)c))
        {
            get_char_undo(c);
            break;
        }
        result = (result * 10) + (c - '0');
    }
    return result;
}


static inline bool
is_binary_digit(int c)
{
    return ((c == '0' || c == '1'));
}


int
srec_input_file_spectrum::get_binary()
{
    int c;
    for (;;)
    {
        c = get_char();
        if (c < 0)
        {
            format_error:
            fatal_error("binary number expected");
        }
        if (isspace((unsigned char)c))
            continue;
        if (!is_binary_digit((unsigned char)c))
            goto format_error;
        break;
    }
    int result = c - '0';
    for (;;)
    {
        c = get_char();
        if (!is_binary_digit(c))
        {
            get_char_undo(c);
            break;
        }
        result = (result << 1) + (c - '0');
    }
    return result;
}


int
srec_input_file_spectrum::read(srec_record &record)
{
    //
    // Check for the file header
    //
    if (!header_seen)
    {
        bool garbage_warning = false;
        for (;;)
        {
            int c = get_char();
            if (c < 0)
                fatal_error("no start character");
            if (c == 2)
            {
                header_seen = true;
                break;
            }
            if (!garbage_warning)
            {
                warning("ignoring garbage lines");
                garbage_warning = true;
            }
        }
    }

    //
    // Check for the file trailer.
    //
    if (trailer_seen)
        return 0;
    for (;;)
    {
        int c = get_char();
        if (c < 0 || c == 3)
        {
            if (!file_contains_data)
                fatal_error("file contains no data");
            trailer_seen = true;
            return 0;
        }
        if (isspace((unsigned char)c))
            continue;
        get_char_undo(c);
        break;
    }

    unsigned long address = get_decimal();
    unsigned char data = get_binary();
    record = srec_record(srec_record::type_data, address, &data, 1);
    file_contains_data = true;
    return 1;
}

const char *
srec_input_file_spectrum::get_file_format_name ()
    const
{
    return "Spectrum";
}
