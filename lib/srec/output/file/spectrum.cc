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

#include <lib/srec/output/file/spectrum.h>
#include <lib/srec/record.h>


srec_output_file_spectrum::~srec_output_file_spectrum()
{
}


srec_output_file_spectrum::srec_output_file_spectrum(
        const std::string &a_filename) :
    srec_output_file(a_filename)
{
}


srec_output::pointer
srec_output_file_spectrum::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_spectrum(a_file_name));
}


void
srec_output_file_spectrum::put_decimal(unsigned long n)
{
    put_stringf("%4.4lu", n);
}


void
srec_output_file_spectrum::put_binary(unsigned char n)
{
    for (unsigned char bit = 0x80; bit; bit >>= 1)
    {
        put_char((n & bit) ? '1' : '0');
    }
}


void
srec_output_file_spectrum::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        if (!data_only_flag)
            put_char(2);
        break;

    case srec_record::type_data:
        {
            unsigned long address = record.get_address();
            int length = record.get_length();
            for (int j = 0; j < length; ++j)
            {
                put_decimal(address + j);
                put_char(' ');
                put_binary(record.get_data(j));
                put_char('\n');
            }
        }
        break;

    case srec_record::type_execution_start_address:
        if (!data_only_flag)
            put_char(3);
        break;

    case srec_record::type_unknown:
    case srec_record::type_data_count:
        // ignore
        break;
    }
}


void
srec_output_file_spectrum::line_length_set(int)
{
    // ignore
}


void
srec_output_file_spectrum::address_length_set(int)
{
    // ignore
}


int
srec_output_file_spectrum::preferred_block_size_get()
    const
{
    return srec_record::max_data_length;
}


const char *
srec_output_file_spectrum::format_name()
    const
{
    return "Spectrum";
}
