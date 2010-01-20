//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008, 2010 Peter Miller
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

#include <srecord/input/catenate.h>
#include <srecord/record.h>


srec_input_catenate::~srec_input_catenate()
{
}


srec_input_catenate::srec_input_catenate(const pointer &a1, const pointer &a2) :
    in1(a1),
    in2(a2)
{
    assert(!!in1);
    assert(!!in2);
}


srec_input::pointer
srec_input_catenate::create(const pointer &a1, const pointer &a2)
{
    return pointer(new srec_input_catenate(a1, a2));
}


bool
srec_input_catenate::read(srec_record &record)
{
    while (in1)
    {
        bool ok = in1->read(record);
        if (!ok)
        {
            in1.reset();
            break;
        }
        switch (record.get_type())
        {
        case srec_record::type_unknown:
        case srec_record::type_data_count:
            break;

        default:
            return true;
        }
    }
    for (;;)
    {
        bool ok = in2->read(record);
        if (!ok)
            return false;
        switch (record.get_type())
        {
        case srec_record::type_unknown:
        case srec_record::type_data_count:
            break;

        default:
            return true;
        }
    }
}


std::string
srec_input_catenate::filename()
    const
{
    return (in1 ? in1 : in2)->filename();
}


std::string
srec_input_catenate::filename_and_line()
    const
{
    return (in1 ? in1 : in2)->filename_and_line();
}


const char *
srec_input_catenate::get_file_format_name()
    const
{
    return (in1 ? in1 : in2)->get_file_format_name();
}


void
srec_input_catenate::disable_checksum_validation()
{
    if (in1)
        in1->disable_checksum_validation();
    in2->disable_checksum_validation();
}


// vim:ts=8:sw=4:et
