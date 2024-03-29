//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILTER_MESSAGE_FLETCHER16_H
#define SRECORD_INPUT_FILTER_MESSAGE_FLETCHER16_H

#include <srecord/fletcher16.h>
#include <srecord/endian.h>
#include <srecord/input/filter/message.h>

namespace srecord
{

/**
  * The srecord::input_filter_message_fletcher16 class is used to represent
  * the state of a checksum filter that inserts an Fletcher 16 checksum
  * into the data.
  */
class input_filter_message_fletcher16:
    public input_filter_message
{
public:
    /**
      * The destructor.
      */
    ~input_filter_message_fletcher16() override = default;

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param address
      *     where to place the checksum
      * @param end
      *     The byte order.
      */
    input_filter_message_fletcher16(const input::pointer &deeper,
        uint32_t address, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param address
      *     where to place the checksum
      * @param end
      *     The byte order.
      */
    static pointer create(const input::pointer &deeper, uint32_t address,
        endian_t end);

protected:
    // See base class for documentation.
    void process(const memory &input, record &output) override;

    // See base class for documentation.
    const char *get_algorithm_name() const override;

    // See base class for documentation.
    void command_line(srecord::arglex_tool *cmdln) override;

private:
    /**
      * The address instance variable is used to remember where to place
      * the Fletcher 16 checksum in memory.
      */
    uint32_t address;

    /**
      * The end instance variable is used to remember whether the byte
      * order is big-endian or little-endian.
      */
    endian_t end;

    uint8_t sum1{0xFF};

    uint8_t sum2{0xFF};

    int answer{-1};

public:
    /**
      * The default constructor.
      */
    input_filter_message_fletcher16() = delete;

    /**
      * The copy constructor.
      */
    input_filter_message_fletcher16(
        const input_filter_message_fletcher16 &) = delete;

    /**
      * The assignment operator.
      */
    input_filter_message_fletcher16 &operator=(
        const input_filter_message_fletcher16 &) = delete;
};

};

#endif // SRECORD_INPUT_FILTER_MESSAGE_FLETCHER16_H
