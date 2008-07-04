//
//      srecord - manipulate eprom load files
//      Copyright (C) 2000, 2002, 2005-2008 Peter Miller
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


#include <iostream>

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


srec_memory_walker_compare::pointer
srec_memory_walker_compare::create(const srec_memory &a1, bool a2)
{
    return pointer(new srec_memory_walker_compare(a1, a2));
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


static std::string
spaces(int n)
{
    // inefficient, but it isn't used much.
    std::string result;
    while (n > 0)
    {
        result += " ";
        --n;
    }
    return result;
}


void
srec_memory_walker_compare::print(const char *caption)
    const
{
    std::ios::fmtflags old =
        std::cout.setf
        (
            static_cast<std::ios::fmtflags>(std::ios::showbase + std::ios::hex),
            static_cast<std::ios::fmtflags>
            (
                    std::ios::showbase
                +
                    std::ios::hex
                +
                    std::ios::dec
                +
                    std::ios::oct
            )
        );
    if (!wrong.empty())
        std::cout << "Different:      " << wrong << std::endl;
    if (!unset.empty())
    {
        std::string s(caption);
        std::cout << s << " only:" << spaces(10 - s.size()) << unset
            << std::endl;
    }
    std::cout.flags(old);
}


bool
srec_memory_walker_compare::same()
    const
{
    return (wrong.empty() && unset.empty());
}
