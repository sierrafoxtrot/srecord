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
 * MANIFEST: interface definition for lib/srec/input.cc
 */

#ifndef INCLUDE_SREC_INPUT_H
#define INCLUDE_SREC_INPUT_H

#pragma interface "srec_input"

#include <string>
#include <stdarg.h>

class srec_input
{
public:
	srec_input();
	srec_input(const srec_input &);
	srec_input &operator=(const srec_input &);
	virtual ~srec_input();

	virtual int read(class srec_record &) = 0;
	virtual void fatal_error(const char *, ...) const;
	virtual void fatal_error_v(const char *, va_list) const;
	virtual void fatal_error_errno(const char *, ...) const;
	virtual void fatal_error_errno_v(const char *, va_list) const;
	virtual void warning(const char *, ...) const;
	virtual void warning_v(const char *, va_list) const;
	virtual const string filename() const = 0;
	virtual const string filename_and_line() const;
};

#endif /* INCLUDE_SREC_INPUT_H */
