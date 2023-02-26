//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_FOUR_PACKED_CODE_H
#define SRECORD_OUTPUT_FILE_FOUR_PACKED_CODE_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_four_packed_code class is used to emit files
  * in the Four Packed Code (FPC) format.
  */
class output_file_four_packed_code:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_four_packed_code() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_four_packed_code(const std::string &file_name);

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
    const char *format_name() const override;

private:
    /**
      * The pref_block_size instance variable is used to
      * remember the preferred block size.  Read by the the
      * preferred_block_size_get method.  Set by the line_length_set
      * method.
      */
    int pref_block_size{32};

    /**
      * The put_byte_pos instance variable is used by the put_byte
      * method to remember where we are positioned within each
      * 4-byte chunk.
      */
    unsigned put_byte_pos{0};

    /**
      * The put_byte_value instance variable is used by the put_byte
      * method to remember the cumulative value of each 4-byte chunk.
      * At the end of 4 bytes, the 5-character base85 encoding
      * is issued.
      */
    uint32_t put_byte_value{0};

    /**
      * The write_inner method is used by the write method to
      * write a single line of output.
      */
    void write_inner(uint32_t address, const void *data,
        int data_length);

    /**
      * See base class for documentation.  We over-ride this method
      * so that we can do the base85 encoding of each 4-byte chunk.
      */
    void put_byte(uint8_t) override;

public:
    /**
      * The default constructor.
      */
    output_file_four_packed_code() = delete;

    /**
      * The copy constructor.
      */
    output_file_four_packed_code(const output_file_four_packed_code &) = delete;

    /**
      * The assignment operator.
      */
    output_file_four_packed_code &operator=(
        const output_file_four_packed_code &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_FOUR_PACKED_CODE_H
