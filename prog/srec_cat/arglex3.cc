/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 2000, 2001 Peter Miller;
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
 * MANIFEST: functions to impliment the srec_cat_arglex3 class
 */

#pragma implementation "srec_cat_arglex3"

#include <arglex3.h>


srec_cat_arglex3::srec_cat_arglex3(int argc, char **argv)
        : srec_arglex(argc, argv)
{
	static table_ty table[] =
	{
		{ "-Address_Length", token_address_length, },
		{ "-Data_Only", token_data_only, },
		{ "-Line_Length", token_line_length, },
		{ "-HEAder", token_header, },
		ARGLEX_END_MARKER
	};

	table_set(table);
}


srec_cat_arglex3::srec_cat_arglex3()
	: srec_arglex(0, 0)
{
	/* bug */
}


srec_cat_arglex3::srec_cat_arglex3(const srec_cat_arglex3 &)
	: srec_arglex(0, 0)
{
	/* bug */
}


srec_cat_arglex3 &
srec_cat_arglex3::operator=(const srec_cat_arglex3 &arg)
{
	/* bug */
	return *this;
}


srec_cat_arglex3::~srec_cat_arglex3()
{
}
