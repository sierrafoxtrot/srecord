//
// srecord - manipulate eprom load files
// Copyright (C) 2004, 2006-2008, 2010-2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_AOMF_H
#define SRECORD_INPUT_FILE_AOMF_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_aomf class is used to parse Intel Absolute Object
  * Module Format (AOMF) file.
  */
class input_file_aomf:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_aomf() override;

private:
    /**
      * The constructor.  The input is read from the named file (or
      * the standard input if the file name is "-"). It is private on
      * purpose, use the "create" class method instead.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    explicit input_file_aomf(const std::string &file_name);

public:
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
    bool is_binary() const override;

    // See base class for documentation.
    int format_option_number() const override;

private:
    /**
      * The get_byte method is used to fetch a byte of input, and
      * update the checksum.  We over-ride the base implementation,
      * because we use raw bytes rather than two hex digits.
      */
    int get_byte() override;

    /**
      * The current_buffer instance variable is used to remember the
      * base of an array which buffers the current input record.
      */
    uint8_t *current_buffer{nullptr};

    /**
      * The current_length instance variable is used to remember
      * the length of the current record.  It is zero if there is no
      * "current" record.
      */
    size_t current_length{0};

    /**
      * The current_maximum instance variable is used to remember the
      * length of the current_buffer array.  It is zero if there is no
      * "current" buffer.
      */
    size_t current_maximum{0};

    /**
      * The current_pos instance variable is used to remember the
      * position within the current_buffer array.
      */
    size_t current_pos{0};

    /**
      * The current_address instance variable is used to track the
      * load address of the current record.  It is updated each time
      * we return a partial block, so that we always return the
      * correct load address.
      */
    uint32_t current_address{0};

    enum state_t
    {
        expecting_header,
        expecting_data,
        expecting_eof
    };

    /**
      * The state instance variable is used to remember what to expect
      * next from the file.
      */
    state_t state{expecting_header};

    /**
      * The slurp method is used to fill the current_buffer array,
      * and set the current_length.
      */
    int slurp();

public:
    /**
      * The copy constructor.
      */
    input_file_aomf(const input_file_aomf &) = delete;

    /**
      * The assignment operator.
      */
    input_file_aomf &operator=(const input_file_aomf &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_AOMF_H
