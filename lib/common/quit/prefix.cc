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
 * MANIFEST: functions to impliment the quit_prefix class
 */

#pragma implementation "quit_prefix"

#include <strstream.h>
#include <quit/prefix.h>


quit_prefix::quit_prefix(quit &a1, const char *a2)
	: prefix(a2), deeper(a1)
{
}


quit_prefix::quit_prefix(quit &a1, const string &a2)
	: prefix(a2), deeper(a1)
{
}


quit_prefix::quit_prefix(const quit_prefix &arg)
	: prefix(arg.prefix), deeper(arg.deeper)
{
}


quit_prefix &
quit_prefix::operator=(const quit_prefix &arg)
{
	if (this != &arg)
	{
		prefix = arg.prefix;
		deeper = arg.deeper;
	}
	return *this;
}


quit_prefix::~quit_prefix()
{
}


void
quit_prefix::exit(int n)
{
	deeper.exit(n);
}


void
quit_prefix::message_v(const char *fmt, va_list ap)
{
	if (prefix != string(""))
	{
		strstream hold;
		hold.vform(fmt, ap);
		hold << '\0';
		deeper.message("%s: %s", prefix.data(), hold.str());
	}
	else
		deeper.message_v(fmt, ap);
}
