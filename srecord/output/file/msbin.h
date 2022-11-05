//
// srecord - manipulate eprom load files
// Copyright (C) 2009-2011 Peter Miller
//
// Code contribution by David Kozub <zub@linux.fjfi.cvut.cz>
// Copyright assigned to Peter Miller 27-Jan-2010.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_MSBIN_H
#define SRECORD_OUTPUT_FILE_MSBIN_H

#include <cstdint>
#include <srecord/output/file.h>
#include <srecord/record.h>
#include <vector>

namespace srecord
{

/**
  * The srecord::output_file_msbin class is used to represent an output
  * file in Windows CE Binary Image Data Format.
  *
  * See http://msdn.microsoft.com/en-us/library/ms924510.aspx
  * for a description of the format.
  */
class output_file_msbin:
    public output_file
{
public:
    /**
      * The destructor.
      */
    ~output_file_msbin() override;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open and write output to.
      */
    output_file_msbin(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
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

    // See base class for documentation.
    void notify_upper_bound(unsigned long addr) override;

    // See base class for documentation.
    auto is_binary() const -> bool override;

private:
    /**
      * MAX_PENDING_DATA_SIZE is the upper limit of the size of data
      * we are willing to keep in the pending_records vector.
      * (50MiB)
      */
    enum { MAX_PENDING_DATA_SIZE = 50*1024*1024 };

    /**
      * The write_dword_le method is used to write a little endian
      * double word into the output.
      *
      * @param d
      *     The data to be written.
      */
    void write_dword_le(uint32_t d);

    /**
      * The checksum method is used to calculate the checksum of a given
      * data block.
      *
      * The checksum is additive, so:
      *  checksum([Block1,Block2]) = checksum(Block1) + checksum(Block2)
      *
      * @param data
      *     The data to be check-summed.
      * @param len
      *     The length of the data to be check-summed.
      */
    static auto checksum(const unsigned char *data, size_t len) -> uint32_t;

    /**
      * The write_file_header method is used to write the file header
      * (including the magic bytes) into the output.
      *
      * @param start
      *     The least address in the file.
      * @param length
      *     The span of addresses in the file.
      */
    void write_file_header(uint32_t start, uint32_t length);

    /**
      * The write_record_header method is used to write a record header
      * into the output.
      *
      * @param addr
      *     The address of the first by of the record.
      * @param length
      *     The number of bytes of data in the record.
      * @param checksum
      *     The checksum of the data bytes in the record.
      */
    void write_record_header(uint32_t addr, uint32_t length, uint32_t checksum);

    /**
      * The write_data method is used to write the data contained in
      * a record to the output.
      *
      * @param r
      *     The record containing the data to be written to output.
      */
    void write_data(const record &r);

    /**
      * The flush_pending_records method is used to write out all the
      * data contained in the vector pending_records plus optionally
      * a record r to the output, forming a single record in the output.
      * The vector pending_records is cleared.
      *
      * @param r
      *     Optional pointer to a record that should be treated as if
      *     it's the last element of pending_records vector.
      */
    void flush_pending_records(const record *r = nullptr);

    /**
      * The append_pending_record handles adding a new record. If the
      * new record could form a single continuous record in the output
      * file and this would fit in the MAX_PENDING_DATA_SIZE limit,
      * it's appended to the pending_records vector. Otherwise,
      * pending records are written to the output and depending on the
      * size of the record to be added, it's either copied to the
      * pending_records vector, or written directly to the output.
      *
      * @param r
      *     The record to be added.
      */
    void append_pending_record(const record &r);

    /**
      * The start_address_set instance variable is used to remember
      * whether the #start_address instance variable is valid.
      *
      * Becomes true once we encounter a record of type
      * record::type_execution_start_address.
      */
    bool start_address_set;

    /**
      * The start_address instance variable is used to remember
      * the execution start address, taken from a record of type
      * record::type_execution_start_address.
      */
    record::address_t start_address;

    /**
      * The upper_bound instance variable is used to remember the upper
      * bound address as received via the #notify_upper_bound method.
      */
    record::address_t upper_bound{};

    /**
      * The beginning_of_file instance variable is used to remember
      * that we haven't written the file header yet.
      */
    bool beginning_of_file;

    /**
      * Type holding copies of records.
      */
    using record_vector = std::vector<std::shared_ptr<record>>;

    /**
      * The pending_records instance variable is used to keep pending
      * records before they can be written to the output file.
      * This is needed to implement the concatenation of adjacent
      * records.
      */
    record_vector pending_records;

    /**
      * The default constructor.  Do not use.
      */
    output_file_msbin() = delete;

    /**
      * The copy constructor.  Do not use.
      */
    output_file_msbin(const output_file_msbin &) = delete;

    /**
      * The copy constructor.  Do not use.
      */
    auto operator=(const output_file_msbin &) -> output_file_msbin & = delete;
};

};

#endif // SRECORD_OUTPUT_FILE_MSBIN_H
