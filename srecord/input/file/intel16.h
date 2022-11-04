//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_INTEL16_H
#define SRECORD_INPUT_FILE_INTEL16_H

#include <srecord/input/file.h>

namespace srecord
{

/**
  * The srecord::input_file_intel16 class is used to represent the parse state
  * of an INHX16 formatted file.
  */
class input_file_intel16:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_intel16() override;

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
    auto read(record &result) -> bool override;

    // See base class for documentation.
    auto get_file_format_name() const -> const char * override;

    // See base class for documentation.
    auto format_option_number() const -> int override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_intel16(const std::string &file_name);

    /**
      * Read one record from the file.  The read method is a wrapper
      * around this method.
      */
    auto read_inner(record &) -> bool;

    /**
      * The data_record_count instance variable is used to remember the
      * number of data records seen to date.
      */
    int data_record_count;

    /**
      * The garbage_warning instance variable is used to remember whether
      * or not a warning has already been issued about garbage lines
      * of input.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not the file contains any data.
      */
    bool seen_some_input;

    /**
      * The termination_seen instance variable is used to remember
      * whether or not a termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The mode instance variable is used to remember what addressing
      * mode the file is currently in.
      */
    enum { linear, segmented } mode;

    /**
      * The address_base instance variable is used to remember the
      * segment base address when in segmented addressing mode.
      */
    unsigned long address_base;

    /**
      * The pushback instance variable is used to remember the previous
      * record in the file.  This is needed in some instances, but not always.
      */
    record *pushback;

    /**
      * The end_seen instance variable is used to remember whether or
      * not the end of file has been seen yet.
      */
    bool end_seen;

    /**
      * The default constructor.  Do not use.
      */
    input_file_intel16() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_intel16(const input_file_intel16 &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_file_intel16 &) -> input_file_intel16 & = delete;
};

};

#endif // SRECORD_INPUT_FILE_INTEL16_H
