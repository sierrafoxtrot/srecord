//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006-2008 Peter Miller
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

#ifndef LIB_SREC_OUTPUT_FILE_SPECTRUM_H
#define LIB_SREC_OUTPUT_FILE_SPECTRUM_H

#include <lib/srec/output/file.h>

/**
  * The srec_output_file_spectrum class is used to represent the running
  * state when writing a file in Spectrum format.
  */
class srec_output_file_spectrum:
    public srec_output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_output_file_spectrum();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    srec_output_file_spectrum(const std::string &file_name);

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
    void write(const srec_record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    const char *format_name() const;

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
    srec_output_file_spectrum();

    /**
      * The copy constructor.  Do not use.
      */
    srec_output_file_spectrum(const srec_output_file_spectrum &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_output_file_spectrum &operator=(const srec_output_file_spectrum &);
};

#endif // LIB_SREC_OUTPUT_FILE_SPECTRUM_H
