//
// srecord - manipulate eprom load files
// Copyright (C) 2001-2003, 2005-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <srecord/input/file/signetics.h>
#include <srecord/record.h>


srecord::input_file_signetics::~input_file_signetics()
{
}


srecord::input_file_signetics::input_file_signetics(
        const std::string &a_filename) :
    srecord::input_file(a_filename),
    garbage_warning(false),
    seen_some_input(false)
{
}


srecord::input::pointer
srecord::input_file_signetics::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_signetics(a_file_name));
}


void
srecord::input_file_signetics::checksum_add(unsigned char n)
{
    checksum ^= n;
    checksum = (checksum << 1) | ((checksum >> 7) & 1);
}


int
srecord::input_file_signetics::read_inner(srecord::record &record)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return 0;
        if (c == ':')
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
    checksum_reset();
    long address = get_word();
    int length = get_byte();
    if (length == 0)
    {
        // this is the end indicator
        if (get_char() != '\n')
            fatal_error("end-of-line expected");
        return 0;
    }

    int running_checksum = checksum_get();
    int csum = get_byte();
    if (use_checksums() && running_checksum != csum)
    {
        fatal_error
        (
            "address checksum mismatch (%02X != %02X)",
            running_checksum,
            csum
        );
    }

    checksum_reset();
    unsigned char buffer[256];
    for (int j = 0; j < length; ++j)
        buffer[j] = get_byte();

    running_checksum = checksum_get();
    csum = get_byte();
    if (use_checksums() && running_checksum != csum)
    {
        fatal_error
        (
            "data checksum mismatch (%02X != %02X",
            running_checksum,
            csum
        );
    }

    if (get_char() != '\n')
        fatal_error("end-of-line expected");

    srecord::record::type_t type = srecord::record::type_data;
    record = srecord::record(type, address, buffer, length);
    return 1;
}


bool
srecord::input_file_signetics::read(srecord::record &record)
{
    if (!read_inner(record))
    {
        if (!seen_some_input && garbage_warning)
            fatal_error("file contains no data");
        return false;
    }
    seen_some_input = true;
    return true;
}


const char *
srecord::input_file_signetics::get_file_format_name()
    const
{
    return "Signetics";
}
