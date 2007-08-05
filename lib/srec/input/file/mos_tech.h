//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2003, 2006, 2007 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILE_MOS_TECH_H
#define INCLUDE_SREC_INPUT_FILE_MOS_TECH_H


#include <lib/srec/input/file.h>

/**
  * The mos_tech class is used to parse a Mos Technologies format file.
  */
class srec_input_file_mos_tech:
    public srec_input_file
{
public:
    /**
      * A constructor.  The input is read from the named file (or
      * the standard input if the file anme is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    srec_input_file_mos_tech(const string &file_name);

    /**
      * The destructor.
      */
    virtual ~srec_input_file_mos_tech();

    // See base class for documentation.
    int read(srec_record &);

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * Read one line (record) of input.  Called by the read() method.
      * Returns false at end of file.
      */
    int read_inner(srec_record &);

    /**
      * The garbage_warning instance variable is used to remember
      * whther or not a warning has been issued about non-format
      * lines in the file.  Only one warning per file is issued.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to
      * remember whether any input has been seen.
      */
    bool seen_some_input;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_file_mos_tech();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_file_mos_tech(const srec_input_file_mos_tech &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_file_mos_tech &operator=(const srec_input_file_mos_tech &);
};

#endif // INCLUDE_SREC_INPUT_FILE_MOS_TECH_H
