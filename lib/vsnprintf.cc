//
//      srecord - manipulate eprom load files
//      Copyright (C) 2002, 2006, 2007 Peter Miller
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
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to impliment the common_vsnprintf class
//


#include <lib/config.h>


#ifndef HAVE_SNPRINTF

extern "C" int
snprintf(char *str, size_t size, const char *format, ...)
{
    va_start(ap, format);
    vsnprintf(str, size, format, ap);
    va_end(ap);
}

#endif


#ifndef HAVE_VSNPRINTF

extern "C" int
vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    // We'll just have to hope for the best...
    vsprintf(str, format, ap);
}

#endif
