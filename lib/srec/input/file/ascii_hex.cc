//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2003, 2005-2008 Peter Miller
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

#include <cctype>

#include <lib/srec/input/file/ascii_hex.h>
#include <lib/srec/record.h>


srec_input_file_ascii_hex::~srec_input_file_ascii_hex()
{
    // check termination?
}


srec_input_file_ascii_hex::srec_input_file_ascii_hex(const string &a_filename) :
    srec_input_file(a_filename),
    garbage_warning(false),
    seen_some_input(false),
    address(0),
    state(state_initial)
{
}


srec_input::pointer
srec_input_file_ascii_hex::create(const string &a_filename)
{
    return pointer(new srec_input_file_ascii_hex(a_filename));
}


int
srec_input_file_ascii_hex::read_inner(srec_record &record)
{
    if (state == state_ignore)
        return 0;

    if (state == state_initial)
    {
        for (;;)
        {
            int c = get_char();
            if (c < 0)
            {
                state = state_ignore;
                return 0;
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
            return 0;
        }
        if (isxdigit(c))
        {
            unsigned char c = get_byte();
            record = srec_record(srec_record::type_data, address, &c, 1);
            int sep = get_char();
            if (sep >= 0 && !isspace((unsigned char)sep))
                fatal_error("not execution character");
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
            return 1;
        }
        c = get_char();
        switch (c)
        {
        case 3:
            state = state_ignore;
            return 0;

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
                if (c == ',' || c == '.')
                    break;
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


bool
srec_input_file_ascii_hex::read(srec_record &record)
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
srec_input_file_ascii_hex::get_file_format_name()
    const
{
    return "Ascii Hex";
}
