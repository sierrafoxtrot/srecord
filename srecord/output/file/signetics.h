//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2005-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_SIGNETICS_H
#define SRECORD_OUTPUT_FILE_SIGNETICS_H

#include <srecord/output/file.h>

namespace srecord
{

class record; // forward

/**
  * The srecord::output_file_signetics class is used to represent output to
  * a file using the Signetics format.
  */
class output_file_signetics:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_signetics() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_signetics(const std::string &file_name);

public:
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

    /**
      * See base class for documentation.  We over-ride the base
      * implementation because signetics uses its own XOR-ROL algorithm.
      */
    void checksum_add(uint8_t) override;

private:
    /**
      * The pref_block_size is used to remember the preferred block
      * size.  Set by the line_length_set() method.  Read by the
      * preferred_block_size_get() method.
      */
    int pref_block_size{32};

    /**
      * The last_address instance variable is used to remember the
      * address immediately beyond the last address of data in the file.
      * This is used to write the file termination record.
      */
    uint32_t last_address{0};

    /**
      * The write_inner method is used to write one line/record to the
      * output.  It is called by the write() method.
      */
    void write_inner(int, uint32_t, int, const void *, int);

public:
    /**
      * The default constructor.
      */
    output_file_signetics() = delete;

    /**
      * The copy constructor.
      */
    output_file_signetics(const output_file_signetics &) = delete;

    /**
      * The assignment operator.
      */
    output_file_signetics &operator=(const output_file_signetics &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_SIGNETICS_H
