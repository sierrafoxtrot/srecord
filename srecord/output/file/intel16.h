//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_INTEL16_H
#define SRECORD_OUTPUT_FILE_INTEL16_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_intel16 class is used to write an EPROM load file
  * in INHX16 format.
  */
class output_file_intel16:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_intel16() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_intel16(const std::string &file_name);

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
      * The write_inner method is used to write a single line to the
      * output file.
      */
    void write_inner(int, uint32_t, const void *, int);

    /**
      * The address_base instance variable is used to remember the
      * current position within the output file
      */
    uint32_t address_base{0};

    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of bytes on each line of the output file.
      */
    int pref_block_size{32};

public:
    /**
      * The default constructor.
      */
    output_file_intel16() = delete;

    /**
      * The copy constructor.
      */
    output_file_intel16(const output_file_intel16 &) = delete;

    /**
      * The assignment operator.
      */
    output_file_intel16 &operator=(const output_file_intel16 &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_INTEL16_H
