//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006-2008 Peter Miller
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

#include <lib/srec/output/file/fairchild.h>
#include <lib/srec/record.h>


srec_output_file_fairchild::~srec_output_file_fairchild()
{
}


srec_output_file_fairchild::srec_output_file_fairchild(
        const std::string &a_file_name) :
    srec_output_file(a_file_name),
    address(~0uL)
{
}


srec_output::pointer
srec_output_file_fairchild::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_fairchild(a_file_name));
}


void
srec_output_file_fairchild::put_nibble(unsigned n)
{
    srec_output_file::put_nibble(n);
    checksum_add(n & 15);
}


void
srec_output_file_fairchild::put_byte(unsigned char n)
{
    // This differs from srec_output_file::put_byte only in that it
    // doesn't add to the checksum.
    put_nibble(n >> 4);
    put_nibble(n);
}


void
srec_output_file_fairchild::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
    case srec_record::type_unknown:
    case srec_record::type_data_count:
        // ignore
        break;

    case srec_record::type_data:
        {
            int len = record.get_length();
            unsigned long new_addr = record.get_address();
            if ((new_addr & 7) || (len & 7))
                fatal_alignment_error(8);
            if (address != new_addr)
            {
                put_stringf("S%4.4lX\n", new_addr);
                address = new_addr;
            }
            for (int j = 0; j < len; j += 8)
            {
                put_char('X');
                checksum_reset();

                //
                // Emit the one line of data.
                //
                put_byte(record.get_data(j));
                put_byte(record.get_data(j + 1));
                put_byte(record.get_data(j + 2));
                put_byte(record.get_data(j + 3));
                put_byte(record.get_data(j + 4));
                put_byte(record.get_data(j + 5));
                put_byte(record.get_data(j + 6));
                put_byte(record.get_data(j + 7));
                address += 8;

                put_nibble(checksum_get());
                put_char('\n');
            }
        }
        break;

    case srec_record::type_execution_start_address:
        put_string("*\n");
        break;
    }
}


void
srec_output_file_fairchild::line_length_set(int)
{
    // ignore
}


void
srec_output_file_fairchild::address_length_set(int)
{
    // ignore
}


int
srec_output_file_fairchild::preferred_block_size_get()
    const
{
    return 8;
}


const char *
srec_output_file_fairchild::format_name()
    const
{
    return "Fairchild";
}
