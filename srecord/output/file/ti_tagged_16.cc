//
//      srecord - manipulate eprom load files
//      Copyright (C) 2007-2010 Peter Miller
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

#include <srecord/output/file/ti_tagged_16.h>
#include <srecord/record.h>

#include <cctype>


srec_output_file_ti_tagged_16::~srec_output_file_ti_tagged_16()
{
    if (column)
        put_eoln();
    if (enable_footer_flag)
    {
        put_char(':');
        put_char('\n');
    }
}


srec_output_file_ti_tagged_16::srec_output_file_ti_tagged_16(
        const std::string &a_file_name) :
    srec_output_file(a_file_name),
    address(0),
    column(0),
    line_length(74),
    csum(0)
{
}


srec_output::pointer
srec_output_file_ti_tagged_16::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_ti_tagged_16(a_file_name));
}


void
srec_output_file_ti_tagged_16::put_char(int c)
{
    if (c == '\n')
    {
        csum = 0;
        column = 0;
    }
    else
    {
        csum += (unsigned char)c;
        ++column;
    }
    inherited::put_char(c);
}


void
srec_output_file_ti_tagged_16::put_eoln()
{
    put_char('7');
    put_word(-csum);
    put_char('F');
    put_char('\n');
}


void
srec_output_file_ti_tagged_16::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        if (enable_header_flag)
        {
            put_stringf("K%4.4X", (int)(5 + record.get_length()));
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
                if (!isprint(c))
                    c = ' ';
                put_char(c);
            }
        }
        break;

    case srec_record::type_data:
        {
            if (record.get_address() + record.get_length() > (1UL << 17))
                data_address_too_large(record);

            //
            // we can't start at an odd address,
            // but don't test for odd lengths
            //
            if (record.get_address() & 1)
                fatal_alignment_error(2);

            // assert(record.get_length() > 0);
            if (record.get_length() == 0)
                break;
            if (address != record.get_address())
            {
                address = record.get_address();
                if (column + 5 > line_length)
                    put_eoln();
                put_char('9');
                put_word(address >> 1);
            }
            size_t pos = 0;
            for (; pos + 2 <= record.get_length(); pos += 2)
            {
                if (column + 5 > line_length)
                    put_eoln();
                put_char('B');
                put_byte(record.get_data(pos));
                put_byte(record.get_data(pos + 1));
                address += 2;
            }
            for (; pos < record.get_length(); ++pos)
            {
                if (column + 3 > line_length)
                    put_eoln();
                put_char('*');
                put_byte(record.get_data(pos));
                ++address;
            }
        }
        break;

    case srec_record::type_data_count:
    case srec_record::type_execution_start_address:
        // ignore
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_ti_tagged_16::line_length_set(int linlen)
{
    // reduce the line length by 6 characters, to account for the
    // checksum, so we don't keep subtracting it later.
    line_length = linlen - 6;

    // make sure the line is long enough to do anything useful
    if (line_length < 5)
        line_length = 5;
}


void
srec_output_file_ti_tagged_16::address_length_set(int)
{
    // ignore (addresses are always 16 bits)
}


int
srec_output_file_ti_tagged_16::preferred_block_size_get()
    const
{
    int n = (line_length / 5) * 2;
    if (n < 2)
        n = 2;
    if (n > srec_record::max_data_length)
        n = srec_record::max_data_length;
    return (n & ~1);
}


const char *
srec_output_file_ti_tagged_16::format_name()
    const
{
    return "TI-Tagged-16";
}
