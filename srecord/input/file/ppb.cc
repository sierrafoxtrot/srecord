//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011, 2013 Peter Miller
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

#include <cassert>

#include <srecord/arglex/tool.h>
#include <srecord/input/file/ppb.h>
#include <srecord/record.h>


srecord::input_file_ppb::input_file_ppb(const std::string &filename) :
    input_file(filename)
{
}


srecord::input_file_ppb::pointer
srecord::input_file_ppb::create(const std::string &filename)
{
    return pointer(new input_file_ppb(filename));
}


bool
srecord::input_file_ppb::get_packet()
{
    int c;

    enum { SOH = 1 }; // Start of header marks beginning of record

    // Skip ASCII prologue (if any) and hunt for SOH
    do{
        c = get_char();
        if (c < 0)
            return false;
        // tweak to original patch to handle CR/LF/CRLF
//        if ((c == '\n') || (c == '\r'))
//            continue;
    }while(c != SOH && /*c >= ' ' &&*/ c < 0x7f);

    if (c != SOH)
        packet_format_error();

    unsigned char hdr[8];
    unsigned char csum = 0;
    for (int n = 0; n < 8; ++n)
    {
        c = get_char();
        if (c < 0)
            packet_format_error();
        hdr[n] = c;
        csum += c;
    }

    // 4 Byte data size for this block
    packet_length = record::decode_big_endian(hdr, 4);
    if (packet_length > sizeof(packet))
    {
        fatal_error
        (
            "payload size too large (%ld > %ld)",
            packet_length,
            sizeof(packet)
        );
    }

    // 4 Byte start address
    packet_address = record::decode_big_endian(hdr + 4, 4);

    // Payload
    for (size_t j = 0; j < packet_length; ++j)
    {
        if (j > 0 && (j % 1024) == 0)
        {
            c = get_char();
            if (c < 0)
                packet_format_error();
            if (c != (unsigned char)-csum && use_checksums())
            {
                fatal_error
                (
                    "intermediate checksum mismatch (expected %d, read %d)",
                    (unsigned char)-csum,
                    c
                );
            }
        }
        c = get_char();
        if (c < 0)
            packet_format_error();
        packet[j] = c;
        csum += c;
    }
    c = get_char();
    if (c < 0)
        packet_format_error();
    if (c != (unsigned char)-csum && use_checksums())
    {
        fatal_error
        (
            "packet checksum mismatch (expected %d, read %d)",
            (unsigned char)-csum,
            c
        );
    }
    packet_used = 0;
    return true;
}


bool
srecord::input_file_ppb::read(record &result)
{
    if (packet_used >= packet_length)
    {
        if (!get_packet())
            return false;
        if (packet_length == 0)
            return false;
    }
    size_t size = packet_length - packet_used;
    if (size > record::max_data_length)
        size = record::max_data_length;
    result =
        record
        (
            record::type_data,
            packet_address + packet_used,
            packet + packet_used,
            size
        );
    packet_used += size;
    return true;
}


void
srecord::input_file_ppb::packet_format_error()
{
    fatal_error("packet not formatted correctly");
}


bool
srecord::input_file_ppb::is_binary()
    const
{
    return true;
}


const char *
srecord::input_file_ppb::get_file_format_name()
    const
{
    return "Stag Prom Programmer binary (PPB)";
}


int
srecord::input_file_ppb::format_option_number()
    const
{
    return arglex_tool::token_ppb;
}
