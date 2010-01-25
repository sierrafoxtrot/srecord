//
// srecord - manipulate eprom load files
// Copyright (C) 2009 David Kozub <zub@linux.fjfi.cvut.cz>
// Copyright (C) 2009, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/output/file/msbin.h>


srecord::output_file_msbin::~output_file_msbin()
{
    if (start_address_set && enable_goto_addr_flag)
    {
        write_record_header(0, start_address, 0);
    }
    else
    {
        warning
        (
            "no execution start address record present, although "
            "it's required by format specification"
        );
    }
}


srecord::output_file_msbin::output_file_msbin(
        const std::string &a_file_name) :
    srecord::output_file(a_file_name),
    start_address_set(false),
    start_address(0),
    beginning_of_file(true)
#ifdef MSBIN_CONCATENATE_ADJACENT_RECORDS
    ,current_addr(0)
#endif
{
    if (line_termination == line_termination_native)
        line_termination = line_termination_binary;
}


srecord::output::pointer
srecord::output_file_msbin::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_msbin(a_file_name));
}


void
srecord::output_file_msbin::write_qword_le(uint32_t d)
{
    unsigned char c[sizeof(uint32_t)];

    srecord::record::encode_little_endian(c, d, sizeof(c));

    for (size_t i = 0; i < sizeof(c); ++i)
        put_char(c[i]);
}


uint32_t
srecord::output_file_msbin::checksum(const unsigned char *data, size_t len)
{
    uint32_t sum = 0;

    for (size_t i = 0; i < len; ++i)
        sum += data[i];

    return sum;
}


void
srecord::output_file_msbin::write_file_header(uint32_t start, uint32_t length)
{
    // Write magic
    static const unsigned char Magic[7] =
        { 'B', '0', '0', '0', 'F', 'F', '\n' };
    for (size_t i = 0; i < sizeof(Magic); ++i)
        put_char(Magic[i]);

    // Write header itself
    write_qword_le(start);
    write_qword_le(length);
}


void
srecord::output_file_msbin::write_record_header(uint32_t addr, uint32_t length,
    uint32_t checksum)
{
    write_qword_le(addr);
    write_qword_le(length);
    write_qword_le(checksum);
}


void
srecord::output_file_msbin::notify_upper_bound(unsigned long addr)
{
    upper_bound = addr;
}


void
srecord::output_file_msbin::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_execution_start_address:
        // Just note down the execution start address. It must be
        // the last record in the MsBin format.
        start_address = record.get_address();
        start_address_set = true;
        break;

    case srecord::record::type_data:
        {
            // Write data.
            if (beginning_of_file)
            {
                // Write file header
                const unsigned long start = record.get_address();
                const unsigned long length = upper_bound - start;
                write_file_header(start, length);
                beginning_of_file = false;
            }

            assert(record.get_length() == 0 ||
                (record.get_address() + record.get_length() <= upper_bound));

#ifdef MSBIN_CONCATENATE_ADJACENT_RECORDS
            if (current_addr != record.get_address())
#endif
            {
                if (record.get_address() == 0)
                {
                    fatal_error
                    (
                        "the MsBin format cannot express a data value at "
                        "the address 0"
                    );
                }
                write_record_header
                (
                    record.get_address(),
                    record.get_length(),
                    checksum(record.get_data(), record.get_length())
                );
#ifdef MSBIN_CONCATENATE_ADJACENT_RECORDS
                current_addr = record.get_address();
#endif
            }

            const unsigned char *data = record.get_data();
            size_t length = record.get_length();
            while (length-- > 0)
                put_char(*data++);

#ifdef MSBIN_CONCATENATE_ADJACENT_RECORDS
            current_addr += record.get_length();
#endif
        }
        break;

    case srecord::record::type_header:
    case srecord::record::type_data_count:
    case srecord::record::type_unknown:
        // Ignore.
        break;
    }
}


void
srecord::output_file_msbin::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srecord::output_file_msbin::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


int
srecord::output_file_msbin::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_msbin::format_name()
    const
{
    return "Windows CE Binary Image Data Format";
}
