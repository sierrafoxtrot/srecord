//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_EMON52_H
#define SRECORD_INPUT_FILE_EMON52_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_emon52 class is used to parse an EMON52
  * formatted file (Elektor Monitor, dunno what the 52 is for).
  */
class input_file_emon52:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_emon52() override = default;

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

private:
    /**
      * A constructor.  The input is read from the named file (or
      * the standard input if the file name is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_emon52(const std::string &file_name);

    /**
      * The skip_white_space method is used to skip space characters.
      * The format requires spaces in some locations, this method
      * skips the space of present.
      */
    void skip_white_space();

public:
    /**
      * The default constructor.
      */
    input_file_emon52() = delete;

    /**
      * The copy constructor.
      */
    input_file_emon52(const input_file_emon52 &) = delete;

    /**
      * The assignment operator.
      */
    input_file_emon52 &operator=(const input_file_emon52 &) = delete;
};

};

#endif // SRECORD_INPUT_FILE_EMON52_H
