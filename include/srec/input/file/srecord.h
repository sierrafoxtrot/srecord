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
 * MANIFEST: interface definition for lib/input/file/srecord.cc
 */

#ifndef INCLUDE_INPUT_FILE_SRECORD_H
#define INCLUDE_INPUT_FILE_SRECORD_H

#include <srec/input/file.h>

#pragma interface

class srec_input_file_srecord: public srec_input_file
{
public:
	srec_input_file_srecord();
	srec_input_file_srecord(const char *);
	virtual ~srec_input_file_srecord();
	int read(srec_record &);

private:
	srec_input_file_srecord(const srec_input_file_srecord &);
	srec_input_file_srecord &operator=(const srec_input_file_srecord &);
	int data_record_count;
	int read_inner(srec_record &);
	bool garbage_warning;
	bool seen_some_input;
	bool header_seen;
	bool termination_seen;
};

#endif /* INCLUDE_INPUT_FILE_SRECORD_H */
