//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************

//
// Quaternion <-> Euler angle conversion code:
// Used:
//			http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
//	as a reference.
//
#ifndef QUATERNION_H__
#define QUATERNION_H__

#include <math.h>

#include <complex>

#include <Math/MathConstants.h>
#include <Math/epsilon.h>
#include <Math/Vector3.h>
#include <Math/EulerRotation.h>
#include <Math/clamp.h>



template <typename Real> class Quaternion
{
public:
	Real x, y, z, w;


	inline Quaternion() : x( (Real)0.0 ), y( (Real)0.0 ), z( (Real)0.0 ), w( (Real)1.0 )
	{
	}

	inline Quaternion(Real ix, Real iy , Real iz, Real iw)
							: x( ix ), y( iy ), z( iz ), w( iw )
	{
	}

	template <typename S> inline Quaternion(const Quaternion<S> &q)
		: x( (Real)q.x ), y( (Real)q.y ), z( (Real)q.z ), w( (Real)q.w )
	{
	}



	inline Quaternion<Real> operator+(const Quaternion<Real> &q) const
	{
		return Quaternion<Real>( x + q.x,  y + q.y,  z + q.z,  w + q.w );
	}

	inline void operator+=(const Quaternion<Real> &q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
	}


	inline Quaternion<Real> operator-(const Quaternion<Real> &q) const
	{
		return Quaternion<Real>( x - q.x,  y - q.y,  z - q.z,  w - q.w );
	}

	inline void operator-=(const Quaternion<Real> &q)
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
	}


	inline Quaternion operator*(const Quaternion<Real> &q) const
	{
		return Quaternion<Real>( ( w * q.x ) + ( x * q.w ) + ( y * q.z ) - ( z * q.y ),
							( w * q.y ) - ( x * q.z ) + ( y * q.w ) + ( z * q.x ),
							( w * q.z ) + ( x * q.y ) - ( y * q.x ) + ( z * q.w ),
							( w * q.w ) - ( x * q.x ) - ( y * q.y ) - ( z * q.z ) );
	}

	inline void operator*=(const Quaternion<Real> &q)
	{
		Real tempW = ( w * q.w ) - ( x * q.x ) - ( y * q.y ) - ( z * q.z );
		Real tempX = ( w * q.x ) + ( x * q.w ) + ( y * q.z ) - ( z * q.y );
		Real tempY = ( w * q.y ) - ( x * q.z ) + ( y * q.w ) + ( z * q.x );
		z			 = ( w * q.z ) + ( x * q.y ) - ( y * q.x ) + ( z * q.w );
		w = tempW;
		x = tempX;
		y = tempY;
	}


	inline Quaternion<Real> operator/(const Quaternion<Real> &q) const
	{
		return q.inverse()  *  *this;
	}

	inline void operator/=(const Quaternion<Real> &q)
	{
		*this = q.inverse()  *  *this;
	}



	inline Real norm() const
	{
		return ( x * x )  +  ( y * y )  +  ( z * z )  +  ( w * w );
	}

	inline Real modulus() const
	{
		return sqrt( norm() );
	}

	inline Quaternion<Real> conjugate() const
	{
		return Quaternion<Real>( w, -x, -y, -z );
	}

	inline Quaternion<Real> inverse() const
	{
		Real normRecip = 1.0 / norm();

		return Quaternion<Real>( -x * normRecip,  -y * normRecip,  -z * normRecip,  w * normRecip );
	}


	inline EulerRotation<Real> toEuler() const
	{
 		EulerRotation<Real> euler;

		Real sqw = w*w;
		Real sqx = x*x;
		Real sqy = y*y;
		Real sqz = z*z;

		Real length = sqx + sqy + sqz + sqw;
		Real alpha = x*y + z*w;

		if ( alpha  >  length * (Real)( 0.5 - EPSILON ) )
		{
			// singularity at north pole
			euler.y = (Real)2.0 * (Real)atan2( x, w );
			euler.z = (Real)( M_PI / 2.0 );
			euler.x = (Real)0.0;
		}
		else if (alpha  <  -length * (Real)( 0.5 - EPSILON ))
		{
			// singularity at south pole
			euler.y = (Real)-2.0 * (Real)atan2( x, w );
			euler.z = (Real)( -M_PI / 2.0 );
			euler.x = (Real)0.0;
		}
		else
		{
			euler.y = (Real)atan2( (Real)2.0 * y * w  -  (Real)2.0 * x * z , sqx - sqy - sqz + sqw);
			euler.z = (Real)asin( (Real)2.0*alpha / length);
			euler.x = (Real)atan2( (Real)2.0 * x * w  -  (Real)2.0 * y * z , -sqx + sqy - sqz + sqw);
		}

		return euler;
	}


	//rotation
	inline static Quaternion<Real> rotateXSinCos(Real sinHalfTheta, Real cosHalfTheta)
	{
		return Quaternion<Real>( sinHalfTheta, (Real)0.0, (Real)0.0,  cosHalfTheta );
	}

	inline static Quaternion<Real> rotateYSinCos(Real sinHalfTheta, Real cosHalfTheta)
	{
		return Quaternion<Real>( (Real)0.0, sinHalfTheta, (Real)0.0,  cosHalfTheta );
	}

	inline static Quaternion<Real> rotateZSinCos(Real sinHalfTheta, Real cosHalfTheta)
	{
		return Quaternion<Real>( (Real)0.0, (Real)0.0, sinHalfTheta,  cosHalfTheta );
	}

	inline static Quaternion<Real> rotateX(Real angle)
	{
		angle *= (Real)0.5;
		return rotateXSinCos( sin( angle ), cos( angle ) );
	}

	inline static Quaternion<Real> rotateY(Real angle)
	{
		angle *= (Real)0.5;
		return rotateYSinCos( sin( angle ), cos( angle ) );
	}

	inline static Quaternion<Real> rotateZ(Real angle)
	{
		angle *= (Real)0.5;
		return rotateZSinCos( sin( angle ), cos( angle ) );
	}


	inline static Quaternion<Real> rotateAxisSinCos(const Vector3<Real> &axis, Real sinHalfTheta, Real cosHalfTheta)
	{
		return Quaternion<Real>( axis.x * sinHalfTheta, axis.y * sinHalfTheta, axis.z * sinHalfTheta,  cosHalfTheta );
	}

	inline static Quaternion<Real> rotateAxis(const Vector3<Real> &axis, Real angle)
	{
		angle *= (Real)0.5;
		return rotateAxisSinCos( axis, sin( angle ), cos( angle ) );
	}

	inline static Quaternion<Real> rotate(const EulerRotation<Real> &euler)
	{
		Real sx = sin(euler.x * (Real)0.5);
		Real cx = cos(euler.x * (Real)0.5);
		Real sy = sin(euler.y * (Real)0.5);
		Real cy = cos(euler.y * (Real)0.5);
		Real sz = sin(euler.z * (Real)0.5);
		Real cz = cos(euler.z * (Real)0.5);

		Quaternion<Real> q;
		q.w = cy * cz * cx  -  sy * sz * sx;
		q.x = sy * sz * cx  +  cy * cz * sx;
		q.y = sy * cz * cx  +  cy * sz * sx;
		q.z = cy * sz * cx  -  sy * cz * sx;

		return q;
	}

	inline static Quaternion<Real> axisToAxis(const Vector3<Real> &src, const Vector3<Real> &dst)
	{
		//find the angle between the vectors
		//and generate a rotation matrix, rotating that amount, where the axis
		//of rotation is the vector that is perpendicular to src and dst
		Real cosAngle = src.dot( dst );

		//check if src and dst are the same vector (include margin of error)
		if ( cosAngle  >  (Real)( 1.0 - EPSILON ) )
		{
			//if so, result is identity rotation
			return Quaternion<Real>();
		}
		else if ( cosAngle  <  (Real)( -1.0 + EPSILON ) )		//check if they oppose
		{
			//get a vector perpendicular to the src vector
			Vector3<Real> axis = src.perpendicularVector();
			axis.normalise();

			//now get a vector perpendicular to both
			Vector3<Real> axis2 = src.cross( axis );
			axis2.normalise();

			//prefer axis with strongest Y-component
			if ( fabs( axis2.y )  > fabs( axis.y ) )
			{
				axis = axis2;
			}

			//rotate around the chosen axis by 180 degrees
			return Quaternion<Real>::rotate( axis, M_PI );
		}
		else
		{
			//calculate rotation axis
			Vector3<Real> axis = src.cross( dst );
			axis.normalise();
			//calculate angle
			Real angle = acos( cosAngle );
			//generate the quaternion
			return Quaternion<Real>::rotate( axis, angle );
		}
	}



	inline void read(FILE *f)
	{
		fread( &w, sizeof(Real), 1, f );
		fread( &x, sizeof(Real), 1, f );
		fread( &y, sizeof(Real), 1, f );
		fread( &z, sizeof(Real), 1, f );
	}

	inline void write(FILE *f) const
	{
		fwrite( &w, sizeof(Real), 1, f );
		fwrite( &x, sizeof(Real), 1, f );
		fwrite( &y, sizeof(Real), 1, f );
		fwrite( &z, sizeof(Real), 1, f );
	}
};



typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;



#endif
