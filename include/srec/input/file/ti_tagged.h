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
 * MANIFEST: interface definition for lib/srec/input/file/ti_tagged.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILE_TI_TAGGED_H
#define INCLUDE_SREC_INPUT_FILE_TI_TAGGED_H

#pragma interface "srec_input_file_ti_tagged"

#include <srec/input/file.h>

class srec_input_file_ti_tagged: public srec_input_file
{
public:
	srec_input_file_ti_tagged();
	srec_input_file_ti_tagged(const char *);
	virtual ~srec_input_file_ti_tagged();
	int read(srec_record &);

protected:
	int get_char();

private:
	typedef srec_input_file inherited;
	srec_input_file_ti_tagged(const srec_input_file_ti_tagged &);
	srec_input_file_ti_tagged &operator=(const srec_input_file_ti_tagged &);
	int read_inner(srec_record &);
	unsigned long address;
	int csum;
};

#endif /* INCLUDE_SREC_INPUT_FILE_TI_TAGGED_H */
