/*
 *	srecord - manipulate eprom load files
 *	Copyright (C) 1998 Peter Miller;
 *	All rights reserved.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * MANIFEST: interface definition for lib/common/interval.cc
 */

#ifndef INCLUDE_INTERVAL_H
#define INCLUDE_INTERVAL_H

#pragma interface "interval"

#include <stddef.h>

class interval
{
public:
	typedef unsigned long data_t;

	interval();
	interval(data_t, data_t);
	interval(const interval &);
	interval &operator=(const interval &);
	~interval();

	static interval union_(const interval &, const interval &);
	static interval intersection(const interval &, const interval &);
	static interval difference(const interval &, const interval &);
	static bool equal(const interval &, const interval &);
	bool member(data_t) const;
	bool empty() const;
	void first_interval_only();

	void scan_begin();
	bool scan_next(data_t &);
	void scan_end();

	data_t get_lowest() const;
	data_t get_highest() const;

	void print(class ostream &) const;

private:
	size_t length;
	size_t size;
	size_t scan_index;
	data_t scan_next_datum;
	data_t *data;

	bool valid() const;
	void append(data_t);
};


inline bool
operator == (const interval &lhs, const interval &rhs)
{
	return interval::equal(lhs, rhs);
}


inline bool
operator != (const interval &lhs, const interval &rhs)
{
	return !interval::equal(lhs, rhs);
}


inline interval
operator * (const interval &lhs, const interval &rhs)
{
	return interval::intersection(lhs, rhs);
}


inline interval &
operator *= (interval &lhs, const interval &rhs)
{
	lhs = interval::intersection(lhs, rhs);
	return lhs;
}


inline interval
operator + (const interval &lhs, const interval &rhs)
{
	return interval::union_(lhs, rhs);
}


inline interval &
operator += (interval &lhs, const interval &rhs)
{
	lhs = interval::union_(lhs, rhs);
	return lhs;
}


inline interval
operator - (const interval &lhs, const interval &rhs)
{
	return interval::difference(lhs, rhs);
}


inline interval &
operator -= (interval &lhs, const interval &rhs)
{
	lhs = interval::difference(lhs, rhs);
	return lhs;
}

inline interval
operator - (const interval &arg)
{
	return (interval(0, 0) - arg);
}

inline class ostream &
operator << (ostream &os, const interval &val)
{
	val.print(os);
	return os;
}

#endif /* INCLUDE_INTERVAL_H */
