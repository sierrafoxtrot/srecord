//
//	srecord - manipulate eprom load files
//	Copyright (C) 1998-2000, 2002, 2003 Peter Miller;
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
// MANIFEST: functions to impliment the srec_input_filter class
//

#pragma implementation "srec_input_filter"

#include <srec/input/filter.h>


srec_input_filter::srec_input_filter(srec_input *arg) :
    ifp(arg)
{
}


srec_input_filter::~srec_input_filter()
{
    delete ifp;
}


const string
srec_input_filter::filename()
    const
{
    return ifp->filename();
}


const string
srec_input_filter::filename_and_line()
    const
{
    return ifp->filename_and_line();
}


int
srec_input_filter::read(srec_record &record)
{
    return ifp->read(record);
}


const char *
srec_input_filter::get_file_format_name()
    const
{
    return ifp->get_file_format_name();
}
