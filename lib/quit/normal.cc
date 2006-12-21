//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000, 2002, 2006 Peter Miller
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the quit_normal class
//


#include <cstdio>
#include <cstdarg>

#include <iostream>
using namespace std;
#include <lib/progname.h>
#include <lib/quit/normal.h>


quit_normal::quit_normal()
{
}


quit_normal::quit_normal(const quit_normal &arg)
{
}


quit_normal &
quit_normal::operator=(const quit_normal &arg)
{
	return *this;
}


quit_normal::~quit_normal()
{
}


void
quit_normal::exit(int n)
{
	::exit(n);
}


void
quit_normal::message_v(const char *fmt, va_list ap)
{
	cout.flush();
	cerr << progname_get() << ": ";
	char buf[1024];
	vsnprintf(buf, sizeof(buf), fmt, ap);
	cerr << buf << endl;
	cerr.flush();
}


static quit_normal quit_normal_default;
quit &quit_default = quit_normal_default;
