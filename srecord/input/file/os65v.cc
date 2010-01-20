//
//      srecord - manipulate eprom load files
//      Copyright (C) 2002, 2006-2008, 2010 Peter Miller
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

#include <srecord/input/file/os65v.h>
#include <srecord/record.h>


srec_input_file_os65v::~srec_input_file_os65v()
{
}


srec_input_file_os65v::srec_input_file_os65v(const std::string &a_file_name) :
    srec_input_file(a_file_name),
    seen_some_input(false),
    address(0),
    state(0),
    ignore_the_rest(false)
{
}


srec_input::pointer
srec_input_file_os65v::create(const std::string &a_file_name)
{
    return pointer(new srec_input_file_os65v(a_file_name));
}


int
srec_input_file_os65v::read_inner(srec_record &record)
{
    for (;;)
    {
        if (ignore_the_rest)
            return 0;
        int c = get_char();
        switch (c)
        {
        case -1:
            //
            // End of file.
            //
            return 0;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            {
                //
                // We're looking at a number.
                // It could be an address or a data byte.
                //
                get_char_undo(c);
                unsigned long n = 0;
                for (;;)
                {
                    n = (n << 4) | get_nibble();
                    switch (peek_char())
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                        continue;
                    }
                    break;
                }
                if (state == '.')
                {
                    address = n;
                    continue;
                }
                if (state == '/')
                {
                    //
                    // As a special case, writing 0 to address 0x00FD
                    // also ends input (probably poking the "do I read
                    // audio input" flag).
                    //
                    if (address == 0x00FD && n == 0)
                    {
                        ignore_the_rest = true;
                        return 0;
                    }
                    unsigned char buf[1];
                    buf[0] = n;
                    record =
                        srec_record
                        (
                            srec_record::type_data,
                            address,
                            buf,
                            1
                        );
                    return 1;
                }
                fatal_error("mode not set");
                return 0;
            }

        case 'G':
            record =
                srec_record
                (
                    srec_record::type_execution_start_address,
                    address,
                    0,
                    0
                );
            ignore_the_rest = true;
            return 1;

        case '\r':
            if (state == '/')
                ++address;
            if (peek_char() == '\n')
                get_char();
            continue;

        case '\n':
            if (state == '/')
                ++address;
            continue;

        case '.':
        case '/':
            state = c;
            continue;

        default:
            fatal_error("unknown command");
            return 0;
        }
    }
}


bool
srec_input_file_os65v::read(srec_record &record)
{
    if (!read_inner(record))
    {
        if (!seen_some_input)
            fatal_error("file contains no data");
        return false;
    }
    seen_some_input = true;
    return true;
}


const char *
srec_input_file_os65v::get_file_format_name()
    const
{
    return "Ohio Scientific";
}
