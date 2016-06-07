//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef SEGMENT3_H__
#define SEGMENT3_H__

#include <stdio.h>

#include <Math/clamp.h>

#include <Math/Point3.h>
#include <Math/Plane.h>
#include <Math/Vector3.h>
#include <Math/Segment2.h>
#include <Math/Side.h>

/*
3D LINE SEGMENT
*/



template <typename Real> class Segment3
{
public:
	Point3<Real> a, b;


	inline Segment3()
	{
		a = b = Point3<Real>();
	}

	inline Segment3(const Point3<Real> &p1, const Point3<Real> &p2)
	{
		a = p1;
		b = p2;
	}

	inline Segment3(const Point3<Real> &p, const Vector3<Real> &direction)
	{
		a = p;
		b = p + direction;
	}

	template <typename S> inline Segment3(const Segment3<S> &s)
		: a( s.a ), b( s.b )
	{
	}



public:
	Segment2<Real> toSegment2() const
	{
		return Segment2<Real>( a.toPoint2(), b.toPoint2() );
	}




	//get direction of line
	inline Vector3<Real> getDirection() const
	{
		return b - a;
	}


	//get length of line
	inline Real length() const
	{
		return a.distanceTo( b );
	}

	inline Real sqrLength() const
	{
		return a.sqrDistanceTo( b );
	}


	//translation
	inline Segment3<Real> operator+(const Vector3<Real> &v) const
	{
		return Segment3<Real>( a + v, b + v );
	}

	inline void operator+=(const Vector3<Real> &v)
	{
		a += v;
		b += v;
	}

	inline Segment3<Real> operator-(const Vector3<Real> &v) const
	{
		return Segment3( a - v, b - v );
	}

	inline void operator-=(const Vector3<Real> &v)
	{
		a -= v;
		b -= v;
	}


	//get the closest point on the line to p
	inline Point3<Real> closestParamAndPointTo(const Point3<Real> &p, Real &t) const
	{
		Vector3<Real> ab = getDirection();
		Vector3<Real> ap = p - a;

		//t is the position of p along ab
		//if t == 0: closest point is at a
		//if t == 1: closest point is at b
		t = ap.dot( ab )  /  ab.dot( ab );

		t = clamp( t, (Real)0.0, (Real)1.0 );

		return a  +  ab * t;
	}

	inline Point3<Real> closestPointTo(const Point3<Real> &p) const
	{
		Real t;
		return closestPointTo( p, t );
	}

	//distance from the line to p
	inline Real distanceTo(const Point3<Real> &p) const
	{
		return p.distanceTo( closestPointTo( p ) );
	}

	//distance squared from the line to p
	inline Real sqrDistanceTo(const Point3<Real> &p) const
	{
		return p.sqrDistanceTo( closestPointTo( p ) );
	}


	//compute the intersection between the plane p and this
	//returns:
	//		true if intersection found

	//	t will contain the distance along the line of the intersection
	// intersection will contain the intersection point
	inline bool intersectPlane(const Plane<Real> &p, Real &t, Point3<Real> &intersection) const
	{
		//raytrace the line against the plane
		Real nDotDirection = p.n.dot( getDirection() );

		if ( nDotDirection == (Real)0.0 )
		{
			return false;
		}

		t = ( p.d - a.dot( p.n ) ) / nDotDirection;

		if ( t >= (Real)0.0  &&  t <= (Real)1.0 )
		{
			intersection = a + ( getDirection() * t );
			return true;
		}
		else
		{
			return false;
		}
	}


	//clip this so that it is wholly on the positive side of p
	bool clipWithPlane(const Plane<Real> &p)
	{
		Real aDotN, bDotN;
		Real t;
		Point3<Real> intersection;



		//find the distance along the plane normal, of a and b
		aDotN = a.dot( p.n );
		bDotN = b.dot( p.n );

		//if aDotN == bDotN, then the line lies on p, so return true
		//if one of a point on the line is on the positive side of p
		if ( aDotN == bDotN )
		{
			return a >= p;
		}

		//need to clip: compute t:
		//t is the distance of the intersection between the line and p,
		//along the line
		t = ( p.d - aDotN ) / ( bDotN - aDotN );

		if ( t <= (Real)0.0 )		//intersection before a: all of line on one side of p
		{
			return b >= p;		//if b is on +ve side of p, then so is all of the line
		}
		else if ( t >= (Real)1.0 )	//intersection after b: all of line on one side of p
		{
			return a >= p;		//if a is on +ve side of p, then so is all of the line
		}
		else
		{
			//line segment straddles plane
			//compute the intersection point
			intersection = a  +  ( getDirection() * t );

			if ( aDotN < p.d )		//a on -ve side of plane
			{
				a = intersection;		//replace a with intersection point
			}
			else if ( aDotN > p.d )	//a on +ve side of plane
			{
				b = intersection;		//replace b with intersection point
			}
			//else a is on plane, no need to replace a with intersection point
			return true;
		}
	}


	//translate the segment by @v
	inline void translate(const Vector3<Real> &v)
	{
		a += v;
		b += v;
	}

	//scale the line by @s
	inline void scale(Real s)
	{
		//scale the start and end points
		a.scale( s );
		b.scale( s );
	}



	//get the mid-point
	inline Point3<Real> getMidPoint() const
	{
		return Point3<Real>::average( a, b );
	}


	//get a point on the line
	inline Point3<Real> getPoint(Real t) const
	{
		return Point3<Real>::lerp( a, b, t );
	}


	//compute which side of the plane *this is on
	inline Side side(const Plane<Real> &p) const
	{
		if ( a == p  &&  b == p )
		{
			return SIDE_ON;
		}
		else if ( a >= p  &&  b >= p )
		{
			return SIDE_POSITIVE;
		}
		else if ( a <= p  &&  b <= p )
		{
			return SIDE_NEGATIVE;
		}
		else
		{
			return SIDE_BOTH;
		}
	}



	//read and write methods
	inline void read(FILE *f)
	{
		a.read( f );
		b.read( f );
	}

	inline void write(FILE *f) const
	{
		a.write( f );
		b.write( f );
	}
};



typedef Segment3<float> Segment3f;
typedef Segment3<double> Segment3d;



#endif
