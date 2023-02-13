//
// srecord - manipulate eprom load files
// Copyright (C) 2000 Hendrik De Vloed <hendrik.devloed@ugent.be>
// Copyright (C) 2006-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_VHDL_H
#define SRECORD_OUTPUT_FILE_VHDL_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_vhdl class is used to represent the state out
  * an output file in VHDL format.
  */
class output_file_vhdl:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_vhdl() override;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_vhdl(const std::string &file_name);

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
    // See base class for documentation
    void write(const record &) override;

    // See base class for documentation
    void line_length_set(int) override;

    // See base class for documentation
    void address_length_set(int) override;

    // See base class for documentation
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

    // See base class for documentation
    void command_line(arglex_tool *cmdln) override;

    // See base class for documentation.
    const char *format_name() const override;

private:
    /**
      * The bytes_per_word instance variable is used to remember how
      * many bytes to emit per output row.  This is because words are
      * packed into an array of values larger than a single byte.
      */
    unsigned bytes_per_word{1};

    /**
      * The prefix instance variable is used to remember the prefix of
      * the various names emitted into the output.
      */
    std::string prefix{"eprom"};

    /**
      * The header_done instance variable is used to remember whether
      * the emit_header method has already been called.
      */
    bool header_done{false};

    /**
      * The emit_header method is used to emit the file header,
      * if necessary.
      */
    void emit_header();

public:
    /**
      * The default constructor.
      */
    output_file_vhdl() = delete;

    /**
      * The copy constructor.
      */
    output_file_vhdl(const output_file_vhdl &) = delete;

    /**
      * The assignment operator.
      */
    output_file_vhdl &operator=(const output_file_vhdl &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_VHDL_H
