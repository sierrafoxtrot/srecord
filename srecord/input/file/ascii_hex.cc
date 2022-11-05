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

#include <cctype>

#include <srecord/arglex/tool.h>
#include <srecord/input/file/ascii_hex.h>
#include <srecord/record.h>


srecord::input_file_ascii_hex::~input_file_ascii_hex()
{
    // check termination?
}


srecord::input_file_ascii_hex::input_file_ascii_hex(
    const std::string &a_filename
) :
    input_file(a_filename),
    garbage_warning(false),
    seen_some_input(false),
    address(0),
    state(state_initial)
{
}


auto
srecord::input_file_ascii_hex::create(const std::string &a_filename) -> srecord::input_file::pointer
{
    return pointer(new input_file_ascii_hex(a_filename));
}


auto
srecord::input_file_ascii_hex::read_inner(record &result) -> bool
{
    if (state == state_ignore) {
        return false;
}

    if (state == state_initial)
    {
        for (;;)
        {
            int c = get_char();
            if (c < 0)
            {
                state = state_ignore;
                return false;
            }
            if (c == 2)
            {
                state = state_body;
                break;
            }
            if (!garbage_warning)
            {
                warning("ignoring garbage lines");
                garbage_warning = true;
            }
        }
    }

    for (;;)
    {
        int c = peek_char();
        if (c < 0)
        {
            return false;
        }
        if (isxdigit(c) != 0)
        {
            unsigned char c = get_byte();
            result = record(record::type_data, address, &c, 1);
            int sep = get_char();
            if (sep >= 0 && (isspace((unsigned char)sep) == 0)) {
                fatal_error("not execution character");
}
            ++address;
            switch (peek_char())
            {
            case '\'':
            case ',':
            case '%':
            case ' ':
                // The documentation calls these an "execution" character.
                // Strictly speaking, the space isn't optional.
                get_char();
                break;
            }
            return true;
        }
        c = get_char();
        switch (c)
        {
        case 3:
            state = state_ignore;
            return false;

        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '\f':
            break;

        default:
            fatal_error("illegal character");

        case '$':
            int command = get_char();
            unsigned long value = 0;
            for (;;)
            {
                value = (value << 4) + get_nibble();
                int c = get_char();
                if (c == ',' || c == '.') {
                    break;
}
                get_char_undo(c);
            }
            switch (command)
            {
            default:
                fatal_error("unknown command");

            case 'A':
                address = value;
                break;

            case 'S':
                if (use_checksums())
                {
                    unsigned short chk1 = checksum_get16();
                    unsigned short chk2 = value & 0xFFFF;
                    if (chk1 != chk2)
                    {
                        fatal_error
                        (
                            "checksum mismatch (%4.4X != %4.4X)",
                            chk1,
                            chk2
                        );
                    }
                }
                break;
            }
            break;
        }
    }
}


auto
srecord::input_file_ascii_hex::read(record &result) -> bool
{
    if (!read_inner(result))
    {
        if (!seen_some_input) {
            fatal_error("file contains no data");
}
        return false;
    }
    seen_some_input = true;
    return true;
}


auto
srecord::input_file_ascii_hex::get_file_format_name()
    const -> const char *
{
    return "Ascii Hex";
}


auto
srecord::input_file_ascii_hex::format_option_number()
    const -> int
{
    return arglex_tool::token_ascii_hex;
}
