//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001, 2002, 2006, 2007 Peter Miller
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
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//

#include <lib/srec/output/file/spasm.h>
#include <lib/srec/record.h>


srec_output_file_spasm::srec_output_file_spasm(const std::string &a_file_name,
        bool endianness) :
    srec_output_file(a_file_name),
    bigend(endianness)
{
}


srec_output_file_spasm::~srec_output_file_spasm()
{
}


void
srec_output_file_spasm::write(const srec_record &record)
{
    // This format can't do header records or termination records
    if (record.get_type() != srec_record::type_data)
        return;

    if (record.get_address() + record.get_length() > (1UL << 17))
    {
        fatal_error
        (
            "data address (0x%lX..0x%lX) too large",
            record.get_address(),
            record.get_address() + record.get_length() - 1
        );
    }

    long address = record.get_address();
    if ((address & 1) || (record.get_length() & 1))
        fatal_alignment_error(2);

    for (int j = 0; j < record.get_length(); j += 2)
    {
        put_word(address / 2);
        put_char(' ');
        if (bigend)
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
srec_output_file_spasm::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srec_output_file_spasm::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


int
srec_output_file_spasm::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    // But make sure it is an even number of bytes long.
    //
    return (srec_record::max_data_length & ~1);
}


const char *
srec_output_file_spasm::format_name()
    const
{
    return "Spasm";
}
