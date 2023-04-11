//
// srecord - manipulate eprom load files
// Copyright (C) 2023 Daniel Anselmi
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>
#include <cctype>

#include <srecord/arglex/tool.h>
#include <srecord/input/file/gowin_fs.h>
#include <srecord/record.h>


srecord::input_file_gowin_fs::input_file_gowin_fs(
    const std::string &a_filename
) :
    input_file(a_filename),
    address(0),
    done(false)
{
}

srecord::input_file::pointer
srecord::input_file_gowin_fs::create(const std::string &a_filename)
{
    return pointer(new input_file_gowin_fs(a_filename));
}

// See base class for documentation.
bool srecord::input_file_gowin_fs::read(record &record)
{
    if (done)
        return false;

    int length = 0;
    srecord::record::data_t data[srecord::record::max_data_length];

    for (;;)
    {
        int c = peek_char();
        if (c == EOF)
        {
            done = true;
            break;
        }
        else if (c == '/')
        {
            skip_until_eol();
            continue;
        }
        else if (c == '\n' || c == ' ' || c == '\t')
        {
            // Files from Gowin IDE don't have spaces or tabs between the
            // data or lines with only spaces. We accept them here anyway.
            get_char();
            continue;
        }

        /* reading 8 chars to get a byte */
        srecord::record::data_t d = get_byte_binary();
        data[length++] = d;

        if (length >= (int)sizeof(data))
            break;
    }

    record = srecord::record(srecord::record::type_data, address, data, length);
    address += length;
    return true;
}

void
srecord::input_file_gowin_fs::skip_until_eol()
{
    while (get_char() != '\n');
}

int
srecord::input_file_gowin_fs::get_byte_binary()
{
    int val = 0;
    for (int i = 0x80; i; i >>= 1)
    {
        int c = get_binary_digit_value();
        if (c < 0)
            fatal_error("expecting binary digit");
        if (c)
            val |= i;
    }
    return val;
}

int
srecord::input_file_gowin_fs::get_binary_digit_value()
{
    int c = get_char();
    if (c != '0' && c != '1')
        return -1;
    return c - '0';
}

const char *
srecord::input_file_gowin_fs::get_file_format_name()
    const
{
    return "Gowin bitstream";
}


int
srecord::input_file_gowin_fs::format_option_number()
    const
{
    return arglex_tool::token_gowin_fs;
}
