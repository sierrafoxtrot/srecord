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

#include <interval.h>
#include <srec/output/file/vhdl.h>
#include <srec/record.h>
#include <progname.h>


srec_output_file_vhdl::srec_output_file_vhdl()
	: srec_output_file("-"), prefix("no_name_given"), line_length(75)
{
}


srec_output_file_vhdl::srec_output_file_vhdl(const char *filename, const int bytesperword, const char *a2)
	: srec_output_file(filename), bytes_per_word(bytesperword), prefix(a2), line_length(75)
{
}


srec_output_file_vhdl::srec_output_file_vhdl(const srec_output_file_vhdl &)
	: srec_output_file("-"), line_length(75)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
}


srec_output_file_vhdl &
srec_output_file_vhdl::operator=(const srec_output_file_vhdl &)
{
	fatal_error("bug (%s, %d)", __FILE__, __LINE__);
	return *this;
}


void
srec_output_file_vhdl::emit_header()
{
	if (header_done)
		return;
	const char *pref = prefix.c_str();
	put_stringf(
		"--\n"
		"-- Generated automatically by %s -VHDL - do not edit\n"
		"--\n",
		progname_get()
	);
	if(!data_only_flag){
		put_stringf(
			"library IEEE;\n"
			"use IEEE.numeric_std.all;\n"
			"use work.%s_defs_pack.all;\n\n",
			pref
		);
		put_stringf(
			"package %s_pack is\n"
			" constant %s_rom : %s_rom_array;\n"
			"end package %s_pack;\n\n",
			pref, // package begin
			pref, pref, // constant
			pref // package end
		);
		put_stringf(
			"package body %s_pack is\n",
			pref // package body
		);
	}
	put_stringf(
		" constant %s_rom : %s_rom_array := %s_rom_array'(\n",
		pref, pref, pref // constant
	);
	header_done = true;
}


void
srec_output_file_vhdl::emit_word(int address, unsigned long n)
{
	const int last_byte = abs(bytes_per_word);
	static int current_byte = 0;
	static int prev_address = -2;
	static unsigned long word = 0;
	if(prev_address+1 != address){
		current_byte=word=0;
	}
	if(bytes_per_word > 0){
		word<<=8; word |= n & 0xFF;
	}else{
		word |= ( (n & 0xFF) << (current_byte*8));
	}
	if(++current_byte == last_byte){
		put_stringf("  %d => %s_entry(%ld),\n", address/last_byte, prefix.c_str(), word);
		current_byte=word=0;
	}
	prev_address = address;
}


srec_output_file_vhdl::~srec_output_file_vhdl()
{
	emit_header();
	if (range.empty()){}
	if (column)
		put_char('\n');
	put_stringf(
		"  others => %s_dont_care\n"
		" );\n",
		prefix.c_str()
	);
	if(!data_only_flag){
		put_stringf(
			"end package body %s_pack;\n",
			prefix.c_str()
		);
	}
}


void
srec_output_file_vhdl::write(const srec_record &record)
{
	switch (record.get_type())
	{
	default:
		// This format can't do header records. (Actually,
		// we could probably output a comment, like the C format.)
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
		emit_header();
		while (current_address < record.get_address())
		{
			++current_address;
		}
		for (int j = 0; j < record.get_length(); ++j)
		{
			if (record.get_address() + j < current_address)
				continue;
			emit_word(current_address, record.get_data(j));
			current_address++;
		}
		break;

	case srec_record::type_start_address:
		taddr = record.get_address();
		break;
	}
}


void
srec_output_file_vhdl::line_length_set(int n)
{
	n = (n + 1) / 6;
	if (n < 1)
		n = 1;
	n = n * 6 - 1;
	line_length = n;
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
