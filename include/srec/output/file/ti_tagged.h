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
 * MANIFEST: interface definition for include/srec/output/file/ti_tagged.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_TI_TAGGED_H
#define INCLUDE_SREC_OUTPUT_FILE_TI_TAGGED_H

#pragma interface "srec_output_file_ti_tagged"

#include <srec/output/file.h>

class srec_output_file_ti_tagged: public srec_output_file
{
public:
	virtual ~srec_output_file_ti_tagged();
	srec_output_file_ti_tagged();
	srec_output_file_ti_tagged(const char *);
	void write(const class srec_record &);
	virtual void line_length_set(int);
	virtual int preferred_block_size_get() const;

protected:
	virtual void put_char(int);

private:
	typedef srec_output_file inherited;
	unsigned long address;
	int column;
	int line_length;
	int csum;
	void put_eoln();
	srec_output_file_ti_tagged(const srec_output_file_ti_tagged &);
	srec_output_file_ti_tagged &operator=(const srec_output_file_ti_tagged &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_TI_TAGGED_H */
