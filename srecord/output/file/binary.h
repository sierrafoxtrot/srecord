//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2005-2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_BINARY_H
#define SRECORD_OUTPUT_FILE_BINARY_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_binary class is used to represent an output
  * file in raw binary format.
  */
class output_file_binary:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_binary() override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open and write output to.
      */
    output_file_binary(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
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

    // See base class for documentation.
    auto is_binary() const -> bool override;

private:
    /**
      * The default constructor.  Do not use.
      */
    output_file_binary() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    output_file_binary(const output_file_binary &) = delete;

    /**
      * The copy constructor.  Do not use.
      */
    auto operator=(const output_file_binary &) -> output_file_binary & = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_BINARY_H
