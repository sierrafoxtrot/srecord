//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006 Peter Miller
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
// MANIFEST: functions to impliment the srec_output_file_asm class
//


#include <lib/interval.h>
#include <lib/srec/output/file/asm.h>
#include <lib/srec/record.h>
#include <cstdio> // for sprintf


srec_output_file_asm::srec_output_file_asm(const char *filename) :
    srec_output_file(filename),
    taddr(0),
    column(0),
    current_address(0),
    line_length(75),
    org_warn(false)
{
}


void
srec_output_file_asm::emit_byte(int n)
{
    char buffer[8];
    sprintf(buffer, "%d", (unsigned char)n);
    int len = strlen(buffer);
    if (column && (column + 1 + len) > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (!column)
    {
        put_string("\tDB\t");
        column = 16;
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


srec_output_file_asm::~srec_output_file_asm()
{
    if (range.empty())
        emit_byte(0xFF);
    if (column)
        put_char('\n');

    if (!data_only_flag)
    {
        put_stringf("; upper bound = 0x%4.4lX\n", range.get_highest());
        put_stringf("; lower bound = 0x%4.4lX\n", range.get_lowest());
    }
    unsigned long len = range.get_highest() - range.get_lowest();
    put_stringf("; length =      0x%4.4lX\n", len);
}


void
srec_output_file_asm::write(const srec_record &record)
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
		    put_string("; ");
		if (isprint(c))
		    put_char(c);
		bol = false;
            }
            if (!bol)
		put_char('\n');
        }
        break;

    case srec_record::type_data:
        if (current_address != record.get_address())
        {
            if (column)
            {
                put_char('\n');
                column = 0;
            }
            current_address = record.get_address();
	    if (range.empty())
	    {
		put_stringf
	       	(
                    "; To avoid this next ORG directive, use the "
			"--offset -0x%lX filter.\n",
		    current_address
		);
	    }
	    else if (!org_warn)
	    {
		org_warn = true;
		put_string
	       	(
                    "; To avoid this next ORG directive, use the --fill "
			"filter.\n"
		);
	    }
	    put_stringf("\tORG\t%lu\n", current_address);
        }
        range +=
            interval
            (
                record.get_address(),
                record.get_address() + record.get_length()
            );
        for (int j = 0; j < record.get_length(); ++j)
            emit_byte(record.get_data(j));
        break;

    case srec_record::type_start_address:
        taddr = record.get_address();
	if (!data_only_flag)
	{
            if (column)
            {
                put_char('\n');
                column = 0;
            }
	    put_stringf("; start addr =  0x%4.4lX\n", taddr);
	}
        break;
    }
}


void
srec_output_file_asm::line_length_set(int n)
{
    line_length = n;
}


void
srec_output_file_asm::address_length_set(int n)
{
    // ignore
}


int
srec_output_file_asm::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srec_record::max_data_length;
}
