/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000 Hendrik De Vloed - hendrik.devloed@rug.ac.be
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 * MANIFEST: interface definition for lib/srec/output/file/vhdl.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_VHDL_H
#define INCLUDE_SREC_OUTPUT_FILE_VHDL_H

#pragma interface "srec_output_file_vhdl"

#include <srec/output/file.h>

class srec_output_file_vhdl: public srec_output_file
{
public:
	virtual ~srec_output_file_vhdl();
	srec_output_file_vhdl();
	srec_output_file_vhdl(const char *, const int, const char *);
	virtual void write(const srec_record &);
	void line_length_set(int);
	void address_length_set(int);
	int preferred_block_size_get() const;

private:
	int bytes_per_word;
	string prefix;
	unsigned long taddr;
	class interval range;
	bool header_done;
	int column;
	unsigned long current_address;
	int line_length;
	void emit_header();
	void emit_word(int,unsigned long);
	srec_output_file_vhdl(const srec_output_file_vhdl &);
	srec_output_file_vhdl &operator=(const srec_output_file_vhdl &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_VHDL_H */
