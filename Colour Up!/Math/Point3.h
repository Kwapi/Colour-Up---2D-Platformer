//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef POINT3_H__
#define POINT3_H__

#include <algorithm>

#include "Math/Vector3.h"
#include "Math/Point2.h"



/**
Point3: Point/position in 3D space


This is a template class, so 'Real' is the type used to store x, y and z co-ordinates.
Real would normally be float or double.
Point3f and Point3d are aliases for Point3<float> and Point3<double> respectively.

**NOTE**: If you wish to represent an offset or direction, use the Vector3 class instead.



x, y and z co-ordinates can be accessed via:


a.x, a.y, and a.z

OR

a.v[0], a.v[1], and a.v[2]


a.x is the same thing as a.v[0]


Conversion from Vector3 is achieved using the constructor.

Conversion to Vector3 is achieved using the toVector3() method.
*/
template <typename Real> class Point3
{
public:
	union
	{
		struct
		{
			Real x, y, z;
		};

		Real v[3];
	};


	/**
	Default constructor: initialise to origin [0,0,0]
	*/
	inline Point3() : x( 0.0 ), y( 0.0 ), z( 0.0 )
	{
	}

	/**
	Constructor
	\param ix X-co-ordinate
	\param iy Y-co-ordinate
	\param iz Z-co-ordinate
	*/
	inline Point3(Real ix, Real iy, Real iz) : x( ix ), y( iy ), z( iz )
	{
	}

	/**
	Constructor: convert from Point3 with different Real number type
	*/
	template <typename S> inline Point3(const Point3<S> &p) : x( (Real)p.x ), y( (Real)p.y ), z( (Real)p.z )
	{
	}

	/**
	Constructor: convert from Point2
	*/
	inline Point3(const Point2<Real> &p2) : x( p2.x ), y( p2.y ), z( (Real)0.0 )
	{
	}

	/**
	Constructor: convert from Point2, z-co-ordinate passed in \a iz
	*/
	inline Point3(const Point2<Real> &p2, Real iz) : x( p2.x ), y( p2.y ), z( iz )
	{
	}

	/**
	Constructor: convert from Vector3
	*/
	inline Point3(const Vector3<Real> &v) : x( v.x ), y( v.y ), z( v.z )
	{
	}



	/**
	Equality test
	*/
	inline bool operator==(const Point3<Real> &p) const
	{
		return ( x == p.x )  &&  ( y == p.y )  &&  ( z == p.z );
	}

	/**
	Inequality test
	*/
	inline bool operator!=(const Point3<Real> &p) const
	{
		return ( x != p.x )  ||  ( y != p.y )  ||  ( z != p.z );
	}


	/**
	Addition operator:  Point3d(a, b, c)  +  Vector3d(p, q, r)  ==  Point3d(a+p, b+q, c+r)

	Point3 + Vector3 -> Point3
	*/
	inline Point3<Real> operator+(const Vector3<Real> &v) const
	{
		return Point3<Real>( x + v.x,  y + v.y,  z + v.z );
	}

	/**
	Addition operator - modifies \a this
	*/
	inline void operator+=(const Vector3<Real> &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	/**
	Subtraction operator:  Point3d(a, b, c)  -  Vector3d(p, q, r)  =  Point3d(a-b, p-q, c-r)

	Point3 - Vector3 -> Point3
	*/
	inline Point3<Real> operator-(const Vector3<Real> &v) const
	{
		return Point3<Real>( x - v.x,  y - v.y,  z - v.z );
	}

	/**
	Subtraction operator - modifies \a this
	*/
	inline void operator-=(const Vector3<Real> &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	/**
	Subtraction operator:  Point3d(a, b, c)  -  Point3d(p, q, r)  =  Vector3d(a-b, p-q, c-r)

	Point3 - Point3 -> Vector3
	*/
	inline Vector3<Real> operator-(const Point3<Real> &v) const
	{
		return Vector3<Real>( x - v.x,  y - v.y,  z - v.z );
	}

	/**
	Convert to Vector3
	*/
	inline Vector3<Real> toVector3() const
	{
		return Vector3<Real>( x, y, z );
	}

	/**
	Convert to Point2 - loses Z-co-ordinate
	*/
	inline Point2<Real> toPoint2() const
	{
		return Point2<Real>( x, y );
	}

	/**
	Convert to Point2 - effectively rotates \a axis to the +Z axis, then loses the z-co-ordinate
	*/
	inline Point2<Real> toPoint2Axis(Axis axis) const
	{
		switch( axis )
		{
		case AXIS_X:
			return Point2<Real>( -z, y );
		case AXIS_NEGATIVE_X:
			return Point2<Real>( z, y );
		case AXIS_Y:
			return Point2<Real>( x, -z );
		case AXIS_NEGATIVE_Y:
			return Point2<Real>( x, z );
		case AXIS_Z:
			return Point2<Real>( x, y );
		case AXIS_NEGATIVE_Z:
			return Point2<Real>( -x, y );
		default:
			return Point2<Real>( x, y );
		}
	}



	/**
	Dot product - Point3d(a, b, c).dot(Vector3d(p, q, r)) = a*p + b*q + c*r
	*/
	inline Real dot(const Vector3<Real> &v) const
	{
		return ( x * v.x ) + ( y * v.y ) + ( z * v.z );
	}

	/**
	Square of the distance to the point \a p
	*/
	inline Real sqrDistanceTo(const Point3<Real> &p) const
	{
		return ( *this - p ).sqrLength();
	}

	/**
	Distance to the point \a p
	*/
	inline Real distanceTo(const Point3<Real> &p) const
	{
		return ( *this - p ).length();
	}




	/**
	Get the component specified by \a axis. If \a axis is a negative axis, the component is negated.
	*/
	inline Real get(Axis axis) const
	{
		switch( axis )
		{
		case AXIS_X:
			return x;
		case AXIS_NEGATIVE_X:
			return -x;
		case AXIS_Y:
			return y;
		case AXIS_NEGATIVE_Y:
			return -y;
		case AXIS_Z:
			return z;
		case AXIS_NEGATIVE_Z:
			return -z;
		default:
			return (Real)0.0;
		}
	}

	/**
	Get the component specified by the absolute of \a axis - negative and positive axes result in the same.
	*/
	inline Real getAbs(Axis axis) const
	{
		switch( axis )
		{
		case AXIS_NEGATIVE_X:
		case AXIS_X:
			return x;
		case AXIS_NEGATIVE_Y:
		case AXIS_Y:
			return y;
		case AXIS_NEGATIVE_Z:
		case AXIS_Z:
			return z;
		default:
			return (Real)0.0;
		}
	}


	/**
	Set the component specified by \a axis. If \a axis is a negative axis, the component is negated.
	*/
	inline void set(Axis axis, Real v)
	{
		switch( axis )
		{
		case AXIS_X:
			x = v;
			break;
		case AXIS_NEGATIVE_X:
			x = -v;
			break;
		case AXIS_Y:
			y = v;
			break;
		case AXIS_NEGATIVE_Y:
			y = -v;
			break;
		case AXIS_Z:
			z = v;
			break;
		case AXIS_NEGATIVE_Z:
			z = -v;
			break;
		default:
			break;
		}
	}

	/**
	Set the component specified by the absolute of \a axis - negative and positive axes result in the same.
	*/
	inline void setAbs(Axis axis, Real v)
	{
		switch( axis )
		{
		case AXIS_NEGATIVE_X:
		case AXIS_X:
			x = v;
			break;
		case AXIS_NEGATIVE_Y:
		case AXIS_Y:
			y = v;
			break;
		case AXIS_NEGATIVE_Z:
		case AXIS_Z:
			z = v;
			break;
		default:
			break;
		}
	}


	/**
	Set the component specified by the \a axis to 0.
	*/
	inline void flatten(Axis axis)
	{
		set( axis, (Real)0.0 );
	}




	/**
	Cumulative point addition.

	p.cumulativeAdd(q);

	is equivalent to:

	p += q.toVector3();
	*/
	inline void cumulativeAdd(const Point3<Real> &p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
	}

	/**
	Point subtraction

	p.sub(q);

	is equivalent to:

	p -= q.toVector3();
	*/
	inline void sub(const Point3<Real> &p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
	}

	/**
	Scale by real


	p.scale(s);


	is equivalent to:


	p.x *= s;

	p.y *= s;

	p.z *= s;
	*/
	inline void scale(Real s)
	{
		x *= s;
		y *= s;
		z *= s;
	}


	/**
	Multiply by real
	*/
	inline static Point3<Real> mul(const Point3<Real> &p, Real s)
	{
		return Point3<Real>( p.x * s,  p.y * s,  p.z * s );
	}


	/**
	Add two points
	*/
	inline static Point3<Real> sum(const Point3<Real> &a, const Point3<Real> &b)
	{
		return Point3( a.x + b.x,  a.y + b.y,  a.z + b.z );
	}


	/**
	Component-wise minimum
	*/
	inline static Point3<Real> pointMin(const Point3<Real> &a, const Point3<Real> &b)
	{
		return Point3<Real>( std::min( a.x, b.x ), std::min( a.y, b.y ), std::min( a.z, b.z ) );
	}

	/**
	Component-wise maximum
	*/
	inline static Point3<Real> pointMax(const Point3<Real> &a, const Point3<Real> &b)
	{
		return Point3<Real>( std::max( a.x, b.x ), std::max( a.y, b.y ), std::max( a.z, b.z ) );
	}


	/**
	Average of (mid-point between) two points
	*/
	inline static Point3<Real> average(const Point3<Real> &a, const Point3<Real> &b)
	{
		return mul( sum( a, b ), (Real)0.5 );
	}


	/**
	Weighted Average of two points
	\param a - point A
	\param wa - weight of point A
	\param b - point B
	\param wb - weight of point B
	*/
	inline static Point3<Real> weightedAverage(const Point3<Real> &a, Real wa, const Point3<Real> &b, Real wb)
	{
		return Point3<Real>( a.x * wa  +  b.x * wb,
							a.y * wa  +  b.y * wb,
							a.z * wa  +  b.z * wb );
	}



	/**
	Normalised weighted Average of two points
	Like weightedAverage(), but scales \a wa and \a wb so that they sum to 1 first.
	*/
	inline static Point3<Real> normalisedWeightedAverage(const Point3<Real> &a, Real wa, const Point3<Real> &b, Real wb)
	{
		Real oneOverWTotal = (Real)1.0 / ( wa + wb );
		return weightedAverage( a, wa * oneOverWTotal,
							b, wb * oneOverWTotal );
	}


	/**
	Interpolate from \a a to \a b, using parameter \a t.
	
	a + (b-a)*t
	*/
	inline static Point3<Real> lerp(const Point3<Real> &a, const Point3<Real> &b, Real t)
	{
		return a  +  ( b - a ) * t;
	}



	/**
	Read from FILE \a f.
	*/
	inline void read(FILE *f)
	{
		fread( &x, sizeof(Real), 1, f );
		fread( &y, sizeof(Real), 1, f );
		fread( &z, sizeof(Real), 1, f );
	}

	/**
	Write to FILE \a f.
	*/
	inline void write(FILE *f) const
	{
		fwrite( &x, sizeof(Real), 1, f );
		fwrite( &y, sizeof(Real), 1, f );
		fwrite( &z, sizeof(Real), 1, f );
	}
};


typedef Point3<float> Point3f;
typedef Point3<double> Point3d;


#endif
