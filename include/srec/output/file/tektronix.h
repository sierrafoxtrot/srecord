/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/output/file/tektronix.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_TEKTRONIX_H
#define INCLUDE_SREC_OUTPUT_FILE_TEKTRONIX_H

#pragma interface "srec_output_file_tektronix"

#include <srec/output/file.h>

class srec_output_file_tektronix: public srec_output_file
{
public:
	virtual ~srec_output_file_tektronix();
	srec_output_file_tektronix();
	srec_output_file_tektronix(const char *);
	virtual void write(const srec_record &);
	virtual void line_length_set(int);
	virtual int preferred_block_size_get() const;

private:
	int pref_block_size;
	void write_inner(unsigned long, const void *, int);
	srec_output_file_tektronix(const srec_output_file_tektronix &);
	srec_output_file_tektronix &operator=(const srec_output_file_tektronix &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_TEKTRONIX_H */
