//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2006-2008 Peter Miller
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

#ifndef LIB_SREC_MEMORY_WALKER_CRC16_H
#define LIB_SREC_MEMORY_WALKER_CRC16_H

#include <lib/crc16.h>
#include <lib/srec/memory/walker.h>

/**
  * The srec_memory_walker_crc16 class is used to represent the parse
  * state of a memory walker which calculates a running CRC16 checksum.
  */
class srec_memory_walker_crc16:
    public srec_memory_walker
{
public:
    typedef boost::shared_ptr<srec_memory_walker_crc16> pointer;

    /**
      * The destructror.
      */
    virtual ~srec_memory_walker_crc16();

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * #create class method instead.
      *
      * @param seed_mode
      *     The selector for the initial seed for the calculation
      * @param augment_flag
      *     Whether or not to augment the calculation
      */
    srec_memory_walker_crc16(crc16::seed_mode_t seed_mode, bool augment_flag);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param seed_mode
      *     The selector for the initial seed for the calculation
      * @param augment_flag
      *     Whether or not to augment the calculation
      */
    static pointer create(crc16::seed_mode_t seed_mode, bool augment_flag);

    /**
      * The get method is used to get the CRC16 checksum once all memory
      * chunks have been processed by calls to our observe method.
      */
    unsigned get() const;

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int);

private:
    /**
      * The checksum instance variable is used to remember the running
      * state of the CRC16 checksum calculation.
      */
    crc16 *checksum;

    /**
      * The copy constructor.  No not use.
      */
    srec_memory_walker_crc16(const srec_memory_walker_crc16 &);

    /**
      * The assignment operator.  No not use.
      */
    srec_memory_walker_crc16 &operator=(const srec_memory_walker_crc16 &);
};

#endif // LIB_SREC_MEMORY_WALKER_CRC16_H
