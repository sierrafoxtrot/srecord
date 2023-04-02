/**
 * srecord - manipulate eprom load files
 * Copyright (C) 2002, 2006-2010 Peter Miller
 *
 * Ohio Scientific 65A input is:
 *
 * Copyright (c) 2023 Glitch Works, LLC
 * http://www.glitchwrks.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRECORD_INPUT_FILE_OS65A_H
#define SRECORD_INPUT_FILE_OS65A_H


#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_os65a class is used to represent the input state
  * of an Ohio Scientific 65A formatted file.
  */
class input_file_os65a:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_os65a() override = default;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    bool read(record &record) override;

    // See base class for documentation.
    const char *get_file_format_name() const override;

    // See base class for documentation.
    int format_option_number() const override;

public:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_os65a(const std::string &file_name);

    /**
      * The default constructor.
      */
    input_file_os65a() = delete;

    /**
      * The copy constructor.
      */
    input_file_os65a(const input_file_os65a &) = delete;

    /**
      * The assignment operator.
      */
    input_file_os65a &operator=(const input_file_os65a &) = delete;

    /**
      * The read_inner method is used by the read method to get
      * another portion of input.
      */
    bool read_inner(record &);

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not some valid input has been seen.
      */
    bool seen_some_input{false};

    /**
      * The ignore_the_rest instance variable is used to remember when
      * it is time to ignore the rest of the file.  This happens after a
      * "return to monitor" sequence, or after a "GO" command.
      */
    bool ignore_the_rest{false};

private:
    // Current file location
    uint16_t address;

    // High and low program counter addresses, used with GO
    uint8_t go_address_high;
    uint8_t go_address_low;

    // State tracking for the PC high and low bytes
    bool go_address_high_loaded;
    bool go_address_low_loaded;

    // Processing state
    enum state_t
    {
        state_null,         // NULL initial state
        state_load_address, // Loading memory address
        state_load_data,    // Loading memory data
        state_pch_loaded,   // PC high byte loaded
        state_pcl_loaded,   // PC low byte loaded
        state_command,      // In command mode
        state_go            // GO command emitted
    };

    // Hold our current state, see above.
    state_t state;
};

};

#endif // SRECORD_INPUT_FILE_OS65A_H
