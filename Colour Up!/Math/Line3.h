//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef LINE3_H__
#define LINE3_H__

#include <Math/Point3.h>
#include <Math/Plane.h>
#include <Math/Vector3.h>



/**
Line3: a line passing through a point (\a origin), extending infinitely along a vector (\a direction), in both directions.

This is a template class, so 'Real' is the type used to store real numbers.
Line3f and Line3d alias Line3<float> and Line3<double> respectively.
*/
template <typename Real> class Line3
{
public:
	Point3<Real> origin;
	Vector3<Real> direction;


	/**
	Default constructor: constructs a line that lies on the X-axis, and passes through the origin.
	*/
	inline Line3()
		: direction(Vector3<Real>((Real)1.0, (Real)0.0, (Real)0.0))
	{
	}

	/**
	Constructor: line passes through two points
	\param p1 first point
	\param p2 secont point
	*/
	inline Line3(const Point3<Real> &p1, const Point3<Real> &p2)
			: origin( p1 ), direction( p2 - p1 )
	{
	}

	/**
	Constructor: line passing through a point \a p in the direction \a direction
	\param p point
	\param direction direction
	*/
	inline Line3(const Point3<Real> &p, const Vector3<Real> &direction)
			: origin( p ), direction( direction )
	{
	}

	/**
	Constructor: convert from a Line3 with a different Real number type
	*/
	template <typename S> inline Line3(const Line3<S> &l)
		: origin( l.origin ), direction( l.direction )
	{
	}



	/**
	Offset position of line by Vector3 \a v
	*/
	inline Line3<Real> operator+(const Vector3<Real> &v) const
	{
		return Line3<Real>( origin + v, direction );
	}

	/**
	Offset position of line by Vector3 \a v
	*/
	inline void operator+=(const Vector3<Real> &v)
	{
		origin += v;
	}

	/**
	Offset position of line by Vector3 \a -v
	*/
	inline Line3<Real> operator-(const Vector3<Real> &v) const
	{
		return Line3<Real>( origin - v, direction );
	}

	/**
	Offset position of line by Vector3 \a -v
	*/
	inline void operator-=(const Vector3<Real> &v)
	{
		origin -= v;
	}



	/**
	Get a point on the line
	\param t the parameter
	Returns \a origin + \a direction * \a t.
	*/
	inline Point3<Real> getPoint(Real t) const
	{
		return origin  +  direction * t;
	}



	/**
	Get the 't'-value of the closest point on the line to the point \a p.
	\param p the point
	The t-value is the proportion of \a direction along the line from \a origin at which the closest point is located.
	*/
	inline Real closestPointParam(const Point3<Real> &p) const
	{
		return direction.dot( p - origin )  /  direction.sqrLength();
	}

	/**
	Get the closest point, and the 't'-value of the closest point on the line to the point \a p.
	\param p the point
	\param t a reference to where the 't'-value should be stored
	*/
	inline Point3<Real> closestParamAndPointTo(const Point3<Real> &p, Real &t) const
	{
		t = closestPointParam( p );
		return getPoint( t );
	}

	/**
	Get the closest point on the line to the point \a p.
	\param p the point
	*/
	inline Point3<Real> closestPointTo(const Point3<Real> &p) const
	{
		return getPoint( closestPointParam( p ) );
	}

	/**
	Get the distance from the point \a p to \a this (The distance from \a p to the closest point on \a this).
	*/
	inline Real distanceTo(const Point3<Real> &p) const
	{
		return p.distanceTo( closestPointTo( p ) );
	}

	/**
	Get the square of the distance from the point \a p to \a this (The distance from \a p to the closest point on \a this).
	*/
	inline Real sqrDistanceTo(const Point3<Real> &p) const
	{
		return p.sqrDistanceTo( closestPointTo( p ) );
	}


	/**
	Get the 't'-values of the closest points on a pair of lines - \a this and \a l1
	\param l1 the second line
	\param t0 a reference to where the 't'-value on \a this should be stored
	\param t1 a reference to where the 't'-value on \a l1 should be stored
	\return true if lines are not parallel, false if they are parallel

	Note: if the lines are parallel, then any pair of points could be used. In this case, we give \a t0 a value of 0.0 (the origin of \a this), and \a t1 a 't'-value of the closest point on \a l1.
	*/
	inline bool closestPointsOnLineParams(const Line3<Real> &l1, Real &t0, Real &t1) const
	{
		/*
		L = A + sB		(@this)
		M = C + tD		(@l1)

		sqrDist = |M-L|^2  =  | C + tD  - A - sB |^2

		Q = C - A

		sqrDist = ( Q + tD - sB ) . ( Q + tD - sB )
				= Q.Q + t^2D.D + s^2B.B + 2tQ.D - 2sQ.b - 2stD.B

		sqrDist is minimum when the derivative of the above is 0:

		with respect to s:   2sB.B - 2tD.B - 2Q.B = 0
									sB.B - tD.B = Q.B

		with respect to t:  -2sD.B + 2tD.D + 2Q.D = 0
								-sD.B + tD.D = -Q.D

		As a matix equation:

		[  B.B	-B.D ] x  [ s ] = [  Q.B ]
		[ -B.D	 D.D ]    [ t ]   [ -Q.D ]

		*/

		Vector3<Real> q = l1.origin - origin;

		Real bDotB = direction.dot( l1.direction );
		Real bDotD = direction.dot( l1.direction );
		Real dDotD = l1.direction.dot( l1.direction );
		Real qDotB = q.dot( direction );

		Real sqrLengthProduct = bDotB * dDotD;

		Real determinant = sqrLengthProduct  -  bDotD * bDotD;

		if ( (Real)fabs( determinant )  <  (Real)( sqrLengthProduct * EPSILON ) )
		{
			//lines are parallel, use any points
			t0 = (Real)0.0;
			t1 = -qDotB / bDotB;
			return false;
		}
		else
		{
			Real invDet = (Real)1.0 / determinant;

			Real qDotD = q.dot( l1.direction );

			t0 = ( ( dDotD * qDotB )  -  ( bDotD * qDotD ) )  *  invDet;
			t1 = ( ( bDotD * qDotB )  -  ( bDotB * qDotD ) )  *  invDet;
			return true;
		}
	}

	/**
	Get the closest points, and 't'-values of the closest points on a pair of lines - \a this and \a l1
	\param l1 the second line
	\param p0 a reference to where the closest point on \a this should be stored
	\param p1 a reference to where the closest point on \a l1 should be stored
	\param t0 a reference to where the 't'-value on \a this should be stored
	\param t1 a reference to where the 't'-value on \a l1 should be stored
	\return true if lines are not parallel, false if they are parallel

	Note: if the lines are parallel, then any pair of points could be used. In this case, we give \a t0 a value of 0.0 (the origin of \a this), and \a t1 a 't'-value of the closest point on \a l1.
	*/
	inline bool closestPointsAndParamsOnLine(const Line3<Real> &l1, Point3<Real> &p0, Point3<Real> &p1, Real &t0, Real &t1) const
	{
		bool nonParallel = closestPointsParams( l1, t0, t1 );
		p0 = getPoint( t0 );
		p1 = l1.getPoint( t1 );
		return nonParallel;
	}

	/**
	Get the closest points on a pair of lines - \a this and \a l1
	\param l1 the second line
	\param p0 a reference to where the closest point on \a this should be stored
	\param p1 a reference to where the closest point on \a l1 should be stored
	\return true if lines are not parallel, false if they are parallel

	Note: if the lines are parallel, then any pair of points could be used. In this case, we give \a t0 a value of 0.0 (the origin of \a this), and \a t1 a 't'-value of the closest point on \a l1.
	*/
	inline bool closestPointsOnLine(const Line3<Real> &l1, Point3<Real> &p0, Point3<Real> &p1) const
	{
		Real t0, t1;
		return closestPoints( l1, p0, p1, t0, t1 );
	}

	/**
	Get the distance from \a this to \a l1
	*/
	inline Real distanceToLine(const Line3<Real> &l1) const
	{
		Point3<Real> p0, p1;
		closestPoints( l1, p0, p1 );
		return p0.distanceTo( p1 );
	}

	/**
	Get the square of the distance from \a this to \a l1
	*/
	inline Real sqrDistanceToLine(const Line3<Real> &l1) const
	{
		Point3<Real> p0, p1;
		closestPoints( l1, p0, p1 );
		return p0.sqrDistanceTo( p1 );
	}


	/**
	Intersect \a this with the plane \a p.
	\param p the plane with which to intersect
	\param t a reference to where the 't'-value of the intersection point along \a this should be stored
	\param intersection a reference to where the intersection point should be stored
	\return true if intersection exists, false if not (line is parallel with plane)
	*/
	inline bool intersectPlane(const Plane<Real> &p, Real &t, Point3<Real> &intersection) const
	{
		//raytrace the line against the plane
		Real nDotDirection = p.n.dot( direction );

		if ( nDotDirection == (Real)0.0 )
		{
			return false;
		}

		t = ( p.d - origin.dot( p.n ) ) / nDotDirection;

		intersection = getPoint( t );
		return true;
	}
};


typedef Line3<float> Line3f;
typedef Line3<double> Line3d;



#endif
