//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2005-2008, 2010-2012 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_DEC_BINARY_H
#define SRECORD_OUTPUT_FILE_DEC_BINARY_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_dec_binary class is used to write a DEC Binary
  * (XXDP) formatted file.
  */
class output_file_dec_binary:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_dec_binary() override;

private:
    /**
      * A constructor.  The input will be read from the named file (or
      * the standard input if the file name is "-"). It is private on
      * purpose, use the create class method instead.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_dec_binary(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.  The input will be read from
      * the named file (or the standard input if the file name is "-").
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const record &) override;

    // See base class for documentation.
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    const char *format_name() const override;

    // See base class for documentation.
    bool is_binary() const override;

private:
    /**
      * See base class for documentation.  We are over-riding it because
      * we use raw binary, so we call the put_char() method.  This
      * method also tracks the byte_offset, so that we can align to
      * specific boundaries.  Calls the checksum_add() method.
      */
    void put_byte(unsigned char) override;

    /**
      * The byte_offset instance variable is used to track the location
      * in the output file.  Maintained by the put_byte() method.
      */
    unsigned long byte_offset;

    /**
      * The pref_block_size is used to remember the preferred
      * block size.  Set by the constructor.  Read by the
      * preferred_block_size_get() method.
      */
    int pref_block_size;

    /**
      * The preferred_block_size_calculate method is used to determine
      * the best block size to pack into 512 byte blocks.
      */
    static int preferred_block_size_calculate();

    /**
      * The default constructor.  Do not use.
      */
    output_file_dec_binary() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    output_file_dec_binary(const output_file_dec_binary &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    output_file_dec_binary &operator=(const output_file_dec_binary &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_DEC_BINARY_H
