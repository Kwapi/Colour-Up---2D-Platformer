//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef EULERROTATION_H__
#define EULERROTATION_H__

#include <math.h>

#include <Math/Vector3.h>



/**
EulerRotation: Euler rotation


This is a template class, so 'Real' is the type used to store x, y and z co-ordinates.
Real would normally be float or double.
Vector3f and Vector3d are aliases for Vector3<float> and Vector3<double> respectively.


x, y and z co-ordinates can be accessed via:


a.x, a.y, and a.z

OR

a.v[0], a.v[1], and a.v[2]


a.x is the same thing as a.v[0]



Construct a rotation matrix from an EulerRotation with the following code:


EulerRotationf e(M_PI, M_PI*2, M_PI*3);

Matrix4f m = Matrix4f::rotate(e);
*/
template <typename Real> class EulerRotation
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
	Constructor: initialise to identity
	*/
	inline EulerRotation() : x( (Real)0.0 ), y( (Real)0.0 ), z( (Real)0.0 )
	{
	}

	/**
	Constructor

	\param ix rotation around X-axis
	\param iy rotation around Y-axis
	\param iz rotation around Z-axis
	*/
	inline EulerRotation(Real ix, Real iy, Real iz)
								: x( ix ), y( iy ), z( iz )
	{
	}

	/**
	Constructor: initialise X, Y, and Z rotations from components of a Vector3.
	*/
	inline EulerRotation(const Vector3<Real> &v)
								: x( v.x ), y( v.y ), z( v.z )
	{
	}



	/**
	Accessor: get the rotation around the axis identified by \a i.

	\param i The axis - X=0, Y=1, Z=2
	*/
	inline Real & operator[](int i)
	{
		return v[i];
	}

	/**
	Accessor: get the rotation around the axis identified by \a i.

	\param i The axis - X=0, Y=1, Z=2
	*/
	inline const Real & operator[](int i) const
	{
		return v[i];
	}



	/**
	Convert to a Vector3.
	*/
	inline Vector3<Real> toVector3() const
	{
		return Vector3<Real>( x, y, z );
	}
};


typedef EulerRotation<float> EulerRotationf;
typedef EulerRotation<double> EulerRotationd;



#endif
