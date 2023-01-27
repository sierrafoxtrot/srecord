//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_NEEDHAM_H
#define SRECORD_OUTPUT_FILE_NEEDHAM_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_needham class is used to represent
  */
class output_file_needham:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_needham() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_needham(const std::string &file_name);

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

private:
    /**
      * The address instance variable is used to remember where in the
      * file the output has reached.  This is used to fill gaps.
      */
    unsigned long address{0};

    /**
      * The column instance variable is used to remember the column of
      * the output text we have reached.  This is used when calculating
      * when to throw new lines.
      */
    int column{0};

    /**
      * The pref_block_size instance variable is used to remember The
      * preferred line length of the output text.
      */
    int pref_block_size{16};

    /**
      * The address_length instance variable is used to remember the
      * minimum number of bytes to use for addresses in the output text.
      */
    int address_length{4};

public:
    /**
      * The default constructor.
      */
    output_file_needham() = delete;

    /**
      * The copy constructor.
      */
    output_file_needham(const output_file_needham &) = delete;

    /**
      * The assignment operator.
      */
    output_file_needham &operator=(const output_file_needham &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_NEEDHAM_H
