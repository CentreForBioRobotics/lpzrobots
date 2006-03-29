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
 *   Revision 1.2.4.5  2006-03-29 15:10:11  martius
 *   osgMatrix2matrixlib
 *
 *   Revision 1.2.4.4  2006/02/07 15:49:03  martius
 *   axis
 *
 *   Revision 1.2.4.3  2005/12/15 17:04:32  martius
 *   getAngle
 *   min, max and so on are template functions now
 *
 *   Revision 1.2.4.2  2005/12/14 15:37:38  martius
 *   rotation matrix for axis
 *
 *   Revision 1.2.4.1  2005/11/24 16:21:45  fhesse
 *   multMatrixPosition added
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
#include <math.h>
#include <osg/Matrix>
#include "mathutils.h"
#include "axis.h"

using namespace matrix;

namespace lpzrobots {

  /**
     converts osg matrix to matrix of matrixlib
   */
  Matrix osgMatrix2Matrixlib(const osg::Matrix& m){
    Matrix m2(4,4);
    for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
	m2.val(i,j) = m(i,j);
      }
    }
    return m2;
  }


  /**
     returns a Rotation matrix that rotates the x-axis along with the given axis. 
     The other 2 axis (y,z) are ambiguous.
  */
  osg::Matrix rotationMatrixFromAxisX(const Axis& axis){
    return osg::Matrix::rotate(osg::Vec3(1,0,0), axis.vec3());
  }

  /**
     returns a Rotation matrix that rotates the z-axis along with the given axis. 
     The other 2 axis (x,y) are ambiguous.
  */
  osg::Matrix rotationMatrixFromAxisZ(const Axis& axis){
    return osg::Matrix::rotate(osg::Vec3(0,0,1), axis.vec3());
  }


  /**
   * returns the angle between two vectors (in rad)
   */
  double getAngle(const osg::Vec3& a, const osg::Vec3& b) {
    // Cosinus Satz
    // here a*b is the dot product (Skalarprodukt)
    return acos(a*b / (a.length()*b.length())); 
  }


  /*******************************************************************************/


  Position multMatrixPosition(const Matrix& r, Position& p){
    assert(r.getM()==3 && r.getN()==3);
    Matrix pm(3,1,p.toArray());
    Matrix rv = r*pm;
    return Position(rv.val(0,0), rv.val(1,0), rv.val(2,0));
  }

  /**
   * returns a rotation matrix for a rotation in x, y plane about the given angle
   */
  Matrix getRotationMatrix(const double& angle) {
    double data[16]={cos(angle),sin(angle),0,0,
		     -sin(angle),cos(angle),0,0,
		     0,0,1,0,
		     0,0,0,1};
    return Matrix(4,4,data);
  }

  /**
   * returns a translation matrix with the given Position
   */
  Matrix getTranslationMatrix(const Position& p) {
    double data[16]={0,0,0,p.x,
		     0,0,0,p.y,
		     0,0,1,p.z,
		     0,0,0,1};
    return Matrix(4,4,data);
  }

  /**
   * removes the translation in the matrix
   */
  Matrix removeTranslationInMatrix(const Matrix& pose){
    Matrix t(pose);
    // remove the three last values of the column 3
    t.val(0,3)=0.0f;
    t.val(1,3)=0.0f;
    t.val(2,3)=0.0f;
    return t;
  }

  /**
   * removes the rotation in the matrix
   */
  Matrix removeRotationInMatrix(const Matrix& pose){
    Matrix t(pose);
    t.val(0,0)=1.0f; t.val(0,1)=0.0f;
    t.val(1,0)=0.0f; t.val(1,1)=1.0f;
    return t;
  }

  /**
   * returns the angle between two vectors
   */
  double getAngle(Position a, Position b) {
    Matrix p(3,1,a.toArray()); // row wise
    Matrix q(1,3,b.toArray()); // column wise
    return acos((p * q).val(0,0) / (a.length()*b.length()) );
  }

}
