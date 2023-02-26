//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <cstring>

#include <srecord/output/file/ppb.h>
#include <srecord/record.h>


srecord::output_file_ppb::~output_file_ppb()
{
    if (!seen_some_data)
        fatal_error("no data records");
    if (buffer_length > 0)
        buffer_flush();
    packet(0, 0, 0);
}


srecord::output_file_ppb::output_file_ppb(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name)
{
}


srecord::output::pointer
srecord::output_file_ppb::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_ppb(a_file_name));
}


void
srecord::output_file_ppb::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    default:
        // ignore
        break;

    case srecord::record::type_header:
        // ignore
        break;

    case srecord::record::type_data:
        for (size_t j = 0; j < record.get_length(); ++j)
        {
            uint8_t data = record.get_data(j);
            uint32_t data_address = record.get_address() + j;

            if (data_address != address)
            {
                buffer_flush();
                address = data_address;
            }
            buffer[buffer_length++] = data;
            ++address;
            if (buffer_length >= sizeof(buffer))
                buffer_flush();
            seen_some_data = true;
        }
        break;

    case srecord::record::type_execution_start_address:
        // ignore
        break;
    }
}


void
srecord::output_file_ppb::line_length_set(int)
{
    // Ignore.
}


bool
srecord::output_file_ppb::preferred_block_size_set(int nbytes)
{
    return (nbytes >= 2 && nbytes <= record::max_data_length);
}


void
srecord::output_file_ppb::address_length_set(int)
{
    // Ignore
}


int
srecord::output_file_ppb::preferred_block_size_get()
    const
{
    // Use the largest we can get.
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_ppb::format_name()
    const
{
    return "Stag Prom Programmer Binary";
}


void
srecord::output_file_ppb::put_bin_4be(uint32_t value)
{
    put_char(value >> 24);
    put_char(value >> 16);
    put_char(value >> 8);
    put_char(value);
}


uint8_t
srecord::output_file_ppb::sum_ulong(uint32_t value, uint8_t sum)
{
    sum += (value >> 24);
    sum += (value >> 16);
    sum += (value >> 8);
    sum += (value);

    return sum;
}


void
srecord::output_file_ppb::packet(uint32_t address,
    const uint8_t *data, size_t data_size)
{
    enum { SOH = 1 };
    enum { CSLEN = 1024 };

    put_char(SOH);
    put_bin_4be(data_size);
    put_bin_4be(address);

    uint8_t chksum = sum_ulong(data_size, 0);
    chksum = sum_ulong(address, chksum);

    for (size_t j = 0; j < data_size; ++j)
    {
        if (j > 0 && (j % CSLEN) == 0)
            put_char(-chksum);
        put_char(data[j]);
        chksum += data[j];
    }
    put_char(-chksum);
}


void
srecord::output_file_ppb::buffer_flush()
{
    if (buffer_length > 0)
    {
        packet(address - buffer_length, buffer, buffer_length);
        buffer_length = 0;
    }
}


bool
srecord::output_file_ppb::is_binary()
    const
{
    return true;
}
