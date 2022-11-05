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

#ifndef SRECORD_OUTPUT_FILE_LOGISIM_H
#define SRECORD_OUTPUT_FILE_LOGISIM_H

#include <srecord/output/file.h>

namespace srecord {

/**
  * The output_file_logisim class is used to represent
  * the processing necessary to write a Logisim ROM file.
  */
class output_file_logisim:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_logisim() override;

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static auto create(const std::string &file_name) -> pointer;

protected:
    // See base class for documentation.
    void write(const record &) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    auto preferred_block_size_get() const -> int override;

    // See base class for documentation.
    auto preferred_block_size_set(int nbytes) -> bool override;

    // See base class for documentation.
    auto format_name() const -> const char * override;

private:
    /**
      * The constructor.
      * Use #create instead.
      */
    output_file_logisim(const std::string &file_name);

    long address;
    int line_length;
    int column;

    void emit(int count, int value);

    /**
      * The default constructor.
      * Do not use.
      */
    output_file_logisim() = delete;

    /**
      * The copy constructor.
      * Do not use.
      */
    output_file_logisim(const output_file_logisim &rhs) = delete;

    /**
      * The assignment operator.
      * Do not use.
      */
    auto operator=(const output_file_logisim &rhs) -> output_file_logisim & = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_LOGISIM_H
