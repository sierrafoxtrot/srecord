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
 * MANIFEST: interface definition for lib/quit.cc
 */

#ifndef INCLUDE_QUIT_H
#define INCLUDE_QUIT_H

#pragma interface "quit"

#include <stdarg.h>

class quit
{
public:
	virtual ~quit();
	quit();
	quit(const quit &);
	quit &operator=(const quit &);

	virtual void fatal_error(const char *, ...);
	virtual void fatal_error_v(const char *, va_list);
	virtual void fatal_error_errno(const char *, ...);
	virtual void fatal_error_errno_v(const char *, va_list);
	virtual void warning(const char *, ...);
	virtual void warning_v(const char *, va_list);
	virtual void message(const char *, ...);
	virtual void message_v(const char *, va_list) = 0;
	virtual void exit(int) = 0;
};

extern quit &quit_default;

#endif /* INCLUDE_QUIT_H */
