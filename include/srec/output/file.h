/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2000 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/output/file.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_H
#define INCLUDE_SREC_OUTPUT_FILE_H

#pragma interface "srec_output_file"

#include <string>
#include <srec/output.h>

class srec_output_file: public srec_output
{
public:
	srec_output_file();
	srec_output_file(const char *);
	virtual ~srec_output_file();
	virtual const string filename() const;
	static void data_only();

protected:
	virtual void put_char(int);
	void put_nibble(int);
	virtual void put_byte(int);
	virtual void put_word(int);
	void checksum_reset();
	void checksum_add(int n) { checksum += (unsigned char)n; }
	int checksum_get();
	int checksum_get16();
	void seek_to(unsigned long);
	void put_string(const char *);
	void put_stringf(const char *, ...);
	virtual const char *mode() const;
	static bool data_only_flag;

private:
	string file_name;
	int line_number;
	void *vfp;
	int checksum;

	void *get_fp();
	srec_output_file(const srec_output_file &);
	srec_output_file &operator=(const srec_output_file &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_H */
