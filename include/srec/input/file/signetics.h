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
 * MANIFEST: interface definition for include/srec/input/file/signetics.cc
 */

#ifndef INCLUDE_SREC_INPUT_FILE_SIGNETICS_H
#define INCLUDE_SREC_INPUT_FILE_SIGNETICS_H

#pragma interface "srec_input_file_signetics"

#include <srec/input/file.h>

class srec_input_file_signetics: public srec_input_file
{
public:
	srec_input_file_signetics();
	srec_input_file_signetics(const char *);
	virtual ~srec_input_file_signetics();
	int read(srec_record &);
	const char *get_file_format_name() const;

protected:
	void checksum_add(unsigned char);

private:
	srec_input_file_signetics(const srec_input_file_signetics &);
	srec_input_file_signetics &operator=(const srec_input_file_signetics &);
	int data_record_count;
	int read_inner(srec_record &);
	bool garbage_warning;
	bool seen_some_input;
	bool header_seen;
	bool termination_seen;
};

#endif /* INCLUDE_SREC_INPUT_FILE_SIGNETICS_H */
