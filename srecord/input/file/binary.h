//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2000, 2002, 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_BINARY_H
#define SRECORD_INPUT_FILE_BINARY_H

#include <srecord/input/file.h>

/**
  * The srec_input_file_binary class is used to represent the parse
  * state of a binary input stream.
  */
class srec_input_file_binary:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_binary();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_binary(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name);

protected:
    // See nase class for documentation.
    bool read(srec_record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The address instance variable is used to remember where we are
      * up to in the file.
      */
    unsigned long address;

    // See base class for documentation.
    const char *mode() const;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_binary();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_binary(const srec_input_file_binary &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_binary &operator=(const srec_input_file_binary &);
};

#endif // SRECORD_INPUT_FILE_BINARY_H
