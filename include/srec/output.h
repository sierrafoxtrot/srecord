/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998, 1999, 2001 Peter Miller;
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
 * MANIFEST: interface definition for lib/srec/output.cc
 */

#ifndef INCLUDE_SREC_OUTPUT_H
#define INCLUDE_SREC_OUTPUT_H

#pragma interface "srec_output"

#include <stdarg.h>
#include <string>

class srec_output
{
public:
	virtual ~srec_output();
	virtual void write(const class srec_record &) = 0;
	virtual void write_header();
	virtual void write_data(unsigned long, const void *, size_t);
	virtual void write_termination(unsigned long = 0);

	virtual void line_length_set(int) = 0;
	virtual void address_length_set(int) = 0;
	virtual int preferred_block_size_get() const = 0;

	virtual void fatal_error(const char *, ...) const;
	virtual void fatal_error_v(const char *, va_list) const;
	virtual void fatal_error_errno(const char *, ...) const;
	virtual void fatal_error_errno_v(const char *, va_list) const;
	virtual void warning(const char *, ...) const;
	virtual void warning_v(const char *, va_list) const;

	virtual const string filename() const = 0;

protected:
	srec_output();

private:
	srec_output(const srec_output &);
	srec_output &operator=(const srec_output &);
};

#endif /* INCLUDE_SREC_OUTPUT_H */
