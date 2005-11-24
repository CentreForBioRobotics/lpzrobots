/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    frankguettler@gmx.de                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 *   $Log$
 *   Revision 1.3.4.2  2005-11-24 16:21:45  fhesse
 *   multMatrixPosition added
 *
 *   Revision 1.3.4.1  2005/11/14 17:37:25  martius
 *   moved to selforg
 *
 *   Revision 1.3  2005/11/10 09:09:55  martius
 *   use defines for definitions of sqrt, min, max...
 *
 *   Revision 1.2  2005/10/27 14:16:11  martius
 *   some bugs fixed, module now works
 *   some functions from controller_misc.h are here now
 *
 *   Revision 1.1  2005/10/27 12:15:22  robot3
 *   several useful functions that provide mathematic operations
 *
 *                                                                         *
 ***************************************************************************/
#ifndef __MATHUTILS_H
#define __MATHUTILS_H

#include <matrix.h>
using namespace matrix;
#include <selforg/position.h>

#ifndef MIN_MAX_AND_SO_ON
#define MIN_MAX_AND_SO_ON
#define min(a,b) ( (a) < (b) ? a : b )
#define max(a,b) ( (a) > (b) ? a : b )
#define sign(x)  ( (x) < 0 ? -1 : 1 )
#define sqr(x)   ( (x)*(x) )
#define clip(x, lobound, highbound) ( (x)<(lobound) ? (lobound) : ( (x) > (highbound) ? (highbound) : (x) ) )
#endif

/**
   Multiplies 3x3 matrix with position
*/
Position multMatrixPosition(const Matrix& r, Position& p);

/**
 * returns a rotation matrix with the given angle
 */
Matrix getRotationMatrix(const double& angle);


/**
 * returns a translation matrix with the given Position
 */
Matrix getTranslationMatrix(const Position& p) ;


/**
 * removes the translation in the matrix
 */
Matrix removeTranslationInMatrix(const Matrix& pose);


/**
 * removes the rotation in the matrix
 */
Matrix removeRotationInMatrix(const Matrix& pose) ;


/**
 * returns the angle between two vectors
 */
double getAngle(Position a, Position b) ;


#endif
