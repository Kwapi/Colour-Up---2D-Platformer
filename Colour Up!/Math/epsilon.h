//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef EPSILON_H__
#define EPSILON_H__

#include <algorithm>


//define EPSILON for use in mathematics functions
#define EPSILON 1.0e-10
#define SQR_EPSILON (EPSILON * EPSILON)
#define ONEMINUSEPSILON (1.0 - EPSILON)
#define ONEPLUSEPSILON (1.0 + EPSILON)


/**
Returns true of \a a and a\ b are almost equal - if the absolute difference between them is less then EPSILON
*/
template <typename Real> bool almostEqualAbsolute(Real a, Real b)
{
	return (Real)fabs( a - b )  <  Real( EPSILON );
}

/**
Returns true of \a a and a\ b are almost equal - if the relative difference between them is less then EPSILON
*/
template <typename Real> bool almostEqualRelative(Real a, Real b)
{
	Real epsilon = std::max( (Real)fabs( a ), (Real)fabs( b ) )  *  EPSILON;
	return (Real)fabs( a - b )  <  epsilon;
}


#endif

