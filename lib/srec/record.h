//
//      srecord - manipulate eprom load files
//      Copyright (C) 1998, 1999, 2001-2003, 2005-2007 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: interface definition for lib/srec/record.cc
//

#ifndef INCLUDE_SREC_RECORD_H
#define INCLUDE_SREC_RECORD_H


/**
  * The srec_record class is used to represent a data record read
  * from a file.  (It is not limited to any particular file format.)
  * The records may be of various types.
  */
class srec_record
{
public:
    /**
      * The destructor.  It isn't vurtual, so don't derived anything
      * from this class.
      */
    ~srec_record();

    /**
      * The type of the various record types.
      */
    enum type_t
    {
        type_unknown,
        type_header,
        type_data,
        type_data_count,
        type_start_address
    };

    /**
      * The type of record addresses.
      */
    typedef unsigned long address_t;

    /**
      * The type of record data values.
      */
    typedef unsigned char data_t;

    /**
      * The default constructor.  The record will have an
      * indeterminate type, zero address, and no data.
      */
    srec_record();

    /**
      * The copy constructor.
      */
    srec_record(const srec_record &);

    /**
      * A constructor.  The record will have the given type, a zero
      * address and no data.
      */
    srec_record(type_t);

    /**
      * A constructor.  The record will have the given type, the
      * given address and no data.
      */
    srec_record(type_t, address_t);

    /**
      * A constructor.  The record will have the given type, the
      * given address and a copy of the given data.
      */
    srec_record(type_t the_type, address_t the_address, const data_t *the_data,
        int the_data_length);

    /**
      * The assignment operator.
      */
    srec_record &operator=(const srec_record &);

    /**
      * The get_address method is used to get the address of the
      * record.
      */
    address_t get_address() const { return address; }

    /**
      * The get_address_end method is used to get the address "off
      * the end" of this record.
      */
    address_t get_address_end() const { return (address + length); }

    /**
      * The set_address method is used to set the address of the
      * record.
      */
    void set_address(address_t arg) { address = arg; }

    /**
      * The get_length method is used to get the length (number of
      * bytes) of the record data.
      */
    int get_length() const { return length; }

    /**
      * The set_length method is used to set the number of data
      * bytes in the reocrd data.
      *
      * Note that you can reduce the length, but you can't increase it.
      */
    void
    set_length(int arg)
    {
        if (arg < 0)
            arg = 0;
        if (arg < length)
            length = arg;
    }

    /**
      * The get_data method is used to get a ponter to the baseof
      * the record data.
      *
      * Note: Accessing beyond get_length() bytes will give an
      * undefined value.
      */
    const data_t *get_data() const { return data; }

    /**
      * The get_data method is used to fetch the nth data value.
      *
      * Note: For perfoemance reasons, no range checking is
      * performed.  Accessing beyond get_length() bytes will give
      * an undefined value.
      */
    int get_data(int n) const { return data[n]; }

    /**
      * The is_all_zero method is used to determin if the record
      * contains data bytes which are all zero.
      */
    bool is_all_zero() const;

    /**
      * The set_data method is used to set values in the data array.
      * No range checking is performed.  The record length is not
      * consulted or adjusted.
      */
    void set_data(int n, int d) { data[n] = d; }

    /**
      * The set_data_extend method is used to set values in the data array.
      * The record length is adjusted if necessary.
      */
    void set_data_extend(int n, int d);

    /**
      * The get_type method is used to get the type of the record.
      */
    type_t get_type() const { return type; }

    /**
      * The set_type method is used to set the type of the record.
      */
    void set_type(type_t arg) { type = arg; }

    /**
      * The maximum_data_length method is used to determine the
      * maximum data length possible within a record, for a given
      * address.
      */
    static int maximum_data_length(address_t);

    /**
      * The decode_big_endian method is used to extract 'len'
      * bytes from the given 'data' and assemble a big-endian value
      * (most significant byte first).
      */
    static address_t decode_big_endian(data_t *data, int len);

    /**
      * The encode_big_endian method is used to break down 'val' into
      * 'len' bytes of 'data' orderdd big-endian (most significan
      * byte first).
      */
    static void encode_big_endian(data_t *data, address_t val, int len);

    /**
      * The encode_little_endian method is used to break down
      * 'val' into 'len' bytes of 'data' orderdd big-endian (least
      * significan byte first).
      */
    static void encode_little_endian(data_t *data, address_t val, int len);

    enum {
    /**
      * The max_data_length is the largest number of data bytes
      * which any record can hold.
      */
    max_data_length = 255 };

private:
    /**
      * The type instance variable is used to remember the type of
      * the record.
      */
    type_t type;

    /**
      * The address instance variable is used to remember the address
      * of the record.
      */
    address_t address;

    /**
      * The length instance variable is used to remember the number
      * of valid bytes in the 'data' array.
      */
    int length;

    /**
      * The data instance variable is used to remember the data
      * of the record.  Only the first 'length' bytes are valid,
      * the rest are undefined.
      */
    data_t data[max_data_length];
};

#endif // INCLUDE_SREC_RECORD_H
