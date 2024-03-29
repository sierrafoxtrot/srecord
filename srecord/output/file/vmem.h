//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_VMEM_H
#define SRECORD_OUTPUT_FILE_VMEM_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_vmem class is used to write a file in
  * Verilog VMEM format, to be loaded using the $readmemh() call.
  */
class output_file_vmem:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_vmem() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_vmem(const std::string &file_name);

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
    void command_line(arglex_tool *cmdln) override;

    // See base class for documentation.
    const char *format_name() const override;

private:
    /**
      * The bytes_per_word instance variable is used to remember how
      * many bytes go into each word of data.
      */
    unsigned bytes_per_word;

    /**
      * The address instance variable is used to remember where we are
      * up to in the output.
      */
    uint32_t address;

    /**
      * The column instance variable is used to remember which column
      * we are up to in the output.  Used to limit the length of lines
      * in the output.
      */
    int column;

    /**
      * The pref_block_size instance variable is used to remember the
      * number of bytes in the preferred block size.
      */
    int pref_block_size;

    /**
      * The width_shift instance variable is used to remember the number
      * of bits to shift addresses when emitting them.  This is based
      * on the memory width passed to the constructor.
      */
    unsigned width_shift;

    /**
      * The width_mask instance variable is used to remember the lower
      * bits to the right of the addresses shift.  This is most frequently
      * used to determine the boundaries of memory-width chunks of bytes.
      * This value is simply a pre-calculation of ((1u << width_shift) - 1u).
      */
    unsigned width_mask;

public:
    /**
      * The default constructor.
      */
    output_file_vmem() = delete;

    /**
      * The copy constructor.
      */
    output_file_vmem(const output_file_vmem &) = delete;

    /**
      * The assignment operator.
      */
    output_file_vmem &operator=(const output_file_vmem &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_VMEM_H
