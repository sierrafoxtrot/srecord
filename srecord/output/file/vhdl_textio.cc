//
// srecord - manipulate eprom load files
// Copyright (C) 2023 Daniel Anselmi <danselmi@gmx.ch>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <algorithm>
#include <srecord/arglex/tool.h>
#include <srecord/interval.h>
#include <srecord/output/file/vhdl_textio.h>
#include <srecord/record.h>
#include <srecord/progname.h>


srecord::output_file_vhdl_textio::output_file_vhdl_textio(const std::string &a_file_name) :
    srecord::output_file(a_file_name)
{
}


srecord::output::pointer
srecord::output_file_vhdl_textio::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_vhdl_textio(a_file_name));
}


void
srecord::output_file_vhdl_textio::command_line(srecord::arglex_tool *cmdln)
{
    if (cmdln->can_get_number())
    {
        int in_width = cmdln->get_number("input_width");
        if (in_width % 8)
        {
            warning("input width %d is not a multiple of 8, rounding up!\n", in_width);
            in_width += in_width % 8;
        }

        int out_width = in_width;
        if (cmdln->can_get_number())
        {
            out_width = cmdln->get_number("output_width");
            if (out_width > in_width)
            {
                warning("output width %d is greater than input width!\n", out_width, in_width);
                out_width = in_width;
            }
        }
        consume_bytes_per_word = (in_width + 7) / 8;
        gen_bits_per_word = out_width;
    }
}

void
srecord::output_file_vhdl_textio::prepend_bits(srecord::record::data_t data, size_t nbits, std::string &line)
{
    for (size_t i = 0; i < nbits ; ++i)
    {
        if (data % 2)
            line = "1" + line;
        else
            line = "0" + line;
        data /= 2;
    }
}

void
srecord::output_file_vhdl_textio::write(const srecord::record &record)
{

    switch (record.get_type())
    {
    case srecord::record::type_unknown:
    case srecord::record::type_header:
    case srecord::record::type_data_count:
    case srecord::record::type_execution_start_address:
        // Ignore.
        break;
    case srecord::record::type_data:
        //
        // Make sure the data is aligned.
        //
        if ((record.get_address() % consume_bytes_per_word) ||
            (record.get_length() % consume_bytes_per_word))
            fatal_alignment_error(consume_bytes_per_word);

        for (size_t j = 0; j < record.get_length(); j += consume_bytes_per_word)
        {
            std::string line = "\n";
            unsigned num_bits = gen_bits_per_word;
            srecord::record::address_t current_word = 0;
            for (unsigned k = 0; k < consume_bytes_per_word; ++k)
            {
                prepend_bits(record.get_data(j + k), std::min(num_bits, 8u), line);
                num_bits -= 8;
            }
            put_string(line);
        }
        break;

    }
}


bool
srecord::output_file_vhdl_textio::preferred_block_size_set(int nbytes)
{
    // this looks really wrong, from vhdl.cc ???
    if (nbytes > 1 || nbytes > record::max_data_length)
        return false;

    if (consume_bytes_per_word > 1 && nbytes % consume_bytes_per_word)
        return false;
    return true;
}


int
srecord::output_file_vhdl_textio::preferred_block_size_get()
    const
{
    //
    // Use the largest we can get, but it has to be a multiple of our
    // input word size.
    //
    int n = srecord::record::max_data_length;
    if (consume_bytes_per_word > 1)
        n -= (n % consume_bytes_per_word);
    return n;
}


const char *
srecord::output_file_vhdl_textio::format_name()
    const
{
    return "VHDL textio";
}
