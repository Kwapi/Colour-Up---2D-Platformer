//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
/*	by:					Geoffrey French
	last revision:		14/Nov/2002
	title:				4x4 transformation matrix class with mathematical operations.

  Implemented as follows:

  use the macros iMA, iMB,.... iMP to access the members by letter
  use the macros iMyx where x and y are numbers in the range 0..3 to access the
   members where x and y are the coordinates of the matrix member
*/
#ifndef MATRIX4_H__
#define MATRIX4_H__

#include <stdio.h>

#include <Math/radians.h>

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Point2.h>
#include <Math/Point3.h>
#include <Math/Point4.h>
#include <Math/Plane.h>
#include <Math/EulerRotation.h>
#include <Math/Quaternion.h>
#include <Math/Segment3.h>
#include <Math/Segment2.h>
#include <Math/Triangle2.h>
#include <Math/Triangle3.h>
#include <Math/epsilon.h>



//declaration of Vector3 operator*(const Vector3 &v, const Matrix4 &m)
//needed in Matrix4::transformUnit()
template <typename Real> class Matrix4;
template <typename Real> inline Vector3<Real> operator*(const Vector3<Real> &v, const Matrix4<Real> &m);


/**
Matrix4 - 4x4 matrix

This is a template class, so 'Real' is the type used to store the matrix elements.
Real would normally be float or double.
Matrix4f and Matrix4d are aliases for Matrix4<float> and Matrix4<double> respectively.

Members of matrix M can be accessed by:
	
	M.d: an array of 16 reals
	
	OR:

	M.iMA, M.iMB, M.IMC, M.IMD

	M.iME, M.iMF, M.IMG, M.IMH

	M.iMI, M.iMJ, M.IMK, M.IML

	M.iMM, M.iMN, M.IMO, M.IMP

	OR:

	M.iM00, M.iM01, M.iM02, M.IM03

	M.iM10, M.iM11, M.iM12, M.IM13

	M.iM20, M.iM21, M.iM22, M.IM23

	M.iM30, M.iM31, M.iM32, M.IM33

	They all access the same underlying elements.



Notes on transformations:

Rotations are counter-clockwise, from the perspective of looking from the rotation axis towards the origin.
For example, a rotation of 45 degrees around the X axis would appear to be counter-clockwise when looking at the origin
from the right (positive X).
*/
template <typename Real> class Matrix4
{
public:
	union
	{
		Real d[16];					//16 (4x4) floats

		struct
		{
			Real iMA, iME, iMI, iMM;
			Real iMB, iMF, iMJ, iMN;
			Real iMC, iMG, iMK, iMO;
			Real iMD, iMH, iML, iMP;
		};

		struct
		{
			Real iM00, iM10, iM20, iM30;
			Real iM01, iM11, iM21, iM31;
			Real iM02, iM12, iM22, iM32;
			Real iM03, iM13, iM23, iM33;
		};
	};



	/**
	Default constructor - identity matrix
	*/
	inline Matrix4()
	{
		iM00 = (Real)1.0;	iM01 = (Real)0.0;	iM02 = (Real)0.0;	iM03 = (Real)0.0;
		iM10 = (Real)0.0;	iM11 = (Real)1.0;	iM12 = (Real)0.0;	iM13 = (Real)0.0;
		iM20 = (Real)0.0;	iM21 = (Real)0.0;	iM22 = (Real)1.0;	iM23 = (Real)0.0;
		iM30 = (Real)0.0;	iM31 = (Real)0.0;	iM32 = (Real)0.0;	iM33 = (Real)1.0;
	}

	/**
	Constructor - takes 16 Real values
	*/
	inline Matrix4(Real m00, Real m01, Real m02, Real m03,
				Real m10, Real m11, Real m12, Real m13,
				Real m20, Real m21, Real m22, Real m23,
				Real m30, Real m31, Real m32, Real m33)
	{
		iM00 = m00;	iM01 = m01;	iM02 = m02;	iM03 = m03;
		iM10 = m10;	iM11 = m11;	iM12 = m12;	iM13 = m13;
		iM20 = m20;	iM21 = m21;	iM22 = m22;	iM23 = m23;
		iM30 = m30;	iM31 = m31;	iM32 = m32;	iM33 = m33;
	}

	/**
	Constructor - from base vectors

	\param i - I'  The vector [1,0,0] transformed by the matrix
	\param j - J'  The vector [0,1,0] transformed by the matrix
	\param k - K'  The vector [0,0,1] transformed by the matrix


	i.x		j.x		k.x		0

	i.y		j.y		k.y		0

	i.z		j.z		k.z		0

	0.0		0.0		0.0		1
	*/
	inline Matrix4(const Vector3<Real> &i, const Vector3<Real> &j, const Vector3<Real> &k)
	{
		iM00 = i.x;				iM01 = j.x;				iM02 = k.x;				iM03 = (Real)0.0;
		iM10 = i.y;				iM11 = j.y;				iM12 = k.y;				iM13 = (Real)0.0;
		iM20 = i.z;				iM21 = j.z;				iM22 = k.z;				iM23 = (Real)0.0;
		iM30 = (Real)0.0;		iM31 = (Real)0.0;		iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}

	/**
	Constructor - from base vectors, with translation

	\param i - I'  The vector [1,0,0] transformed by the matrix
	\param j - J'  The vector [0,1,0] transformed by the matrix
	\param k - K'  The vector [0,0,1] transformed by the matrix
	\param translation - translation


	i.x		j.x		k.x		translation.x

	i.y		j.y		k.y		translation.y

	i.z		j.z		k.z		translation.z

	0.0		0.0		0.0		1
	*/
	inline Matrix4(const Vector3<Real> &i, const Vector3<Real> &j, const Vector3<Real> &k, const Vector3<Real> &translation)
	{
		iM00 = i.x;				iM01 = j.x;				iM02 = k.x;				iM03 = translation.x;
		iM10 = i.y;				iM11 = j.y;				iM12 = k.y;				iM13 = translation.y;
		iM20 = i.z;				iM21 = j.z;				iM22 = k.z;				iM23 = translation.z
		iM30 = (Real)0.0;		iM31 = (Real)0.0;		iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}

	/**
	Constructor - from base vectors, with origin

	\param o - origin
	\param i - I'  The vector [1,0,0] transformed by the matrix
	\param j - J'  The vector [0,1,0] transformed by the matrix
	\param k - K'  The vector [0,0,1] transformed by the matrix


	i.x		j.x		k.x		o.x

	i.y		j.y		k.y		o.y

	i.z		j.z		k.z		o.z

	0.0		0.0		0.0		1
	*/
	inline Matrix4(const Point3<Real> &o, const Vector3<Real> &i, const Vector3<Real> &j, const Vector3<Real> &k)
	{
		iM00 = i.x;				iM01 = j.x;				iM02 = k.x;				iM03 = o.x;
		iM10 = i.y;				iM11 = j.y;				iM12 = k.y;				iM13 = o.y;
		iM20 = i.z;				iM21 = j.z;				iM22 = k.z;				iM23 = o.z
		iM30 = (Real)0.0;		iM31 = (Real)0.0;		iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}

	/**
	Constructor - from base vectors, as origin and points

	\param o - origin
	\param oi - I'  The point [1,0,0] transformed by the matrix
	\param oj - J'  The point [0,1,0] transformed by the matrix
	\param ok - K'  The point [0,0,1] transformed by the matrix

	i = oi - o

	j = oj - o

	k = ok - o


	i.x		j.x		k.x		o.x

	i.y		j.y		k.y		o.y

	i.z		j.z		k.z		o.z

	0.0		0.0		0.0		1
	*/
	inline Matrix4(const Point3<Real> &o, const Point3<Real> &oi, const Point3<Real> &oj, const Point3<Real> &ok)
	{
		Vector3<Real> i = oi - o;
		Vector3<Real> j = oj - o;
		Vector3<Real> k = ok - o;
		iM00 = i.x;				iM01 = j.x;				iM02 = k.x;				iM03 = o.x;
		iM10 = i.y;				iM11 = j.y;				iM12 = k.y;				iM13 = o.y;
		iM20 = i.z;				iM21 = j.z;				iM22 = k.z;				iM23 = o.z
		iM30 = (Real)0.0;		iM31 = (Real)0.0;		iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}

	/**
	Constructor - from base vectors - 2D

	\param i - I'  The vector [1,0] transformed by the matrix
	\param j - J'  The vector [0,1] transformed by the matrix


	i.x		j.x		0		0

	i.y		j.y		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline Matrix4(const Vector2<Real> &i, const Vector2<Real> &j)
	{
		iM00 = i.x;			iM01 = j.x;			iM02 = (Real)0.0;		iM03 = (Real)0.0;
		iM10 = i.y;			iM11 = j.y;			iM12 = (Real)0.0;		iM13 = (Real)0.0;
		iM20 = (Real)0.0;	iM21 = (Real)0.0;	iM22 = (Real)1.0;		iM23 = (Real)0.0;
		iM30 = (Real)0.0;	iM31 = (Real)0.0;	iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}

	/**
	Constructor - from base vectors with translation - 2D

	\param i -I'  The vector [1,0] transformed by the matrix
	\param j -J'  The vector [0,1] transformed by the matrix
	\param translation - translation


	i.x		j.x		0		translation.x

	i.y		j.y		0		translation.y

	0		0		1		0

	0		0		0		1
	*/
	inline Matrix4(const Vector2<Real> &i, const Vector2<Real> &j, const Vector2<Real> &translation)
	{
		iM00 = i.x;			iM01 = j.x;			iM02 = (Real)0.0;		iM03 = translation.x;
		iM10 = i.y;			iM11 = j.y;			iM12 = (Real)0.0;		iM13 = translation.y;
		iM20 = (Real)0.0;	iM21 = (Real)0.0;	iM22 = (Real)1.0;		iM23 = (Real)0.0;
		iM30 = (Real)0.0;	iM31 = (Real)0.0;	iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}

	/**
	Constructor - from base vectors with origin - 2D

	\param o - origin
	\param i -I'  The vector [1,0] transformed by the matrix
	\param j -J'  The vector [0,1] transformed by the matrix


	i.x		j.x		0		o.x

	i.y		j.y		0		o.y

	0		0		1		0

	0		0		0		1
	*/
	inline Matrix4(const Point2<Real> &o, const Vector2<Real> &i, const Vector2<Real> &j)
	{
		iM00 = i.x;			iM01 = j.x;			iM02 = (Real)0.0;		iM03 = o.x;
		iM10 = i.y;			iM11 = j.y;			iM12 = (Real)0.0;		iM13 = o.y;
		iM20 = (Real)0.0;	iM21 = (Real)0.0;	iM22 = (Real)1.0;		iM23 = (Real)0.0;
		iM30 = (Real)0.0;	iM31 = (Real)0.0;	iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}

	/**
	Constructor - from base vectors, as origin and points

	\param o - origin
	\param oi - I'  The point [1,0] transformed by the matrix
	\param oj - J'  The point [0,1] transformed by the matrix


	i = oi - o

	j = oj - o


	i.x		j.x		0		o.x

	i.y		j.y		0		o.y

	0		0		1		0

	0		0		0		1
	*/
	inline Matrix4(const Point2<Real> &o, const Point2<Real> &oi, const Point2<Real> &oj)
	{
		Vector2<Real> i = oi - o;
		Vector2<Real> j = oj - o;
		iM00 = i.x;			iM01 = j.x;			iM02 = (Real)0.0;		iM03 = (Real)o.x;
		iM10 = i.y;			iM11 = j.y;			iM12 = (Real)0.0;		iM13 = (Real)o.y;
		iM20 = (Real)0.0;	iM21 = (Real)0.0;	iM22 = (Real)1.0;		iM23 = (Real)0.0;
		iM30 = (Real)0.0;	iM31 = (Real)0.0;	iM32 = (Real)0.0;		iM33 = (Real)1.0;
	}




	/**
	Read from file
	*/
	inline void read(FILE *f)
	{
		fread( d, sizeof(Real), 16, f );
	}

	/**
	Write to file
	*/
	inline void write(FILE *f) const
	{
		fwrite( d, sizeof(Real), 16, f );
	}




	/**
	Access value at \a i,\a j  m[\a i,\a j]
	*/
	inline Real & at(int i, int j)
	{
		return d[ i*4 + j ];
	}

	/**
	Access value at \a i,\a j  m[\a i,\a j]
	*/
	inline const Real & at(int i, int j) const
	{
		return d[ i*4 + j ];
	}



	/**
	Assignment operator
	*/
	inline Matrix4<Real> &operator=(const Matrix4<Real> &m)
	{
		iM00 = m.iM00;	iM01 = m.iM01;	iM02 = m.iM02;	iM03 = m.iM03;
		iM10 = m.iM10;	iM11 = m.iM11;	iM12 = m.iM12;	iM13 = m.iM13;
		iM20 = m.iM20;	iM21 = m.iM21;	iM22 = m.iM22;	iM23 = m.iM23;
		iM30 = m.iM30;	iM31 = m.iM31;	iM32 = m.iM32;	iM33 = m.iM33;
		return *this;
	}

	/**
	Addition operator: M + N
	*/
	inline Matrix4<Real> operator+(const Matrix4<Real> &m) const
	{
		return Matrix4<Real>(
			iM00 + m.iM00,	iM01 + m.iM01,	iM02 + m.iM02,	iM03 + m.iM03,
			iM10 + m.iM10,	iM11 + m.iM11,	iM12 + m.iM12,	iM13 + m.iM13,
			iM20 + m.iM20,	iM21 + m.iM21,	iM22 + m.iM22,	iM23 + m.iM23,
			iM30 + m.iM30,	iM31 + m.iM31,	iM32 + m.iM32,	iM33 + m.iM33 );
	}

	/**
	Subtraction operator: M - N
	*/
	inline Matrix4<Real> operator-(const Matrix4<Real> &m) const
	{
		return Matrix4<Real>(
			iM00 - m.iM00,	iM01 - m.iM01,	iM02 - m.iM02,	iM03 - m.iM03,
			iM10 - m.iM10,	iM11 - m.iM11,	iM12 - m.iM12,	iM13 - m.iM13,
			iM20 - m.iM20,	iM21 - m.iM21,	iM22 - m.iM22,	iM23 - m.iM23,
			iM30 - m.iM30,	iM31 - m.iM31,	iM32 - m.iM32,	iM33 - m.iM33 );
	}

	/**
	Multiplication by real operator: M * s
	*/
	inline Matrix4<Real> operator*(Real s) const
	{
		return Matrix4<Real>(
			iM00 * s,	iM01 * s,	iM02 * s,	iM03 * s,
			iM10 * s,	iM11 * s,	iM12 * s,	iM13 * s,
			iM20 * s,	iM21 * s,	iM22 * s,	iM23 * s,
			iM30 * s,	iM31 * s,	iM32 * s,	iM33 * s );
	}

	/**
	Multiplication/contatenation operator: M * N
	*/
	inline Matrix4<Real> operator*(const Matrix4<Real> &m) const
	{
		return Matrix4<Real>(
			iM00 * m.iM00	+	iM01 * m.iM10	+	iM02 * m.iM20	+	iM03 * m.iM30,
			iM00 * m.iM01	+	iM01 * m.iM11	+	iM02 * m.iM21	+	iM03 * m.iM31,
			iM00 * m.iM02	+	iM01 * m.iM12	+	iM02 * m.iM22	+	iM03 * m.iM32,
			iM00 * m.iM03	+	iM01 * m.iM13	+	iM02 * m.iM23	+	iM03 * m.iM33,

			iM10 * m.iM00	+	iM11 * m.iM10	+	iM12 * m.iM20	+	iM13 * m.iM30,
			iM10 * m.iM01	+	iM11 * m.iM11	+	iM12 * m.iM21	+	iM13 * m.iM31,
			iM10 * m.iM02	+	iM11 * m.iM12	+	iM12 * m.iM22	+	iM13 * m.iM32,
			iM10 * m.iM03	+	iM11 * m.iM13	+	iM12 * m.iM23	+	iM13 * m.iM33,

			iM20 * m.iM00	+	iM21 * m.iM10	+	iM22 * m.iM20	+	iM23 * m.iM30,
			iM20 * m.iM01	+	iM21 * m.iM11	+	iM22 * m.iM21	+	iM23 * m.iM31,
			iM20 * m.iM02	+	iM21 * m.iM12	+	iM22 * m.iM22	+	iM23 * m.iM32,
			iM20 * m.iM03	+	iM21 * m.iM13	+	iM22 * m.iM23	+	iM23 * m.iM33,

			iM30 * m.iM00	+	iM31 * m.iM10	+	iM32 * m.iM20	+	iM33 * m.iM30,
			iM30 * m.iM01	+	iM31 * m.iM11	+	iM32 * m.iM21	+	iM33 * m.iM31,
			iM30 * m.iM02	+	iM31 * m.iM12	+	iM32 * m.iM22	+	iM33 * m.iM32,
			iM30 * m.iM03	+	iM31 * m.iM13	+	iM32 * m.iM23	+	iM33 * m.iM33 );
	}

	/**
	Negation operator: -M
	*/
	inline Matrix4<Real> operator-() const
	{
		return Matrix4<Real>( -iM00,	-iM01,	-iM02,	-iM03,
							 -iM10,		-iM11,	-iM12,	-iM13,
							 -iM20,		-iM21,	-iM22,	-iM23,
							 -iM30,		-iM31,	-iM32,	-iM33 );
	}


	/**
	Compute the determinant: |M|
	*/
	inline Real determinant() const
	{
		//determinants of 2x2 submatrices
		Real kplo = iMK*iMP-iML*iMO,	jpln = iMJ*iMP-iML*iMN,	jokn = iMJ*iMO-iMK*iMN;
		Real iplm = iMI*iMP-iML*iMM,	iokm = iMI*iMO-iMK*iMM,	injm = iMI*iMN-iMJ*iMM;
		//determinants of 3x3 submatrices
		Real d00 = iMA * (iMF*kplo - iMG*jpln + iMH*jokn);
		Real d01 = iMB * (iME*kplo - iMG*iplm + iMH*iokm);
		Real d02 = iMC * (iME*jpln - iMF*iplm + iMH*injm);
		Real d03 = iMD * (iME*jokn - iMF*iokm + iMG*injm);
		//determinant of the 4x4
		return (iMA*d00 - iMB*d01 + iMC*d02 - iMD*d03);
	}

	/**
	Compute the inverse: M^-1
	*/
	inline Matrix4<Real> inverse() const
	{
		//determinants of 2x2 submatrices
		Real kplo = iMK*iMP-iML*iMO, jpln = iMJ*iMP-iML*iMN,	jokn = iMJ*iMO-iMK*iMN;
		Real iplm = iMI*iMP-iML*iMM, iokm = iMI*iMO-iMK*iMM,	injm = iMI*iMN-iMJ*iMM;
		Real gpho = iMG*iMP-iMH*iMO, ifhn = iMF*iMP-iMH*iMN,	fogn = iMF*iMO-iMG*iMN;
		Real ephm = iME*iMP-iMH*iMM, eogm = iME*iMO-iMG*iMM,	enfm = iME*iMN-iMF*iMM;
		Real glhk = iMG*iML-iMH*iMK, flhj = iMF*iML-iMH*iMJ,	fkgj = iMF*iMK-iMG*iMJ;
		Real elhi = iME*iML-iMH*iMI, ekgi = iME*iMK-iMG*iMI,	ejfi = iME*iMJ-iMF*iMI;
		//determinants of 3x3 submatrices
		Real d00 = (iMF*kplo - iMG*jpln + iMH*jokn);
		Real d01 = (iME*kplo - iMG*iplm + iMH*iokm);
		Real d02 = (iME*jpln - iMF*iplm + iMH*injm);
		Real d03 = (iME*jokn - iMF*iokm + iMG*injm);

		Real d10 = (iMB*kplo - iMC*jpln + iMD*jokn);
		Real d11 = (iMA*kplo - iMC*iplm + iMD*iokm);
		Real d12 = (iMA*jpln - iMB*iplm + iMD*injm);
		Real d13 = (iMA*jokn - iMB*iokm + iMC*injm);

		Real d20 = (iMB*gpho - iMC*ifhn + iMD*fogn);
		Real d21 = (iMA*gpho - iMC*ephm + iMD*eogm);
		Real d22 = (iMA*ifhn - iMB*ephm + iMD*enfm);
		Real d23 = (iMA*fogn - iMB*eogm + iMC*enfm);

		Real d30 = (iMB*glhk - iMC*flhj + iMD*fkgj);
		Real d31 = (iMA*glhk - iMC*elhi + iMD*ekgi);
		Real d32 = (iMA*flhj - iMB*elhi + iMD*ejfi);
		Real d33 = (iMA*fkgj - iMB*ekgi + iMC*ejfi);
		//reciprocal of the determinant of the 4x4
		Real dr = (Real)1.0 / (iMA*d00 - iMB*d01 + iMC*d02 - iMD*d03);

		return Matrix4<Real>( d00*dr,	-d10*dr,		 d20*dr,	-d30*dr,
							 -d01*dr,	 d11*dr,		-d21*dr,	 d31*dr,
							  d02*dr,	-d12*dr,		 d22*dr,	-d32*dr,
							 -d03*dr,	 d13*dr,		-d23*dr,	 d33*dr);
	}

	/**
	Compute the inverse of a *RIGID BODY TRANSFORMATION*

	A rigid body transformation is a matrix transformations that consists of ONLY translations and rotations,
	e.g. no scaling/shearing, etc.

	It is much faster than \a inverse(), but is only applicable in certain circumstances

	It works by transposing the rotation part (3x3 submatrix), and negating the translation part, and multiplying it by the transposed rotation
	*/
	inline Matrix4<Real> rigidBodyInverse() const
	{
		return Matrix4<Real>( iM00,			iM10,		iM20,		-IM03 * iM00  +  -IM13 * iM10  +  -IM23 * iM20,
							  iM01,			iM11,		iM21,		-IM03 * iM01  +  -IM13 * iM11  +  -IM23 * iM21,
							  iM02,			iM12,		iM22,		-IM03 * iM02  +  -IM13 * iM12  +  -IM23 * iM22,
							  (Real)0.0,	(Real)0.0,	(Real)0.0,	(Real)1.0 );
	}


	/**
	Weighted average of two matrices \a a and \a b, with weights \a wa and \a wb:  a * wa  +  b * wb
	*/
	inline static Matrix4<Real> weightedAverage(const Matrix4<Real> &a, Real wa, const Matrix4<Real> &b, Real wb)
	{
		return Matrix4<Real>(
			a.iM00 * wa  +  b.iM00 * wb,		a.iM01 * wa  +  b.iM01 * wb,
			a.iM02 * wa  +  b.iM02 * wb,		a.iM03 * wa  +  b.iM03 * wb,
			a.iM10 * wa  +  b.iM10 * wb,		a.iM11 * wa  +  b.iM11 * wb,
			a.iM12 * wa  +  b.iM12 * wb,		a.iM13 * wa  +  b.iM13 * wb,
			a.iM20 * wa  +  b.iM20 * wb,		a.iM21 * wa  +  b.iM21 * wb,
			a.iM22 * wa  +  b.iM22 * wb,		a.iM23 * wa  +  b.iM23 * wb,
			a.iM30 * wa  +  b.iM30 * wb,		a.iM31 * wa  +  b.iM31 * wb,
			a.iM32 * wa  +  b.iM32 * wb,		a.iM33 * wa  +  b.iM33 * wb );
	}

	/**
	Interpolate between two matrices \a a and \a b, by parameter \a t:  a  +  t(b-a)
	*/
	inline static Matrix4<Real> lerp(const Matrix4<Real> &a, const Matrix4<Real> &b, Real t)
	{
		return a  +  ( b - a ) * t;
	}






	/**
	Transformation: translation

	\param t: translation


	1		0		0		t.x

	0		1		0		t.y

	0		0		1		t.z

	0		0		0		1
	*/
	inline static Matrix4<Real> translate(const Vector3<Real> &t)
	{
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	t.x,
			0.0,	1.0,	0.0,	t.y,
			0.0,	0.0,	1.0,	t.z,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: translation 2D

	\param t: translation


	1		0		0		t.x

	0		1		0		t.y

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> translate(const Vector2<Real> &t)
	{
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	t.x,
			0.0,	1.0,	0.0,	t.y,
			0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: translation

	\param tx: translation X
	\param ty: translation Y
	\param tz: translation Z (default=0.0)


	1		0		0		t.x

	0		1		0		t.y

	0		0		1		t.z

	0		0		0		1
	*/
	inline static Matrix4<Real> translate(Real tx, Real ty, Real tz = 0.0)
	{
		/*
		1		0		0		tx
		0		1		0		ty
		0		0		1		tz
		0		0		0		1
		*/
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	tx,
			0.0,	1.0,	0.0,	ty,
			0.0,	0.0,	1.0,	tz,
			0.0,	0.0,	1.0,	1.0);
	}





	/**
	Transformation: scale

	\param s: scale factor vector


	s.x		0		0		0

	0		s.y		0		0

	0		0		s.z		0

	0		0		0		1
	*/
	inline static Matrix4<Real> scale(const Vector3<Real> &s)
	{
		return Matrix4<Real>(
			s.x,	0.0,	0.0,	0.0,
			0.0,	s.y,	0.0,	0.0,
			0.0,	0.0,	s.z,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: scale 2D

	\param s: scale factor vector


	s.x		0		0		0

	0		s.y		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> scale(const Vector2<Real> &s)
	{
		return Matrix4<Real>(
			s.x,	0.0,	0.0,	0.0,
			0.0,	s.y,	0.0,	0.0,
			0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: scale

	\param sx : scale X
	\param sy : scale Y
	\param sz : scale Z


	sx		0		0		0

	0		sy		0		0

	0		0		sz		0

	0		0		0		1
	*/
	inline static Matrix4<Real> scale(Real sx, Real sy, Real sz = 1.0)
	{
		return Matrix4<Real>(
			sx,		0.0,	0.0,	0.0,
			0.0,	sy,		0.0,	0.0,
			0.0,	0.0,	sz,		0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: uniform scale

	\param s : scale factor


	s		0		0		0

	0		s		0		0

	0		0		s		0

	0		0		0		1
	*/
	inline static Matrix4<Real> scale(Real s)
	{
		return Matrix4<Real>(
			s,		0.0,	0.0,	0.0,
			0.0,	s,		0.0,	0.0,
			0.0,	0.0,	s,		0.0,
			0.0,	0.0,	0.0,	1.0);
	}



	/**
	Transformation: scale along arbitrary axis

	\param axis: axis to scale along
	\param s: scale factor

	e.g.
	scale(Vector3d(1, 0, 0), 2)  will scale by 2x in the X-axis

	The axis can be any unit vector though
	*/
	inline static Matrix4<Real> scale(const Vector3<Real> &axis, Real s)
	{
		//compute the transformed base vectors
		Vector3<Real> i( 1.0, 0.0, 0.0 );
		Vector3<Real> j( 0.0, 1.0, 0.0 );
		Vector3<Real> k( 0.0, 0.0, 1.0 );

		Real sMinus1 = s - 1;

		//computing each vector:
		//
		//v = v  +  axis  *  v dot axis * ( s - 1 )
		//
		i += axis  *  ( axis.x * sMinus1 );		//i dot axis  ==  axis.x
		j += axis  *  ( axis.y * sMinus1 );		//j dot axis  ==  axis.y
		k += axis  *  ( axis.z * sMinus1 );		//k dot axis  ==  axis.z

		//build the matrix
		return Matrix4<Real>( i, j, k );
	}

	/**
	Transformation: scale along arbitrary axis 2D

	\param axis: axis to scale along
	\param s: scale factor

	scale(Vector2d(1, 0), 2)  will scale by 2x in the X-axis

	The axis can be any unit vector though
	*/
	inline static Matrix4<Real> scale(const Vector2<Real> &axis, Real s)
	{
		//compute the transformed base vectors
		Vector2<Real> i( 1.0, 0.0 );
		Vector2<Real> j( 0.0, 1.0 );

		Real sMinus1 = s - 1;

		//computing each vector:
		//
		//v = v  +  axis  *  v dot axis * ( s - 1 )
		//
		i += axis  *  ( axis.x * sMinus1 );		//i dot axis  ==  axis.x
		j += axis  *  ( axis.y * sMinus1 );		//j dot axis  ==  axis.y

		//build the matrix
		return Matrix4<Real>( i, j );
	}





	/**
	Transformation: rotate around X axis

	\param a : rotation angle (in radians)


	let s = sin(a)

			c = cos(a)


	1		0		0		0

	0		c		-s		0

	0		s		c		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateX(Real a)
	{
		Real s = sin(a);
		Real c = cos(a);
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	0.0,
			0.0,	c,		-s,		0.0,
			0.0,	s,		c,		0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate around X axis

	\param s : sin(rotation_angle)
	\param c : cos(rotation_angle)


	1		0		0		0

	0		c		-s		0

	0		s		c		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateX(Real s, Real c)
	{
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	0.0,
			0.0,	c,		-s,		0.0,
			0.0,	s,		c,		0.0,
			0.0,	0.0,	0.0,	1.0);
	}


	/**
	Transformation: rotate 90 degrees around X axis


	1		0		0		0

	0		0		-1		0

	0		1		0		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateX90()
	{
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	0.0,
			0.0,	0.0,	-1.0,	0.0,
			0.0,	1.0,	0.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 180 degrees around X axis


	1		0		0		0

	0		-1		0		0

	0		0		-1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateX180()
	{
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	0.0,
			0.0,	-1.0,	0.0,	0.0,
			0.0,	0.0,	-1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 270 degrees around X axis


	1		0		0		0

	0		0		1		0

	0		-1		0		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateX270()
	{
		return Matrix4<Real>(
			1.0,	0.0,	0.0,	0.0,
			0.0,	0.0,	1.0,	0.0,
			0.0,	-1.0,	0.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}



	/**
	Transformation: rotate around Y axis

	\param a : rotation angle (in radians)


	let s = sin(a)

			c = cos(a)


	c		0		s		0

	0		1		0		0

	-s		0		c		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateY(Real a)
	{
		Real s = sin(a);
		Real c = cos(a);
		return Matrix4<Real>(
			c,		0.0,	s,	0.0,
			0.0,	1.0,	0.0,	0.0,
			-s,		0.0,	c,		0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate around Y axis

	\param s : sin(rotation_angle)
	\param c : cos(rotation_angle)


	c		0		s		0

	0		1		0		0

	-s		0		c		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateY(Real s, Real c)
	{
		return Matrix4<Real>(
			c,		0.0,	s,	0.0,
			0.0,	1.0,	0.0,	0.0,
			-s,		0.0,	c,		0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 90 degrees around Y axis


	1		0		0		0

	0		0		-1		0

	0		1		0		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateY90()
	{
		return Matrix4<Real>(
			0.0,	0.0,	1.0,	0.0,
			0.0,	1.0,	0.0,	0.0,
			-1.0,	0.0,	0.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 180 degrees around Y axis


	-1		0		0		0

	0		1		0		0

	0		0		-1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateY180()
	{
		return Matrix4<Real>(
			-1.0,	0.0,	0.0,	0.0,
			0.0,	1.0,	0.0,	0.0,
			0.0,	0.0,	-1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 270 degrees around Y axis


	0		0		-1		0

	0		1		0		0

	1		0		0		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateY270()
	{
		return Matrix4<Real>(
			0.0,	0.0,	-1.0,	0.0,
			0.0,	1.0,	0.0,	0.0,
			1.0,	0.0,	0.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}



	/**
	Transformation: rotate around Z axis

	\param a : rotation angle (in radians)


	let s = sin(a)

			c = cos(a)


	c		-s		0		0

	s		c		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateZ(Real a)
	{
		Real s = sin(a);
		Real c = cos(a);
		return Matrix4<Real>(
			c,		-s,		0.0,	0.0,
			s,		c,		0.0,	0.0,
			0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate around Z axis

	\param s : sin(rotation_angle)
	\param c : cos(rotation_angle)


	c		-s		0		0

	s		c		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateZ(Real s, Real c)
	{
		return Matrix4<Real>(
			c,		-s,		0.0,	0.0,
			s,		c,		0.0,	0.0,
			0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 90 degrees around Z axis


	0		-1		0		0

	1		0		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateZ90()
	{
		return Matrix4<Real>(
			0.0,	-1.0,	0.0,	0.0,
			1.0,	0.0,	0.0,	0.0,
			0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 180 degrees around Z axis


	-1		0		0		0

	0		-1		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateZ180()
	{
		return Matrix4<Real>(
			-1.0,	0.0,	0.0,	0.0,
			0.0,	-1.0,	0.0,	0.0,
			0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}

	/**
	Transformation: rotate 90 degrees around Z axis


	0		1		0		0

	-1		0		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> rotateZ270()
	{
		return Matrix4<Real>(
			0.0,	1.0,	0.0,	0.0,
			-1.0,	0.0,	0.0,	0.0,
			0.0,	0.0,	1.0,	0.0,
			0.0,	0.0,	0.0,	1.0);
	}



	/**
	Transformation: rotate \a a radians around the Axis \a axis
	*/
	inline static Matrix4<Real> rotate(Axis axis, Real a)
	{
		switch( axis )
		{
		case AXIS_X:
			return Matrix4<Real>::rotateX( a );
			break;
		case AXIS_NEGATIVE_X:
			return Matrix4<Real>::rotateX( -a );
			break;
		case AXIS_Y:
			return Matrix4<Real>::rotateY( a );
			break;
		case AXIS_NEGATIVE_Y:
			return Matrix4<Real>::rotateY( -a );
			break;
		case AXIS_Z:
			return Matrix4<Real>::rotateZ( a );
			break;
		case AXIS_NEGATIVE_Z:
			return Matrix4<Real>::rotateZ( -a );
			break;
		default:
			return Matrix4<Real>();
			break;
		}
	}

	/**
	Transformation: rotate \a a radians around an arbitrary \a axis
	*/
	inline static Matrix4<Real> rotate(const Vector3<Real> &axis, Real a)
	{
		Real w, x, y, z;
		Real xx, yy, zz, xy, xz, yz, wx, wy, wz;
		Real s;

		a *= (Real)0.5;
		s = sin( a );
		w = cos( a );
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;

		xx = x * x * (Real)2.0;		yy = y * y * (Real)2.0;		zz = z * z * (Real)2.0;
		xy = x * y * (Real)2.0;		xz = x * z * (Real)2.0;		yz = y * z * (Real)2.0;
		wx = w * x * (Real)2.0;		wy = w * y * (Real)2.0;		wz = w * z * (Real)2.0;

		/*
		the quaternion q:
								w=cos(a/2)
								x=axis.x*sin(a/2)
								y=axis.y*sin(a/2)
								z=axis.z*sin(a/2) represents the rotation

		matrix:
		1 - 2yy - 2zz		2xy - 2wz			2xz + 2wy			0
		2xy + 2wz			1 - 2xx - 2zz		2yz - 2wx			0
		2xz - 2wy			2yz + 2wx			1 - 2xx - 2yy		0
		0					0					0					1
		*/
		return Matrix4<Real>(
			1.0 - (yy + zz),		xy - wz,				xz + wy,				0.0,
			xy + wz,				1.0 - (xx + zz),		yz - wx,				0.0,
			xz - wy,				yz + wx,				1.0 - (xx + yy),		0.0,
			0.0,					0.0,					0.0,					1.0);
	}

	/**
	Transformation: rotation, specified by the Euler rotation \a euler
	*/
	inline static Matrix4<Real> rotate(const EulerRotation<Real> &euler)
	{
		Quaternion<Real> q = Quaternion<Real>::rotate( euler );
		return Matrix4<Real>::rotate( q );
	}

	/**
	Transformation: rotation, specified by the Quaternion \a q


	let x, y, z, and w be the components of the quaternion \a q:
	

	1 - 2yy - 2zz		2xy - 2wz			2xz + 2wy			0

	2xy + 2wz			1 - 2xx - 2zz		2yz - 2wx			0

	2xz - 2wy			2yz + 2wx			1 - 2xx - 2yy		0

	0					0					0					1
	*/
	inline static Matrix4<Real> rotate(const Quaternion<Real> &q)
	{
		Real xx, yy, zz, xy, xz, yz, wx, wy, wz;

		xx = q.x * q.x * 2.0;	yy = q.y * q.y * 2.0;	zz = q.z * q.z * 2.0;
		xy = q.x * q.y * 2.0;	xz = q.x * q.z * 2.0;	yz = q.y * q.z * 2.0;
		wx = q.w * q.x * 2.0;	wy = q.w * q.y * 2.0;	wz = q.w * q.z * 2.0;


		// matrix:
		// 1 - 2yy - 2zz		2xy - 2wz			2xz + 2wy			0
		// 2xy + 2wz			1 - 2xx - 2zz		2yz - 2wx			0
		// 2xz - 2wy			2yz + 2wx			1 - 2xx - 2yy		0
		// 0					0					0					1

		return Matrix4<Real>(
			1.0 - (yy + zz),	xy - wz,			xz + wy,			0.0,
			xy + wz,			1.0 - (xx + zz),	yz - wx,			0.0,
			xz - wy,			yz + wx,			1.0 - (xx + yy),	0.0,
			0.0,				0.0,				0.0,				1.0);
	}



	/**
	Transformation: rotate 90 degrees around the Axis \a axis
	*/
	inline static Matrix4<Real> rotate90(Axis axis)
	{
		switch( axis )
		{
		case AXIS_X:
			return Matrix4<Real>::rotateX90();
			break;
		case AXIS_NEGATIVE_X:
			return Matrix4<Real>::rotateX270();
			break;
		case AXIS_Y:
			return Matrix4<Real>::rotateY90();
			break;
		case AXIS_NEGATIVE_Y:
			return Matrix4<Real>::rotateY270();
			break;
		case AXIS_Z:
			return Matrix4<Real>::rotateZ90();
			break;
		case AXIS_NEGATIVE_Z:
			return Matrix4<Real>::rotateZ270();
			break;
		default:
			return Matrix4<Real>();
			break;
		}
	}

	/**
	Transformation: rotate 180 degrees around the Axis \a axis
	*/
	inline static Matrix4<Real> rotate180(Axis axis)
	{
		switch( axis )
		{
		case AXIS_X:
			return Matrix4<Real>::rotateX180();
			break;
		case AXIS_NEGATIVE_X:
			return Matrix4<Real>::rotateX180();
			break;
		case AXIS_Y:
			return Matrix4<Real>::rotateY180();
			break;
		case AXIS_NEGATIVE_Y:
			return Matrix4<Real>::rotateY180();
			break;
		case AXIS_Z:
			return Matrix4<Real>::rotateZ180();
			break;
		case AXIS_NEGATIVE_Z:
			return Matrix4<Real>::rotateZ180();
			break;
		default:
			return Matrix4<Real>();
			break;
		}
	}

	/**
	Transformation: rotate 270 degrees around the Axis \a axis
	*/
	inline static Matrix4<Real> rotate270(Axis axis)
	{
		switch( axis )
		{
		case AXIS_X:
			return Matrix4<Real>::rotateX270();
			break;
		case AXIS_NEGATIVE_X:
			return Matrix4<Real>::rotateX90();
			break;
		case AXIS_Y:
			return Matrix4<Real>::rotateY270();
			break;
		case AXIS_NEGATIVE_Y:
			return Matrix4<Real>::rotateY90();
			break;
		case AXIS_Z:
			return Matrix4<Real>::rotateZ270();
			break;
		case AXIS_NEGATIVE_Z:
			return Matrix4<Real>::rotateZ90();
			break;
		default:
			return Matrix4<Real>();
			break;
		}
	}





	/**
	Transformation: shear the X-Axis

	\param yGrad: Y shear gradient  (y/x)
	\param zGrad: Z shear gradient  (z/x)


	1		0		0		0

	yGrad	1		0		0

	zGrad	0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> shearX(Real yGrad, Real zGrad)
	{
		/*
		1			sy/sx		sz/sx		0
		0			1			0			0
		0			0			1			0
		0			0			0			1
		*/
		return Matrix4<Real>(
				1.0,			0.0,			0.0,			0.0,
				yGrad,			1.0,			0.0,			0.0,
				zGrad,			0.0,			1.0,			0.0,
				0.0,			0.0,			0.0,			1.0);
	}

	/**
	Transformation: shear the Y-Axis

	\param xGrad: X shear gradient  (x/y)
	\param zGrad: Z shear gradient  (z/y)


	1		xGrad	0		0

	0		1		0		0

	0		zGrad	1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> shearY(Real xGrad, Real zGrad)
	{
		/*
		1			0			0			0
		sx/sy		1			sz/sy		0
		0			0			1			0
		0			0			0			1
		*/
		return Matrix4<Real>(
				1.0,			xGrad,			0.0,			0.0,
				0.0,			1.0,			0.0,			0.0,
				0.0,			zGrad,			1.0,			0.0,
				0.0,			0.0,			0.0,			1.0);
	}

	/**
	Transformation: shear the Z-Axis

	\param xGrad: X shear gradient  (x/z)
	\param yGrad: Y shear gradient  (y/z)


	1		0		xGrad	0

	0		1		yGrad	0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> shearZ(Real xGrad, Real yGrad)
	{
		return Matrix4<Real>(
				1.0,			0.0,			xGrad,			0.0,
				0.0,			1.0,			yGrad,			0.0,
				0.0,			0.0,			1.0,			0.0,
				0.0,			0.0,			0.0,			1.0);
	}

	/**
	Transformation: shear an arbitrary axis

	\param shearAxis: the axis to be sheared
	\param shearVector: the shear offset vector
	*/
	inline static Matrix4<Real> shear(const Vector3<Real> &shearAxis, const Vector3<Real> &shearVector)
	{
		Real i = shearAxis.x;			// [1,0,0] (dot) shearAxis
		Real j = shearAxis.y;			// [0,1,0] (dot) shearAxis
		Real k = shearAxis.z;			// [0,0,1] (dot) shearAxis

		//compute the base vectors
		Vector3<Real> baseI = Vector3<Real>( 1.0, 0.0, 0.0 )  +  shearVector * i;
		Vector3<Real> baseJ = Vector3<Real>( 0.0, 1.0, 0.0 )  +  shearVector * j;
		Vector3<Real> baseK = Vector3<Real>( 0.0, 0.0, 1.0 )  +  shearVector * k;

		return Matrix4<Real>( baseI, baseJ, baseK );
	}

	/**
	Transformation: shear an arbitrary axis 2D

	\param shearAxis: the axis to be sheared
	\param shearVector: the shear offset vector
	*/
	inline static Matrix4<Real> shear(const Vector2<Real> &shearAxis, const Vector2<Real> &shearVector)
	{
		Real i = shearAxis.x;			// [1,0] (dot) shearAxis
		Real j = shearAxis.y;			// [0,1] (dot) shearAxis

		//compute the base vectors
		Vector2<Real> baseI = Vector2<Real>( 1.0, 0.0 )  +  shearVector * i;
		Vector2<Real> baseJ = Vector2<Real>( 0.0, 1.0 )  +  shearVector * j;

		return Matrix4<Real>( baseI, baseJ );
	}





	/**
	Transformation: reflect in a place perpendicular to \a n


	let x, y, and z be the components of \a n


	1 - 2xx			-2xy			-2xz			0

	-2yx			1 - 2yy			-2yz			0

	-2zx			-2zy			1 - 2zz			0

	0				0				0				1
	*/
	inline static Matrix4<Real> reflect(const Vector3<Real> &n)
	{
		Real nx = -2.0 * n.x,   ny = -2.0 * n.y,   nz = -2.0 * n.z;
		return Matrix4<Real>(
			1.0 + nx * n.x,		nx * n.y, 			nx * n.z,			0.0,
			ny * n.x,			1.0 + ny * n.y,		ny * n.z,			0.0,
			nz * n.x,			nz * n.y,			1.0 + nz * n.z,		0.0,
			0.0,				0.0,				0.0,				1.0);
	}

	/**
	Transformation: reflect in the plane \a p


	let x, y, and z be the components of the normal of \a p

	let d be the distance from \a p to the origin


	1 - 2xx			-2xy			-2xz			2dx

	-2yx			1 - 2yy			-2yz			2dy

	-2zx			-2zy			1 - 2zz			2dz

	0				0				0				1
	*/
	inline static Matrix4<Real> reflect(const Plane<Real> &p)
	{
		Real nx = -2 * p.n.x,   ny = -2 * p.n.y,   nz = -2 * p.n.z;
		return Matrix4<Real>(
			1.0 + nx * p.n.x,	nx * p.n.y, 		nx * p.n.z,			-nx * p.d,
			ny * p.n.x,			1.0 + ny * p.n.y,	ny * p.n.z,			-ny * p.d,
			nz * p.n.x,			nz * p.n.y,			1.0 + nz * p.n.z,	-nz * p.d,
			0.0,				0.0,				0.0,				1.0);
	}






	/**
	Transformation: space to space

	\param i - I'  the vector [1,0,0] transformed into the source space
	\param j - J'  the vector [0,1,0] transformed into the source space
	\param k - K'  the vector [0,0,1] transformed into the source space
	\param u - I''  the vector [1,0,0] transformed into the destination space
	\param v - J''  the vector [0,1,0] transformed into the destination space
	\param w - K''  the vector [0,0,1] transformed into the destination space

	effectively [u,v,w]  *  [i,j,k]^-1
	*/
	inline static Matrix4<Real> spaceToSpace(const Vector3<Real> &i, const Vector3<Real> &j, const Vector3<Real> &k, const Vector3<Real> &u, const Vector3<Real> &v, const Vector3<Real> &w)
	{
		//inverse of matrix ijk multiplied by matrix uvw
		return Matrix4<Real>( u, v, w )  *  Matrix4<Real>( i, j, k ).inverse();
	}

	/**
	Transformation: space to space 2D

	\param i - I'  the vector [1,0] transformed into the source space
	\param j - J'  the vector [0,1] transformed into the source space
	\param u - I''  the vector [1,0] transformed into the destination space
	\param v - J''  the vector [0,1] transformed into the destination space

	effectively (u,v)  *  (i,j)^-1
	*/
	inline static Matrix4<Real> spaceToSpace(const Vector2<Real> &i, const Vector2<Real> &j, const Vector2<Real> &u, const Vector2<Real> &v)
	{
		//inverse of matrix ij multiplied by matrix uv
		return Matrix4<Real>( u, v )  *  Matrix4<Real>( i, j ).inverse();
	}

	/**
	Transformation: space to space - triangle to triangle

	\param t0 - the triangle whose edges and normal form the base vectors of the source space
	\param t1 - the triangle whose edges and normal form the base vectors of the destination space

	Computes the matrix that will transform triangle \a t0 to triangle \a t1

	effectively [t1.edge0,t1.edge1,t1.normal,t1.origin]  *  [t0.edge0,t0.edge1,t0.normal,t0.origin]^-1
	*/
	inline static Matrix4<Real> spaceToSpace(const Triangle3<Real> &t0, const Triangle3<Real> &t1)
	{
		Vector3<Real> i = t0.b - t0.a;
		Vector3<Real> j = t0.c - t0.a;
		Vector3<Real> k = i.cross( j );

		Vector3<Real> u = t1.b - t1.a;
		Vector3<Real> v = t1.c - t1.a;
		Vector3<Real> w = u.cross( v );

		//inverse of matric ijk multiplied by matrix uvw
		return Matrix4<Real>( t1.a, u, v, w )  *  Matrix4<Real>( t0.a, i, j, k ).inverse();
	}

	/**
	Transformation: space to space - triangle to triangle

	\param t0 - the triangle whose edges and normal form the base vectors of the source space
	\param t1 - the triangle whose edges and normal form the base vectors of the destination space

	Computes the matrix that will transform triangle \a t0 to triangle \a t1

	effectively [t1.edge0,t1.edge1,t1.origin]  *  [t0.edge0,t0.edge1,t0.origin]^-1
	*/
	inline static Matrix4<Real> spaceToSpace(const Triangle2<Real> &t0, const Triangle2<Real> &t1)
	{
		Vector2<Real> i = t0.b - t0.a;
		Vector2<Real> j = t0.c - t0.a;

		Vector2<Real> u = t1.b - t1.a;
		Vector2<Real> v = t1.c - t1.a;

		//inverse of matric ijk multiplied by matrix uvw
		return Matrix4<Real>( t1.a, u, v )  *  Matrix4<Real>( t0.a, i, j ).inverse();
	}




	/**
	Transformation: axis to axis - rotation from one arbitrary vector to another

	Computes the rotation that will rotate a point on the line of vector \a src, onto the line of vector \a dst
	*/
	inline static Matrix4<Real> axisToAxis(const Vector3<Real> &src, const Vector3<Real> &dst)
	{
		//find the angle between the vectors
		//and generate a rotation matrix, rotating that amount, where the axis
		//of rotation is the vector that is perpendicular to src and dst
		Real cosAngle = src.dot( dst );

		//check if src and dst are the same vector (include margin of error)
		if ( cosAngle  >  1.0 - EPSILON )
		{
			//if so, result is identity matrix
			return Matrix4<Real>();
		}
		else if ( cosAngle  <  -1.0 + EPSILON )		//check if they oppose
		{
			//get a vector perpendicular to the src vector
			Vector3<Real> axis = src.perpendicularVector();
			axis.normalise();

			//rotate around this by 180 degrees
			return Matrix4<Real>::rotate( axis, M_PI * 2.0 );
		}
		else
		{
			//calculate rotation axis
			Vector3<Real> axis = src.cross( dst );
			axis.normalise();
			//calculate angle
			Real angle = acos( cosAngle );
			//generate the matrix
			return Matrix4<Real>::rotate( axis, angle );
		}
	}

	/**
	Transformation: axis to axis - rotation from one arbitrary Axis to another
	*/
	inline static Matrix4<Real> axisToAxis(Axis src, Axis dst)
	{
		//s is a scale factor, used to reduce code size, it is used to
		//invert the effect of the matrix in the case of a negative axis
		Real s = 1.0;

		switch( src )
		{
		case AXIS_NEGATIVE_X:
			s = -1.0;
		case AXIS_X:
			switch( dst )
			{
			case AXIS_NEGATIVE_X:
				s = -s;
			case AXIS_X:
				return Matrix4<Real>(
						s,		0.0,	0.0,	0.0,
						0.0,	1.0,	0.0,	0.0,
						0.0,	0.0,	s,		0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			case AXIS_NEGATIVE_Y:
				s = -s;
			case AXIS_Y:
				return Matrix4<Real>(
						0.0,	-s,		0.0,	0.0,
						s,		0.0,	0.0,	0.0,
						0.0,	0.0,	1.0,	0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			case AXIS_NEGATIVE_Z:
				s = -s;
			case AXIS_Z:
				return Matrix4<Real>(
						0.0,	0.0,	-s,		0.0,
						0.0,	1.0,	0.0,	0.0,
						s,		0.0,	0.0,	0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			default:
				return Matrix4<Real>();
			}
			break;

		case AXIS_NEGATIVE_Y:
			s = -1.0;
		case AXIS_Y:
			switch( dst )
			{
			case AXIS_NEGATIVE_X:
				s = -s;
			case AXIS_X:
				return Matrix4<Real>(
						0.0,	s,	0.0,	0.0,
						-s,		0.0,	0.0,	0.0,
						0.0,	0.0,	1.0,	0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			case AXIS_NEGATIVE_Y:
				s = -s;
			case AXIS_Y:
				return Matrix4<Real>(
						1.0,	0.0,	0.0,	0.0,
						0.0,	s,		0.0,	0.0,
						0.0,	0.0,	s,		0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			case AXIS_NEGATIVE_Z:
				s = -s;
			case AXIS_Z:
				return Matrix4<Real>(
						1.0,	0.0,	0.0,	0.0,
						0.0,	0.0,	-s,		0.0,
						0.0,	s,		0.0,	0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			default:
				return Matrix4<Real>();
			}
			break;

		case AXIS_NEGATIVE_Z:
			s = -1.0;
		case AXIS_Z:
			switch( dst )
			{
			case AXIS_NEGATIVE_X:
				s = -s;
			case AXIS_X:
				return Matrix4<Real>(
						0.0,	0.0,	s,		0.0,
						0.0,	1.0,	0.0,	0.0,
						-s,		0.0,	0.0,	0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			case AXIS_NEGATIVE_Y:
				s = -s;
			case AXIS_Y:
				return Matrix4<Real>(
						1.0,	0.0,	0.0,	0.0,
						0.0,	0.0,	s,		0.0,
						0.0,	-s,		0.0,	0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			case AXIS_NEGATIVE_Z:
				s = -s;
			case AXIS_Z:
				return Matrix4<Real>(
						s,		0.0,	0.0,	0.0,
						0.0,	1.0,	0.0,	0.0,
						0.0,	0.0,	s,		0.0,
						0.0,	0.0,	0.0,	1.0 );
				break;
			default:
				return Matrix4<Real>();
			}
			break;
		default:
			return Matrix4();
		}
	}





	/**
	Transformation: look at

	\param position - position of camera
	\param lookatPoint - focus of camera
	\param up - vector pointing upwards (from perpsective of camera)
	*/
	inline static Matrix4<Real> lookat(const Point3<Real> &position, const Point3<Real> &lookatPoint, const Vector3<Real> &up)
	{
		Vector3<Real> lookDirection = lookatPoint - position;
		Vector3<Real> right = lookDirection.cross( up );

		if ( right.sqrLength() < SQR_EPSILON )
		{
			right = Vector3( 1.0, 0.0, 0.0 );
		}

		Vector3<Real> realUp = right.cross( lookDirection );

		lookDirection.normalise();
		right.normalise();
		realUp.normalise();

		return Matrix4<Real>::translate( position.toVector3() )  *  Matrix4<Real>( right, realUp, -lookDirection );
	}

	/**
	Transformation: look at

	\param lookDirection - camera's 'forward' direction
	\param up - vector pointing upwards (from perpsective of camera)
	*/
	inline static Matrix4<Real> lookat(const Vector3<Real> &lookDirection, const Vector3<Real> &up)
	{
		Vector3<Real> right = lookDirection.cross( up );

		if ( right.sqrLength() < SQR_EPSILON )
		{
			right = Vector3( 1.0, 0.0, 0.0 );
		}

		Vector3<Real> realUp = right.cross( lookDirection );

		right.normalise();
		realUp.normalise();

		return Matrix4<Real>( right, realUp, -lookDirection.getNormalised() );
	}





	/**
	Transformation: perspective projection

	\param nearClip - near clip plane distance
	\param farClip - far clip plane distance


	let a = far  /  ( near - far )

		b = far * near  /  ( near - far )


	Input point will have:

		z -> in range (-near,-far)						(input)

		w = 1.0											(input)


	We transform it like so:

		z' = az + b										(transformation)

		w' = -z											(transformation)

		z'' = ( z' / w' )  ->  in range (0,1)  (homogeneous divide)


	1		0		0		0

	0		1		0		0

	0		0		a		b

	0		0		-1		0
	*/
	inline static Matrix4<Real> perspective(Real nearClip, Real farClip)
	{
		/*
		*/
		Real a = farClip  /  ( nearClip - farClip );
		Real b = farClip * nearClip  /  ( nearClip - farClip );
		return Matrix4<Real>(
			1.0,		0.0,		0.0,		0.0,
			0.0,		1.0,		0.0,		0.0,
			0.0,		0.0,		a,			b,
			0.0,		0.0,		-1.0,		0.0 );
	}





	/**
	Transformation: orthographic projection

	\param nearClip - near clip plane distance
	\param farClip - far clip plane distance


	let a = 1  /  ( near - far )

		b = near  /  ( near - far )


	Input point will have:

		z -> in range (-near,-far)						(input)

		w = 1.0											(input)


	We transform it like so:

		z' = az + b										(transformation)

		w' = 1.0											(transformation)

		z'' = z' / w'  =  z' / 1.0  =  z' z'  ->  in range (0,1)  (homogeneous divide)


	1		0		0		0

	0		1		0		0

	0		0		a		b

	0		0		0		1
	*/
	inline static Matrix4<Real> orthographic(Real nearClip, Real farClip)
	{
		Real a = 1.0  /  ( nearClip - farClip );
		Real b = nearClip  /  ( nearClip - farClip );
		return Matrix4<Real>(
			1.0,		0.0,		0.0,		0.0,
			0.0,		1.0,		0.0,		0.0,
			0.0,		0.0,		a,			b,
			0.0,		0.0,		0.0,		1.0 );
	}





	/**
	Transformation: camera field of view transformation
	
	\param fovY - vertical field of view in radians
	\param aspectRatio - the aspect ratio of the screen


	let y = tan(fovY/2)

		x = y*aspectRatio


	1/x		0		0		0

	0		1/y		0		0

	0		0		1		0

	0		0		0		1
	*/
	inline static Matrix4<Real> camera(Real fovY, Real aspectRatio)
	{
		Real x, y;

		y = tan( fovY * 0.5 );
		x = y * aspectRatio;

		return Matrix4<Real>(
			1.0 / x,	0.0,		0.0,		0.0,
			0.0,		1.0 / y,	0.0,		0.0,
			0.0,		0.0,		1.0,		0.0,
			0.0,		0.0,		0.0,		1.0 );
	}





	/**
	Transformation: screen - transforms the range [-1,1]->[-1,1] to [0,0]->[width,height]

	\param width - the width of the screen
	\param height - the width of the height
	

	width/2		0			0		width/w

	0			height/2	0		height/2

	0			0			1		0

	0			0			0		1
	*/
	inline static Matrix4<Real> screen(Real width, Real height)
	{
		Real halfW = width * 0.5;
		Real halfH = height * 0.5;
		return Matrix4<Real>(
			halfW,	0.0,		0.0,		halfW,
			0.0,		halfH,	0.0,		halfH,
			0.0,		0.0,	1.0,		0.0,
			0.0,		0.0,	0.0,		1.0 );
	}








	/**
	Transform a unit vector, ensuring thats its length squared is within the range [minLengthSquared,maxLengthSquared]
	*/
	inline Vector3<Real> transformUnit(const Vector3<Real> &v, Real minLengthSquared, Real maxLengthSquared)
	{
		Vector3<Real> result = v * (*this);
		Real l = result.sqrLength();
		if ( ( l < minLengthSquared )  ||  ( l > maxLengthSquared ) )
		{
			//length not within [minLengthSquared,maxLengthSquared],
			//normalise result
			result.normalise();
		}

		return result;
	}


	/**
	Homogeneous transformation: transforms a Point3<Real> \a p3 by \a this, and performs the perpsective divide

	Use this when working the transformations that are constructed using either the \a perspective() or \a orthographic() methods
	*/
	inline Point3<Real> transformHomogeneous(const Point3<Real> &p3) const
	{
		Point4<Real> p = Point4<Real>( p3 );

		Real x =	p.x * iM00  +  p.y * iM01  +  p.z * iM02  +  p.w * iM03;
		Real y =	p.x * iM10  +  p.y * iM11  +  p.z * iM12  +  p.w * iM13;
		Real z =	p.x * iM20  +  p.y * iM21  +  p.z * iM22  +  p.w * iM23;
		p.w =		p.x * iM30  +  p.y * iM31  +  p.z * iM32  +  p.w * iM33;
		p.x = x;
		p.y = y;
		p.z = z;

		return p.convertToPoint3();
	}

	/**
	Homogeneous transformation: transforms a Point2<Real> \a p2 with z-co-ordinate in \a z by \a this, and performs the perpsective divide

	Use this when working the transformations that are constructed using either the perspective() or orthographic() methods
	*/	inline Point3<Real> transformHomogeneous(const Point2<Real> &p2, Real z) const
	{
		return transformHomogeneous( Point3<Real>( p2, z ) );
	}

	/**
	Homogeneous transformation: transforms a Point3<Real> \a p3 by \a this, performs the perpsective divide, then converts to a Point2<Real>

	Use this when working the transformations that are constructed using either the perspective() or orthographic() methods
	*/
	inline Point2<Real> transformHomogeneous2d(const Point3<Real> &p3) const
	{
		return transformHomogeneous( p3 ).toPoint2();
	}


	/**
	Homogeneous transformation: transforms a Segment3<Real> \a seg3 by \a this, and performs the perpsective divide

	Use this when working the transformations that are constructed using either the perspective() or orthographic() methods
	*/
	inline Segment3<Real> transformHomogeneous(const Segment3<Real> &seg3) const
	{
		return Segment3<Real>( transformHomogeneous( seg3.a ), transformHomogeneous( seg3.b ) );
	}

	/**
	Homogeneous transformation: transforms a Segment3<Real> \a seg3 by \a this, performs the perpsective divide, then converts to a Segment2<Real>

	Use this when working the transformations that are constructed using either the perspective() or orthographic() methods
	*/
	inline Segment2<Real> transformHomogeneous2d(const Segment3<Real> &seg3) const
	{
		return Segment2<Real>( transformHomogeneous2d( seg3.a ), transformHomogeneous2d( seg3.b ) );
	}


	/**
	Homogeneous transformation: transforms a Triangle3<Real> \a t3 by \a this, and performs the perpsective divide

	Use this when working the transformations that are constructed using either the perspective() or orthographic() methods
	*/
	inline Triangle3<Real> transformHomogeneous(const Triangle3<Real> &t3) const
	{
		return Triangle3<Real>( transformHomogeneous( t3.a ),
								transformHomogeneous( t3.b ),
								transformHomogeneous( t3.c ) );
	}

	/**
	Homogeneous transformation: transforms a Triangle3<Real> \a t3 by \a this, performs the perpsective divide, then converts to a Triangle2<Real>

	Use this when working the transformations that are constructed using either the perspective() or orthographic() methods
	*/
	inline Triangle2<Real> transformHomogeneous2d(const Triangle3<Real> &t3) const
	{
		return Triangle2<Real>( transformHomogeneous2d( t3.a ),
								transformHomogeneous2d( t3.b ),
								transformHomogeneous2d( t3.c ) );
	}


	/**
	Transforms a point \ p by this, returning only the z-co-ordinate of the resulting point
	*/
	inline Real transformZ(const Point3<Real> &p) const
	{
		return p.x * iM20 +	p.y * iM21 +	p.z * iM22 +	iM23;
	}

	/**
	Transforms a point \a p by this, performs the perpsective divide, returning only the z-co-ordinate of the resulting point
	*/
	inline Real transformZHomogeneous(const Point3<Real> &p) const
	{
		Real z = p.x * iM20  +  p.y * iM21  +  p.z * iM22  +  iM32;
		Real w = p.x * iM30  +  p.y * iM31  +  p.z * iM32  +  iM33;
		return z / w;
	}
};


/**
Multiply a Matrix4<Real> by a Point2<Real>

Applies the transformation represented by \a m to \a p
*/
template <typename Real> inline Point2<Real> operator*(const Matrix4<Real> &m, const Point2<Real> &p)
{
	return Point2<Real>(
		p.x * m.iM00  +  p.y * m.iM01  +  m.iM03,
		p.x * m.iM10  +  p.y * m.iM11  +  m.iM13 );
}

/**
Multiply a Matrix4<Real> by a Point3<Real>

Applies the transformation represented by \a m to \a p
*/
template <typename Real> inline Point3<Real> operator*(const Matrix4<Real> &m, const Point3<Real> &p)
{
	return Point3<Real>(
		p.x * m.iM00  +  p.y * m.iM01  +  p.z * m.iM02  +  m.iM03,
		p.x * m.iM10  +  p.y * m.iM11  +  p.z * m.iM12  +  m.iM13,
		p.x * m.iM20  +  p.y * m.iM21  +  p.z * m.iM22  +  m.iM23 );
}

/**
Multiply a Matrix4<Real> by a Point4<Real>

Applies the transformation represented by \a m to \a p
*/
template <typename Real> inline Point4<Real> operator*(const Matrix4<Real> &m, const Point4<Real> &p)
{
	return Point4<Real>(
		p.x * m.iM00  +  p.y * m.iM01  +  p.z * m.iM02  +  p.w * m.iM03,
		p.x * m.iM10  +  p.y * m.iM11  +  p.z * m.iM12  +  p.w * m.iM13,
		p.x * m.iM20  +  p.y * m.iM21  +  p.z * m.iM22  +  p.w * m.iM23,
		p.x * m.iM30  +  p.y * m.iM31  +  p.z * m.iM32  +  p.w * m.iM33 );
}

/**
Multiply a Matrix4<Real> by a Vector2<Real>

Applies the transformation represented by \a m to \a v

NOTE: translations are *NOT* applied to vectors
*/
template <typename Real> inline Vector2<Real> operator*(const Matrix4<Real> &m, const Vector2<Real> &v)
{
	return Vector2<Real>(
		v.x * m.iM00  +  v.y * m.iM01,
		v.x * m.iM10  +  v.y * m.iM11 );
}

/**
Multiply a Matrix4<Real> by a Vector3<Real>

Applies the transformation represented by \a m to \a v

NOTE: translations are *NOT* applied to vectors
*/
template <typename Real> inline Vector3<Real> operator*(const Matrix4<Real> &m, const Vector3<Real> &v)
{
	return Vector3<Real>(
		v.x * m.iM00  +  v.y * m.iM01  +  v.z * m.iM02,
		v.x * m.iM10  +  v.y * m.iM11  +  v.z * m.iM12,
		v.x * m.iM20  +  v.y * m.iM21  +  v.z * m.iM22 );
}

/**
Multiply a Matrix4<Real> by a Plane<Real>

Applies the transformation represented by \a m to \a p
*/
template <typename Real> inline Plane<Real> operator*(const Matrix4<Real> &m, const Plane<Real> &p)
{
	Point3<Real> a = m  *  p.getPointOnPlane();
	Vector3<Real> n = ( m * p.n ).getNormalised();
	return Plane<Real>( n, a );
}

/**
Multiply a Matrix4<Real> by a Segment3<Real>

Applies the transformation represented by \a m to \a seg
*/
template <typename Real> inline Segment3<Real> operator*(const Matrix4<Real> &m, const Segment3<Real> &seg)
{
	return Segment3<Real>( m * seg.a, m * seg.b );
}

/**
Multiply a Matrix4<Real> by a Triangle3<Real>

Applies the transformation represented by \a m to \a t
*/
template <typename Real> inline Triangle3<Real> operator*(const Matrix4<Real> &m, const Triangle3<Real> &t)
{
	return Triangle3<Real>( m * t.a, m * t.b, m * t.c );
}



typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;


#endif
