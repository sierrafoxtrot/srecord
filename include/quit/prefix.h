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
 * MANIFEST: interface definition for include/quit/prefix.cc
 */

#ifndef INCLUDE_QUIT_PREFIX_H
#define INCLUDE_QUIT_PREFIX_H

#pragma interface "quit_prefix"

#include <string>
#include <quit.h>

class quit_prefix:
	public quit
{
public:
	virtual ~quit_prefix();
	quit_prefix(quit &, const char *);
	quit_prefix(quit &, const string &);

	virtual void exit(int);
	virtual void message_v(const char *, va_list);

private:
	string prefix;
	quit &deeper;

	quit_prefix();
	quit_prefix(const quit_prefix &);
	quit_prefix &operator=(const quit_prefix &);
};

#endif /* INCLUDE_QUIT_PREFIX_H */
