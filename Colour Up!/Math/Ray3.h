//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef RAY3_H__
#define RAY3_H__

#include <Math/clamp.h>

#include <Math/Point3.h>
#include <Math/Plane.h>
#include <Math/Vector3.h>

/*
3D LINE
*/



template <typename Real> class Ray3
{
public:
	Point3<Real> origin;
	Vector3<Real> direction;


	inline Ray3()
	{
	}

	inline Ray3(const Point3<Real> &p1, const Point3<Real> &p2)
			: origin( p1 ), direction( p2 - p1 )
	{
	}

	inline Ray3(const Point3<Real> &p, const Vector3<Real> &direction)
			: origin( p ), direction( direction )
	{
	}

	template <typename S> inline Ray3(const Ray3<S> &r)
		: origin( r.origin ), direction( r.direction )
	{
	}



	//translation
	inline Ray3<Real> operator+(const Vector3<Real> &v) const
	{
		return Ray3<Real>( origin + v, direction );
	}

	inline void operator+=(const Vector3<Real> &v)
	{
		origin += v;
	}

	inline Ray3<Real> operator-(const Vector3<Real> &v) const
	{
		return Ray3<Real>( origin - v, direction );
	}

	inline void operator-=(const Vector3<Real> &v)
	{
		origin -= v;
	}



	//get a point on the line
	inline Point3<Real> getPoint(Real t) const
	{
		return origin  +  direction * t;
	}



	//get the closest point on the line to p
	inline Real closestPointParam(const Point3<Real> &p) const
	{
		Real t = direction.dot( p - origin )  /  direction.sqrLength();
		return clampLower( t, (Real)0.0 );
	}

	inline Point3<Real> closestParamAndPointTo(const Point3<Real> &p, Real &t) const
	{
		t = closestPointParam( p );
		return getPoint( t );
	}

	inline Point3<Real> closestPointTo(const Point3<Real> &p) const
	{
		return getPoint( closestPointParam( p ) );
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
		Real nDotDirection = p.n.dot( direction );

		if ( nDotDirection == (Real)0.0 )
		{
			return false;
		}

		t = ( p.d - origin.dot( p.n ) ) / nDotDirection;

		if ( t >= (Real)0.0 )
		{
			intersection = getPoint( t );
			return true;
		}
		else
		{
			return false;
		}
	}
};


typedef Ray3<float> Ray3f;
typedef Ray3<double> Ray3d;



#endif
