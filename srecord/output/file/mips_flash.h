//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_MIPS_FLASH_H
#define SRECORD_OUTPUT_FILE_MIPS_FLASH_H

#include <srecord/output/file.h>
#include <srecord/endian.h>

namespace srecord {

/**
  * The output_file_mips_flash class is used to write a MIPS-Flash
  * formatted file.
  */
class output_file_mips_flash:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_mips_flash() override;

    /**
      * The create_le class method is used to create new dynamically
      * allocated instances of this class, using little-endian byte
      * ordering.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create_le(const std::string &file_name);

    /**
      * The create_be class method is used to create new dynamically
      * allocated instances of this class, using big-endian byte
      * ordering.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create_be(const std::string &file_name);

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
      * The constructor.  It is private on purpose, use the #create_le
      * or #create_be class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      * @param endian
      *     The byte order to expect.
      */
    output_file_mips_flash(const std::string &file_name, endian_t endian);

    /**
      * The write_inner method is used to write a single line (record)
      * to the file.  Use by the write() method.
      */
    void write_inner(int type, uint32_t addr, int addr_len,
        const void *data, int data_len);

    /**
      * The endian instance variable is used to remember whether this
      * file is big-endian or little-endian.
      */
    endian_t endian;

    /**
      * The address instance variable is used to remember the address at
      * which the next byte is to be placed.
      */
    uint32_t address{0};

    /**
      * The base instance variable is used to remember the base address
      * of the current flash segment.
      */
    uint32_t base{0};

    /**
      * The base_set instance variable is used to remember whether or
      * not the #base instance variable has been set yet.  This also
      * implies no data has been seen yet.
      */
    bool base_set{false};

    /**
      * The buffer instance variable is used to remember the accumulated
      * data so far.  Must be a multiple of 4 bytes long.
      */
    uint8_t buffer[256]{};

    /**
      * The buffer_length instance variable is used to remember how many
      * bytes of the #buffer array have been consumed to date.
      */
    size_t buffer_length{0};

    /**
      * The buffer_flush method is used to write the #buffer_length
      * bytes of data in the #buffer array to the file.  The
      * #buffer_length has been reset to zero on return.
      */
    void buffer_flush();

    /**
      * The buffer_flush_newline method is used to write the data,
      * using the #buffer_flush method, and then issue a newline if the
      * output is not at the beginning of a new line.
      */
    void buffer_flush_newline();

    /**
      * The line_length instance variable is used to remember the
      * preferred line length for the output, in fixed width character
      * columns.
      */
    int line_length{80};

    /**
      * The column instance variable is used to remember the current
      * output column.  If zero, it means we are positioned at the
      * beginning of a new line of text.
      */
    int column{0};

public:
    /**
      * The default constructor.
      */
    output_file_mips_flash() = delete;

    /**
      * The copy constructor.
      */
    output_file_mips_flash(const output_file_mips_flash &) = delete;

    /**
      * The assignment operator.
      */
    output_file_mips_flash &operator=(const output_file_mips_flash &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_MIPS_FLASH_H
