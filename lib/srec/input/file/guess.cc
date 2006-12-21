//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000-2006 Peter Miller
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
//
// MANIFEST: functions to guess the format
//

#include <lib/quit/exception.h>
#include <lib/srec/input/file/aomf.h>
#include <lib/srec/input/file/ascii_hex.h>
#include <lib/srec/input/file/atmel_generic.h>
#include <lib/srec/input/file/binary.h>
#include <lib/srec/input/file/dec_binary.h>
#include <lib/srec/input/file/emon52.h>
#include <lib/srec/input/file/fairchild.h>
#include <lib/srec/input/file/fastload.h>
#include <lib/srec/input/file/formatted_binary.h>
#include <lib/srec/input/file/four_packed_code.h>
#include <lib/srec/input/file/guess.h>
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
#include <lib/srec/input/file/vmem.h>
#include <lib/srec/input/file/wilson.h>
#include <lib/srec/record.h>


static srec_input *
create_aomf(const char *fn)
{
    return new srec_input_file_aomf(fn);
}

static srec_input *
create_ascii_hex(const char *fn)
{
    return new srec_input_file_ascii_hex(fn);
}

static srec_input *
create_atmel_generic(const char *fn)
{
    return new srec_input_file_atmel_generic(fn);
}

static srec_input *
create_dec_binary(const char *fn)
{
    return new srec_input_file_dec_binary(fn);
}

static srec_input *
create_formatted_binary(const char *fn)
{
    return new srec_input_file_formatted_binary(fn);
}

static srec_input *
create_four_packed_code(const char *fn)
{
    return new srec_input_file_four_packed_code(fn);
}

static srec_input *
create_emon52(const char *fn)
{
    return new srec_input_file_emon52(fn);
}

static srec_input *
create_fairchild(const char *fn)
{
    return new srec_input_file_fairchild(fn);
}

static srec_input *
create_fastload(const char *fn)
{
    return new srec_input_file_fastload(fn);
}

static srec_input *
create_intel(const char *fn)
{
    return new srec_input_file_intel(fn);
}

static srec_input *
create_intel16(const char *fn)
{
    return new srec_input_file_intel16(fn);
}

static srec_input *
create_mos_tech(const char *fn)
{
    return new srec_input_file_mos_tech(fn);
}

static srec_input *
create_needham(const char *fn)
{
    return new srec_input_file_needham(fn);
}

static srec_input *
create_ohio_scientific(const char *fn)
{
    return new srec_input_file_os65v(fn);
}

static srec_input *
create_signetics(const char *fn)
{
    return new srec_input_file_signetics(fn);
}

static srec_input *
create_spasm(const char *fn)
{
    // don't we need to test the big-endian and little-endian forms?
    // but they are indistinguishable.
    return new srec_input_file_spasm(fn);
}

static srec_input *
create_spectrum(const char *fn)
{
    return new srec_input_file_spectrum(fn);
}

static srec_input *
create_srecord(const char *fn)
{
    return new srec_input_file_srecord(fn);
}

static srec_input *
create_stewie(const char *fn)
{
    return new srec_input_file_stewie(fn);
}

static srec_input *
create_tektronix(const char *fn)
{
    return new srec_input_file_tektronix(fn);
}

static srec_input *
create_tektronix_extended(const char *fn)
{
    return new srec_input_file_tektronix_extended(fn);
}

static srec_input *
create_ti_tagged(const char *fn)
{
    return new srec_input_file_ti_tagged(fn);
}

static srec_input *
create_vmem(const char *fn)
{
    return new srec_input_file_vmem(fn);
}

static srec_input *
create_wilson(const char *fn)
{
    return new srec_input_file_wilson(fn);
}


typedef srec_input *(*func_p)(const char *);

static func_p table[] =
{
    create_aomf,
    create_ascii_hex,
    create_atmel_generic,
    create_dec_binary,
    create_emon52,
    create_fairchild,
    create_fastload,
    create_formatted_binary,
    create_four_packed_code,
    create_intel,
    create_intel16,
    create_mos_tech,
    create_needham,
    create_ohio_scientific,
    create_signetics,
    create_spasm,
    create_spectrum,
    create_srecord,
    create_stewie,
    create_tektronix,
    create_tektronix_extended,
    create_ti_tagged,
    create_vmem,
    create_wilson,
};

#define SIZEOF(a) (sizeof(a) / sizeof(a[0]))
#define ENDOF(a) ((a) + SIZEOF(a))


srec_input *
srec_input_file_guess(const char *fn)
{
    if (!fn || !*fn || (fn[0] == '-' && fn[1] == 0))
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
	srec_input *ifp = func(fn);
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
		delete ifp;

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
	delete ifp;
    }

    //
    // If nothing else works, assume the file is binary.
    //
    quit_default.warning
    (
	"%s: unable to determine the file format, assuming binary",
	fn
    );
    return new srec_input_file_binary(fn);
}
