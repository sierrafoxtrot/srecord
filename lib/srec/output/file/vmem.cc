//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2009 Peter Miller
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

#include <cctype>

#include <lib/srec/arglex.h>
#include <lib/srec/output/file/vmem.h>
#include <lib/srec/record.h>


srec_output_file_vmem::~srec_output_file_vmem()
{
    if (column)
        put_char('\n');
}


//
// The default number of bits is 32.
// If you change this, you must also change the following files:
//     lib/srec/output/file/vmem.cc
//     man/man1/srec_cat.1
//
#define DEFAULT_MEM_WIDTH 32


//
// Calculate log2(bytes_per_word)
//
// For example...
// Return  Num    Num
// Value   Bytes  Bits
//   0      1       8
//   1      2      16
//   2      4      32
//   3      8      64
//   4     16     128
//
static unsigned
calc_width_shift(int x)
{
    //
    // The user could be giving a number of bytes.
    // (But the range is limited to those values not easily confused
    // with a number of bits.)
    //
    if (x == 1)
        return 0;
    if (x == 2)
        return 1;
    if (x == 4)
        return 2;

    //
    // Number of bits.
    //
    if (x == 8)
        return 0;
    if (x == 16)
        return 1;
    if (x == 32)
        return 2;
    if (x == 64)
        return 3;
    if (x == 128)
        return 4;

    //
    // The default number of bits is 32.
    // If you change this, you must also change the following files:
    //     man/man1/srec_cat.1
    //     lib/srec/arglex_output.cc
    //
    return 2;
}


static unsigned
calc_width_mask(int x)
{
    return ((1uL << calc_width_shift(x)) - 1uL);
}


srec_output_file_vmem::srec_output_file_vmem(const std::string &a_file_name) :
    srec_output_file(a_file_name),
    bytes_per_word(4),
    address(0),
    column(0),
    pref_block_size(4 * bytes_per_word),
    width_shift(calc_width_shift(8 * bytes_per_word)),
    width_mask(calc_width_mask(8 * bytes_per_word))
{
    line_length_set(80);
}


srec_output::pointer
srec_output_file_vmem::create(const std::string &a_file_name)
{
    return pointer(new srec_output_file_vmem(a_file_name));
}


void
srec_output_file_vmem::command_line(srec_arglex *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
        int n = cmdln->value_number();
        cmdln->token_next();

        width_shift = calc_width_shift(n);
        bytes_per_word = 1u << width_shift;
        width_mask = calc_width_mask(n);

        //
        // Recalculate the preferred block size
        // for an 80 column line.
        //
        line_length_set(80);
    }
}


void
srec_output_file_vmem::write(const srec_record &record)
{
    switch (record.get_type())
    {
    case srec_record::type_header:
        // emit header records as comments in the file
        if (enable_header_flag && record.get_length() > 0)
        {
            put_string("/* ");
            if (record.get_address() != 0)
            {
                unsigned long addr = record.get_address();
                put_stringf("%08lX: ", addr);
            }
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
                if (c == '\n')
                    put_stringf("\n * ");
                else if (isprint(c) || isspace(c))
                    put_char(c);
                else
                    put_stringf("\\%o", c);
                // make sure we don't end the comment
                if (c == '*' && cp < ep && *cp == '/')
                    put_char(' ');
            }
            put_string(" */\n");
        }
        break;

    case srec_record::type_data:
        //
        // make sure the data is aligned properly
        //
        if
        (
            (record.get_address() & width_mask)
        ||
            (record.get_length() & width_mask)
        )
            fatal_alignment_error(1u << width_shift);

        //
        // If we need to advance the address, it has to be at the start
        // of a line.
        //
        if (address != record.get_address())
        {
            if (column)
            {
                put_char('\n');
                column = 0;
            }
            address = record.get_address();
        }

        //
        // emit the data bytes
        //
        for (size_t j = 0; j < record.get_length(); j += bytes_per_word)
        {
            //
            // Each line starts with an address.
            // The addresses are actually divided by the memory width,
            // rather than being byte adddresses.
            //
            // The presence of the @ character would seem to imply this
            // is optional.  It would be easy to figure out that an
            // address is an address, and not a data byte.
            //
            if (column == 0)
                put_stringf("@%08lX", address >> width_shift);

            //
            // Put a space between each word
            //
            put_char(' ');

            //
            // emit the bytes of the word
            //
            for (unsigned k = 0; k < bytes_per_word; ++k)
            {
                //
                // Write the byte and crank the address.
                //
                put_byte(record.get_data(j + k));
                ++address;

                //
                // Crank the column.
                // If the line is too long, finish it.
                //
                ++column;
                if (column >= pref_block_size)
                {
                    put_char('\n');
                    column = 0;
                }
            }
        }
        break;

    case srec_record::type_data_count:
    case srec_record::type_execution_start_address:
        // ignore
        break;

    case srec_record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srec_output_file_vmem::line_length_set(int linlen)
{
    int nwords = (linlen - 9) / (bytes_per_word * 2 + 1);
    int max_words = srec_record::max_data_length >> width_shift;
    if (nwords > max_words)
        nwords = max_words;
    if (nwords < 1)
        nwords = 1;
    pref_block_size = nwords * bytes_per_word;
}


void
srec_output_file_vmem::address_length_set(int)
{
    // ignore
}


int
srec_output_file_vmem::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srec_output_file_vmem::format_name()
    const
{
    return "VMem";
}
