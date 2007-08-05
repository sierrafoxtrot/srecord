//
//      srecord - The "srecord" program.
//      Copyright (C) 2007 Peter Miller
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

#include <lib/srec/input/generator/repeat.h>


srec_input_generator_repeat::~srec_input_generator_repeat()
{
    delete [] data;
}


srec_input_generator_repeat::srec_input_generator_repeat(
        const interval &a_range, unsigned char *a_data, size_t a_length) :
    srec_input_generator(a_range),
    address(a_range.get_lowest()),
    data(0),
    length(a_length)
{
    // assert(length > 0);
    data = new unsigned char [length];
    for (size_t j = 0; j < length; ++j)
        data[j] = a_data[j];
}


unsigned char
srec_input_generator_repeat::generate_data(unsigned long addr)
{
    return data[(addr - address) % length];
}


string
srec_input_generator_repeat::filename()
    const
{
    return "generate repeat data";
}


const char *
srec_input_generator_repeat::get_file_format_name()
    const
{
    return "generate repeat data";
}
