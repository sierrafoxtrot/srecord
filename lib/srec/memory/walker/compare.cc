//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000, 2002, 2005, 2006 Peter Miller
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
// MANIFEST: functions to impliment the srec_memory_walker_compare class
//


#include <iostream>
using namespace std;
#include <lib/srec/memory.h>
#include <lib/srec/memory/walker/compare.h>


srec_memory_walker_compare::~srec_memory_walker_compare()
{
}


srec_memory_walker_compare::srec_memory_walker_compare(const srec_memory &a1,
	bool a2) :
    other(a1),
    check_wrong(a2)
{
}


void
srec_memory_walker_compare::observe(unsigned long addr, const void *p,
    int len)
{
    interval wrongTemp;
    interval unsetTemp;

    unsigned char *data = (unsigned char *)p;
    for (int j = 0; j < len; ++j)
    {
	if (other.set_p(addr + j))
	{
    	    if (check_wrong && data[j] != other.get(addr + j))
       		wrongTemp += interval(addr + j);
	}
	else
    	    unsetTemp += interval(addr + j);
    }

    wrong += wrongTemp;
    unset += unsetTemp;
}


void
srec_memory_walker_compare::print(const char *caption)
    const
{
    ios::fmtflags old =
	cout.setf
	(
	    static_cast<ios::fmtflags>(ios::showbase + ios::hex),
	    static_cast<ios::fmtflags>(ios::showbase + ios::hex + ios::dec +
		ios::oct)
	);
    if (!wrong.empty())
	    cout << "Different:\t" << wrong << endl;
    if (!unset.empty())
	    cout << caption << " only:\t" << unset << endl;
    cout.flags(old);
}


bool
srec_memory_walker_compare::same()
    const
{
    return (wrong.empty() && unset.empty());
}
