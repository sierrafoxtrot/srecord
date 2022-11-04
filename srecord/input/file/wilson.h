//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_WILSON_H
#define SRECORD_INPUT_FILE_WILSON_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_wilson class is used to represent the parse state
  * of an input file in Wilson format.  (Named after the person who asked
  * for it; I still don't have an official definition of this format,
  * but it is very much like a binary(ish) form of the Motorola format.)
  */
class input_file_wilson:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_wilson() override;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static auto create(const std::string &file_name) -> pointer;

protected:
    // See base class for documentation.
    auto read(record &record) -> bool override;

    // See base class for documentation.
    auto get_file_format_name() const -> const char * override;

    // See base class for documentation.
    auto is_binary() const -> bool override;

    // See base class for documentation.
    auto format_option_number() const -> int override;

    /**
      * The get_byte method is used to get a byte of input.  We override
      * because the byte encoding is one character of input sometimes,
      * and two characters of input at other times.
      */
    auto get_byte() -> int override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_wilson(const std::string &file_name);


    /**
      * The read_inner method is used to read a record from the input.
      * The #read method is a wrapper around this method.
      */
    auto read_inner(record &) -> bool;

    /**
      * The garbage_warning instance variable is used to remember whether
      * a warning about garbage input has been issued yet.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * this file contains any data so far.
      */
    bool seen_some_input;

    /**
      * The termination_seen instance variable is used to remember
      * whether the termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The default constructor.  Do not use.
      */
    input_file_wilson() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_wilson(const input_file_wilson &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_file_wilson &) -> input_file_wilson & = delete;
};

};

#endif // SRECORD_INPUT_FILE_WILSON_H
