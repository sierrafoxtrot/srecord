//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_PPB_H
#define SRECORD_OUTPUT_FILE_PPB_H

#include <srecord/output/file.h>

namespace srecord {

/**
  * The output_file_ppb class is used to represent the processing
  * required to write a Stag Prom Programmer binary file.
  */
class output_file_ppb:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_ppb() override;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const record &) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

    // See base class for documentation.
    const char *format_name() const override;

    // See base class for documentation.
    bool is_binary() const override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_ppb(const std::string &file_name);

    /**
      * The address instance variable is used to remember the address of
      * the next data byte to be parsed.
      */
    uint32_t address{-1U};

    /**
      * The buffer instance variable is used to remember the accumulated
      * data bytes to be written to the file.
      */
    uint8_t buffer[8192]{};

    /**
      * The buffer_length instance variable is used to remember how many
      * bytes are valid in the #buffer array.
      */
    unsigned buffer_length{0};

    /**
      * The seen_some_data instance variable is used to remember whether
      * or not any data has been written to the file yet.
      */
    bool seen_some_data{false};

    /**
      * The buffer_flush method is used to write out the #buffer_length
      * bytes in the #buffer instance variable.
      */
    void buffer_flush();

    /**
      * The packet method is used to write out the #buffer as an
      * appropriately constructed packet.
      */
    void packet(uint32_t address, const uint8_t *data,
        size_t data_size);

    /**
      * The put_bin_4be method is used to write out 4 binary bytes of a
      * 32-bit value, big endian ordering.
      */
    void put_bin_4be(uint32_t value);

    /**
      * 8-bit checksum a 4 byte sequence.
      */
    uint8_t sum_ulong(uint32_t value, uint8_t sum);

public:
    /**
      * The default constructor.
      */
    output_file_ppb() = delete;

    /**
      * The copy constructor.
      */
    output_file_ppb(const output_file_ppb &) = delete;

    /**
      * The assignment operator.
      */
    output_file_ppb &operator=(const output_file_ppb &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_PPB_H
