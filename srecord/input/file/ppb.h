//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_PPB_H
#define SRECORD_INPUT_FILE_PPB_H

#include <srecord/input/file.h>
#include <srecord/record.h>

namespace srecord {

/**
  * The input_file_hexdump class is used to represent the processing
  * required to read in a Stag Prom Programmer binary file.
  */
class input_file_ppb:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_ppb() override;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be read.
      */
    static auto create(const std::string &filename) -> pointer;

protected:
    // See base class for documentation.
    auto read(class record &rec) -> bool override;

    // See base class for documentation.
    auto get_file_format_name() const -> const char * override;

    // See base class for documentation.
    auto is_binary() const -> bool override;

    // See base class for documentation.
    auto format_option_number() const -> int override;

private:
    /**
      * The constructor.
      *
      * @param filename
      *     The name of the file to be read.
      */
    input_file_ppb(const std::string &filename);

    /**
      * The address instance variable is used to remember the current
      * address of the next data record.  This is set and advanced by
      * the #read method.
      */
    record::address_t address;

    /**
      * The data_seen instance variable is used to remember whether or
      * not any data has been seen in the file.  This is used to issue
      * an error when there is apparently no data in the file, and this
      * helps #guess to figure out the file is not of this type.
      */
    bool data_seen;

    /**
      * The packet_address instance variable is used to remember the
      * address of the first byte in the most recently read packet.
      */
    unsigned long packet_address;

    /**
      * The packet instance variable is used to remember the most recent
      * #packet_length data bytes read from the file in the most recent packet.
      */
    record::data_t packet[65536];

    /**
      * The packet_length instance variable is used to remember the
      * number of data bytes in the #packet array.
      */
    size_t packet_length;

    /**
      * The packet_used instance variable is used to remember how many
      * bytes of the most recently read packet have been consumed.
      */
    size_t packet_used;

    /**
      * The get_packet method is used to read another packet, setting
      * the #packet and #packet_length and #packet_address and
      * #packet_used instance variables.
      */
    auto get_packet() -> bool;

    /**
      * The packet_format_error method is used to issue a #fatal_error
      * message when a packet is malformed.
      */
    void packet_format_error();

    /**
      * The default constructor.  Do not use.
      */
    input_file_ppb() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_ppb(const input_file_ppb &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_file_ppb &) -> input_file_ppb & = delete;
};

};

#endif // SRECORD_INPUT_FILE_PPB_H
