//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006-2009 Peter Miller
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

#include <cstdio>
#include <cstring>

#include <lib/interval.h>
#include <lib/srec/output/file/basic.h>
#include <lib/srec/record.h>


srec_output_file_basic::~srec_output_file_basic()
{
    if (range.empty())
        emit_byte(0xFF);
    if (column)
        put_char('\n');

    if (enable_footer_flag)
    {
        put_stringf("REM termination = %lu\n", taddr);
        put_stringf("REM start = %lu\n", range.get_lowest());
        put_stringf("REM finish = %lu\n", range.get_highest());
    }
    unsigned long len = range.get_highest() - range.get_lowest();
    put_stringf("REM length = %lu\n", len);
}


srec_output_file_basic::srec_output_file_basic(const std::string &a_file_name) :
    srec_output_file(a_file_name),
    taddr(0),
    column(0),
    current_address(0),
    line_length(75)
{
}


srec_output::pointer
srec_output_file_basic::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_basic(a_file_name));
}


void
srec_output_file_basic::emit_byte(int n)
{
    char buffer[8];
    sprintf(buffer, "%d", (unsigned char)n);
    int len = strlen(buffer);
    if (column && column + 1 + len > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (!column)
    {
        put_string("DATA ");
        column = 5;
    }
    else
    {
        put_char(',');
        ++column;
    }
    put_string(buffer);
    column += len;
    ++current_address;
}


void
srec_output_file_basic::write(const srec_record &record)
{
    switch (record.get_type())
    {
    default:
        // ignore
        break;

    case srec_record::type_header:
        // emit header records as comments in the file
        {
            bool bol = true;
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
            int c = *cp++;
            if (c == '\n')
            {
                put_char(c);
                bol = true;
                continue;
            }
            if (bol)
                put_string("REM ");
            if (isprint(c))
                put_char(c);
            bol = false;
            }
            if (!bol)
            put_char('\n');
        }
        break;

    case srec_record::type_data:
        if (range.empty())
            current_address = record.get_address();
        range +=
            interval
            (
                record.get_address(),
                record.get_address() + record.get_length()
            );
        while (current_address < record.get_address())
            emit_byte(0xFF);
        for (int j = 0; j < record.get_length(); ++j)
        {
            if (record.get_address() + j < current_address)
                continue;
            emit_byte(record.get_data(j));
        }
        break;

    case srec_record::type_execution_start_address:
        taddr = record.get_address();
        break;
    }
}


void
srec_output_file_basic::line_length_set(int n)
{
    line_length = n;
}


void
srec_output_file_basic::address_length_set(int)
{
    // ignore
}


int
srec_output_file_basic::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srec_record::max_data_length;
}


const char *
srec_output_file_basic::format_name()
    const
{
    return "Basic";
}
