/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998 Peter Miller;
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
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MANIFEST: interface definition for lib/srec/output/file/binary.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_FILE_BINARY_H
#define INCLUDE_SREC_OUTPUT_FILE_BINARY_H

#include <srec/output/file.h>

#pragma interface

class srec_output_file_binary: public srec_output_file
{
public:
	virtual ~srec_output_file_binary();
	srec_output_file_binary();
	srec_output_file_binary(const char *);
	virtual void write(const srec_record &);

private:
	srec_output_file_binary(const srec_output_file_binary &);
	srec_output_file_binary &operator=(const srec_output_file_binary &);
};

#endif /* INCLUDE_SREC_OUTPUT_FILE_BINARY_H */