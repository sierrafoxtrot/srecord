//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_LOGISIM_H
#define SRECORD_INPUT_FILE_LOGISIM_H

#include <cinttypes>
#include <list>

#include <srecord/record.h>
#include <srecord/input/file.h>

namespace srecord {

/**
  * The input_file_logisim class is used to represent
  * the processing required to parse Logisim rom data files.
  *
  *
  * http://ozark.hendrix.edu/~burch/logisim/docs/2.3.0/guide/mem/menu.html
  *
  * Pop-up menus and files
  *
  * The pop-up menu for memory includes four options in addition to the
  * options common to all components:
  *
  * Edit Contents:
  *     Bring up a hex editor for editing the contents of memory.
  * Clear Contents:
  *     Resets all values in memory to 0.
  * Load Image...:
  *     Resets all values in memory based on the values found in a file
  *     using the format described below.
  * Save Image...:
  *     Stores all values in memory into a file using the format
  *     described below.
  *
  * The file format used for image files is intentionally simple; this
  * permits you to write a program, such as an assembler, that generates
  * memory images that can then be loaded into memory. As an example of
  * this file format, if we had a 256-byte memory whose first five bytes
  * were 2, 3, 0, 20, and -1, and all subsequent values were 0, then the
  * image would be the following text file.
  *
  *    v2.0 raw
  *
  *    02
  *    03
  *    00
  *    14
  *    ff
  *
  * The first line identifies the file format used (currently, there is
  * only one file format recognized). Subsequent values list the values
  * in hexadecimal, starting from address 0; you can place several
  * such values on the same line. Logisim will assume that any values
  * unlisted in the file are zero.
  *
  * The image file can use run-length encoding; for example, rather
  * than list the value 00 sixteen times in a row, the file can include
  * 16*00 rather than repeat 00 sixteen times. Notice than the number of
  * repetitions is written in base 10. Files produced by Logisim will
  * use run-length encoding for runs of at least four values
  */
class input_file_logisim:
    public input_file
{
public:
    /**
      * The destructor.
      */
    ~input_file_logisim() override;

    static auto create(const std::string &file_name) -> input_file::pointer;

    enum state_t
    {
        state_line_one,
        state_line_two,
        state_lines_of_data,
    };

protected:
    // See base class for documentation
    auto read(srecord::record &rec) -> bool override;

    // See base class for documentation
    auto get_file_format_name() const -> const char * override;

    // See base class for documentation
    auto format_option_number() const -> int override;

    /**
      * The constructor.
      */
    input_file_logisim(const std::string &file_name);

private:
    srecord::record::address_t address;
    state_t state;

    struct datum_t
    {
        datum_t() : address(0), count(1), value(0xFF) { }
        datum_t(uint32_t aa, long ac, uint32_t av) :
            address(aa), count(ac), value(av) { }
        uint32_t address;
        long count;
        uint32_t value;
        auto representation() const -> std::string;
    };

    std::list<datum_t> jobs;

    auto read_inner(record &rec) -> bool;
    void read_inner_one();
    void read_inner_two();
    void read_inner_job();

    /**
      * The default constructor.
      */
    input_file_logisim();

    /**
      * The copy constructor.
      */
    input_file_logisim(const input_file_logisim &);

    /**
      * The assignment operator.
      */
    auto operator=(const input_file_logisim &) -> input_file_logisim &;
};

};

#endif // SRECORD_INPUT_FILE_LOGISIM_H
