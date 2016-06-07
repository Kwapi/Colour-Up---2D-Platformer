//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef PLANE_H__
#define PLANE_H__

#include <stdio.h>

#include <Math/epsilon.h>

#include <Math/Side.h>
#include <Math/Vector3.h>
#include <Math/Point3.h>
#include <Math/Axis.h>


/**
Plane: an infinite plane, defined by a normal (\a n), and a distance from the origin (\a d).

This is a template class, so 'Real' is the type used to store real numbers.
Planef and Planed alias Plane<float> and Plane<double> respectively.
*/
template <typename Real> class Plane
{
public:
	Vector3<Real> n;
	Real d;


	/**
	Default constructor: initialise plane to X-Y plane that passes throught the origin
	*/
	inline Plane() : n( Vector3<Real>( (Real)0.0, (Real)0.0, (Real)1.0 ) ), d( (Real)0.0 )
	{
	}

	inline Plane(const Vector3<Real> &normal, Real distance)
					 : n( normal ), d( distance )
	{
	}

	inline Plane(const Vector3<Real> &normal, const Point3<Real> &a)
	{
		n = normal;
		d = a.dot( n );
	}

	inline Plane(Axis normal, const Point3<Real> &a)
	{
		n = Vector3<Real>( normal );
		d = a.dot( n );
	}

	inline Plane(const Point3<Real> &a, const Vector3<Real> &u, const Vector3<Real> &v)
	{
		n = u.cross( v ).getNormalised();
		d = a.dot( n );
	}

	inline Plane(const Point3<Real> &a, const Point3<Real> &b, const Point3<Real> &c)
	{
		Vector3<Real> u = b - a;
		Vector3<Real> v = c - a;
		n = u.cross( v ).getNormalised();
		d = a.dot( n );
	}

	/*inline Plane(const Array<Point3> &polygon)
	{
		int b = 1;

		Point3 midPoint = Point3::sum( polygon[0], polygon[1] );

		//compute normal as weighted average of normals of triangles that make up
		//the polygon (weight is the area of the triangle)
		//also, compute the midpoint
		for (int c = 2; c < polygon.size(); c++)
		{
			Vector3 u = polygon[b] - polygon[0];
			Vector3 v = polygon[c] - polygon[0];

			n += u.cross( v );			// u.cross( v )  =  normal * area of triangle
			midPoint.cumulativeAdd( polygon[c] );

			b = c;
		}
		//normalise n
		n.normalise();

		//divide by n (mean)
		midPoint.scale( 1.0 / (Real)polygon.size() );

		d = midPoint.dot( n );
	}*/




	inline bool operator!=(const Plane<Real> &cmp) const
	{
		return n != cmp.n  ||  d != cmp.d;
	}



	inline Plane<Real> operator-() const
	{
		return Plane<Real>( -n, -d );
	}



	inline Side side(const Point3<Real> &p, bool strict) const
	{
		Real pDotN = p.dot( n );

		if ( strict )
		{
			if ( pDotN < d )
			{
				return SIDE_NEGATIVE;
			}
			else if ( pDotN > d )
			{
				return SIDE_POSITIVE;
			}
			else
			{
				return SIDE_ON;
			}
		}
		else
		{
			Real error = (Real)fabs( d )  *  EPSILON;

			if ( pDotN  <  ( d - error ) )
			{
				return SIDE_NEGATIVE;
			}
			if ( pDotN  >  ( d + error ) )
			{
				return SIDE_POSITIVE;
			}
			else
			{
				return SIDE_ON;
			}
		}
	}

/*	inline Side side(const Array<Point3> &poly, bool strict) const
	{
		Side s = side( poly[0], strict );

		for (int polyI = 1; polyI < poly.size(); polyI++)
		{
			Side s2 = side( poly[polyI], strict );

			if ( s2 != s )
			{
				return SIDE_BOTH;
			}
		}

		return s;
	}*/


	inline Side directionSide(const Vector3<Real> &v) const
	{
		Real vDotN = v.dot( n );

		if ( vDotN < (Real)0.0 )
		{
			return SIDE_NEGATIVE;
		}
		else if ( vDotN > (Real)0.0 )
		{
			return SIDE_POSITIVE;
		}
		else
		{
			return SIDE_ON;
		}
	}



	inline Real distanceTo(const Point3<Real> &p) const
	{
		return p.dot( n ) - d;
	}

	inline Point3<Real> getPointOnPlane() const
	{
		return Point3<Real>( n * d );
	}

	inline Point3<Real> closestPointTo(const Point3<Real> &p) const
	{
		Real dist = distanceTo( p );

		return p  -  n * dist;
	}



	inline bool intersect(const Point3<Real> &st, const Vector3<Real> &dir, Real &t) const
	{
		Real dirDotN = dir.dot( n );

		if ( dirDotN != 0.0 )
		{
			t = ( d - st.dot( n ) ) / dirDotN;
			return true;
		}
		else
		{
			t = (Real)0.0;
			return false;
		}
	}

	inline bool intersect(const Point3<Real> &st, const Point3<Real> &en, Real &t) const
	{
		if ( intersect( st, en - st, t ) )
		{
			if ( t >= (Real)0.0  &&  t <= (Real)1.0 )
			{
				return true;
			}
		}
		return false;
	}

	inline bool intersectStrict(const Point3<Real> &st, const Point3<Real> &en, Real &t) const
	{
		if ( intersect( st, en - st, t  ))
		{
			if ( t > (Real)0.0  &&  t < (Real)1.0 )
			{
				return true;
			}
		}
		return false;
	}



	inline bool separates(const Point3<Real> &a, const Point3<Real> &b) const
	{
		Real aDotN = a.dot( n );
		Real bDotN = b.dot( n );

		return ( aDotN > d  &&  bDotN < d )   ||   ( aDotN < d  &&  bDotN > d );
	}



	inline static Plane fastPlane(const Point3<Real> &a, const Vector3<Real> &u, const Vector3<Real> &v)
	{
		Vector3<Real> n = u.cross( v );
		return Plane( n, a.dot( n ) );
	}

	inline static Plane fastPlane(const Point3<Real> &a, const Point3<Real> &b, const Point3<Real> &c)
	{
		return fastPlane( a, b - a, c - a );
	}



	inline void read(FILE *f)
	{
		n.read( f );
		fread( &d, sizeof(Real), 1, f );
	}

	inline void write(FILE *f) const
	{
		n.write( f );
		fwrite( &d, sizeof(Real), 1, f );
	}
};



template <typename Real> inline bool operator==(const Plane<Real> &pl, const Point3<Real> &pt)
{
	return pt.dot( pl.n ) == pl.d;
}

template <typename Real> inline bool operator==(const Point3<Real> &pt, const Plane<Real> &pl)
{
	return pt.dot( pl.n ) == pl.d;
}

template <typename Real> inline bool operator!=(const Plane<Real> &pl, const Point3<Real> &pt)
{
	return pt.dot( pl.n ) != pl.d;
}

template <typename Real> inline bool operator!=(const Point3<Real> &pt, const Plane<Real> &pl)
{
	return pt.dot( pl.n ) != pl.d;
}

template <typename Real> inline bool operator<(const Plane<Real> &pl, const Point3<Real> &pt)
{
	return pt.dot( pl.n ) > pl.d;
}

template <typename Real> inline bool operator<(const Point3<Real> &pt, const Plane<Real> &pl)
{
	return pt.dot( pl.n ) < pl.d;
}

template <typename Real> inline bool operator<=(const Plane<Real> &pl, const Point3<Real> &pt)
{
	return pt.dot( pl.n ) >= pl.d;
}

template <typename Real> inline bool operator<=(const Point3<Real> &pt, const Plane<Real> &pl)
{
	return pt.dot( pl.n ) <= pl.d;
}

template <typename Real> inline bool operator>(const Plane<Real> &pl, const Point3<Real> &pt)
{
	return pt.dot( pl.n ) < pl.d;
}

template <typename Real> inline bool operator>(const Point3<Real> &pt, const Plane<Real> &pl)
{
	return pt.dot( pl.n ) > pl.d;
}

template <typename Real> inline bool operator>=(const Plane<Real> &pl, const Point3<Real> &pt)
{
	return pt.dot( pl.n ) <= pl.d;
}

template <typename Real> inline bool operator>=(const Point3<Real> &pt, const Plane<Real> &pl)
{
	return pt.dot( pl.n ) >= pl.d;
}



typedef Plane<float> Planef;
typedef Plane<double> Planed;



#endif
