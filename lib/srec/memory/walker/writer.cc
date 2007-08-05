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


#include <lib/srec/memory/walker/writer.h>
#include <lib/srec/output.h>


srec_memory_walker_writer::srec_memory_walker_writer(srec_output *arg) :
    op(arg)
{
}


srec_memory_walker_writer::~srec_memory_walker_writer()
{
}


void
srec_memory_walker_writer::notify_upper_bound(unsigned long address)
{
    op->notify_upper_bound(address);
}


void
srec_memory_walker_writer::observe(unsigned long address, const void *data,
    int length)
{
    op->write_data(address, data, length);
}


void
srec_memory_walker_writer::observe_header(const srec_record *rp)
{
    op->write_header(rp);
}


void
srec_memory_walker_writer::observe_start_address(const srec_record *rp)
{
    op->write_start_address(rp);
}
