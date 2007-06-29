//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
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

#include <lib/r250.h>
#include <lib/srec/input/generator/random.h>


srec_input_generator_random::~srec_input_generator_random()
{
}


srec_input_generator_random::srec_input_generator_random(
        const interval &a_range) :
    srec_input_generator(a_range)
{
}


unsigned char
srec_input_generator_random::generate_data(unsigned long)
{
    return r250();
}


string
srec_input_generator_random::filename()
    const
{
    return "random";
}


const char *
srec_input_generator_random::get_file_format_name()
    const
{
    return "random";
}

// vim:ts=8:sw=4:et
