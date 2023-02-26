//
// srecord - Manipulate EPROM load files
// Copyright (C) 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILTER_H
#define SRECORD_OUTPUT_FILTER_H

#include <srecord/output.h>

namespace srecord {

/**
  * The output_filter class is used to represent a generic output
  * manipulation.  Usually the major manipulations are done on input,
  * these filters are output-specific.
  */
class output_filter:
    public output
{
public:
    /**
      * The destructor.
      */
    ~output_filter() override = default;

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param deeper
      *     Where to send our filtered output.
      */
    output_filter(const output::pointer &deeper);

    // See base class for documentation.
    void write(const record &r) override;

    // See base class for documentation.
    void line_length_set(int) override;

    // See base class for documentation.
    void address_length_set(int) override;

    // See base class for documentation.
    int preferred_block_size_get() const override;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes) override;

    // See base class for documentation.
    std::string filename() const override;

    // See base class for documentation.
    const char *format_name() const override;

    // See base class for documentation.
    void notify_upper_bound(uint32_t addr) override;

    // See base class for documentation.
    void command_line(arglex_tool *cmdln) override;

private:
    /**
      * The deeper instance variable is used to remember where to send
      * our filtered output.
      */
    output::pointer deeper;

public:
    /**
      * The default constructor.
      */
    output_filter() = delete;

    /**
      * The copy constructor.
      */
    output_filter(const output_filter &) = delete;

    /**
      * The assignment operator.
      */
    output_filter &operator=(const output_filter &) = delete;
};

};

#endif // SRECORD_OUTPUT_FILTER_H
