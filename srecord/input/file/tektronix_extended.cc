//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2008, 2010 Peter Miller
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

#include <srecord/input/file/tektronix_extended.h>
#include <srecord/record.h>


srecord::input_file_tektronix_extended::~input_file_tektronix_extended()
{
}


srecord::input_file_tektronix_extended::input_file_tektronix_extended(
        const std::string &a_file_name) :
    srecord::input_file(a_file_name),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false)
{
}


srecord::input::pointer
srecord::input_file_tektronix_extended::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_tektronix_extended(a_file_name));
}


bool
srecord::input_file_tektronix_extended::read_inner(srecord::record &record)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return false;
        if (c == '%')
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
                return false;
            if (c == '\n')
                break;
        }
    }
    int length = get_byte();
    int tag = get_nibble();
    int csum = ((length >> 4) & 15) + (length & 15) + tag;
    if (length < 2)
        fatal_error("line length invalid");
    int csumX = get_byte();

    int addr_len = get_nibble();
    csum += addr_len;
    if (addr_len < 1 || addr_len > 8 || length < addr_len + 1)
        fatal_error("address length invalid");
    unsigned long address = 0;
    --length;
    while (addr_len > 0)
    {
        int n = get_nibble();
        csum += n;
        address = address * 16 + n;
        --addr_len;
        --length;
    }
    if (length & 1)
        fatal_error("data length invalid");
    length >>= 1;

    unsigned char buffer[124];
    for (int j = 0; j < length; ++j)
    {
        int n = get_byte();
        buffer[j] = n;
        csum += ((n >> 4) & 15) + (n & 15);
    }
    if (csumX != csum)
        fatal_error("checksum mismatch (%02X)", csum);
    if (get_char() != '\n')
        fatal_error("end-of-line expected");

    srecord::record::type_t type = srecord::record::type_unknown;
    switch (tag)
    {
    default:
        fatal_error("unknown tag (%X)", tag);

    case 6:
        // data
        type = srecord::record::type_data;
        break;

    case 8:
        // termination
        type = srecord::record::type_execution_start_address;
        break;
    }
    record = srecord::record(type, address, buffer, length);
    return true;
}


bool
srecord::input_file_tektronix_extended::read(srecord::record &record)
{
    for (;;)
    {
        if (!read_inner(record))
        {
            if (!seen_some_input)
                fatal_error("file contains no data");
            if (!termination_seen)
            {
                warning("no execution start address record");
                termination_seen = true;
            }
            return false;
        }
        seen_some_input = true;
        if
        (
            record.get_type() != srecord::record::type_execution_start_address
        &&
            termination_seen
        )
        {
            warning("termination record should be last");
            termination_seen = false;
        }
        switch (record.get_type())
        {
        case srecord::record::type_unknown:
        case srecord::record::type_header:
        case srecord::record::type_data_count:
            fatal_error("record type not recognised");
            break;

        case srecord::record::type_data:
            ++data_record_count;
            if (record.get_length() == 0)
            {
                warning("empty data record ignored");
                continue;
            }
            break;

        case srecord::record::type_execution_start_address:
            if (record.get_length() > 0)
            {
                warning("data in execution start address record ignored");
                record.set_length(0);
            }
            if (termination_seen)
                warning("redundant execution start address record");
            termination_seen = true;
            break;
        }
        break;
    }
    return true;
}


const char *
srecord::input_file_tektronix_extended::get_file_format_name()
    const
{
    return "Tektronix Extended";
}
