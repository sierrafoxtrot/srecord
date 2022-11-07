//
// srecord - manipulate eprom load files
// Copyright (C) 2019 fenugrec
// based on motorola.cc and binary.cc
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

/* This input filter works with some "HP 64000 Absolute" files.
 * The code is based on two documents:
 *
 * - HP 64000 Logic Development System - File Format Reference Manual,
 *   #64980-90933, 1985
 * - HP 64700 Operating Environment - Absolute File Translator.
 *   HP Part No. B1471-97008, 1992
 *
 * which disagree on some points, and also do not quite match the files I am
 * working with (firmware update files from early 90's equipment; probably
 * generated from an unknown MC68k toolchain).
 * My other reference is a reflashing tool that parses these files.
 *
 * The main points of disagreement are:
 *
 * - two first bytes of the file, before any header or the
 *   "Processor Information Record", are not officially documented anywhere I
 *   can find. I did not check any 68k linker docs however.
 *   In my files they are set to "82 04", where 04 matches HP doc's "type"
 *   value of 4 for Absolute files.
 * - Header : only mentioned in B1471. 16 bytes of user data ?
 *   Present in my files.
 * - "recsize" (my terminology) : redundant field, before every record,
 *   giving length of its record minus 1,
 *   i.e. recsize == (DataRecord.numbytes + 6) - 1
 */
#include <srecord/arglex/tool.h>
#include <srecord/input/file/hp64k.h>
#include <srecord/record.h>
#include <cstdint>


srecord::input_file_hp64k::~input_file_hp64k()
{
}


srecord::input_file_hp64k::input_file_hp64k(
        const std::string &a_file_name) :
    input_file(a_file_name),
    rec_count(0),
    state(need_hdr)
{
}


srecord::input_file::pointer
srecord::input_file_hp64k::create(const std::string &a_file_name)
{
    return pointer(new input_file_hp64k(a_file_name));
}


void
srecord::input_file_hp64k::command_line(arglex_tool *)
{
    //TODO : possibly different types of header bytes ?
    //TODO : don't expect the redundant recsize field?
}

bool
srecord::input_file_hp64k::read_u16be(uint16_t *dest)
{
    uint16_t tmp;
    int c = get_char();
    if (c < 0)
        return false;
    tmp = (c & 0xFF) << 8;
    c = get_char();
    if (c < 0)
        return false;
    tmp = tmp | (c & 0xFF);
    *dest = tmp;
    return 1;
}

//in this implementation, read_datarec is called at the "recsize" position,
//i.e. 2 bytes before the actual data record.
bool
srecord::input_file_hp64k::read_datarec(record &result)
{
    uint16_t recsize, datasize;
    uint16_t load_l, load_h;
    size_t tmp_addr;

    if (!read_u16be(&recsize))
    {
        return false;
    }
    if (recsize > (2*128 + 5))
    {
        fatal_error("bad record size");
        return false;
    }
    if (!read_u16be(&datasize))
    {
        return false;
    }
    if ((datasize > (2*128)) || (recsize != (datasize + 5)))
    {
        fatal_error("bad datasize");
        return false;
    }

    if (!read_u16be(&load_l))
    {
        return false;
    }
    if (!read_u16be(&load_h))
    {
        return false;
    }
    tmp_addr = (load_h << 16) | load_l;

    unsigned cnt;
    uint8_t buf[256];
    for (cnt = 0; cnt < datasize; cnt++)
    {
        int c = get_char();
        if (c < 0) {
            return false;
        }
        buf[cnt] = (uint8_t) c;
    }
    if (datasize & 1)
    {
        //read dummy byte to maintain u16 alignment
        int c = get_char();
        if (c < 0) {
            return false;
        }
    }

    result = record(srecord::record::type_data, tmp_addr, buf, datasize);
    return true;
}

#define HP64_MAGIC 0x8204U  //file signature, before header
#define HP64_HDRLEN 16      //8 words
bool
srecord::input_file_hp64k::read_hdr(record &result)
{
    uint16_t magic;
    if (!read_u16be(&magic))
    {
        return false;
    }
    if (magic != HP64_MAGIC)
    {
        fatal_error("bad magic");
        return false;
    }

    unsigned cnt;
    unsigned len = HP64_HDRLEN; //initial value assume full buffer.
    uint8_t hdr[HP64_HDRLEN + 1];

    for (cnt=0; cnt < HP64_HDRLEN; cnt++)
    {
        //interpret header as 0-terminated string
        int c = get_char();
        if (c < 0) {
            return false;
        }
        hdr[cnt] = c;
        if ((c == 0) && (cnt < len)) {
            //set once, at first 0 byte
            len = cnt;
        }
    }
    len++;                  //include 0-term
    hdr[HP64_HDRLEN] = 0;   //ensure 0-term
    result = record(record::type_header, 0, hdr, len);
    return true;
}

#define PIR_LEN 8
bool
srecord::input_file_hp64k::read_pir(record &result)
{
    uint16_t pirlen, width, base, xfer_l, xfer_h;
    uint32_t xfer;

    if (!read_u16be(&pirlen))
    {
        return false;
    }
    if (pirlen != (PIR_LEN - 1 ))
    {
        fatal_error("Bad PIR length");
        return false;
    }
    if (!read_u16be(&width))
    {
        return false;
    }
    if (!read_u16be(&base))
    {
        return false;
    }
    if (!read_u16be(&xfer_l))
    {
        return false;
    }
    if (!read_u16be(&xfer_h))
    {
        return false;
    }
    xfer = (xfer_h << 16) | xfer_l;
    result = record(record::type_execution_start_address, xfer);
    return true;
}


bool
srecord::input_file_hp64k::read(record &record)
{
    switch (state)
    {
    case need_hdr:
        if (!read_hdr(record))
        {
            return 0;
        }
        state = need_pir;
        break;

    case need_pir:
        if (!read_pir(record))
        {
            return 0;
        }
        state = data;
        break;

    case data:
        if (!read_datarec(record))
        {
            if (rec_count < 1)
            {
                warning("file contains no data");
            }
            return false;
        }
        rec_count++;
        break;
    }
    return true;
}

bool
srecord::input_file_hp64k::is_binary(void)
    const
{
    return true;
}

const char *
srecord::input_file_hp64k::get_file_format_name(void)
    const
{
    return "HP64000 Absolute";
}


int
srecord::input_file_hp64k::format_option_number(void)
    const
{
    return arglex_tool::token_hp64k;
}
