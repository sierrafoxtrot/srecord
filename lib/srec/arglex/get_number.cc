//
//	srecord - manipulate eprom load files
//	Copyright (C) 2004 Peter Miller;
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
// MANIFEST: functions to impliment the srec_arglex_get_number class
//

#include <interval.h>
#include <srec/arglex.h>
#include <srec/input.h>
#include <srec/input/interval.h>


unsigned long
srec_arglex::get_number(const char *caption)
{
    unsigned long value;
    unsigned long multiple;
    srec_input *ifp;
    interval over;

    switch (token_cur())
    {
    case token_number:
	value = value_number();
	token_next();
	return value;

    case token_minimum:
	token_next();
	ifp = get_input();
	over = srec_input_interval(ifp);
	delete ifp;
	value = over.get_lowest();
	break;

    case token_maximum:
	token_next();
	ifp = get_input();
	over = srec_input_interval(ifp);
	delete ifp;
	value = over.get_highest();
	break;

    case token_length:
	token_next();
	ifp = get_input();
	over = srec_input_interval(ifp);
	delete ifp;
	value = (over.get_highest() - over.get_lowest());
	break;

    default:
	cerr << "number expected for " << caption << endl;
	exit(1);
	return 0;
    }
    switch (token_cur())
    {
    case token_round_down:
	token_next();
	multiple = get_number("-round-down");
	value /= multiple;
	value *= multiple;
	break;

    case token_round_up:
	token_next();
	multiple = get_number("-round-up");
	value = (value + multiple - 1) / multiple;
	value *= multiple;
	break;

    case token_round_nearest:
	token_next();
	multiple = get_number("-round-nearest");
	value = (value + multiple / 2) / multiple;
	value *= multiple;
	break;
    }
    return value;
}
