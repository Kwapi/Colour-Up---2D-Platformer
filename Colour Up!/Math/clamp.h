//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef CLAMP_H__
#define CLAMP_H__

#include <algorithm>


/**
Clamps \a x to be above the value of \a lower; equivalent to max(x, lower).
*/
template <typename T> inline const T & clampLower(const T &x, const T &lower)
{
	return std::max( x, lower );
}

/**
Clamps \a x to be below the value of \a upper; equivalent to min(x, upper).
*/
template <typename T> inline const T & clampUpper(const T &x, const T &upper)
{
	return std::min( x, upper );
}

/**
Clamps \a x to be within the range [\a lower-\a upper]; equivalent to min(max(x, lower), upper).
*/
template <typename T> inline const T & clamp(const T &x, const T &lower, const T &upper)
{
	return clampUpper( clampLower( x, lower ), upper );
}


#endif

