//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2003, 2006, 2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//      02110-1301 USA.
//
// MANIFEST: interface definition for lib/srec/input/file/tektronix_extended.cc
//

#ifndef INCLUDE_SREC_INPUT_FILE_TEKTRONIX_EXTENDED_H
#define INCLUDE_SREC_INPUT_FILE_TEKTRONIX_EXTENDED_H


#include <lib/srec/input/file.h>

/**
  * The srec_input_file_tektronix_extended clas sis used to represent
  * the parse state of an input file in Tektronix Extended format.
  */
class srec_input_file_tektronix_extended:
    public srec_input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_file_tektronix_extended();

    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_tektronix_extended(const string &file_name);

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The data_record_count instance variable is used to remember the
      * number of data records seen in the input so far.
      */
    int data_record_count;

    /**
      * The read_inner method is used to read a record of input.
      * The read method is a wrapper around this method.
      */
    int read_inner(srec_record &);

    /**
      * The garbage_warning instance variable is used to remember
      * whether or not a warning about garbage line in the input has
      * been issued yet.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * any data has been seen in this file yet.
      */
    bool seen_some_input;

    /**
      * The header_seen instance variable is used to remember whether
      * the header record has been seen yet.
      */
    bool header_seen;

    /**
      * The termination_seen instance variable is used to remember
      * whether the termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_tektronix_extended();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_tektronix_extended(
        const srec_input_file_tektronix_extended &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_tektronix_extended &operator=(
        const srec_input_file_tektronix_extended &);
};

#endif // INCLUDE_SREC_INPUT_FILE_TEKTRONIX_EXTENDED_H
