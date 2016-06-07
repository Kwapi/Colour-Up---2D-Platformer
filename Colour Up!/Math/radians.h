//##************************
//##* This source code is licensed under the terms and conditions of the BSD license,
//##* the full text of which can be found in the file named 'License.txt' that
//##* accompanies this code. (C) copyright Geoffrey French 1999-2010.
//##************************
#ifndef RADIANS_H__
#define RADIANS_H__

#include <math.h>

#include <Math/MathConstants.h>


#ifndef DEGTORAD
#define DEGTORAD(a) (a * (M_PI / 180.0))
#endif

#ifndef RADTODEG
#define RADTODEG(a) (a * (180.0 / M_PI))
#endif


#endif
