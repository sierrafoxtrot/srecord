//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2005-2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_WILSON_H
#define SRECORD_OUTPUT_FILE_WILSON_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_wilson class is used to represent an output
  * file which is in "wilson" format.  This file format was reverse
  * engineered, it wasn't documented anywhere.  More information would
  * be welcome.
  *
  * @sa
  *     #srecord::output_file_motorola,
  *     #srecord::output_file_idt,
  */
class output_file_wilson:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_wilson() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_wilson(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static auto create(const std::string &file_name) -> pointer;

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
    void put_byte(unsigned char) override;

    // See base class for documentation.
    auto format_name() const -> const char * override;

    // See base class for documentation.
    auto is_binary() const -> bool override;

private:
    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of data bytes (NOT encoded hex characters) to
      * be placed in each output line.
      */
    int pref_block_size;

    /**
      * The write_inner method is used to write a line of output.
      *
      * @param tag
      *     The tag value at the start of the line.
      * @param address
      *     The address of the first byte of data on the line.
      * @param data
      *     The payload of this line.
      * @param data_nbytes
      *     The number of bytes of payload.
      */
    void write_inner(int tag, unsigned long address, const void *data,
        int data_nbytes);

    /**
      * The default constructor.  Do not use.
      */
    output_file_wilson() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    output_file_wilson(const output_file_wilson &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const output_file_wilson &) -> output_file_wilson & = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_WILSON_H
