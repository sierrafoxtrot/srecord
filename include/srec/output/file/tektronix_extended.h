/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000, 2001 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/output/file/tektronix_extended.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_TEKTRONIX_EXTENDED_H
#define INCLUDE_SREC_OUTPUT_FILE_TEKTRONIX_EXTENDED_H

#pragma interface "srec_output_file_tektronix_extended"

#include <srec/output/file.h>

class srec_output_file_tektronix_extended: public srec_output_file
{
public:
	virtual ~srec_output_file_tektronix_extended();
	srec_output_file_tektronix_extended();
	srec_output_file_tektronix_extended(const char *);
	void write(const class srec_record &);
	virtual void line_length_set(int);
	virtual void address_length_set(int);
	virtual int preferred_block_size_get() const;

private:
	int data_count;
	int pref_block_size;
	int address_length;
	void write_inner(int, unsigned long, int, const void *, int);
	srec_output_file_tektronix_extended(const srec_output_file_tektronix_extended &);
	srec_output_file_tektronix_extended &operator=(const srec_output_file_tektronix_extended &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_TEKTRONIX_EXTENDED_H */
