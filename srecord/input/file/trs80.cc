//
// srecord - manipulate eprom load files
// Copyright (C) 2012 Peter Miller
//
// Code contribution by Eric Smith <eric@brouhaha.com>
// Copyright assigned to Peter Miller 15-Mar-2012.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/endian.h>
#include <srecord/input/file/trs80.h>
#include <srecord/record.h>


srecord::input_file_trs80::~input_file_trs80()
{
}


srecord::input_file_trs80::input_file_trs80(
    const std::string &a_file_name
) :
    srecord::input_file(a_file_name),
    termination_seen(false),
    data_seen(false)
{
}


srecord::input_file::pointer
srecord::input_file_trs80::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_trs80(a_file_name));
}


int
srecord::input_file_trs80::get_byte(void)
{
    int c = get_char();
    if (c < 0)
        fatal_error("premature end-of-file");
    return c;
}


bool
srecord::input_file_trs80::read(srecord::record &result)
{
    if (termination_seen)
        return false;
    for (;;)
    {
        //
        // The general format of a load record is a one-byte record type,
        // a one-byte record length, then the payload.  In the case of a
        // data record, transfer record, or end record, the first two bytes
        // of the payload are a memory address.
        //
        // In the case of Data Records only, Record Length byte values
        // of zero to two are considered to be lengths of 256 to 258,
        // respectively.
        //
        int rec_type = get_byte();
        int length = get_byte();
        if (rec_type == 0x01 && length <= 2)
            length += 256;
        unsigned char data[258];
        for (int j = 0; j < length; ++j)
            data[j] = get_byte();

        switch (rec_type)
        {
        case 0x01:
            // data
            {
                long address = decode_word_le(data);
                record::type_t type = record::type_data;
                result = srecord::record(type, address, data + 2, length - 2);
                data_seen = true;
                return true;
            }

        case 0x02:
            // transfer
        case 0x03:
            // end w/o transfer
            {
                if (length != 2)
                    fatal_error("record length (%d) invalid", length);
                long address = decode_word_le(data);
                record::type_t type = record::type_execution_start_address;
                result = srecord::record(type, address, 0, 0);
                termination_seen = true;
                return true;
            }

        case 0x05:
            // comment
            if (!data_seen)
            {
                // Use the comment record type as a kind of header record,
                // if it occurs early enough in the file.
                long address = 0;
                record::type_t type = record::type_header;
                result = srecord::record(type, address, data, length);
                return true;
            }
            break;

        default:
            // various, ignore
            warning("record type 0x%02X unknown", rec_type);
            break;
        }
    }
}


bool
srecord::input_file_trs80::is_binary(void)
    const
{
    return true;
}


const char *
srecord::input_file_trs80::get_file_format_name(void)
    const
{
    return "TRS80";
}


const char *
srecord::input_file_trs80::format_option_name(void)
    const
{
    return "-TRS80";
}


// vim: set ts=8 sw=4 et :
