//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998-2003, 2005-2008 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILTER_H
#define INCLUDE_SREC_INPUT_FILTER_H


#include <lib/srec/input.h>

/**
  * The input filter class is an abstract interface for all of the
  * various filters that can be applied to an incoming EPROM file.
  *
  * Each filter is-a input, but each also has-a deeper input, from which
  * is obtained the data to be filtered.
  */
class srec_input_filter:
    public srec_input
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter();

    // See base class for documentation.
    virtual int read(class srec_record &);

    // See base class for documentation.
    virtual string filename() const;

    // See base class for documentation.
    virtual string filename_and_line() const;

    // See base class for documentation.
    virtual const char *get_file_format_name() const;

    // See base class for documentation.
    void disable_checksum_validation();

protected:
    /**
      * The constructor.  Only derived classes may call.
      *
      * \param deeper
      *     The deeper input to be filtered.
      */
    srec_input_filter(srec_input::pointer deeper);

    //
    // I'd like this to be private, but srec_input_filter_crc16 and
    // srec_input_filter_crc32 need to access it directly, rather than
    // going through the srec_input_filter::read method.
    //
    srec_input::pointer ifp;

private:
    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter(const srec_input_filter &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter &operator=(const srec_input_filter &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_H
