//
//      srecord - The "srecord" program.
//      Copyright (C) 2007-2010 Peter Miller
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

#include <cstdio>

#include <srecord/input/generator/constant.h>


srec_input_generator_constant::~srec_input_generator_constant()
{
}


srec_input_generator_constant::srec_input_generator_constant(
        const interval &a_range, unsigned char a_datum) :
    srec_input_generator(a_range),
    datum(a_datum)
{
}


srec_input::pointer
srec_input_generator_constant::create(const interval &a_range,
    unsigned char a_datum)
{
    return pointer(new srec_input_generator_constant(a_range, a_datum));
}


unsigned char
srec_input_generator_constant::generate_data(unsigned long)
{
    return datum;
}


std::string
srec_input_generator_constant::filename()
    const
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "constant 0x%2.2X", datum);
    return buffer;
}


const char *
srec_input_generator_constant::get_file_format_name()
    const
{
    return "constant";
}


// vim:ts=8:sw=4:et
