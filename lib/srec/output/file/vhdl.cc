//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000 Hendrik De Vloed - hendrik.devloed@rug.ac.be
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the srec_output_file_vhdl class
//

#pragma implementation "srec_output_file_vhdl"

#include <srec/arglex.h>
#include <interval.h>
#include <srec/output/file/vhdl.h>
#include <srec/record.h>
#include <progname.h>


srec_output_file_vhdl::srec_output_file_vhdl(const char *filename) :
    srec_output_file(filename),
    bytes_per_word(1),
    prefix("eprom"),
    header_done(false),
    current_address(0),
    current_byte(0),
    current_word(0)
{
}


void
srec_output_file_vhdl::command_line(srec_arglex *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
	int a1 = cmdln->value_number();
	cmdln->token_next();

	if (a1 > 0)
	{
	    unsigned a2 = (unsigned)a1;
	    if (a2 > sizeof(unsigned long))
		a2 = sizeof(unsigned long);
	    bytes_per_word = a2;
	}
    }
    if (cmdln->token_cur() == arglex::token_string)
    {
	prefix = cmdln->value_string();
	cmdln->token_next();
    }
}


void
srec_output_file_vhdl::emit_header()
{
    if (header_done)
	return;
    put_stringf
    (
	"--\n"
	"-- Generated automatically by %s -VHDL - do not edit\n"
	"--\n",
	progname_get()
    );
    if (!data_only_flag)
    {
	put_stringf
	(
	    "library IEEE;\n"
	    "use IEEE.numeric_std.all;\n"
	    "use work.%s_defs_pack.all;\n\n",
	    prefix.c_str()
	);
	put_stringf("package %s_pack is\n", prefix.c_str());
	put_stringf
	(
	    " constant %s_rom : %s_rom_array;\n",
	    prefix.c_str(),
	    prefix.c_str()
	);
	put_stringf("end package %s_pack;\n\n", prefix.c_str());
	put_stringf("package body %s_pack is\n", prefix.c_str());
    }
    put_stringf
    (
	" constant %s_rom : %s_rom_array := %s_rom_array'(\n",
	prefix.c_str(),
	prefix.c_str(),
	prefix.c_str()
    );
    header_done = true;
}


void
srec_output_file_vhdl::emit_word(unsigned long address, unsigned n)
{
    //
    // If there is a gap in the address sequence, fill the gap with 0xFF.
    // Be careful of gaps within a single word.
    //
    while (address != current_address && current_byte)
    {
	current_word = (current_word << 8) | 0xFF;
	++current_address;
	++current_byte;
	if (current_byte >= bytes_per_word)
	{
	    put_stringf
	    (
		"  %lu => %s_entry(%lu),\n",
		(current_address / bytes_per_word) - 1,
		prefix.c_str(),
		current_word
	    );
	    current_byte = 0;
	    current_word = 0;
	}
    }

    //
    // If the is the first byte to be set within this current_word, we may need
    // to fill the beginning with 0xFF bytes.
    //
    if (!current_byte)
    {
	for (int partial = address % bytes_per_word; partial > 0; --partial)
	{
	    current_word = (current_word << 8) | 0xFF;
	    ++current_byte;
	}

	//
	// Leap over gaps if necessary.
	//
	current_address = address;
    }

    //
    // Insert the bytes into the current_word.
    // Emit the current_word if we fill it.
    //
    current_word = (current_word << 8) | (n & 0xFF);
    ++current_address;
    ++current_byte;
    if (current_byte >= bytes_per_word)
    {
	put_stringf
	(
	    "  %lu => %s_entry(%lu),\n",
	    (current_address / bytes_per_word) - 1,
	    prefix.c_str(),
	    current_word
	);
	current_byte = 0;
	current_word = 0;
    }
}


srec_output_file_vhdl::~srec_output_file_vhdl()
{
    emit_header();

    //
    // Make sure ther there isn't any pending output.
    //
    while (current_byte)
    {
	current_word = (current_word << 8) | 0xFF;
	++current_address;
	++current_byte;
	if (current_byte >= bytes_per_word)
	{
	    put_stringf
	    (
		"  %lu => %s_entry(%lu),\n",
		(current_address / bytes_per_word) - 1,
		prefix.c_str(),
		current_word
	    );
	    current_byte = 0;
	    current_word = 0;
	}
    }

    put_stringf("  others => %s_dont_care\n" " );\n", prefix.c_str());
    if (!data_only_flag)
    {
	put_stringf("end package body %s_pack;\n", prefix.c_str());
    }
}


void
srec_output_file_vhdl::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_unknown:
	// Ignore.
	break;

    case srec_record::type_header:
	{
	    //
	    // Output the header record as a comment.
	    //
	    put_string("-- ");
	    const unsigned char *cp = record.get_data();
	    const unsigned char *ep = cp + record.get_length();
	    while (cp < ep)
	    {
		unsigned char c = *cp++;
		if (c == '\n')
		{
		    put_string("\n-- ");
		    continue;
		}
		if (!isprint(c))
		    c = ' ';
		put_char(c);
	    }
	    put_char('\n');
	}
	break;

    case srec_record::type_data:
	emit_header();
	for (int j = 0; j < record.get_length(); ++j)
	{
	    emit_word(record.get_address() + j, record.get_data(j));
	}
	break;

    case srec_record::type_data_count:
	// Ignore.
	break;

    case srec_record::type_start_address:
	// Ignore.
	break;
    }
}


void
srec_output_file_vhdl::line_length_set(int n)
{
    // ignore
}


void
srec_output_file_vhdl::address_length_set(int)
{
    // ignore
}


int
srec_output_file_vhdl::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srec_record::max_data_length;
}
