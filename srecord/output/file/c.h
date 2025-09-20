//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2003, 2005-2008, 2010 Peter Miller
// Copyright (C) 2014 Scott Finneran
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_C_H
#define SRECORD_OUTPUT_FILE_C_H

#include <cstdint>

#include <srecord/output/file.h>
#include <srecord/interval.h>

namespace srecord
{

/**
  * The srecord::output_file_c class is used to represent an output file
  * which emits C code.
  */
class output_file_c:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_c() override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    output_file_c(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const record &) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

    // See base class for documentation.
    void command_line(arglex_tool *cmdln) override;

    // See base class for documentation.
    const char *format_name() const override;

private:
    /**
      * The prefix instance variable is used to remember the variable
      * name prefix to be used in the output.
      */
    std::string prefix{"eprom"};

    /**
      * The prefix instance variable is used to remember the definition
      * prefix to be emitted in the header.
      */
    std::string header_prefix;

    /**
      * The postfix instance variable is used to remember the definition
      * postfix to be emitted in the header.
      */
    std::string header_postfix;

    /**
      * The taddr instance variable is used to remember the
      * termination address, to be emitted in the footer.
      */
    unsigned long taddr{0};

    /**
      * The range instance variable is used to remember the range
      * of addresses present in the output.
      */
    interval range;

    /**
      * The header_done instance variable is used t remember whether
      * the emit_header method has been called.
      */
    bool header_done{false};

    /**
      * The column instance variable is used to remember the current
      * printing column on the line.
      */
    int column{0};

    /**
      * The current_address instance variable is used to remember
      * the current address that the file is positioned at.  This is
      * used to know whether we need to add padding.
      */
    unsigned long current_address{0};

    /**
      * The line_length instance variable is used to remember the
      * maximum line length.  The output usually does not exceed it.
      */
    int line_length{75};

    /**
      * The address_length instance variable is used to remember how
      * many bytes to emit when emitting addresses.
      */
    int address_length{4};

    /**
      * The constant instance variable is used to remember whether or
      * not to use the "const" keyword.
      */
    bool constant{true};

    /**
      * The include instance variable is used to remember whether or not
      * to generate an include file.
      */
    bool include{false};

    /**
      * The include_file_name instance variable is used to remember the
      * name of the include file to be generated.
      */
    std::string include_file_name;

    /**
      * The output_type instance variable is used to remember what C data type
      * the input bytes should be emitted as.
      */
    enum {
      output_byte,
      output_unsigned_short,
      output_uint16_t,
      output_uint32_t,
      output_uint64_t,
    } output_type{output_byte};

    /**
     * bytes_per_word is a const function that returns how many bytes are in
     * an output word based on the output_type instance variable.
     */
    unsigned bytes_per_word() const;

    /**
     * output_c_type returns a string that evalutates to the C variable type of
     * the resulting generated array based on the output_type instance
     * vairable.
     */
    char const * output_c_type() const;

    /**
     * max_word_value returns an output word with all of the bits set.
     */
    uint64_t max_word_value() const;

    /**
      * The hex_style instance variable is used to remember whether or
      * not we are to output number in hexadecimal (true) or decimal
      * (false).
      */
    bool hex_style{true};

    /**
      * The section_style instance variable is used to remember whether
      * or not the output is to contain "sections".
      *
      * In non-section output, padding of 0xFF is used to pad the data
      * for correct addressing.  In section output, tables of addresses
      * and lengths are emitted, and the actual data is intended to be
      * relocated at run time.
      */
    bool section_style{false};

    /**
      * The emit_header method is used to emit the initial portion
      * of the array declaration.  It does nothing if header_done
      * is true.
      */
    void emit_header();

    /**
      * The emit_word method is used to emit a single word.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_word(uint64_t);

    /**
      * The format_address method is used to format an address, taking
      * the hex_style and address_length instance variable settings.
      *
      * @param addr
      *     The address to be formatted
      */
    std::string format_address(unsigned long addr) const;

public:
    /**
      * The default constructor.
      */
    output_file_c() = delete;

    /**
      * The copy constructor.
      */
    output_file_c(const output_file_c &) = delete;

    /**
      * The assignment operator.
      */
    output_file_c &operator=(const output_file_c &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_C_H
