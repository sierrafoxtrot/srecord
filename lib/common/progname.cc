//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998, 1999, 2002, 2006 Peter Miller;
//	All rights reserved.
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
// MANIFEST: functions to impliment the progname class
//

#include <cstring>

#include <progname.h>


static char *progname;


void
progname_set(char *s)
{
    for (;;)
    {
	char            *cp1;
	char            *cp2;
	
	cp1 = strrchr(s, '/');
	if (!cp1)
	    cp1 = s;
	else
	{
	    if (!cp1[1])
	    {
		*cp1 = 0;
		continue;
	    }
	    ++cp1;
	}

	cp2 = strrchr(s, '\\');
	if (!cp2)
	    cp2 = s;
	else
	{
	    if (!cp2[1])
	    {
		*cp2 = 0;
		continue;
	    }
	    ++cp2;
	}

	progname = (cp1 > cp2 ? cp1 : cp2);
	return;
    }
}


const char *
progname_get()
{
    return (progname ? progname : "???");
}
