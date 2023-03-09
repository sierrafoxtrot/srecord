//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2005-2008, 2010, 2011, 2013 Peter Miller
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
#include <srecord/input/file/efinix_bit.h>
#include <srecord/record.h>


srecord::input_file_efinix_bit::~input_file_efinix_bit()
{
    // check termination?
}


srecord::input_file_efinix_bit::input_file_efinix_bit(
    const std::string &a_filename
) :
    input_file(a_filename),
    //garbage_warning(false),
    //seen_some_input(false),
    address(0),
    done(false)
{
}


srecord::input_file::pointer
srecord::input_file_efinix_bit::create(const std::string &a_filename)
{
    return pointer(new input_file_efinix_bit(a_filename));
}


// See base class for documentation.
bool srecord::input_file_efinix_bit::read(record &record)
{
    if (done)
        return false;

    int length = 0;
    unsigned char data[srecord::record::max_data_length];

    for (;;)
    {
        if (peek_char() == EOF)
        {
            done = true;
            break;        }

        int c = get_byte(); /* get two hey digits */
        data[length++] = c;

        c = get_char(); // newline
        if (c != '\n')
            return false;

        if (length >= (int)sizeof(data))
            break;
    }

    record = srecord::record(srecord::record::type_data, address, data, length);
    address += length;
    return true;
}

const char *
srecord::input_file_efinix_bit::get_file_format_name()
    const
{
    return "Efinix bitstream";
}


int
srecord::input_file_efinix_bit::format_option_number(void)
    const
{
    return arglex_tool::token_efinix_bit;
}
