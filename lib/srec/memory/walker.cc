//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2002, 2003, 2006, 2007 Peter Miller
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
// MANIFEST: functions to impliment the srec_memory_walker class
//


#include <lib/srec/memory/walker.h>


srec_memory_walker::srec_memory_walker()
{
}


srec_memory_walker::~srec_memory_walker()
{
}


void
srec_memory_walker::notify_upper_bound(unsigned long)
{
}


void
srec_memory_walker::observe_header(const srec_record *)
{
}


void
srec_memory_walker::observe_start_address(const srec_record *)
{
}
