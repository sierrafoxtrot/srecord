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
 * MANIFEST: interface definition for include/srec/input/file/ascii_hex.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILE_ASCII_HEX_H
#define INCLUDE_SREC_INPUT_FILE_ASCII_HEX_H

#pragma interface "srec_input_file_ascii_hex"

#include <srec/input/file.h>

class srec_input_file_ascii_hex: public srec_input_file
{
public:
	srec_input_file_ascii_hex();
	srec_input_file_ascii_hex(const char *);
	virtual ~srec_input_file_ascii_hex();
	int read(srec_record &);

private:
	srec_input_file_ascii_hex(const srec_input_file_ascii_hex &);
	srec_input_file_ascii_hex &operator=(const srec_input_file_ascii_hex &);
	int read_inner(srec_record &);
	bool garbage_warning;
	bool seen_some_input;
	unsigned long address;

	enum { state_initial, state_body, state_ignore } state;
};

#endif /* INCLUDE_SREC_INPUT_FILE_ASCII_HEX_H */
