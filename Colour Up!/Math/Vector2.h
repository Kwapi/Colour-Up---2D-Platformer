//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef VECTOR2_H__
#define VECTOR2_H__

#include <math.h>

#include <algorithm>
#include <complex>



/**
Vector2: 2D vector

This is a template class, so 'Real' is the type used to store x and y co-ordinates.
Real would normally be float or double.
Vector2f and Vector2d are aliases for Vector2<float> and Vector2<double> respectively.


x and y co-ordinates can be accessed via:


a.x and a.y

OR

a.v[0] and a.v[1]


a.x is the same thing as a.v[0]
*/
template <typename Real> class Vector2
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
	Default constructor
	
	Creates a zero vector (0,0)
	*/
	inline Vector2() : x( (Real)0.0 ), y( (Real)0.0 )
	{
	}

	/**
	Constructor, takes x and y as parameters
	
	\param ix - x co-ordinate
	\param iy - y co-ordinate
	*/
	inline Vector2(Real ix, Real iy) : x( ix ), y( iy )
	{
	}

	/**
	Constructor, converts from a Vector2 with a different element type e.g. converts from a Vector2 that uses floats (Vector2<float> or Vector2f)
	to a Vector2 that uses doubles (Vector2<double> or Vector2d).
	*/
	template <typename S> inline Vector2(const Vector2<S> &v) : x( (Real)v.x ), y( (Real)v.y )
	{
	}



	/**
	Equality comparison
	*/
	inline bool operator==(const Vector2<Real> &v) const
	{
		return ( x == v.x )  &&  ( y  ==  v.y );
	}

	/**
	Inequality comparison
	*/
	inline bool operator!=(const Vector2<Real> &v) const
	{
		return ( x != v.x )  ||  ( y != v.y );
	}


	/**
	Addition operator:  Vector2d(a, b)  +  Vector2d(p, q)  ==  Vector2d(a+b, p+q)
	*/
	inline Vector2<Real> operator+(const Vector2<Real> &v) const
	{
		return Vector2<Real>( x + v.x,  y + v.y );
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
	Subtraction operator:  Vector2d(a, b)  -  Vector2d(p, q)  =  Vector2d(a-b, p-q)
	*/
	inline Vector2<Real> operator-(const Vector2<Real> &v) const
	{
		return Vector2<Real>( x - v.x,  y - v.y );
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
	Scale (multiply) a vector by a real number:  Vector2d(x, y) * s  ==  Vector2d(x*s, y*s)
	*/
	inline Vector2<Real> operator*(Real s) const
	{
		return Vector2<Real>( x * s,  y * s );
	}

	/**
	Scale (multiply) a vector by a real number - modifies \a this
	*/
	inline void operator*=(Real s)
	{
		x *= s;
		y *= s;
	}

	/**
	Negation operator:   -Vector2d(x, y)  =  Vector2d(-x, -y)
	*/
	inline Vector2<Real> operator-() const
	{
		return Vector2<Real>( -x, -y );
	}


	/**
	Dot product:  Vector2d(a, b).dot(Vector2d(p, q))  ==  a*p + b*q
	*/
	inline Real dot(const Vector2<Real> &v) const
	{
		return ( x * v.x )  +  ( y * v.y );
	}

	/**
	Cross product:  Vector2d(a, b).cross(Vector2d(p, q))  =  (a*q - b*p)
	*/
	inline Real cross(const Vector2<Real> &v) const
	{
		return x * v.y  -  y * v.x;
	}

	/**
	Square of length:  Vector2d(a,b).sqrLength()  ==  a*a + b*b  ==  |(a,b)|^2   
	*/
	inline Real sqrLength() const
	{
		return dot( *this );
	}

	/**
	Length:  Vector2d(a,b).length()  ==  sqrt(a*a + b*b)  ==  |(a,b)|
	*/
	inline Real length() const
	{
		return sqrt( sqrLength() );
	}

	/**
	Scale to length 1
	*/
	inline void normalise()
	{
		Real oneOverLength = (Real)1.0 / length();
		x *= oneOverLength;
		y *= oneOverLength;
	}

	/**
	Scale to length \a l
	*/
	inline void normaliseToLength(Real l)
	{
		Real lOverLength = l / length();
		x *= lOverLength;
		y *= lOverLength;
	}

	/**
	Scaled to length 1
	*/
	inline Vector2<Real> getNormalised() const
	{
		Real oneOverLength = (Real)1.0 / length();
		return Vector2<Real>( x * oneOverLength, y * oneOverLength );
	}

	/**
	Scaled to length \a l
	*/
	inline Vector2<Real> getNormalisedToLength(Real l) const
	{
		Real lOverLength = l / length();
		return Vector2<Real>( x * lOverLength, y * lOverLength );
	}


	/**
	Creates a vector perpendicular to \a this:  Vector2d(x, y).perpendicular()  ==  Vector2d(y, -x)
	*/
	inline Vector2<Real> perpendicular() const
	{
		return Vector2<Real>( y, -x );
	}


	/**
	'Projects' \a this onto a unit vector.
	
	Imagine an line passing through (0,0), whose direction is \a unitVector, that extends infinitely in both directions (negative and positive).
	\a this is 'projected' onto it - the result is the vector that represents the point on the infinite line that is closest to \a this.
	
	v.projectOntoUnitVector(u)  ==  u * v.dot(u)
	*/
	inline Vector2<Real> projectOntoUnitVector(const Vector2<Real> &unitVector) const
	{
		return unitVector  *  dot( unitVector );
	}


	/**
	Creates a vector that results from rotating \a this by 90 degrees counter-clockwise
	*/
	inline Vector2<Real> getRotated90CCW() const
	{
		return Vector2<Real>( -y, x );
	}

	/**
	Creates a vector that results from rotating \a this by 90 degrees clockwise
	*/
	inline Vector2<Real> getRotated90CW() const
	{
		return Vector2<Real>( y, -x );
	}


	/**
	Absolute value on a per-component basis: Vector2(a,b).abs()  ==  Vector2(abs(a), abs(b))
	*/
	inline Vector2<Real> abs() const
	{
		return Vector2<Real>( (Real)fabs( x ), (Real)fabs( y ) );
	}


	/**
	Polar argument - the 'angle' of this, counter-clockwise from the positive x-axis
	
	If \a this is converted to a complex number C, with \a x as the real part, and \a y as the imaginary part,
	argPolar() computes arg(C)
	*/
	inline Real argPolar() const
	{
		return std::arg( std::complex<Real>( x, y ) );
	}

	/**
	Determines if \a this is parallel with \a v
	*/
	inline bool isParallelWith(const Vector2<Real> &v)
	{
		Real d = dot( v );
		Real sqrLenProduct = sqrLength() * v.sqrLength();

		return d == sqrLenProduct  ||  d == -sqrLenProduct;
	}



	/**
	Component-wise min:  Vector2d::vectorMin(Vector2d(a, b), Vector2d(p, q)) == Vector2d(min(a,p), min(b,q))
	*/
	inline static Vector2<Real> vectorMin(const Vector2<Real> &a, const Vector2<Real> &b)
	{
		return Vector2<Real>( std::min( a.x, b.x ), std::min( a.y, b.y ) );
	}

	/**
	Component-wise max:  Vector2d::vectorMax(Vector2d(a, b), Vector2d(p, q)) == Vector2d(max(a,p), max(b,q))
	*/
	inline static Vector2<Real> vectorMax(const Vector2<Real> &a, const Vector2<Real> &b)
	{
		return Vector2<Real>( std::max( a.x, b.x ), std::max( a.y, b.y ) );
	}



	/**
	Read from a FILE \a f
	*/
	inline void read(FILE *f)
	{
		fread( &x, sizeof(Real), 1, f );
		fread( &y, sizeof(Real), 1, f );
	}

	/**
	Write to a FILE \a f
	*/
	inline void write(FILE *f) const
	{
		fwrite( &x, sizeof(Real), 1, f );
		fwrite( &y, sizeof(Real), 1, f );
	}
};



typedef Vector2<float> Vector2f;
//typedef Vector2<double> Vector2d;


#endif
