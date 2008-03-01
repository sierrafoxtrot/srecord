//
//      srecord - manipulate eprom load files
//      Copyright (C) 2006-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//


#include <lib/srec/memory/walker/continuity.h>


srec_memory_walker_continuity::~srec_memory_walker_continuity()
{
}


srec_memory_walker_continuity::srec_memory_walker_continuity() :
    current_address(0),
    data_seen(false),
    nholes(0)
{
}


srec_memory_walker_continuity::pointer
srec_memory_walker_continuity::create()
{
    return pointer(new srec_memory_walker_continuity());
}


void
srec_memory_walker_continuity::observe(unsigned long addr, const void *,
    int nbytes)
{
    if (data_seen)
    {
        if (current_address != addr)
            ++nholes;
    }
    else
    {
        data_seen = true;
    }
    current_address = addr + nbytes;
}


bool
srec_memory_walker_continuity::is_continuous()
    const
{
    return (data_seen && nholes == 0);
}
