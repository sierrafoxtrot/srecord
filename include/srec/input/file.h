/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998-2001 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/input/file.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILE_H
#define INCLUDE_SREC_INPUT_FILE_H

#pragma interface "srec_input_file"

#include <string>
#include <srec/input.h>

class srec_input_file: public srec_input
{
public:
	srec_input_file();
	srec_input_file(const char *);
	virtual ~srec_input_file();
	virtual const string filename() const;
	virtual const string filename_and_line() const;

protected:
	virtual int get_char();
	int peek_char();
	int get_nibble();
	virtual int get_byte();
	int get_word();
	unsigned long get_3bytes();
	int checksum_get();
	int checksum_get16();
	void checksum_add(int n) { checksum += (unsigned char)n; }
	void checksum_reset();
	void seek_to_end();
	virtual const char *mode() const;

private:
	string file_name;
	int line_number;
	bool prev_was_newline;
	void *vfp;
	int checksum;
	int is_text;

	void *get_fp();
	srec_input_file(const srec_input_file &);
	srec_input_file &operator=(const srec_input_file &);
};

#endif /* INCLUDE_SREC_INPUT_FILE_H */
