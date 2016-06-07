//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef POINT2_H__
#define POINT2_H__

#include "Math/epsilon.h"

#include "Math/Vector2.h"



/**
Point2: Point/position in 2D space


This is a template class, so 'Real' is the type used to store x and y co-ordinates.
Real would normally be float or double.
Point2f and Point2d are aliases for Point2<float> and Point2<double> respectively.

**NOTE**: If you wish to represent an offset or direction, use the Vector2 class instead.


x and y co-ordinates can be accessed via:


a.x, a.y

OR

a.v[0] and a.v[1]


a.x is the same thing as a.v[0]


Conversion from Vector2 is achieved using the constructor.

Conversion to Vector2 is achieved using the toVector2() method.
*/
template <typename Real> class Point2
{
public:
	union
	{
		struct
		{
			Real x, y;
		};

		Real v[2];
	};


	/**
	Default constructor: initialise to origin [0,0]
	*/
	inline Point2() : x( 0.0 ), y( 0.0 )
	{
	}

	/**
	Constructor
	\param ix X-co-ordinate
	\param iy Y-co-ordinate
	*/
	inline Point2(Real ix, Real iy) : x( ix ), y( iy )
	{
	}

	/**
	Constructor: convert from Point2 with different Real number type
	*/
	template <typename S> inline Point2(const Point2<S> &v) : x( (Real)v.x ), y( (Real)v.y )
	{
	}

	/**
	Constructor: convert from Vector2
	*/
	inline Point2(const Vector2<Real> &v) : x( v.x ), y( v.y )
	{
	}




	/**
	Equality test
	*/
	inline bool operator==(const Point2<Real> &p) const
	{
		return ( x == p.x )  &&  ( y == p.y );
	}

	/**
	Inequality test
	*/
	inline bool operator!=(const Point2<Real> &p) const
	{
		return ( x != p.x )  ||  ( y != p.y );
	}



	/**
	Addition operator:  Point2d(a, b)  +  Vector2d(p, q)  ==  Point2d(a+p, b+q)

	Point2 + Vector2 -> Point2
	*/
	inline Point2<Real> operator+(const Vector2<Real> &v) const
	{
		return Point2<Real>( x + v.x,  y + v.y );
	}

	/**
	Addition operator - modifies \a this
	*/
	inline void operator+=(const Vector2<Real> &v)
	{
		x += v.x;
		y += v.y;
	}

	/**
	Subtraction operator:  Point2d(a, b)  -  Vector2d(p, q)  =  Point2d(a-b, p-q)

	Point2 - Vector2 -> Point2
	*/
	inline Point2<Real> operator-(const Vector2<Real> &v) const
	{
		return Point2<Real>( x - v.x,  y - v.y );
	}

	/**
	Subtraction operator - modifies \a this
	*/
	inline void operator-=(const Vector2<Real> &v)
	{
		x -= v.x;
		y -= v.y;
	}

	/**
	Subtraction operator:  Point2d(a, b)  -  Point2d(p, q)  =  Vector2d(a-b, p-q)

	Point2 - Point2 -> Vector2
	*/
	inline Vector2<Real> operator-(const Point2<Real> &v) const
	{
		return Vector2<Real>( x - v.x,  y - v.y );
	}

	/**
	Convert to Vector2
	*/
	inline Vector2<Real> toVector2() const
	{
		return Vector2<Real>( x, y );
	}

	/**
	Dot product - Point2d(a, b).dot(Vector2d(p, q)) = a*p + b*q
	*/
	inline Real dot(const Vector2<Real> &v) const
	{
		return ( x * v.x ) + ( y * v.y );
	}

	/**
	Square of the distance to the point \a p
	*/
	inline Real sqrDistanceTo(const Point2<Real> &p) const
	{
		return ( *this - p ).sqrLength();
	}

	/**
	Distance to the point \a p
	*/
	inline Real distanceTo(const Point2<Real> &p) const
	{
		return ( *this - p ).length();
	}


	/**
	Double the area of the triangle defined by the points \a a, \a b, and \a c.
	*/
	inline static Real areaOfTriangleTimes2(const Point2<Real> &a, const Point2<Real> &b, const Point2<Real> &c)
	{
		return ( b - a ).cross( c - a );
	}

	/**
	On-left test: returns true if \a this would be to your left if you walked from \a a to \a b.
	*/
	inline bool isOnLeft(const Point2<Real> &a, const Point2<Real> &b)
	{
		return areaOfTriangleTimes2( a, b, *this )  >  0.0;
	}

	/**
	On-right test: returns true if \a this would be to your right if you walked from \a a to \a b.
	*/
	inline bool isOnRight(const Point2<Real> &a, const Point2<Real> &b)
	{
		return areaOfTrianglex2( a, b, *this )  <  0.0;
	}

	/**
	Separation test: returns true if \a p and \a q are on different sides of the line defined by the points \a lineA and \a lineB.
	*/
	inline static bool separates(const Point2<Real> &lineA, const Point2<Real> &lineB, const Point2<Real> &p, const Point2<Real> &q)
	{
		Real sqrDist = lineA.sqrDistanceTo( lineB );
		Real tolerence = sqrDist * EPSILON;

		Real pArea = Point2<Real>::areaOfTrianglex2( lineA, lineB, p );
		Real qArea = Point2<Real>::areaOfTrianglex2( lineA, lineB, q );

		if ( ( pArea * pArea )  <  tolerence  ||
			  ( qArea * qArea )  <  tolerence )
		{
			//within tolerence; no separation
			return false;
		}
		else
		{
			//check if they are on either side of the line
			return ( pArea < (Real)0.0  &&  qArea > (Real)0.0 )  ||
					 ( pArea > (Real)0.0  &&  qArea < (Real)0.0 );
		}
	}

	/**
	Line segment intersection test: returns true if the line segments \a a-\a b and \a p-\a q intersect.
	*/
	inline static bool segmentsIntersect(const Point2<Real> &a, const Point2<Real> &b, const Point2<Real> &p, const Point2<Real> &q)
	{
		return separates( a, b, p, q )  &&  separates( p, q, a, b );
	}




	/**
	Multiply by real
	*/
	inline static Point2<Real> mul(const Point2<Real> &p, Real s)
	{
		return Point2<Real>( p.x * s,  p.y * s );
	}



	/**
	Add two points
	*/
	inline static Point2<Real> sum(const Point2<Real> &a, const Point2<Real> &b)
	{
		return Point2<Real>( a.x + b.x,  a.y + b.y );
	}


	/**
	Component-wise minimum
	*/
	inline static Point2<Real> pointMin(const Point2<Real> &a, const Point2<Real> &b)
	{
		return Point2<Real>( std::min( a.x, b.x ), std::min( a.y, b.y ) );
	}

	/**
	Component-wise maximum
	*/
	inline static Point2<Real> pointMax(const Point2<Real> &a, const Point2<Real> &b)
	{
		return Point2<Real>( std::max( a.x, b.x ), std::max( a.y, b.y ) );
	}



	/**
	Average of (mid-point between) two points
	*/
	inline static Point2<Real> average(const Point2<Real> &a, const Point2<Real> &b)
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
	inline static Point2<Real> weightedAverage(const Point2<Real> &a, Real wa, const Point2<Real> &b, Real wb)
	{
		return Point2<Real>( a.x * wa  +  b.x * wb,
					a.y * wa  +  b.y * wb );
	}

	/**
	Normalised weighted Average of two points
	Like weightedAverage(), but scales \a wa and \a wb so that they sum to 1 first.
	*/
	inline static Point2<Real> normalisedWeightedAverage(const Point2<Real> &a, Real wa, const Point2<Real> &b, Real wb)
	{
		Real oneOverWTotal = (Real)1.0 / ( wa + wb );
		return weightedAverage( a, wa * oneOverWTotal,
							b, wb * oneOverWTotal );
	}


	/**
	Interpolate from \a a to \a b, using parameter \a t.
	
	a + (b-a)*t
	*/
	inline static Point2<Real> lerp(const Point2<Real> &a, const Point2<Real> &b, Real t)
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
	}

	/**
	Write to FILE \a f.
	*/
	inline void write(FILE *f) const
	{
		fwrite( &x, sizeof(Real), 1, f );
		fwrite( &y, sizeof(Real), 1, f );
	}
};


typedef Point2<float> Point2f;
typedef Point2<double> Point2d;



#endif
