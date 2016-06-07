//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef TRIANGLE2_H__
#define TRIANGLE2_H__

#include <Math/Point2.h>
#include <Math/Segment2.h>


template <typename Real> class Triangle2
{
public:
	Point2<Real> a, b, c;


	inline Triangle2()
	{
	}

	inline Triangle2(const Point2<Real> &va, const Point2<Real> &vb, const Point2<Real> &vc)
					: a( va ), b( vb ), c( vc )
	{
	}

	template <typename S> inline Triangle2(const Triangle2<S> &t)
		: a( t.a ), b( t.b. ), c( t.c )
	{
	}



	inline Segment2<Real> getEdge(int edge) const
	{
		switch( edge )
		{
		case 0:
			return Segment2<Real>( a, b );
		case 1:
			return Segment2<Real>( b, c );
		case 2:
			return Segment2<Real>( c, a );
		default:
			return Segment2<Real>( a, b );
		}
	}


	inline Real areaX2() const
	{
		return Point2<Real>::areaOfTrianglex2( a, b, c );
	}

	inline Real area() const
	{
		return areaX2() * (Real)0.5;
	}


	inline bool contains(const Point2<Real> &p) const
	{
		if ( areaX2() > (Real)0.0 )
		{
			return getEdge( 0 ).onOrLeft( p )  &&  getEdge( 1 ).onOrLeft( p )  &&  getEdge( 2 ).onOrLeft( p );
		}
		else
		{
			return getEdge( 0 ).onOrRight( p )  &&  getEdge( 1 ).onOrRight( p )  &&  getEdge( 2 ).onOrRight( p );
		}
	}
};


typedef Triangle2<float> Triangle2f;
typedef Triangle2<double> Triangle2d;


#endif

