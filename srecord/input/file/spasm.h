//
//      srecord - manipulate eprom load files
//      Copyright (C) 2001-2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_SPASM_H
#define SRECORD_INPUT_FILE_SPASM_H

#include <srecord/endian.h>
#include <srecord/input/file.h>

/**
  * The srec_input_file_spasm class is used to represent the parse state
  * of a SPASM formatted input file.
  */
class srec_input_file_spasm:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_spasm();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      * @param end
      *     The byte order.
      */
    srec_input_file_spasm(const std::string &file_name,
        endian_t end = endian_big);

public:
    /**
      * The create_be class method is used to create new dynamically
      * allocated instances of this class.  This is used by the --guess
      * format.
      *
      * @param file_name
      *     The name of the file to be read.
      * @param end
      *     The byte order.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create_be(const std::string &file_name);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @param end
      *     The byte order of the file
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name, endian_t end);

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
      * The end instance variable is used to remember whether the
      * file is big-endian or little-endian.
      */
    endian_t end;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_spasm();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_spasm(const srec_input_file_spasm &);

    /**
      * The assigmne toperator.  Do not use.
      */
    srec_input_file_spasm &operator=(const srec_input_file_spasm &);
};

#endif // SRECORD_INPUT_FILE_SPASM_H
