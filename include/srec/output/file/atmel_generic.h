/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2001 Peter Miller;
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
 * MANIFEST: interface definition for include/srec/output/file/atmel_generic.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_ATMEL_GENERIC_H
#define INCLUDE_SREC_OUTPUT_FILE_ATMEL_GENERIC_H

#pragma interface "srec_output_file_atmel_generic"

#include <srec/output/file.h>

class srec_output_file_atmel_generic: public srec_output_file
{
public:
	virtual ~srec_output_file_atmel_generic();
	srec_output_file_atmel_generic();
	srec_output_file_atmel_generic(const char *, bool);
	void write(const srec_record &);
	virtual void line_length_set(int);
	virtual void address_length_set(int);
	virtual int preferred_block_size_get() const;

private:
	bool bigend;
	srec_output_file_atmel_generic(const srec_output_file_atmel_generic &);
	srec_output_file_atmel_generic &operator=(const srec_output_file_atmel_generic &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_ATMEL_GENERIC_H */
