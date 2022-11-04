//
// srecord - manipulate eprom load files
// Copyright (C) 2019 fenugrec
// based on motorola.h
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

#ifndef LIB_INPUT_FILE_HP64K
#define LIB_INPUT_FILE_HP64K

#include <srecord/input/file.h>

namespace srecord {

/**
 * The srecord::input_file_hp64k class is used to represent the parse
 * state of a HP64000 Absolute formatted input file.
 */
class input_file_hp64k:
    public input_file
{
public:
    /**
     * The destructor.
     */
    ~input_file_hp64k() override;

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
    void command_line(arglex_tool *cmdln) override;

    // See base class for documentation.
    auto format_option_number() const -> int override;

private:
    /**
     * The constructor.
     *
     * @param file_name
     *     The name of the file to be read.
     */
    input_file_hp64k(const std::string &file_name);

    // See base class for documentation.
    auto is_binary() const -> bool override;

    /**
     * Number of parsed data records
     */
    unsigned long rec_count;

    /** Helper function: read two bytes, big-endian. Ret 1 if ok */
    auto read_u16be(uint16_t *dest) -> bool;

    /** Read one data record. */
    auto read_datarec(record &) -> bool;

    /** Read file magic and header */
    auto read_hdr(record &) -> bool;

    /** Read Processor Information Record (PIR) */
    auto read_pir(record &) -> bool;

    /**
     * Parsing state. Files always have header, then PIR, then data records.
     */
    enum { need_hdr, need_pir, data } state;

    /**
     * The default constructor.  Do not use.
     */
    input_file_hp64k() = delete;

    /**
     * The copy constructor.  Do not use.
     */
    input_file_hp64k(const input_file_hp64k &) = delete;

    /**
     * The assignment operator.  Do not use.
     */
    auto operator=(const input_file_hp64k &) -> input_file_hp64k & = delete;
};

};

#endif // LIB_INPUT_FILE_HP64K
