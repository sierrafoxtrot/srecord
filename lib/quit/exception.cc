//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//
// MANIFEST: functions to impliment the quit_exception class
//


#include <lib/quit/exception.h>


quit_exception::quit_exception()
{
}


quit_exception::~quit_exception()
{
}


void
quit_exception::exit(int)
{
    throw vomit();
}


void
quit_exception::message_v(const char *, va_list)
{
    // don't say anything
}
