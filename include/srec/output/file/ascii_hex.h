/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000 Peter Miller;
 *	All rights reserved.
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
 * MANIFEST: interface definition for lib/srec/output/file/ascii_hex.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_ASCII_HEX_H
#define INCLUDE_SREC_OUTPUT_FILE_ASCII_HEX_H

#pragma interface "srec_output_file_ascii_hex"

#include <srec/output/file.h>

class srec_output_file_ascii_hex: public srec_output_file
{
public:
	virtual ~srec_output_file_ascii_hex();
	srec_output_file_ascii_hex();
	srec_output_file_ascii_hex(const char *);
	void write(const class srec_record &);
	virtual void line_length_set(int);
	virtual int preferred_block_size_get() const;

private:
	unsigned long address;
	int column;
	int pref_block_size;
	srec_output_file_ascii_hex(const srec_output_file_ascii_hex &);
	srec_output_file_ascii_hex &operator=(const srec_output_file_ascii_hex &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_ASCII_HEX_H */
