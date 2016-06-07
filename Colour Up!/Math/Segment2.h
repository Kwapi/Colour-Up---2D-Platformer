//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef SEGMENT2_H__
#define SEGMENT2_H__

#include <stdio.h>

#include <Math/clamp.h>

#include <Math/Point2.h>
#include <Math/Vector2.h>




/*
2D LINE
*/


template <typename Real> class Segment2
{
public:
	Point2<Real> a, b;


	inline Segment2()
	{
		a = b = Point2<Real>();
	}

	inline Segment2(const Point2<Real> &p1, const Point2<Real> &p2)
	{
		a = p1;
		b = p2;
	}

	inline Segment2(const Point2<Real> &p, const Vector2<Real> &direction)
	{
		a = p;
		b = p + direction;
	}

	template <typename S> inline Segment2(const Segment2<S> &s)
		: a( s.a ), b( s.b )
	{
	}



public:
	//get direction of line
	inline Vector2<Real> getDirection() const
	{
		return b - a;
	}

	//normal: if line is pointing upward, then the normal points to the left
	inline Vector2<Real> getNormal() const
	{
		//return the direction, rotated left by 90 degrees
		Vector2<Real> direction = getDirection();
		return Vector2<Real>( -direction.y, direction.x );
	}



	//get the mid-point
	inline Point2<Real> getMidPoint() const
	{
		return Point2<Real>::average( a, b );
	}


	//get a point on the line
	inline Point2<Real> getPoint(Real t) const
	{
		return Point2<Real>::lerp( a, b, t );
	}


	//return the closest point on this, to p
	inline Point2<Real> closestParamAndPointTo(const Point2<Real> &p, Real &t) const
	{
		Vector2<Real> ab = b - a;
		Vector2<Real> ap = p - a;

		//t is the position of p along ab
		//if t == 0: closest point is at a
		//if t == 1: closest point is at b
		t = ap.dot( ab )  /  ab.dot( ab );

		//clamp t to the region [0,1]; between a and b
		t = clamp( t, (Real)0.0, (Real)1.0 );
		return a  +  ab * t;
	}

	//return the closest point on this, to p
	inline Point2<Real> closestPointTo(const Point2<Real> &p) const
	{
		Real t;
		return closestPointTo( p, t );
	}

	//distance from the line to p
	inline Real distanceTo(const Point2<Real> &p) const
	{
		return p.distanceTo( closestPointTo( p ) );
	}

	//distance squared from the line to p
	inline Real sqrDistanceTo(const Point2<Real> &p) const
	{
		return p.sqrDistanceTo( closestPointTo( p ) );
	}


	//is p on the left side of the line?
	inline bool onLeft(const Point2<Real> &p) const
	{
		return Point2<Real>::areaOfTrianglex2( a, b, p )  >  (Real)0.0;
	}

	//is p on the line or on the left side of the line?
	inline bool onOrLeft(const Point2<Real> &p) const
	{
		return Point2<Real>::areaOfTrianglex2( a, b, p )  >=  (Real)0.0;
	}

	//is p on the right side of the line?
	inline bool onRight(const Point2<Real> &p) const
	{
		return Point2<Real>::areaOfTrianglex2( a, b, p )  <  (Real)0.0;
	}

	//is p on the line or on the right side of the line?
	inline bool onOrRight(const Point2<Real> &p) const
	{
		return Point2<Real>::areaOfTrianglex2( a, b, p )  <=  (Real)0.0;
	}

	//is p on the line?
	inline bool on(const Point2<Real> &p) const
	{
		return Point2<Real>::areaOfTrianglex2( a, b, p )  ==  (Real)0.0;
	}

	//does the line separate p and q
	inline bool separates(const Point2<Real> &p, const Point2<Real> &q) const
	{
		return Point2<Real>::separates( a, b, p, q );
	}


	//see if p is within the bounds of the line
	inline bool boundsContain(const Point2<Real> &p) const
	{
		//get the distance along the direction of p, a, and b
		Vector2<Real> dir = getDirection();
		Real pAlongLine = p.dot( dir );
		Real aAlongLine = a.dot( dir );
		Real bAlongLine = b.dot( dir );

		//p is between a and b
		return  ( pAlongLine >= aAlongLine  &&  pAlongLine <= bAlongLine )  ||
				  ( pAlongLine >= bAlongLine  &&  pAlongLine <= aAlongLine );
	}


	//compute the intersection between line and this
	//returns:
	//		true if intersection found

	//	t will contain the distance along the line of the intersection
	// intersection will contain the intersection point
	inline bool intersect(const Segment2<Real> &seg, Real &t, Point2<Real> &intersection) const
	{
		//need the normal of the other line, represent line as a normal
		//and distance from origin instead of two points
		Vector2<Real> normal = seg.getNormal();


		//raytrace
		Real nDotDirection = normal.dot( getDirection() );

		if ( nDotDirection == (Real)0.0 )
		{
			return false;
		}

		//distance from line to origin
		Real d = seg.a.dot( normal );

		t = ( d - a.dot( normal ) )  /  nDotDirection;
		intersection = a  +  ( getDirection() * t );

		//t must be between 0 and 1 (intersection between a and b)
		//check that the intersection is within the bounds of line
		return ( t >= (Real)0.0 )  &&  ( t <= (Real)1.0 )  &&
				 seg.boundsContain( intersection );
	}

	bool intersects(const Segment2<Real> &seg) const
	{
		return Point2<Real>::segmentsIntersect( a, b, seg.a, seg.b );
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


typedef Segment2<float> Segment2f;
typedef Segment2<double> Segment2d;


#endif
