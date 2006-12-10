//
//	srecord - manipulate eprom load files
//	Copyright (C) 2000, 2002, 2005, 2006 Peter Miller;
//	All rights reserved.
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
// MANIFEST: interface definition for include/srec/memory/walker/compare.cc
//

#ifndef INCLUDE_SREC_MEMORY_WALKER_COMPARE_H
#define INCLUDE_SREC_MEMORY_WALKER_COMPARE_H


#include <interval.h>
#include <srec/memory/walker.h>

class srec_memory; // forward

/**
  * The srec_memory_walker_compare class is used to represent a memory
  * walker which compares memory data with another memory instance.
  *
  * Note that this is strictly an improper subset comparison.  I.e. that
  * all bytes passed to the observe method exists in the other memory
  * instance.
  *
  * For a complete equality test, you need to use this comparison both
  * ways round.
  */
class srec_memory_walker_compare:
    public srec_memory_walker
{
public:
    /**
      * The destructor.
      */
    virtual ~srec_memory_walker_compare();

    /**
      * The constructor.
      *
      * @param other
      *     The other memory instance to be checked against this one.
      * @param check_wrong
      *     Whether or not to check that the data agrees as well as the
      *     address ranges.
      */
    srec_memory_walker_compare(const srec_memory &other, bool check_wrong);

    // See base class for documentation.
    virtual void observe(unsigned long, const void *, int);

    /**
      * The print method is used to print the results of the comparison
      * on the standard output.
      *
      * @param caption
      *     The text to place at the start of the listing.
      */
    void print(const char *caption) const;

    /**
      * The same method is used to discover whether the result of the
      * comparison indicate that the two memory instances are the same.
      */
    bool same() const;

private:
    /**
      * The other instance variable is used to remember the other memory
      * instance to be checked against this one.
      */
    const srec_memory &other;

    /**
      * The check_wrong instance variable is used to remember whether or
      * not to check that the data agrees as well as the address ranges.
      */
    bool check_wrong;

    /**
      * The unset instance variable is used to remember which bytes of
      * data given to the observe method are NOT present in the other
      * memory instance.
      */
    interval unset;

    /**
      * The wrong instance variable is used to remember which bytes of
      * data given to the observer method were different than the ones
      * in the other memory instance.
      */
    interval wrong;

    /**
      * The default constructor.  Do not use.
      */
    srec_memory_walker_compare();

    /**
      * The copy constructor.  Do not use.
      */
    srec_memory_walker_compare(const srec_memory_walker_compare &);

    /**
      * The assignment operator.  Do not use.
      */
    srec_memory_walker_compare &operator=(const srec_memory_walker_compare &);
};

#endif // INCLUDE_SREC_MEMORY_WALKER_COMPARE_H
