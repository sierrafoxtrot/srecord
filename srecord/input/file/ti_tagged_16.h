//
// srecord - manipulate eprom load files
// Copyright (C) 2007, 2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_TI_TAGGED_16_H
#define SRECORD_INPUT_FILE_TI_TAGGED_16_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_ti_tagged_16 class is used to represent the
  * parse state of an input file in Texas Instruments SDSMAC 320 format.
  */
class input_file_ti_tagged_16:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_ti_tagged_16() override = default;

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

    /**
      * The get_char method is used to get a character from the input.
      * We override because the checksum is character based, not byte
      * based.
      */
    int get_char() override;

    // See base class for documentation.
    int format_option_number() const override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_ti_tagged_16(const std::string &file_name);

    typedef input_file inherited;

    /**
      * The read_inner method is used to read a single line of input.
      * the read method is a wrapper around this method.
      */
    bool read_inner(record &);

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file.
      */
    unsigned long address{0};

    /**
      * The csum instance variable is used to remember the running
      * checksum.
      */
    int csum{0};

public:
    /**
      * The default constructor.
      */
    input_file_ti_tagged_16() = delete;

    /**
      * The copy constructor.
      */
    input_file_ti_tagged_16(const input_file_ti_tagged_16 &) = delete;

    /**
      * The assignment operator.
      */
    input_file_ti_tagged_16 &operator=(
        const input_file_ti_tagged_16 &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_TI_TAGGED_16_H
