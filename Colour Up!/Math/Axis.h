//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef AXIS_H__
#define AXIS_H__


/**
Axis enumeration

Identifies one of the three axes (AXIS_X, AXIS_Y, & AXIS_Z), negative ones also (AXIS_NEGATIVE_X, AXIS_NEGATIVE_Y, AXIS_NEGATIVE_Z).
*/
enum Axis
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_NEGATIVE_X,
	AXIS_NEGATIVE_Y,
	AXIS_NEGATIVE_Z
};



/**
Returns true if \a a represents an axis in the positive direction.
*/
inline bool isAxisPositive(Axis a)
{
	return a == AXIS_X  ||  a == AXIS_Y  ||  a == AXIS_Z;
}

/**
Returns true if \a a represents an axis in the negative direction.
*/
inline bool isAxisNegative(Axis a)
{
	return a == AXIS_NEGATIVE_X  ||  a == AXIS_NEGATIVE_Y  ||  a == AXIS_NEGATIVE_Z;
}

/**
Returns the 'absolute' value of \a axis (AXIS_NEGATIVE_X becomes AXIS_X, etc).
*/
inline Axis absoluteAxis(Axis axis)
{
	switch( axis )
	{
	case AXIS_NEGATIVE_X:
		return AXIS_X;
	case AXIS_NEGATIVE_Y:
		return AXIS_Y;
	case AXIS_NEGATIVE_Z:
		return AXIS_Z;
	default:
		return axis;
	}
}

/**
Returns the 'negated' value of \a axis (AXIS_NEGATIVE_X becomes AXIS_X and vice versa, etc).
*/
inline Axis negateAxis(Axis axis)
{
	switch( axis )
	{
	case AXIS_X:
		return AXIS_NEGATIVE_X;
	case AXIS_NEGATIVE_X:
		return AXIS_X;
	case AXIS_Y:
		return AXIS_NEGATIVE_Y;
	case AXIS_NEGATIVE_Y:
		return AXIS_Y;
	case AXIS_Z:
		return AXIS_NEGATIVE_Z;
	case AXIS_NEGATIVE_Z:
		return AXIS_Z;
	default:
		return AXIS_Z;
	}
}


#endif
