//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2000, 2002, 2003, 2005-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <srecord/input/filter.h>

#include <utility>


srecord::input_filter::~input_filter()
= default;


srecord::input_filter::input_filter(srecord::input::pointer arg) :
    ifp(std::move(arg))
{
}


auto
srecord::input_filter::filename()
    const -> std::string
{
    return ifp->filename();
}


auto
srecord::input_filter::filename_and_line()
    const -> std::string
{
    return ifp->filename_and_line();
}


auto
srecord::input_filter::read(srecord::record &record) -> bool
{
    return ifp->read(record);
}


auto
srecord::input_filter::get_file_format_name()
    const -> const char *
{
    return ifp->get_file_format_name();
}


void
srecord::input_filter::disable_checksum_validation()
{
    ifp->disable_checksum_validation();
}
