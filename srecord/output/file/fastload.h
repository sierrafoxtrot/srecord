//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_FASTLOAD_H
#define SRECORD_OUTPUT_FILE_FASTLOAD_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_fastload class is used to write an LSI Logic
  * Fast Load formatted file.
  */
class output_file_fastload:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_fastload() override;

private:
    /**
      * A constructor.  The output will be written to the named file (or
      * the standard output if the file name is "-"). It is private on
      * purpose, use the #create method instead.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_fastload(const std::string &a_filename);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.  The output will be written
      * to the named file (or the standard output if the file name is
      * "-").
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static auto create(const std::string &a_filename) -> pointer;

protected:
    // See base class for documentation.
    void write(const record &) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    auto preferred_block_size_get() const -> int override;

    // See base class for documentation.
    auto preferred_block_size_set(int nbytes) -> bool override;

    // See base class for documentation.
    auto format_name() const -> const char * override;

private:
    /**
      * The line_length instance variable is used to remember the
      * preferred line length for the output.
      */
    int line_length;

    /**
      * The address instance variable is used to track the current
      * address location within the file.
      */
    unsigned long address;

    /**
      * The column instance variable is used to track the current output
      * column, to ensure that the lines don't exceed 80 characters.
      */
    int column;

    /**
      * The bytes_since_checksum instance variable is used to track how
      * many bytes have ben emitted since the last checksum was issued.
      * Don't leave it too long, or the checksum isn't any use.
      */
    int bytes_since_checksum;

    /**
      * The max_since_checksum instance variable is used to remember how
      * many bytes to cover between issuing checksums.  This is done in
      * such a way as to minimize line length.  There is no point going
      * over 256 bytes, because it's a simple 16-bit additive sum.
      */
    int max_since_checksum;

    /**
      * The prev_was_command instance variable is used to track that was
      * last output, because a newline must be thrown between commands
      * and data, but not at any other time.
      */
    bool prev_was_command;

    /**
      * The write_inner method is used to write a single line (record)
      * to the file.  Use by the write() method.
      */
    void write_inner(int type, unsigned long addr, int addr_len,
            const void *data, int data_len);

    /**
      * The put_number method is used to write the given value to the
      * output in base-64 (big endian) notation.
      */
    void put_number(unsigned long n, int min_digits);

    /**
      * The put_command method is used to write a command to the file,
      * including the leading '/' and the trailing newline.
      */
    void put_command(int c, unsigned long n, int ndigits);

    /**
      * The default constructor.  Do not use.
      */
    output_file_fastload();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_fastload(const output_file_fastload &);

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const output_file_fastload &) -> output_file_fastload &;
};

};

#endif // SRECORD_OUTPUT_FILE_FASTLOAD_H
