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
    ~input_file_ppb() override = default;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be read.
      */
    static pointer create(const std::string &filename);

protected:
    // See base class for documentation.
    bool read(class record &rec) override;

    // See base class for documentation.
    const char *get_file_format_name() const override;

    // See base class for documentation.
    bool is_binary() const override;

    // See base class for documentation.
    int format_option_number() const override;

private:
    /**
      * The constructor.
      *
      * @param filename
      *     The name of the file to be read.
      */
    input_file_ppb(const std::string &filename);

    /**
      * The packet_address instance variable is used to remember the
      * address of the first byte in the most recently read packet.
      */
    uint32_t packet_address{-1U};

    /**
      * The packet instance variable is used to remember the most recent
      * #packet_length data bytes read from the file in the most recent packet.
      */
    record::data_t packet[65536]{};

    /**
      * The packet_length instance variable is used to remember the
      * number of data bytes in the #packet array.
      */
    size_t packet_length{0};

    /**
      * The packet_used instance variable is used to remember how many
      * bytes of the most recently read packet have been consumed.
      */
    size_t packet_used{0};

    /**
      * The get_packet method is used to read another packet, setting
      * the #packet and #packet_length and #packet_address and
      * #packet_used instance variables.
      */
    bool get_packet();

    /**
      * The packet_format_error method is used to issue a #fatal_error
      * message when a packet is malformed.
      */
    void packet_format_error();

public:
    /**
      * The default constructor.
      */
    input_file_ppb() = delete;

    /**
      * The copy constructor.
      */
    input_file_ppb(const input_file_ppb &) = delete;

    /**
      * The assignment operator.
      */
    input_file_ppb &operator=(const input_file_ppb &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_PPB_H
