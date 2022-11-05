//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_SPECTRUM_H
#define SRECORD_OUTPUT_FILE_SPECTRUM_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_spectrum class is used to represent the running
  * state when writing a file in Spectrum format.
  */
class output_file_spectrum:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_spectrum() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_spectrum(const std::string &a_filename);

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
      * The put_decimal method is used to write the decimal address to
      * the output, as a decimal (base 10) number.  It will have at
      * least four digits.
      */
    void put_decimal(unsigned long);

    /**
      * The put_binary method is used to write the binary data to the
      * output, as a binary (base 2) number.  It will have exactly
      * eight digits.
      */
    void put_binary(unsigned char);

    /**
      * The default constructor.  Do not use.
      */
    output_file_spectrum() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    output_file_spectrum(const output_file_spectrum &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const output_file_spectrum &) -> output_file_spectrum & = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_SPECTRUM_H
