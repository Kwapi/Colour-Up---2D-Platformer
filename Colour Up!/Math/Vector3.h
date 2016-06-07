//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef VECTOR3_H__
#define VECTOR3_H__

#include <math.h>

#include <algorithm>
#include <complex>

#include "Math/Axis.h"
#include "Math/Vector2.h"





/**
Vector3: 3D vector


This is a template class, so 'Real' is the type used to store x, y and z co-ordinates.
Real would normally be float or double.
Vector3f and Vector3d are aliases for Vector3<float> and Vector3<double> respectively.

**NOTE**: If you wish to represent a point or position, use the Point3 class instead.


x, y and z co-ordinates can be accessed via:


a.x, a.y, and a.z

OR

a.v[0], a.v[1], and a.v[2]


a.x is the same thing as a.v[0]


Converting a Vector3 to a Point3 is achieved through the Point3 constructor.
*/
template <typename Real> class Vector3
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
	Default constructor
	
	Creates a zero vector (0,0,0)
	*/
	inline Vector3() : x( (Real)0.0 ), y( (Real)0.0 ), z( (Real)0.0 )
	{
	}

	/**
	Constructor, takes x, y, and z as parameters
	
	\param ix - x co-ordinate
	\param iy - y co-ordinate
	\param iz - z co-ordinate
	*/
	inline Vector3(Real ix, Real iy, Real iz) : x( ix ), y( iy ), z( iz )
	{
	}

	/**
	Constructor, converts from a Vector3 with a different element type e.g. converts from a Vector3 that uses floats (Vector3<float> or Vector3f)
	to a Vector3 that uses doubles (Vector3<double> or Vector3d).
	*/
	template <typename S> inline Vector3(const Vector3<S> &v) : x( (Real)v.x ), y( (Real)v.y ), y( (Real)v.z )
	{
	}

	/**
	Constructor that builds a Vector3 from a Vector2, with z = 0
	*/
	inline Vector3(const Vector2<Real> &v) : x( v.x ), y( v.y ), z( (Real)0.0 )
	{
	}

	/**
	Constructor that builds a Vector3 from a Vector2, with z supplied as a parameter
	*/
	inline Vector3(const Vector2<Real> &v, Real iz) : x( v.x ), y( v.y ), z( iz )
	{
	}

	/**
	Constructor that builds a Vector3 from an Axis:
	AXIS_X  ->  Vector3(1,0,0)
	AXIS_Y  ->  Vector3(0,1,0)
	AXIS_Z  ->  Vector3(0,0,1)
	AXIS_NEGATIVE_X  ->  Vector3(-1,0,0)
	AXIS_NEGATIVE_Y  ->  Vector3(0,-1,0)
	AXIS_NEGATIVE_Z  ->  Vector3(0,0,-1)
	*/
	inline Vector3(Axis a)
	{
		switch ( a )
		{
		case AXIS_X:
			x = (Real)1.0;
			y = (Real)0.0;
			z = (Real)0.0;
			break;
		case AXIS_Y:
			x = (Real)0.0;
			y = (Real)1.0;
			z = (Real)0.0;
			break;
		case AXIS_Z:
			x = (Real)0.0;
			y = (Real)0.0;
			z = (Real)1.0;
			break;
		case AXIS_NEGATIVE_X:
			x = (Real)-1.0;
			y = (Real)0.0;
			z = (Real)0.0;
			break;
		case AXIS_NEGATIVE_Y:
			x = (Real)0.0;
			y = (Real)-1.0;
			z = (Real)0.0;
			break;
		case AXIS_NEGATIVE_Z:
			x = (Real)0.0;
			y = (Real)0.0;
			z = (Real)-1.0;
			break;
		default:
			x = (Real)0.0;
			y = (Real)0.0;
			z = (Real)0.0;
			break;
		}
	}



	/**
	Equality comparison
	*/
	inline bool operator==(const Vector3<Real> &v) const
	{
		return ( x == v.x )  &&  ( y  ==  v.y )  &&  ( z == v.z );
	}

	/**
	Inequality comparison
	*/
	inline bool operator!=(const Vector3<Real> &v) const
	{
		return ( x != v.x )  ||  ( y != v.y )  ||  ( z != v.z );
	}


	/**
	Addition operator:  Vector3d(a, b, c)  +  Vector3d(p, q, r)  ==  Vector3d(a+p, b+q, c+r)
	*/
	inline Vector3<Real> operator+(const Vector3<Real> &v) const
	{
		return Vector3<Real>( x + v.x,  y + v.y,  z + v.z );
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
	Subtraction operator:  Vector3d(a, b, c)  -  Vector3d(p, q, r)  =  Vector3d(a-b, p-q, c-r)
	*/
	inline Vector3<Real> operator-(const Vector3<Real> &v) const
	{
		return Vector3( x - v.x,  y - v.y,  z - v.z );
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
	Scale (multiply) a vector by a real number:  Vector3d(x, y, z) * s  ==  Vector3d(x*s, y*s, z*s)
	*/
	inline Vector3<Real> operator*(Real s) const
	{
		return Vector3<Real>( x * s,  y * s,  z * s );
	}

	/**
	Scale (multiply) a vector by a real number - modifies \a this
	*/
	inline void operator*=(Real s)
	{
		x *= s;
		y *= s;
		z *= s;
	}

	/**
	Negation operator:   -Vector3d(x, y, z)  =  Vector3d(-x, -y, -z)
	*/
	inline Vector3<Real> operator-() const
	{
		return Vector3<Real>( -x, -y, -z );
	}


	/**
	Dot product:  Vector3d(a, b, c).dot(Vector3d(p, q, r))  ==  a*p + b*q + c*r
	*/
	inline Real dot(const Vector3<Real> &v) const
	{
		return ( x * v.x )  +  ( y * v.y )  +  ( z * v.z );
	}

	/**
	Cross product:  Vector3d(a, b, c).cross(Vector3d(p, q, r)) == Vector3d(b*r - c*q,  c*p - a*r,  a*q - b*p)
	*/
	inline Vector3<Real> cross(const Vector3<Real> &v) const
	{
		return Vector3<Real>( y * v.z  -  z * v.y,
							 z * v.x  -  x * v.z,
							 x * v.y  -  y * v.x );
	}

	/**
	Component-wise vector multiplication:  Vector3d(a, b, c).mul(Vector3d(p, q, r))  ==  Vector3d(a*p, b*q, c*r)
	*/
	inline Vector3<Real> mul(const Vector3<Real> &v) const
	{
		return Vector3( x * v.x,  y * v.y,  z * v.z );
	}

	/**
	Square of length:  Vector3d(a, b, c).sqrLength()  ==  a*a + b*b + c*c  ==  |(a,b,c)|^2   
	*/
	inline Real sqrLength() const
	{
		return dot( *this );
	}

	/**
	Length:  Vector3d(a, b, c).length()  ==  sqrt(a*a + b*b + c*c)  ==  |(a,b,c)|
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
		z *= oneOverLength;
	}

	/**
	Scale to length \a l
	*/
	inline void normaliseToLength(Real l)
	{
		Real lOverLength = l / length();
		x *= lOverLength;
		y *= lOverLength;
		z *= lOverLength;
	}

	/**
	Scaled to length 1
	*/
	inline Vector3<Real> getNormalised() const
	{
		Real oneOverLength = (Real)1.0 / length();
		return Vector3<Real>( x * oneOverLength, y * oneOverLength, z * oneOverLength );
	}

	/**
	Scaled to length \a l
	*/
	inline Vector3<Real> getNormalisedToLength(Real l) const
	{
		Real lOverLength = l / length();
		return Vector3<Real>( x * lOverLength, y * lOverLength, z * lOverLength );
	}


	/**
	'Projects' \a this onto a unit vector.
	
	Imagine an line passing through (0,0,0), whose direction is \a unitVector, that extends infinitely in both directions (negative and positive).
	\a this is 'projected' onto it - the result is the vector that represents the point on the infinite line that is closest to \a this.
	
	v.projectOntoUnitVector(u)  ==  u * v.dot(u)
	*/
	inline Vector3<Real> projectOntoUnitVector(const Vector3<Real> &unitVector) const
	{
		return unitVector  *  dot( unitVector );
	}

	/**
	'Projects' \a this onto a plane
	
	The result is the vector that represents the point on the plane that is closest to \a this

	v.projectOntoPlane(unitNormal)  ==  v - v.projectOntoUnitVector(unitNormal)
	*/
	inline Vector3<Real> projectOntoPlane(const Vector3<Real> &unitNormal) const
	{
		return *this  -  projectOntoUnitVector( unitNormal );
	}


	/**
	Absolute value on a per-component basis: Vector3d(a,b,c).abs()  ==  Vector2d(abs(a), abs(b), abs(c))
	*/
	inline Vector3<Real> abs() const
	{
		return Vector3<Real>( (Real)fabs( x ), (Real)fabs( y ), (Real)fabs( z ) );
	}


	/**
	Returns the axis whose absolute value is largest.
	For Vector3d(x, y, z):
	AXIS_X   if abs(x) > abs(y) and abs(x) > abs(z)
	AXIS_Y   if abs(y) > abs(x) and abs(y) > abs(z)
	AXIS_Z   if abs(z) > abs(x) and abs(z) > abs(y)
	*/
	inline Axis getMajorAxis() const
	{
		Real absX = (Real)fabs( x );
		Real absY = (Real)fabs( y );
		Real absZ = (Real)fabs( z );

		if ( absX > absY )
		{
			if ( absX > absZ )
			{
				return AXIS_X;
			}
			else
			{
				return AXIS_Z;
			}
		}
		else
		{
			if ( absY > absZ )
			{
				return AXIS_Y;
			}
			else
			{
				return AXIS_Z;
			}
		}
	}

	/**
	Returns the axis that is most similar to \a this
	For Vector3d(x, y, z):
	AXIS_X   if abs(x) > abs(y) and abs(x) > abs(z)   and   x >= 0
	AXIS_Y   if abs(y) > abs(x) and abs(y) > abs(z)   and   y >= 0
	AXIS_Z   if abs(z) > abs(x) and abs(z) > abs(y)   and   z >= 0
	AXIS_NEGATIVE_X   if abs(x) > abs(y) and abs(x) > abs(z)   and   x < 0
	AXIS_NEGATIVE_Y   if abs(y) > abs(x) and abs(y) > abs(z)   and   y < 0
	AXIS_NEGATIVE_Z   if abs(z) > abs(x) and abs(z) > abs(y)   and   z < 0
	*/
	inline Axis getMajorSignedAxis() const
	{
		Real absX = (Real)fabs( x );
		Real absY = (Real)fabs( y );
		Real absZ = (Real)fabs( z );

		if ( absX > absY )
		{
			if ( absX > absZ )
			{
				if ( x < (Real)0.0 )
				{
					return AXIS_NEGATIVE_X;
				}
				else
				{
					return AXIS_X;
				}
			}
			else
			{
				if ( z < (Real)0.0 )
				{
					return AXIS_NEGATIVE_Z;
				}
				else
				{
					return AXIS_Z;
				}
			}
		}
		else
		{
			if ( absY > absZ )
			{
				if ( y < (Real)0.0 )
				{
					return AXIS_NEGATIVE_Y;
				}
				else
				{
					return AXIS_Y;
				}
			}
			else
			{
				if ( z < (Real)0.0 )
				{
					return AXIS_NEGATIVE_Z;
				}
				else
				{
					return AXIS_Z;
				}
			}
		}
	}

	/**
	Returns the axis whose absolute value is smallest.
	For Vector3d(x, y, z):
	AXIS_X   if abs(x) < abs(y) and abs(x) < abs(z)
	AXIS_Y   if abs(y) < abs(x) and abs(y) < abs(z)
	AXIS_Z   if abs(z) < abs(x) and abs(z) < abs(y)
	*/
	inline Axis getMinorAxis() const
	{
		Real absX = (Real)fabs( x );
		Real absY = (Real)fabs( y );
		Real absZ = (Real)fabs( z );

		if ( absX < absY )
		{
			if ( absX < absZ )
			{
				return AXIS_X;
			}
			else
			{
				return AXIS_Z;
			}
		}
		else
		{
			if ( absY < absZ )
			{
				return AXIS_Y;
			}
			else
			{
				return AXIS_Z;
			}
		}
	}

	/**
	Returns the Axis that is most perpendicular to \a this, equivalent to getMinorAxis()
	*/
	inline Axis perpendicularAxis() const
	{
		return getMinorAxis();
	}

	/**
	Returns a vector that is perpendicular to \a this.
	
	u.perpendicularVector()  ==  Vector3d(u.perpendicularAxis()).cross(u)
	*/
	inline Vector3<Real> perpendicularVector() const
	{
		return Vector3<Real>( perpendicularAxis() ).cross( *this );
	}

	/**
	Returns the vector that is closest to \a this, but perpendicular to \a axis
		
	v.perpendicularTo(axis) == axis.cross(v).cross(axis)
	*/
	inline Vector3<Real> perpendicularTo(const Vector3<Real> &axis) const
	{
		Vector3<Real> u = axis.cross( *this );
		return u.cross( axis );
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
	Project \a this into 2D by looking from the direction specified by \a axis, toward the origin, and compute the polar angle.
	*/
	inline Real argPolar(Axis axis)
	{
		switch( axis )
		{
		case AXIS_X:
			return std::arg( std::complex<Real>( -z, y ) );
		case AXIS_NEGATIVE_X:
			return -std::arg( std::complex<Real>( -z, y ) );
		case AXIS_Y:
			return std::arg( std::complex<Real>( x, -z ) );
		case AXIS_NEGATIVE_Y:
			return -std::arg( std::complex<Real>( x, -z ) );
		case AXIS_Z:
			return std::arg( std::complex<Real>( x, y ) );
		case AXIS_NEGATIVE_Z:
			return -std::arg( std::complex<Real>( x, y ) );
		default:
			return (Real)0.0;
		}
	}


	/**
	Convert to Vector2<Real> - z-component dropped
	*/
	inline Vector2<Real> toVector2() const
	{
		return Vector2<Real>( x, y );
	}

	/**
	Convert to Vector2<Real> - flattens in direction \a axis
	*/
	inline Vector2<Real> toVector2Axis(Axis axis) const
	{
		switch( axis )
		{
		case AXIS_X:
			return Vector2<Real>( -z, y );
		case AXIS_NEGATIVE_X:
			return Vector2<Real>( z, y );
		case AXIS_Y:
			return Vector2<Real>( x, -z );
		case AXIS_NEGATIVE_Y:
			return Vector2<Real>( x, z );
		case AXIS_Z:
			return Vector2<Real>( x, y );
		case AXIS_NEGATIVE_Z:
			return Vector2<Real>( -x, y );
		default:
			return Vector2<Real>( x, y );
		}
	}


	/**
	Component-wise min:  Vector3d::vectorMin(Vector3d(a, b), Vector3d(p, q)) == Vector3d(min(a,p), min(b,q))
	*/
	static Vector3<Real> vectorMin(const Vector3<Real> &a, const Vector3<Real> &b)
	{
		return Vector3<Real>( std::min( a.x, b.x ), std::min( a.y, b.y ), std::min( a.z, b.z ) );
	}

	/**
	Component-wise max:  Vector3d::vectorMax(Vector3d(a, b), Vector3d(p, q)) == Vector3d(max(a,p), max(b,q))
	*/
	static Vector3<Real> vectorMax(const Vector3<Real> &a, const Vector3<Real> &b)
	{
		return Vector3<Real>( std::max( a.x, b.x ), std::max( a.y, b.y ), std::max( a.z, b.z ) );
	}



	/**
	Read from a FILE \a f
	*/
	inline void read(FILE *f)
	{
		fread( &x, sizeof(Real), 1, f );
		fread( &y, sizeof(Real), 1, f );
		fread( &z, sizeof(Real), 1, f );
	}

	/**
	Write to a FILE \a f
	*/
	inline void write(FILE *f) const
	{
		fwrite( &x, sizeof(Real), 1, f );
		fwrite( &y, sizeof(Real), 1, f );
		fwrite( &z, sizeof(Real), 1, f );
	}
};


typedef Vector3<float> Vector3f;
//typedef Vector3<double> Vector3d;





#endif
