//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2003, 2005-2008 Peter Miller
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

#ifndef INCLUDE_SREC_INPUT_FILTER_CRC32_H
#define INCLUDE_SREC_INPUT_FILTER_CRC32_H

#include <lib/crc32.h>
#include <lib/srec/input/filter.h>
#include <lib/srec/memory.h>

/**
  * The srec_input_filter_crc32 class is used to represent the state of
  * a checksum filter that inserts a CRC32 into the data.
  */
class srec_input_filter_crc32:
    public srec_input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_crc32();

private:
    /**
      * The constructor.
      */
    srec_input_filter_crc32(const srec_input::pointer &deeper,
        unsigned long address, int order);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      */
    static pointer create(const srec_input::pointer &deeper,
        unsigned long address, int order);

protected:
    // See base class for documentation.
    virtual int read(srec_record &);

    // See base class for documentation.
    void command_line(srec_arglex *cmdln);

private:
    /**
      * The address instance variable is used to remember where to place
      * the CRC in memory.
      */
    unsigned long address;

    /**
      * The order instance variable is used to remember whether the byte
      * order is big-endian (0) or little-endian (1).
      */
    int order;

    /**
      * The buffer instance variable is used to remember the contents
      * of the deeper file.  The deeper file must be read completely in
      * order to calculate the CRC, and the input may be out of address
      * order, necessitating this buffer.
      */
    srec_memory buffer;

    /**
      * The buffer_pos instance variable is used to remember where we
      * are up to in processing 'buffer'.
      */
    unsigned long buffer_pos;

    /**
      * The have_forwarded_header instance variable is used to remember
      * whether we have returned the file header to our reader yet.
      */
    bool have_forwarded_header;

    /**
      * The have_given_crc instance variable is used to remember
      * whether we have returned the CRC to our reader yet.
      */
    bool have_given_crc;

    /**
      * The have_forwarded_start_address instance variable is used
      * to remember whether we have returned the start address to our
      * reader yet.
      */
    bool have_forwarded_start_address;

    /**
      * The seed_mode instance variable is used to remember what the
      * user wants for the initial seed.  The default is to use the
      * standard CCITT seed.
      */
    crc32::seed_mode_t seed_mode;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_crc32();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_crc32(const srec_input_filter_crc32 &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_crc32 &operator=(const srec_input_filter_crc32 &);
};

#endif // INCLUDE_SREC_INPUT_FILTER_CRC32_H
