/**
 * srecord - manipulate eprom load files
 * Copyright (C) 2002, 2006-2010 Peter Miller
 *
 * Ohio Scientific 65A output is:
 *
 * Copyright (c) 2022 Glitch Works, LLC
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

#include <srecord/output/file/os65a.h>
#include <srecord/record.h>


srecord::output_file_os65a::~output_file_os65a()
{
    if (state == state_load && enable_footer_flag)
    {
        /**
         * If we're at the end of the file and haven't emitted a GO address,
         * return to command mode.
         */
        put_string("R");
    }
}


srecord::output_file_os65a::output_file_os65a(const std::string &a_file_name) :
    srecord::output_file(a_file_name),
    pref_block_size(32),
    address(0),
    state(state_null)
{
    if (line_termination == line_termination_native)
    {
        // Use CRLF termination, the 65A monitor ignores, but it looks better
        line_termination = line_termination_crlf;
    }
}


srecord::output::pointer
srecord::output_file_os65a::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_os65a(a_file_name));
}


void
srecord::output_file_os65a::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // All header data is discarded
        break;

    case srecord::record::type_data:
        if
        (
            address != record.get_address()
        ||
            (state == state_null && !enable_optional_address_flag)
        )
        {
            address = record.get_address();
            put_stringf("RL%04lX\n", address);
            state = state_load;
        }
        for (size_t j = 0; j < record.get_length(); j++)
        {
            put_byte(record.get_data(j));
            address++;
        }

        // This will trigger line_termination behavior
        put_char('\n');
        break;

    case srecord::record::type_data_count:
        // Not supported by this format, ignore
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            /**
             * Set up PCH and PCL, emit GO command. No newline as we are going
             * to leave the loader.
             */
            put_stringf("RL012E%04XRG", record.get_address());
            state = state_go;
        }
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_os65a::line_length_set(int nchars)
{
    // Divide by two as we'll be doing hex representation
    nchars = nchars / 2;

    // Basic constraints
    if (nchars < 1)
        nchars = 2;
    else if (nchars > 255)
        nchars = 25;

    // Constraint for max data length
    if (nchars > srecord::record::max_data_length)
        nchars = srecord::record::max_data_length;

    pref_block_size = nchars;
}


void
srecord::output_file_os65a::address_length_set(int)
{
    // Always 16-bit, ignore
}


bool
srecord::output_file_os65a::preferred_block_size_set(int nbytes)
{
    // Sanity check
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;

    // Don't allow lines longer than 255 bytes
    if (nbytes > 255)
        return false;

    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_os65a::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srecord::output_file_os65a::mode()
    const
{
    return "wb";
}


const char *
srecord::output_file_os65a::format_name()
    const
{
    return "Ohio Scientific 65A";
}
