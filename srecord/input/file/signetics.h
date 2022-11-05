//
// srecord - manipulate eprom load files
// Copyright (C) 2001-2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_SIGNETICS_H
#define SRECORD_INPUT_FILE_SIGNETICS_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_signetics class is used to represent reading a
  * Signetics format file.
  */
class input_file_signetics:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_signetics() override;

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
    auto format_option_number() const -> int override;

    /**
      * See base class for documentation.  We over-ride this method
      * because signetics uses its own XOR-ROL checksum algorithm.
      */
    void checksum_add(unsigned char) override;

private:
    /**
      * The constructor.  The input will be read from the named file
      * (or the standard input if the filename is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_signetics(const std::string &a_filename);

    /**
      * The default constructor.  Do not use.
      */
    input_file_signetics() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_signetics(const input_file_signetics &) = delete;

    /**
      * The assignment operator.  Do not use.
      */
    auto operator=(const input_file_signetics &) -> input_file_signetics & = delete;

    /**
      * The read_inner method is used to read one line/record from
      * the input.  Used by the `read' method.
      */
    auto read_inner(record &) -> bool;

    /**
      * The garbage_warning instance variable is used to remember
      * whether or not a warning has been issued about non-format
      * lines in the file.  Only one warning per file is issued.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to
      * remember whether any input has been seen.
      */
    bool seen_some_input;
};

};

#endif // SRECORD_INPUT_FILE_SIGNETICS_H
