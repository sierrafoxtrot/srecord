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
 * MANIFEST: interface definition for prog/srec_cat/arglex3.cc
 */

#ifndef PROG_SREC_CAT_ARGLEX3_H
#define PROG_SREC_CAT_ARGLEX3_H

#pragma interface "srec_cat_arglex3"

#include <srec/arglex.h>

class srec_cat_arglex3: public srec_arglex
{
public:
	enum
	{
		token_line_length = srec_arglex::token_MAX,
		token_MAX
	};

	~srec_cat_arglex3();
	srec_cat_arglex3(int, char **);

private:
	srec_cat_arglex3();
	srec_cat_arglex3(const srec_cat_arglex3 &);
	srec_cat_arglex3 &operator=(const srec_cat_arglex3 &);
};

#endif /* PROG_SREC_CAT_ARGLEX3_H */
