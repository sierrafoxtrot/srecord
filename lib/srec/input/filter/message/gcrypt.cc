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

#include <lib/quit.h>
#include <lib/sizeof.h>
#include <lib/srec/input/filter/message/gcrypt.h>
#include <lib/srec/memory/walker/gcrypt.h>
#include <lib/srec/record.h>


srec_input_filter_message_gcrypt::~srec_input_filter_message_gcrypt()
{
}


srec_input_filter_message_gcrypt::srec_input_filter_message_gcrypt(
    const srec_input::pointer &a_deeper,
    unsigned long a_address,
    int a_algo,
    bool a_hmac
) :
    srec_input_filter_message(a_deeper),
    algo(a_algo),
    hmac(a_hmac),
    address(a_address)
{
}


srec_input::pointer
srec_input_filter_message_gcrypt::create(const srec_input::pointer &a_deeper,
    unsigned long a_address, int algo, bool hmac)
{
    return
        pointer
        (
            new srec_input_filter_message_gcrypt
            (
                a_deeper,
                a_address,
                algo,
                hmac
            )
        );
}


srec_input::pointer
srec_input_filter_message_gcrypt::create(const srec_input::pointer &a_deeper,
    unsigned long a_address, const char *name, bool a_hmac)
{
    return create(a_deeper, a_address, algorithm_from_name(name), a_hmac);
}


srec_input::pointer
srec_input_filter_message_gcrypt::create_md5(
    const srec_input::pointer &a_deeper, unsigned long a_address)
{
    return create(a_deeper, a_address, GCRY_MD_MD5);
}


srec_input::pointer
srec_input_filter_message_gcrypt::create_rmd160(
    const srec_input::pointer &a_deeper, unsigned long a_address)
{
    return create(a_deeper, a_address, GCRY_MD_RMD160);
}


srec_input::pointer
srec_input_filter_message_gcrypt::create_sha1(
    const srec_input::pointer &a_deeper, unsigned long a_address)
{
    return create(a_deeper, a_address, GCRY_MD_SHA1);
}


int
srec_input_filter_message_gcrypt::algorithm_from_name(const char *name)
{
    static int table[] =
    {
        GCRY_MD_MD5,
        GCRY_MD_SHA1,
        GCRY_MD_RMD160,
        GCRY_MD_MD2,
        GCRY_MD_TIGER,
        GCRY_MD_HAVAL,
        GCRY_MD_SHA256,
        GCRY_MD_SHA384,
        GCRY_MD_SHA512,
        GCRY_MD_SHA224,
        GCRY_MD_MD4,
        GCRY_MD_CRC32,
        GCRY_MD_CRC32_RFC1510,
        GCRY_MD_CRC24_RFC2440,
        GCRY_MD_WHIRLPOOL,
    };

    for (const int *tp = table; tp < ENDOF(table); ++tp)
    {
        int algo = *tp;
        if (0 == strcasecmp(name, gcry_md_algo_name(algo)))
            return algo;
    }
    quit_default.fatal_error("gcrypt algorithm \"%s\" unknown", name);
    return -1;
}


void
srec_input_filter_message_gcrypt::process(const srec_memory &input,
    srec_record &output)
{
#ifdef HAVE_LIBGCRYPT
    // allocate the message digest handle
    unsigned int flags = 0;
    if (hmac)
        flags |= GCRY_MD_FLAG_HMAC;
    gcry_md_hd_t handle = 0;
    gcry_error_t err = gcry_md_open(&handle, algo, flags);
    if (err)
        fatal_error("gcry_md_open: %s", gcry_strerror(err));

    // traverse the memory
    srec_memory_walker::pointer w = srec_memory_walker_gcrypt::create(handle);
    input.walk(w);

    // generate the result
    const unsigned char *data = gcry_md_read(handle, algo);
    size_t data_size = gcry_md_get_algo_dlen(algo);
    output = srec_record(srec_record::type_data, address, data, data_size);

    // free the message digest handle
    gcry_md_close(handle);
#else
    (void)input;
    fatal_error("not compiled with libgcrypt support");
    output =
        srec_record
        (
            srec_record::type_data,
            address,
            (const unsigned char *)"oops",
            4
        );
#endif
}


const char *
srec_input_filter_message_gcrypt::get_algorithm_name()
    const
{
    return gcry_md_algo_name(algo);
}


// vim:ts=8:sw=4:et
