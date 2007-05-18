//
//      srecord - manipulate eprom load files
//      Copyright (C) 2004, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_output_file_aomf class
//


#include <lib/srec/output/file/aomf.h>
#include <lib/srec/record.h>


srec_output_file_aomf::~srec_output_file_aomf()
{
}


srec_output_file_aomf::srec_output_file_aomf() :
    srec_output_file()
{
}


srec_output_file_aomf::srec_output_file_aomf(const string &a_file_name) :
    srec_output_file(a_file_name)
{
}


void
srec_output_file_aomf::emit_record(int type, const unsigned char *data,
    size_t length)
{
    checksum_reset();
    put_byte(type);
    put_word(length + 1);
    for (size_t j = 0; j < length; ++j)
        put_byte(data[j]);
    put_byte(-checksum_get());
}


void
srec_output_file_aomf::module_header_record(const char *name)
{
    //
    // The TRN ID describes the system which created the file, and (by
    // implication) the CPU type is 8051.  It isn't much good to us,
    // because it doesn't admit that non-Intel systems or other CPUs exist.
    //     0xFD -> ASM51
    //     0xFE -> PL/M-51
    //     0xFF -> RL51
    //
    unsigned char buffer[1 + 255 + 2];
    size_t len = strlen(name);
    if (len > 255)
        len = 255;
    buffer[0] = len;
    memcpy(buffer + 1, name, len);
    buffer[len + 1] = 0; // TRN ID
    buffer[len + 2] = 0; // must be zero
    emit_record(0x02, buffer, len + 3);
}


void
srec_output_file_aomf::content_record(unsigned long address,
    const unsigned char *data, size_t len)
{
    size_t maxlen = 4 * srec_record::max_data_length;
    while (len > 0)
    {
        unsigned char buffer[maxlen + 3];
        buffer[0] = address >> 16; // this byte should be zero ;-)
        buffer[1] = address; // "offset" is little-endian
        buffer[2] = address >> 8;
        int nbytes = (len > maxlen ? maxlen : len);
        memcpy(buffer + 3, data, nbytes);
        emit_record(0x06, buffer, len + 3);
        address += nbytes;
        data += nbytes;
        len -= nbytes;
    }
}


void
srec_output_file_aomf::module_end_record(const char *name)
{
    unsigned char buffer[1 + 255 + 4];
    size_t len = strlen(name);
    if (len > 255)
        len = 255;
    buffer[0] = len;
    memcpy(buffer + 1, name, len);
    buffer[len + 1] = 0; // must be zero
    buffer[len + 2] = 0; // must be zero
    buffer[len + 3] = 0; // REG MSK
    buffer[len + 4] = 0; // must be zero
    emit_record(0x04, buffer, len + 5);
}


void
srec_output_file_aomf::put_byte(unsigned char n)
{
    checksum_add(n);
    put_char(n);
    byte_offset++;
}


void
srec_output_file_aomf::put_word(int n)
{
    put_byte(n);
    put_byte(n >> 8);
}


void
srec_output_file_aomf::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        module_name.assign((const char *)record.get_data(),
            record.get_length());
        module_header_record(module_name.c_str());
        break;

    case srec_record::type_data:
        if (record.get_length() < 1)
                return;
        if (record.get_address() + record.get_length() > (1UL << 24))
        {
                fatal_error
                (
                        "data address (0x%lX..0x%lX) too large",
                        record.get_address(),
                        record.get_address() + record.get_length() - 1
                );
        }

        //
        // Write the data out.
        //
        content_record
        (
            record.get_address(),
            record.get_data(),
            record.get_length()
        );
        break;

    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_start_address:
        module_end_record(module_name.c_str());
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_aomf::line_length_set(int)
{
    // Ignore.
}


void
srec_output_file_aomf::address_length_set(int)
{
    // Ignore (this is only a 16-bit format).
}


int
srec_output_file_aomf::preferred_block_size_get()
    const
{
    return srec_record::max_data_length;
}


const char *
srec_output_file_aomf::mode()
    const
{
    return "wb";
}
