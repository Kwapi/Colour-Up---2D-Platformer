//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef GLMATRIX_H__
#define GLMATRIX_H__

#include <Math/Point3.h>
#include <Math/Vector3.h>
#include <Math/Matrix4.h>



void glGetMatrixf(float *matrix);
void glGetMatrixd(double *matrix);

void glGetMatrixf(Matrix4f &m);
void glGetMatrixd(Matrix4d &m);

void glMultMatrixf(const Matrix4f &m);
void glMultMatrixd(const Matrix4d &m);

void glVertex3f(const Point3f &p);
//void glVertex3d(const Point3d &p);

void glNormal3f(const Vector3f &n);
//void glNormal3d(const Vector3d &n);


#endif
