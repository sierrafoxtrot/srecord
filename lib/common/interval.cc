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
 * MANIFEST: functions to impliment the interval class
 */

#pragma implementation

#include <interval.h>
#include <iostream.h>


/*
 * NAME
 *	interval_create_empty - create an empty interval
 *
 * SYNOPSIS
 *	interval_ty *interval_create_empty(void);
 *
 * DESCRIPTION
 *	The interval_create_empty function is used to create
 *	an empty interval.
 *
 * RETURNS
 *	a pointer to the new interval in dynamic memory
 *
 * CAVEAT
 *	It is the responsibility of the caller to release the
 *	interval to dynamic memory when no longer required.
 *	Use the interval_free function for this purpose.
 */

interval::interval()
{
	length = 0;
	size = 0;
	scan_index = 0;
	scan_next_datum = 0;
	data = 0;
	/* assert(valid()); */
}


static inline long long
promote(interval::data_t datum, size_t pos)
{
	if (datum == 0 && (pos & 1))
		return (1LL << 32);
	return datum;
}


/*
 * NAME
 *	interval_create_range - create a single range interval
 *
 * SYNOPSIS
 *	interval_ty *interval_create_range(interval_data_ty first, interval_data_ty last);
 *
 * DESCRIPTION
 *	The interval_create_range function is used to create an interval
 *	consisting of a single range, from first to last inclusive.
 *
 * ARGUMENTS
 *	first	- the start of the range
 *	last	- the end of the range (inclusive)
 *
 * RETURNS
 *	a pointer to the new interval in dynamic memory
 *
 * CAVEAT
 *	It is the responsibility of the caller to release the
 *	interval to dynamic memory when no longer required.
 *	Use the interval_free function for this purpose.
 */

interval::interval(data_t first, data_t last)
{
	length = 2;
	size = 8;
	data = new data_t[size + 1];
	scan_index = 0;
	scan_next_datum = 0;
	if (first <= promote(last, 1))
	{
		data[0] = first;
		data[1] = last;
	}
	else
	{
		data[0] = last;
		data[1] = first;
	}
	data[2] = 2;
	/* assert(valid()); */
}


interval::interval(const interval &arg)
{
	/* assert(arg.valid()); */
	length = arg.length;
	size = length;
	scan_index = 0;
	scan_next_datum = 0;
	if (size)
	{
		data = new data_t[size + 1];
		for (size_t j = 0; j <= length; ++j)
			data[j] = arg.data[j];
	}
	else
		data = 0;
	/* assert(valid()); */
}


interval &
interval::operator=(const interval &arg)
{
	if (data)
		delete data;
	/* assert(arg.valid()); */
	length = arg.length;
	size = length;
	scan_index = 0;
	scan_next_datum = 0;
	if (size)
	{
		data = new data_t[size + 1];
		for (size_t j = 0; j <= length; ++j)
			data[j] = arg.data[j];
	}
	else
		data = 0;
	/* assert(valid()); */
	return *this;
}


/*
 * NAME
 *	interval_free - release interval memory
 *
 * SYNOPSIS
 *	void interval_free(interval_ty *ip);
 *
 * DESCRIPTION
 *	The interval_free function is used to release the dynamic
 *	memory used by an interval back to the dynamic memory pool.
 *
 * ARGUMENTS
 *	ip	- the interval to release
 */

interval::~interval()
{
	/* assert(valid()); */
	if (data)
		delete data;
}


/*
 * NAME
 *	interval_valid - internal consistency check
 *
 * SYNOPSIS
 *	int interval_valid(interval_ty *ip);
 *
 * DESCRIPTION
 *	The interval_valid function is used to check the internal
 *	consistency of an interval.
 *
 * ARGUMENTS
 *	ip	- pointer to interval to check
 *
 * RETURNS
 *	int	1 if interval is valid
 *		0 if interval is not valid
 *
 * CAVEAT
 *	This function is only available if DEBUG is defined,
 *	and is intended for use in assert() statements.
 */

bool
interval::valid()
	const
{
	if (length > size)
		return false;
	if (length & 1)
		return false;
	if ((size == 0) != (data == 0))
		return false;
	if (length == 0)
		return true;
	if (data[length] != length)
		return false;

	/*
	 * As a special case, an upper bound of zero means
	 * positive infinity.  It has to be the last one.
	 */
	size_t max = length;
	if (data[max - 1] == 0)
		--max;

	for (size_t j = 1; j < max; ++j)
		if (data[j - 1] >= data[j])
			return false;
	return true;
}


/*
 * NAME
 *	append - append datum to interval data
 *
 * SYNOPSIS
 *	void append _((interval_ty **ipp, interval_data_ty datum));
 *
 * DESCRIPTION
 *	The append function is used to append a datum to
 *	the end of an interval under construction.
 *	
 * ARGUMENTS
 *	ipp	- pointer to inerval pointer.
 *	datum	- value to append.
 *
 * CAVEAT
 *	The interval may move in dynamic memory, with is why ** is used.
 *	The interval will need to be normalized before you
 *	next use interval_valid.
 */

void
interval::append(data_t datum)
{
	/*
	 * should always be increasing
	 */
	/* assert
	(
		length < 1
	||
		promote(datum, length) >= promote(data[length - 1], length - 1)
	); */

	/*
	 * make it larger if necessary
	 */
	if (length >= size)
	{
		size = size * 2 + 8;
		data_t *tmp = new data_t[size + 1];
		if (data)
		{
			for (size_t k = 0; k < length; ++k)
				tmp[k] = data[k];
			delete data;
		}
		data = tmp;
	}

	/*
	 * remeber the datum
	 */
	data[length++] = datum;

	/*
	 * elide empty sequences
	 */
	if
	(
		length >= 2
	&&
		data[length - 1] == data[length - 2]
	)
		length -= 2;
}


/*
 * NAME
 *	interval_union - union of two intervals
 *
 * SYNOPSIS
 *	interval_ty *interval_union(interval_ty *left, interval_ty *right);
 *
 * DESCRIPTION
 *	The interval_union function is used to form the
 *	union of two intervals.
 *
 * ARGUMENTS
 *	left	- interval to be unioned with
 *	right	- another interval
 *
 * RETURNS
 *	a pointer to the new interval in dynamic memory
 *
 * CAVEAT
 *	It is the responsibility of the caller to release the
 *	interval to dynamic memory when no longer required.
 *	Use the interval_free function for this purpose.
 */

interval
interval::union_(const interval &left, const interval &right)
{
	/* assert(left.valid()); */
	/* assert(right.valid()); */
	interval result;
	size_t left_pos = 0;
	size_t right_pos = 0;
	int count = 0;
	for (;;)
	{
		int old_count = count;
		data_t place;
		if (left_pos < left.length)
		{
			if (right_pos < right.length)
			{
				long long left_val = promote(left.data[left_pos], left_pos);
				long long right_val = promote(right.data[right_pos], right_pos);
				if (left_val < right_val)
				{
					count += (left_pos & 1 ? -1 : 1);
					place = left.data[left_pos++];
				}
				else
				{
					count += (right_pos & 1 ? -1 : 1);
					place = right.data[right_pos++];
				}
			}
			else
			{
				count += (left_pos & 1 ? -1 : 1);
				place = left.data[left_pos++];
			}
		}
		else
		{
			if (right_pos < right.length)
			{
				count += (right_pos & 1 ? -1 : 1);
				place = right.data[right_pos++];
			}
			else
				break;
		}
		if ((count >= 1) != (old_count >= 1))
			result.append(place);
	}
	if (result.length)
		result.data[result.length] = result.length;
	/* assert(result.valid()); */
	return result;
}


/*
 * NAME
 *	interval_intersection - intersection of two intervals
 *
 * SYNOPSIS
 *	interval_ty *interval_intersection(interval_ty *left, interval_ty *right);
 *
 * DESCRIPTION
 *	The interval_intersection function is used to form the
 *	intersection of two intervals.
 *
 * ARGUMENTS
 *	left	- interval to be intersected with
 *	right	- another interval
 *
 * RETURNS
 *	a pointer to the new interval in dynamic memory
 *
 * CAVEAT
 *	It is the responsibility of the caller to release the
 *	interval to dynamic memory when no longer required.
 *	Use the interval_free function for this purpose.
 */

interval
interval::intersection(const interval &left, const interval &right)
{
	/* assert(left.valid()); */
	/* assert(right.valid()); */
	interval result;
	size_t left_pos = 0;
	size_t right_pos = 0;
	int count = 0;
	for (;;)
	{
		int old_count = count;
		data_t place;
		if (left_pos < left.length)
		{
			if (right_pos < right.length)
			{
				long long left_val = promote(left.data[left_pos], left_pos);
				long long right_val = promote(right.data[right_pos], right_pos);
				if (left_val < right_val)
				{
					count += (left_pos & 1 ? -1 : 1);
					place = left.data[left_pos++];
				}
				else
				{
					count += (right_pos & 1 ? -1 : 1);
					place = right.data[right_pos++];
				}
			}
			else
			{
				count += (left_pos & 1 ? -1 : 1);
				place = left.data[left_pos++];
			}
		}
		else
		{
			if (right_pos < right.length)
			{
				count += (right_pos & 1 ? -1 : 1);
				place = right.data[right_pos++];
			}
			else
				break;
		}
		if ((count >= 2) != (old_count >= 2))
			result.append(place);
	}
	if (result.length)
		result.data[result.length] = result.length;
	/* assert(result.valid()); */
	return result;
}


/*
 * NAME
 *	interval_difference - difference of two intervals
 *
 * SYNOPSIS
 *	interval_ty *interval_difference(interval_ty *left, interval_ty *right);
 *
 * DESCRIPTION
 *	The interval_difference function is used to form the
 *	difference of two intervals.
 *
 * ARGUMENTS
 *	left	- interval to take things out of
 *	right	- things to take out of it
 *
 * RETURNS
 *	a pointer to the new interval in dynamic memory
 *
 * CAVEAT
 *	It is the responsibility of the caller to release the
 *	interval to dynamic memory when no longer required.
 *	Use the interval_free function for this purpose.
 */

interval
interval::difference(const interval &left, const interval &right)
{
	/* assert(left.valid()); */
	/* assert(right.valid()); */
	interval result;
	size_t left_pos = 0;
	size_t right_pos = 0;
	int count = 0;
	for (;;)
	{
		int old_count = count;
		data_t place;
		if (left_pos < left.length)
		{
			if (right_pos < right.length)
			{
				long long left_val = promote(left.data[left_pos], left_pos);
				long long right_val = promote(right.data[right_pos], right_pos);
				if (left_val < right_val)
				{
					count += (left_pos & 1 ? -1 : 1);
					place = left.data[left_pos++];
				}
				else
				{
					count -= (right_pos & 1 ? -1 : 1);
					place = right.data[right_pos++];
				}
			}
			else
			{
				count += (left_pos & 1 ? -1 : 1);
				place = left.data[left_pos++];
			}
		}
		else
		{
			if (right_pos < right.length)
			{
				count -= (right_pos & 1 ? -1 : 1);
				place = right.data[right_pos++];
			}
			else
				break;
		}
		if ((count >= 1) != (old_count >= 1))
			result.append(place);
	}
	if (result.length)
		result.data[result.length] = result.length;
	/* assert(result.valid()); */
	return result;
}


/*
 * NAME
 *	interval_member - test for membership
 *
 * SYNOPSIS
 *	int interval_member(interval_ty *, interval_data_ty datum);
 *
 * DESCRIPTION
 *	The interval_member function is used to test if a particular
 *	datum is included in an interval.
 *
 * ARGUMENTS
 *	ip	- interval to test
 *	datum	- value to test for
 *
 * RETURNS
 *	int	1 if is a member
 *		0 if is not a member
 */

bool
interval::member(data_t datum)
	const
{
	/* assert(valid()); */
	size_t min = 0;
	size_t max = length - 2;
	while (min <= max)
	{
		size_t mid = ((min + max) / 2) & ~1;
		data_t lo = data[mid];
		long long hi = promote(data[mid + 1], mid + 1);
		if (lo <= datum && datum < hi)
			return true;
		if (lo < datum)
			min = mid + 2;
		else
			max = mid - 2;
	} 
	return false;
}


/*
 * NAME
 *	interval_scan_begin
 *
 * SYNOPSIS
 *	void interval_scan_begin(interval_ty *ip);
 *
 * DESCRIPTION
 *	The interval_scan_begin function is used to
 *	start traversing every datum in the interval.
 *
 * ARGUMENTS
 *	ip	- interval to scan
 */

void
interval::scan_begin()
{
	/* assert(valid()); */
	/* assert(!scan_index); */
	scan_index = 1;
	if (length)
		scan_next_datum = data[0];
	else
		scan_next_datum = 0;
}


/*
 * NAME
 *	interval_scan_next
 *
 * SYNOPSIS
 *	int interval_scan_next(interval_ty *ip, interval_data_ty *datum);
 *
 * DESCRIPTION
 *	The interval_scan_next function is used to
 *	traverse every datum in the interval.
 *
 * ARGUMENTS
 *	ip	- interval to scan
 *	datum	- pointer to where to place datum
 *
 * RETURNS
 *	int	1 if datum available
 *		0 if reached end of interval
 */

bool
interval::scan_next(data_t &datum)
{
	/* assert(valid()); */
	/* assert(scan_index & 1); */
	if (scan_index >= length)
		return false;
	if (scan_next_datum >= promote(data[scan_index], scan_index))
	{
		scan_index += 2;
		if (scan_index >= length)
			return false;
		scan_next_datum = data[scan_index - 1];
	}
	datum = scan_next_datum++;
	return true;
}


/*
 * NAME
 *	interval_scan_end
 *
 * SYNOPSIS
 *	void interval_scan_end(interval_ty *ip);
 *
 * DESCRIPTION
 *	The interval_scan_end function is used to
 *	finish traversing every datum in the interval.
 *
 * ARGUMENTS
 *	ip	- interval to scan
 */

void
interval::scan_end()
{
	/* assert(valid()); */
	/* assert(scan_index & 1); */
	scan_index = 0;
	scan_next_datum = 0;
}


void
interval::first_interval_only()
{
	/* assert(valid()); */
	if (length > 2)
	{
		length = 2;
		data[length] = length;
	}
}


bool
interval::empty()
	const
{
	return (length == 0);
}


bool
interval::equal(const interval &lhs, const interval &rhs)
{
	if (lhs.length != rhs.length)
		return false;
	for (size_t j = 0; j < lhs.length; ++j)
		if (lhs.data[j] != rhs.data[j])
			return false;
	return true;
}


interval::data_t
interval::get_lowest()
	const
{
	/* assert(valid()); */
	return (length > 0 ? data[0] : 0);
}


interval::data_t
interval::get_highest()
	const
{
	/* assert(valid()); */
	return (length > 0 ? data[length - 1] : 0);
}


void
interval::print(ostream &os)
	const
{
	if (length != 2)
		os << "(";
	for (size_t j = 0; j < length; j += 2)
	{
		if (j)
			os << ", ";
		os << "(" << data[j] << ", " << (data[j + 1] - 1) << ")";
	}
	if (length != 2)
		os << ")";
}
