//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001, 2002, 2006-2009 Peter Miller
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

#include <lib/srec/output/file/atmel_generic.h>
#include <lib/srec/record.h>


srec_output_file_atmel_generic::~srec_output_file_atmel_generic()
{
}


srec_output_file_atmel_generic::srec_output_file_atmel_generic(
        const std::string &a_file_name, endian_t a_end) :
    srec_output_file(a_file_name),
    end(a_end)
{
}


srec_output::pointer
srec_output_file_atmel_generic::create(const std::string &a_file_name,
    endian_t a_end)
{
    return pointer(new srec_output_file_atmel_generic(a_file_name, a_end));
}


void
srec_output_file_atmel_generic::write(const srec_record &record)
{
    // This format can't do header records or termination records
    if (record.get_type() != srec_record::type_data)
        return;

    if (record.get_address() + record.get_length() > (1UL << 17))
        data_address_too_large(record);

    long address = record.get_address();
    if ((address & 1) || (record.get_length() & 1))
        fatal_alignment_error(2);
    for (size_t j = 0; j < record.get_length(); j += 2)
    {
        put_3bytes(address / 2);
        put_char(':');
        if (end == endian_big)
        {
            put_byte(record.get_data(j + 1));
            put_byte(record.get_data(j));
        }
        else
        {
            put_byte(record.get_data(j));
            put_byte(record.get_data(j + 1));
        }
        put_char('\n');
        address += 2;
    }
}


void
srec_output_file_atmel_generic::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srec_output_file_atmel_generic::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


int
srec_output_file_atmel_generic::preferred_block_size_get()
    const
{
    //
    // Use the largest we can get,
    // but make sure it is an even number of bytes long.
    //
    return (srec_record::max_data_length & ~1);
}


const char *
srec_output_file_atmel_generic::format_name()
    const
{
    return "Atmel-Generic";
}
