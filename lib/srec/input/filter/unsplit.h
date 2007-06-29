//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001, 2002, 2005-2007 Peter Miller
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
// MANIFEST: interface definition for lib/srec/input/filter/unsplit.cc
//

#ifndef INCLUDE_SREC_INPUT_FILTER_UNSPLIT_H
#define INCLUDE_SREC_INPUT_FILTER_UNSPLIT_H


#include <lib/srec/input/filter.h>
#include <lib/srec/record.h>

/**
  * The srec_input_filter_unsplit class is used to represent a filter
  * whoch explosed the deeper input source.  This is the inverse of the
  * srec_input_filter_split class.  This can be used to takes the images
  * of thwo parallel EPROM images and create a unified image when they
  * are interleaved rather than consecutive.
  *
  * @see_also
  *    srec_input_filter_split
  */
class srec_input_filter_unsplit:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_unsplit();

    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input source to be filtered.
      * @param modulus
      *     The number of bytes wide each swathe is.
      * @param offset
      *     The offset within the swathe.
      * @param width
      *     The width of each stripe within the swathe.
      */
    srec_input_filter_unsplit(srec_input *deeper, int modulus, int offset,
        int width);

    // See base class for documentation.
    virtual int read(srec_record &);

private:
    /**
      * The modulus instance variable is used to remember the number of
      * bytes wide each swathe is.
      */
    int modulus;

    /**
      * The offset instance variable is used to remember the offset
      * within the swathe.
      */
    int offset;

    /**
      * The width instance variable is used to remember the width of
      * each stripe within the swathe.
      */
    int width;

    /**
      * The buffer instance variable is used to remember the last lot
      * of data read from the deeper inputs source, and currently being
      * processed.
      */
    srec_record buffer;

    /**
      * The buffer_pos instance variable is used to remember where we
      * are up to in the "buffer" instance varaible.
      */
    int buffer_pos;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_unsplit();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_unsplit(const srec_input_filter_unsplit &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_unsplit &operator=(const srec_input_filter_unsplit &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_UNSPLIT_H
