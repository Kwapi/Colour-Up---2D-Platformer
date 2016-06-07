//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef TRIANGLE3_H__
#define TRIANGLE3_H__

#include <Math/Point3.h>
#include <Math/Plane.h>
#include <Math/Segment3.h>
#include <Math/Line3.h>
#include <Math/Side.h>
#include <Math/Triangle2.h>



template <typename Real> class Triangle3
{
public:
	Point3<Real> a, b, c;


	Triangle3()
	{
	}

	Triangle3(const Point3<Real> &va, const Point3<Real> &vb, const Point3<Real> &vc)
				: a( va ), b( vb ), c( vc )
	{
	}

	Triangle3(const Triangle2<Real> &t) : a ( t.a ), b( t.b ), c( t.c )
	{
	}

	template <typename S> Triangle3(const Triangle3<S> &t)
		: a( t.a ), b( t.b ), c( t.c )
	{
	}



	Segment3<Real> getEdge(int edge) const
	{
		switch( edge )
		{
		case 0:
			return Segment3<Real>( a, b );
		case 1:
			return Segment3<Real>( b, c );
		case 2:
			return Segment3<Real>( c, a );
		default:
			return Segment3<Real>( a, b );
		}
	}

	Plane<Real> getFastEdgePlane(int edge) const
	{
		Plane<Real> plane = getFastPlane();
		Segment3<Real> edgeLine = getEdge( edge );

		switch( edge )
		{
		case 0:
			return Plane<Real>::fastPlane( a, plane.n, edgeLine.getDirection() );
		case 1:
			return Plane<Real>::fastPlane( b, plane.n, edgeLine.getDirection() );
		case 2:
			return Plane<Real>::fastPlane( c, plane.n, edgeLine.getDirection() );
		default:
			return Plane<Real>::fastPlane( a, plane.n, edgeLine.getDirection() );
		}
	}

	Plane<Real> getPlane() const
	{
		return Plane<Real>( a, b, c );
	}

	Plane<Real> getFastPlane() const
	{
		return Plane<Real>::fastPlane( a, b, c );
	}


	bool raytrace(const Segment3<Real> &ray, Real &t, Real &u, Real &v) const
	{
		//Uses Moller and Trumbore's algorithm

		//edges
		Vector3<Real> e1 = b - a;
		Vector3<Real> e2 = c - a;
		Vector3<Real> rayDir = ray.getDirection();
		Vector3<Real> p = rayDir.cross( e2 );

		//compute determinant of matrix
		Real determinant = p.dot( e1 );
		if ( determinant == (Real)0.0 )
		{
			return false;
		}
		Real inverseDeterminant = (Real)1.0 / determinant;

		//compute u
		Vector3<Real> s = ray.a - a;
		Real ru = s.dot( p )  *  inverseDeterminant;
		if ( ru < (Real)0.0  ||  ru > (Real)1.0 )
		{
			return false;
		}

		//compute v
		Vector3<Real> q = s.cross( e1 );
		Real rv = rayDir.dot( q )  *  inverseDeterminant;
		if ( rv < (Real)0.0  ||  ( ru + rv ) > (Real)1.0 )
		{
			return false;
		}

		//compute t
		Real rt = e2.dot( q )  *  inverseDeterminant;


		if ( rt >= (Real)0.0  &&  rt <= (Real)1.0 )
		{
			u = ru;
			v = rv;
			t = rt;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool raytrace(const Line3<Real> &ray, Real &t, Real &u, Real &v) const
	{
		//Uses Moller and Trumbore's algorithm

		//edges
		Vector3<Real> e1 = b - a;
		Vector3<Real> e2 = c - a;
		Vector3<Real> p = ray.direction.cross( e2 );

		//compute determinant of matrix
		Real determinant = p.dot( e1 );
		if ( determinant == (Real)0.0 )
		{
			return false;
		}
		Real inverseDeterminant = (Real)1.0 / determinant;

		//compute u
		Vector3<Real> s = ray.origin - a;
		Real ru = s.dot( p )  *  inverseDeterminant;
		if ( ru < (Real)0.0  ||  ru > (Real)1.0 )
		{
			return false;
		}

		//compute v
		Vector3<Real> q = s.cross( e1 );
		Real rv = ray.direction.dot( q )  *  inverseDeterminant;
		if ( rv < (Real)0.0  ||  ( ru + rv ) > (Real)1.0 )
		{
			return false;
		}

		//compute t
		Real rt = e2.dot( q )  *  inverseDeterminant;

		u = ru;
		v = rv;
		t = rt;
		return true;
	}


	Point3<Real> uvToPoint(const Point3<Real> &uv) const
	{
		return a   +    ( b - a ) * uv.x   +   ( c - a ) * uv.y;
	}


	//compute which side of the plane *this is on
	inline Side side(const Plane<Real> &p) const
	{
		if ( a == p  &&  b == p  &&  c == p )
		{
			return SIDE_ON;
		}
		else if ( a >= p  &&  b >= p  &&  c >= p )
		{
			return SIDE_POSITIVE;
		}
		else if ( a <= p  &&  b <= p  &&  c <= p )
		{
			return SIDE_NEGATIVE;
		}
		else
		{
			return SIDE_BOTH;
		}
	}
};


typedef Triangle3<float> Triangle3f;
typedef Triangle3<double> Triangle3d;




#endif
