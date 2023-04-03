/**
 * srecord - manipulate eprom load files
 * Copyright (C) 2002, 2006-2010 Peter Miller
 *
 * Ohio Scientific 65A input is:
 *
 * Copyright (c) 2023 Glitch Works, LLC
 * http://www.glitchwrks.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <srecord/arglex/tool.h>
#include <srecord/input/file/os65a.h>
#include <srecord/record.h>


srecord::input_file_os65a::input_file_os65a(const std::string &a_file_name) :
    srecord::input_file(a_file_name),
    address(0),
    go_address_high(0),
    go_address_low(0),
    go_address_high_loaded(false),
    go_address_low_loaded(false),
    state(state_null)
{
}


srecord::input_file::pointer
srecord::input_file_os65a::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_os65a(a_file_name));
}


bool
srecord::input_file_os65a::read_inner(srecord::record &record)
{
    for (;;)
    {
        if (ignore_the_rest)
            return false;

        int c = get_char();
        switch (c)
        {

        // Return command, back to command state
        case 'R':
            state = state_command;
            continue;

        // Load state, next two bytes are address, following are data
        case 'L':
            state = state_load_address;
            address = get_word_be();
            state = state_load_data;

            if (address == 0x012E)
            {
                // Loading address 0x012E is a special case, it is the value
                // placed in the high byte of the program counter (PC) when the
                // GO command is executed.
                state = state_pch_loaded;
                go_address_high = get_byte();
                go_address_high_loaded = true;
            }

            if (address == 0x012F)
            {
                // This would be an unusual case where the PC is loaded in two
                // different chunks. Not likely, but permissable.
                go_address_low = get_byte();
                go_address_low_loaded = true;
                warning("PC value written in two separate load commands");
            }

            continue;

        // Go command, start executing a program
        case 'G':
            state = state_go;

            if (!go_address_high_loaded || !go_address_low_loaded)
            {
                warning("GO command found, but program counter not initialized!");
            }

            address = (go_address_high << 8) | go_address_low;

            record =
                srecord::record
                (
                    srecord::record::type_execution_start_address,
                    address,
                    0,
                    0
                );

            ignore_the_rest = true;
            return true;

        // End-of-file
        case -1:
            return false;

        // Eat whitespace
        case '\r':
        case '\n':
        case ' ':
            continue;

        // None of the state changes or whitespace, so it's either data or
        // an error.
        default:
            if (state == state_pch_loaded)
            {
                // We've got the PC high address byte, and if we're here the
                // next thing is the low address byte.
                get_char_undo(c);
                go_address_low = get_byte();
                go_address_low_loaded = true;
                state = state_load_data;
                continue;
            }

            if (state == state_load_data)
            {
                // At this point, we know we've gotten the first letter of a
                // byte in hex.
                get_char_undo(c);
                
                uint8_t buf[1];
                buf[0] = get_byte();
                record =
                    srecord::record
                    (
                        srecord::record::type_data,
                        address++,
                        buf,
                        1
                    );

                return true;
            }

            if (state == state_null)
            {
                // Not in a known state, so there must be something non-65A in
                // the input file.
                fatal_error("unknown command");
                return false;
            }
        }
    }
}


bool
srecord::input_file_os65a::read(srecord::record &record)
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
srecord::input_file_os65a::get_file_format_name()
    const
{
    return "Ohio Scientific 65A";
}


int
srecord::input_file_os65a::format_option_number()
    const
{
    return arglex_tool::token_os65a;
}
