/**
 * srecord - manipulate eprom load files
 * Copyright (C) 2002, 2006-2010 Peter Miller
 *
 * Ohio Scientific 65A output is:
 *
 * Copyright (c) 2022 Glitch Works, LLC
 * http://www.glitchwrks.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRECORD_OUTPUT_FILE_OS65A_H
#define SRECORD_OUTPUT_FILE_OS65A_H

#include <srecord/output/file.h>

namespace srecord
{

/**
 * The srecord::output_file_os65a class is used to represent the file state
 * of an OS65A output file, in OS65A format.
 */
class output_file_os65a:
    public output_file
{
public:
    // The destructor
    virtual ~output_file_os65a();

private:
    /**
     * The constructor. It is private on purpose, use the #create
     * class method instead.
     *
     * @param file_name
     *     The name of the file to be written.  The special name "-"
     *     indicates the standard output is to be used.
     */
    output_file_os65a(const std::string &file_name);

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
    void write(const record &);

    // See base class for documentation
    void line_length_set(int);

    // See base class for documentation
    void address_length_set(int);

    // See base class for documentation
    int preferred_block_size_get() const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation
    const char *mode(void) const;

    // See base class for documentation.
    const char *format_name() const;

private:
    // Preferred bytes per block
    int pref_block_size;

    // Current file location
    unsigned long address;

    // Processing state
    enum state_t
    {
        state_null,     // NULL initial state
        state_load,     // Loading memory
        state_command,  // In command mode
        state_go        // GO command emitted
    };

    // Hold our current state, see above.
    state_t state;

    // The default constructor. Do not use.
    output_file_os65a();

    // Copy constructor. Do not use.
    output_file_os65a(const output_file_os65a &);

    // Assignment operator. Do not use.
    output_file_os65a &operator=(const output_file_os65a &);
};

};

#endif // SRECORD_OUTPUT_FILE_OS65A_H
