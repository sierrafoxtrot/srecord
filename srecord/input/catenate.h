//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008, 2010, 2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_CATENATE_H
#define SRECORD_INPUT_CATENATE_H

#include <srecord/input.h>

namespace srecord {

/**
  * The srecord::input_catenate class is used to represent an input source
  * which presents two input sources as if they were a single input
  * source.
  */
class input_catenate:
    public input
{
public:
    /**
      * The destructor.
      */
    ~input_catenate() override = default;

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param in1
      *     The first of the inputs to be read.
      * @param in2
      *     The second of the inputs to be read.
      */
    input_catenate(const pointer &in1, const pointer &in2);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param in1
      *     The first of the inputs to be read.
      * @param in2
      *     The second of the inputs to be read.
      */
    static pointer create(const pointer &in1, const pointer &in2);

protected:
    // See base class for documentation.
    bool read(record &record) override;

    // See base class for documentation.
    std::string filename() const override;

    // See base class for documentation.
    std::string filename_and_line() const override;

    // See base class for documentation.
    const char *get_file_format_name() const override;

    // See base class for documentation.
    void disable_checksum_validation() override;

private:
    /**
      * The in1 instance variable is used to remember the first of the
      * inputs to be read.
      */
    pointer in1;

    /**
      * The in2 instance variable is used to remember the second of the
      * inputs to be read.
      */
    pointer in2;

public:
    /**
      * The default constructor.
      */
    input_catenate() = delete;

    /**
      * The copy constructor.
      */
    input_catenate(const input_catenate &) = delete;

    /**
      * The assignment operator.
      */
    input_catenate &operator=(const input_catenate &) = delete;
};

};

#endif // SRECORD_INPUT_CATENATE_H
