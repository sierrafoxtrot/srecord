//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2000, 2002, 2003, 2005-2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_input_filter class
//


#include <lib/srec/input/filter.h>


srec_input_filter::srec_input_filter(srec_input *arg) :
    ifp(arg)
{
}


srec_input_filter::~srec_input_filter()
{
    delete ifp;
}


string
srec_input_filter::filename()
    const
{
    return ifp->filename();
}


string
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


void
srec_input_filter::disable_checksum_validation()
{
    ifp->disable_checksum_validation();
}
