//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009 Peter Miller
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

#ifndef LIB_SREC_INPUT_FILTER_MESSAGE_GCRYPT_H
#define LIB_SREC_INPUT_FILTER_MESSAGE_GCRYPT_H

#include <lib/srec/input/filter/message.h>

/**
  * The srec_input_filter_message_gcrypt class is used to represent a
  * filter that runs the data through one of the hashing algorithms in
  * the gcrypt library.
  *
  * http://freshmeat.net/projects/libgcrypt/
  * http://directory.fsf.org/project/libgcrypt/
  */
class srec_input_filter_message_gcrypt:
    public srec_input_filter_message
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_input_filter_message_gcrypt();

private:
    /**
      * The constructor.  It is private on purpose, use one of the
      * create_* class method instead.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param algo
      *     The algorithm number (private to class).
      * @param hmac
      *     Turn the hash into a HMAC.
      * @param address
      *     Where to place the hash in memory.
      */
    srec_input_filter_message_gcrypt(const srec_input::pointer &deeper,
        int algo, bool hmac, unsigned long address);

public:
    /**
      * The create_md5 class method is used to create a new dynamically
      * allocated instance of this class, that calculates MD5 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_md5(const srec_input::pointer &deeper,
        unsigned long address);

protected:
    // See base class for documentation.
    void process(const srec_memory &input, srec_record &output);

    // See base class for documentation.
    const char *get_algorithm_name() const;

private:
    /**
      * The algo instance variable is used to remember the algorithm
      * number.
      */
    int algo;

    /**
      * The hmac instance variable is used to remember whether or not to
      * turn the hash into a HMAC.
      */
    bool hmac;

    /**
      * The address instance variable is used to remember where to place
      * the hash in memory.
      */
    unsigned long address;

    /**
      * The default constructor.  Do not use.
      */
    srec_input_filter_message_gcrypt();

    /**
      * The copy constructor.  Do not use.
      */
    srec_input_filter_message_gcrypt(const srec_input_filter_message_gcrypt &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_input_filter_message_gcrypt &operator=(
        const srec_input_filter_message_gcrypt &);
};

// vim:ts=8:sw=4:et
#endif // LIB_SREC_INPUT_FILTER_MESSAGE_GCRYPT_H
