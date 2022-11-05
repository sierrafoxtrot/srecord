//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_TEKTRONIX_EXTENDED_H
#define SRECORD_OUTPUT_FILE_TEKTRONIX_EXTENDED_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_tektronix_extended class is used to represent
  * an output file which is in Tektronix Extended format.
  */
class output_file_tektronix_extended:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_tektronix_extended() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_tektronix_extended(const std::string &file_name);

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
    auto format_name() const -> const char * override;

private:
    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of data bytes (NOT encoded hex characters) to
      * be placed in each output line.
      */
    int pref_block_size;

    /**
      * The address_length instance variable is used to remember the
      * minimum number of address bytes to be emitted into output lines.
      */
    int address_length;

    /**
      * The write_inner method is used to write a line of output.
      *
      * @param tag
      *     The tag value at the start of the line.
      * @param address
      *     The address of the first byte of data on the line.
      * @param address_nbytes
      *     The number of bytes of address to emit.
      * @param data
      *     The payload of this line.
      * @param data_nbytes
      *     The number of bytes of payload.
      */
    void write_inner(int tag, unsigned long address, int addr_nbytes,
        const void *data, int data_nbytes);

    /**
      * The default constructor.  Do not use.
      */
    output_file_tektronix_extended() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    output_file_tektronix_extended(const output_file_tektronix_extended &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(
        const output_file_tektronix_extended &) -> output_file_tektronix_extended & = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_TEKTRONIX_EXTENDED_H
