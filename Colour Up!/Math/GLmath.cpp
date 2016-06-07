//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef GLMATRIX_CPP__
#define GLMATRIX_CPP__

#include <Math/GLmath.h>

#include <windows.h>
#include <GL/gl.h>



/**
Get the current OpenGL matrix stack.
*/
static GLenum glCurrentMatrixStack()
{
	GLint mode;
	glGetIntegerv( GL_MATRIX_MODE, &mode );
	return mode == GL_MODELVIEW ? GL_MODELVIEW_MATRIX :
			 mode == GL_PROJECTION ? GL_PROJECTION_MATRIX :
			 GL_TEXTURE_MATRIX;
}

/**
Get the matrix on the top of the current OpenGL matrix stack.

\param matrix A pointer to an array of 16 floats in which to store the current matrix.
*/
void glGetMatrixf(float *matrix)
{
	glGetFloatv( glCurrentMatrixStack(), matrix );
}

/**
Get the matrix on the top of the current OpenGL matrix stack.

\param matrix A pointer to an array of 16 doubles in which to store the current matrix.
*/
void glGetMatrixd(double *matrix)
{
	glGetDoublev( glCurrentMatrixStack(), matrix );
}


/**
Get the matrix on the top of the current OpenGL matrix stack.

\param m A reference to a Matrix4f in which to store the current matrix.
*/
void glGetMatrixf(Matrix4f &m)
{
	glGetFloatv( glCurrentMatrixStack(), m.d );
}

/**
Get the matrix on the top of the current OpenGL matrix stack.

\param m A reference to a Matrix4d in which to store the current matrix.
*/
void glGetMatrixd(Matrix4d &m)
{
	glGetDoublev( glCurrentMatrixStack(), m.d );
}


/**
Multiply the matrix on the top of the current OpenGL matrix stack by the matrix \a m
*/
void glMultMatrixf(const Matrix4f &m)
{
	glMultMatrixf( m.d );
}

/**
Multiply the matrix on the top of the current OpenGL matrix stack by the matrix \a m
*/
void glMultMatrixd(const Matrix4d &m)
{
	glMultMatrixd( m.d );
}


/**
Pass a vertex to OpenGL in the form of a Point3f.
*/
void glVertex3f(const Point3f &p)
{
	glVertex3f( p.x, p.y, p.z );
}

/**
Pass a vertex to OpenGL in the form of a Point3d.
*/
//void glVertex3d(const Point3d &p)
//{
//	glVertex3d( p.x, p.y, p.z );
//}


/**
Pass a normal to OpenGL in the form of a Vector3f.
*/
void glNormal3f(const Vector3f &n)
{
	glNormal3f( n.x, n.y, n.z );
}

/**
Pass a normal to OpenGL in the form of a Vector3d.
*/
//void glNormal3d(const Vector3d &n)
//{
//	glNormal3d( n.x, n.y, n.z );
//}




#endif
