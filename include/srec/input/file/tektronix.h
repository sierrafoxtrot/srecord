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
 * MANIFEST: interface definition for lib/srec/input/file/tektronix.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILE_TEKTRONIX_H
#define INCLUDE_SREC_INPUT_FILE_TEKTRONIX_H

#include <srec/input/file.h>

#pragma interface

class srec_input_file_tektronix: public srec_input_file
{
public:
	srec_input_file_tektronix();
	srec_input_file_tektronix(const char *);
	virtual ~srec_input_file_tektronix();
	int read(srec_record &);

private:
	srec_input_file_tektronix(const srec_input_file_tektronix &);
	srec_input_file_tektronix &operator=(const srec_input_file_tektronix &);
	int read_inner(srec_record &);
	int data_record_count;
	bool garbage_warning;
	bool seen_some_input;
	bool termination_seen;
};

#endif /* INCLUDE_SREC_INPUT_FILE_TEKTRONIX_H */
