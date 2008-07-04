//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000-2008 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#include <lib/quit/exception.h>
#include <lib/srec/input/file/aomf.h>
#include <lib/srec/input/file/ascii_hex.h>
#include <lib/srec/input/file/atmel_generic.h>
#include <lib/srec/input/file/binary.h>
#include <lib/srec/input/file/brecord.h>
#include <lib/srec/input/file/dec_binary.h>
#include <lib/srec/input/file/emon52.h>
#include <lib/srec/input/file/fairchild.h>
#include <lib/srec/input/file/fastload.h>
#include <lib/srec/input/file/formatted_binary.h>
#include <lib/srec/input/file/four_packed_code.h>
#include <lib/srec/input/file/intel.h>
#include <lib/srec/input/file/intel16.h>
#include <lib/srec/input/file/mos_tech.h>
#include <lib/srec/input/file/needham.h>
#include <lib/srec/input/file/os65v.h>
#include <lib/srec/input/file/signetics.h>
#include <lib/srec/input/file/spasm.h>
#include <lib/srec/input/file/spectrum.h>
#include <lib/srec/input/file/srecord.h>
#include <lib/srec/input/file/stewie.h>
#include <lib/srec/input/file/tektronix.h>
#include <lib/srec/input/file/tektronix_extended.h>
#include <lib/srec/input/file/ti_tagged.h>
#include <lib/srec/input/file/ti_txt.h>
#include <lib/srec/input/file/vmem.h>
#include <lib/srec/input/file/wilson.h>
#include <lib/srec/record.h>


typedef srec_input::pointer (*func_p)(const std::string &file_name);

static func_p table[] =
{
    srec_input_file_aomf::create,
    srec_input_file_ascii_hex::create,
    srec_input_file_atmel_generic::create_be,
    srec_input_file_brecord::create,
    srec_input_file_dec_binary::create,
    srec_input_file_emon52::create,
    srec_input_file_fairchild::create,
    srec_input_file_fastload::create,
    srec_input_file_formatted_binary::create,
    srec_input_file_four_packed_code::create,
    srec_input_file_intel::create,
    srec_input_file_intel16::create,
    srec_input_file_mos_tech::create,
    srec_input_file_needham::create,
    srec_input_file_os65v::create,
    srec_input_file_signetics::create,
    srec_input_file_spasm::create_be,
    srec_input_file_spectrum::create,
    srec_input_file_srecord::create,
    srec_input_file_stewie::create,
    srec_input_file_tektronix::create,
    srec_input_file_tektronix_extended::create,
    srec_input_file_ti_tagged::create,
    srec_input_file_ti_txt::create,
    srec_input_file_vmem::create,
    srec_input_file_wilson::create,
};

#define SIZEOF(a) (sizeof(a) / sizeof(a[0]))
#define ENDOF(a) ((a) + SIZEOF(a))


srec_input::pointer
srec_input_file::guess(const std::string &fn)
{
    if (fn.empty() || fn == "-")
    {
        quit_default.fatal_error
        (
            "the file format of the standard input must be specified explicitly"
        );
    }

    //
    // Try each file format in turn.
    //
    quit_exception quitter;
    for (func_p *tp = table; tp < ENDOF(table); ++tp)
    {
        //
        // Create a new file reader
        //
        func_p func = *tp;
        srec_input::pointer ifp = func(fn);
        try
        {
            //
            // Set the exception-throwing quitter
            // (it also silences all error and warning messages).
            //
            ifp->set_quit(quitter);
            srec_record record;

            //
            // Try to read something from the file.  If it is
            // the wrong format, it will throw an exception.
            //
            if (ifp->read(record))
            {
                //
                // It is necessary to nuke the old file reader.
                // (a) Because it has the wrong quitter, but more
                // importantly (b) because it is no longer positioned
                // at the start of the file, and the user *will* miss
                // some data, also (c) we need a chance to use the
                // input::command_line() method.
                //
                ifp.reset();

                //
                // Return a brand-new file reader.
                //
                return func(fn);
            }
        }
        catch (quit_exception::vomit)
        {
        }

        //
        // Wrong format.
        // Toss this one, and try another.
        //
        ifp.reset();
    }

    //
    // If nothing else works, assume the file is binary.
    //
    quit_default.warning
    (
        "%s: unable to determine the file format, assuming binary",
        fn.c_str()
    );
    return srec_input_file_binary::create(fn);
}
