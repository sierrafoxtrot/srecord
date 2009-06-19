//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <lib/srec/memory/walker/gcrypt.h>


srec_memory_walker_gcrypt::~srec_memory_walker_gcrypt()
{
}


srec_memory_walker_gcrypt::srec_memory_walker_gcrypt(gcry_md_hd_t a_handle) :
    handle(a_handle)
{
}


srec_memory_walker::pointer
srec_memory_walker_gcrypt::create(gcry_md_hd_t a_handle)
{
    return pointer(new srec_memory_walker_gcrypt(a_handle));
}


void
srec_memory_walker_gcrypt::observe(unsigned long, const void *data, int length)
{
    gcry_md_write(handle, data, length);
}


// vim:ts=8:sw=4:et
