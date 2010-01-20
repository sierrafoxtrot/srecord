//
//      srecord - manipulate eprom load files
//      Copyright (C) 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_NEEDHAM_H
#define SRECORD_INPUT_FILE_NEEDHAM_H

#include <srecord/input/file.h>

/**
  * The srec_input_file_needham class is used to represent the parse
  * state of a file in Needham Hex format.
  */
class srec_input_file_needham:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_needham();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_needham(const std::string &file_name);

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
    // See base class for documentation.
    bool read(srec_record &record);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    int read_inner(srec_record &);
    bool garbage_warning;
    bool seen_some_input;
    unsigned long address;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_needham();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_needham(const srec_input_file_needham &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_needham &operator=(const srec_input_file_needham &);
};

#endif // SRECORD_INPUT_FILE_NEEDHAM_H
