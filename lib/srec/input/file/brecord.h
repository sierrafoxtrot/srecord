//
//      srecord - The "srecord" program.
//      Copyright (C) 2007, 2008 Peter Miller
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

#ifndef LIB_SREC_INPUT_FILE_BRECORD_H
#define LIB_SREC_INPUT_FILE_BRECORD_H

#include <lib/srec/input/file.h>

/**
  * The srec_input_file_brecord class is used to represent the parse
  * state of a Motorola MC68EZ328 bootstrap b-record format input file.
  */
class srec_input_file_brecord:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_brecord();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_brecord(const string &file_name);

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
    static pointer create(const string &file_name);

protected:
    // See base class for documentation.
    bool read(srec_record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The read_inner method is used to read a record from the file.
      * The read method is a wrapper around it.
      */
    int read_inner(srec_record &);

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not any data has been seen from this file to date.
      */
    bool seen_some_input;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_brecord();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_brecord(const srec_input_file_brecord &);

    /**
      * The assigmne toperator.  Do not use.
      */
    srec_input_file_brecord &operator=(const srec_input_file_brecord &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_INPUT_FILE_BRECORD_H
