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

#ifndef SRECORD_OUTPUT_FILE_VHDL_TEXTIO_H
#define SRECORD_OUTPUT_FILE_VHDL_TEXTIO_H

#include <srecord/output/file.h>
#include <srecord/record.h>

namespace srecord
{

/**
  * The srecord::output_file_vhdl_textio class is used to represent the
  * state out an output file in VHDL textio format.
  */
class output_file_vhdl_textio:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_vhdl_textio() override = default;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_vhdl_textio(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation
    void write(const record &) override;

    // See base class for documentation
    void line_length_set(int) override {}

    // See base class for documentation
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

    // See base class for documentation
    void command_line(arglex_tool *cmdln) override;

    // See base class for documentation.
    const char *format_name() const override;

    // See base class for documentation.
    void address_length_set(int /*nbytes*/) override {}

private:
    /**
      * The consume_bytes_per_word instance variable is used to remember how
      * many bytes to consume for each output row.
      */
    unsigned consume_bytes_per_word{1};

    /**
      * The gen_bits_per_word instance variable is used to remember how
      * many bits to generate on each output row.
      * must be smaller or equal to consume_bytes_per_word * 8
      */
    unsigned gen_bits_per_word{8};

    /**
      * helper function to append nbits from inp to line
      * converts nbits from byte data_t inp to a string of '0' and '1',
      * appending each to the string line.
      */
    static void append_bits(record::data_t inp, size_t nbits,
                            std::string &line);

public:
    /**
      * The default constructor.
      */
    output_file_vhdl_textio() = delete;

    /**
      * The copy constructor.
      */
    output_file_vhdl_textio(const output_file_vhdl_textio &) = delete;

    /**
      * The assignment operator.
      */
    output_file_vhdl_textio &operator=(const output_file_vhdl_textio&) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_VHDL_TEXTIO_H
