//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILTER_MESSAGE_ADLER32_H
#define SRECORD_INPUT_FILTER_MESSAGE_ADLER32_H

#include <srecord/adler32.h>
#include <srecord/endian.h>
#include <srecord/input/filter/message.h>

/**
  * The srec_input_filter_message_adler32 class is used to represent the
  * state of a checksum filter that inserts an Adler 32 checksum into
  * the data.
  */
class srec_input_filter_message_adler32:
    public srec_input_filter_message
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_message_adler32();

private:
    /**
      * The constructor.
      *
      * @param end
      *     The byte order.
      */
    srec_input_filter_message_adler32(const srec_input::pointer &deeper,
        unsigned long address, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param end
      *     The byte order.
      */
    static pointer create(const srec_input::pointer &deeper,
        unsigned long address, endian_t end);

protected:
    // See base class for documentation.
    void process(const srec_memory &input, srec_record &output);

    // See base class for documentation.
    const char *get_algorithm_name() const;

private:
    /**
      * The address instance variable is used to remember where to place
      * the Adler 32 checksum in memory.
      */
    unsigned long address;

    /**
      * The end instance variable is used to remember whether the byte
      * order is big-endian or little-endian.
      */
    endian_t end;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_message_adler32();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_message_adler32(
        const srec_input_filter_message_adler32 &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_message_adler32 &operator=(
        const srec_input_filter_message_adler32 &);
};

// vim:ts=8:sw=4:et
#endif // SRECORD_INPUT_FILTER_MESSAGE_ADLER32_H
