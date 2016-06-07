//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef POINT4_H__
#define POINT4_H__

#include "Math/Point3.h"



template <typename Real> class Point4
{
public:
	union
	{
		struct
		{
			Real x, y, z, w;
		};

		Real v[4];
	};


	inline Point4() : x( (Real)0.0 ), y( (Real)0.0 ), z( (Real)0.0 ), w( (Real)1.0 )
	{
	}

	inline Point4(Real ix, Real iy, Real iz)
					  : x( ix ), y( iy ), z( iz ), w( 1.0 )
	{
	}

	inline Point4(Real ix, Real iy, Real iz, Real iw)
					  : x( ix ), y( iy ), z( iz ), w( iw )
	{
	}

	template <typename S> Point4(const Point4<S> &p)
		: x( (Real)p.x ), y( (Real)p.y ), z( (Real)p.z ), w( (Real)p.w )
	{
	}

	inline Point4(const Point3<Real> &p3)
					  : x( p3.x ), y( p3.y ), z( p3.z ), w( (Real)1.0 )
	{
	}

	inline Point4(const Point3<Real> &p3, Real iw)
					  : x( p3.x ), y( p3.y ), z( p3.z ), w( iw )
	{
	}



	inline Point3<Real> convertToPoint3() const
	{
		Real oneOverW = (Real)1.0 / w;
		return Point3<Real>( x * oneOverW, y * oneOverW, z * oneOverW );
	}

	inline Point3<Real> inverseConvertToPoint3() const
	{
		return Point3<Real>( x * w, y * w, z * w );
	}

	inline Point3<Real> toPoint3() const
	{
		return Point3<Real>( x, y, z );
	}
};


typedef Point4<float> Point4f;
typedef Point4<double> Point4d;



#endif

