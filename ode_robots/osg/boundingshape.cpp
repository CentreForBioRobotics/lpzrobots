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
 ***************************************************************************
 *                                                                         *
 *  DESCRIPTION                                                            *
 *                                                                         *
 *   $Log$
 *   Revision 1.1.2.3  2006-06-26 21:52:38  robot3
 *   Mesh works now with bbox file
 *
 *   Revision 1.1.2.2  2006/06/23 08:54:53  robot3
 *   made some changes on primitive Mesh (including boundingshape)
 *
 *   Revision 1.1.2.1  2006/06/22 11:33:30  robot3
 *   moved boundingshape implementation to .cpp-file
 *
 *   Revision 1.1.2.3  2006/05/29 22:22:07  martius
 *   added std includes
 *
 *   Revision 1.1.2.2  2006/03/29 15:03:19  martius
 *   format documented
 *
 *   Revision 1.1.2.1  2006/03/29 14:51:45  martius
 *   class for reading bounding shape description files and creates the appropriate geoms
 *
 *   Revision 1.1.2.1  2005/12/06 17:38:21  martius
 *   *** empty log message ***
 *
 *                                                                 *
 ***************************************************************************/

#include "boundingshape.h"
#include "mathutils.h"


#include <iostream>

namespace lpzrobots {

  /**
     class for reading bounding shape description files (.bbox) and to create appropriate geoms
     File Format: Lines wise, every line stands for one primitive. Possible lines are:
     sphere radius (x,y,z)
     cylinder radius height (x,y,z) (alpha, beta, gamma)
     capsule radius height (x,y,z) (alpha, beta, gamma)
     box length width height (x,y,z) (alpha, beta, gamma)
     (x,y,z) is the position vector and (alpha, beta, gamma) are
     the rotation angles about x,y,z axis respectively        
  */

  BoundingShape::BoundingShape(const std::string& filename, Primitive* parent) : filename(filename), parent(parent) {
    active=false;
  }
  
  BoundingShape::~BoundingShape(){}

  bool BoundingShape::readBBoxFile(std::string& filename, const OdeHandle& odeHandle, const OsgHandle& osgHandle, 
				   const osg::Matrix& pose, double scale, char mode){
    // TODO: use search path (maybe try to use some osgDB function)
    FILE* f = fopen(filename.c_str(),"r");
    if(!f) return false;
      char buffer[128];
      float r,h,x,y,z;
      while(fgets(buffer,128,f)){
	if(strstr(buffer,"sphere")==buffer){
	  if(sscanf(buffer+7, "%g (%g,%g,%g)", &r, &x, &y, &z)==4){
	    Sphere* s = new Sphere(r * scale);
	    //	    s->init(odeHandle,0,osgHandle, mode);
	    //	    s->setPose(osg::Matrix::translate(scale*x,scale*y,scale*z) * pose);
	    geoms.push_back(s);
	    poses.push_back(osg::Matrix::translate(scale*x,scale*y,scale*z));
	    std::cout << "Sphere as bounding added." << scale*x << "," << scale*y << "," << scale*z << std::endl;
	  } else{ fprintf(stderr, "Syntax error : %s", buffer); }
	} else if(strstr(buffer,"capsule")==buffer){
	  if(sscanf(buffer+7, "%g %g (%g,%g,%g)", &r, &h, &x, &y, &z)==5){
	    Capsule* c = new Capsule(r * scale,h * scale);
	    //	    c->init(odeHandle,0,osgHandle, mode);
	    //	    c->setPose(osg::Matrix::translate(scale*x,scale*y,scale*z) * pose);
	    geoms.push_back(c);
	    poses.push_back(osg::Matrix::translate(scale*x,scale*y,scale*z));
	    Primitive* Trans = new Transform(parent,c,osg::Matrix::translate(scale*x,scale*y,scale*z));
	    Trans->init(odeHandle, 0, osgHandle);
	    std::cout << osgMatrix2Matrixlib(osg::Matrix::translate(scale*x,scale*y,scale*z));
	    std::cout << "Capsule as bounding added:" << scale*x << "," << scale*y << "," << scale*z << std::endl;
	  } else{ fprintf(stderr, "Syntax error : %s", buffer); }
	} else if(strstr(buffer,"cylinder")==buffer){
	  fprintf(stderr, "Not implemented : %s", buffer);
	} else if(strstr(buffer,"box")==buffer){
	  fprintf(stderr, "Not implemented : %s", buffer);
	}else {
	  fprintf(stderr, "Unknown Line: %s", buffer);
	}
      }
      active=true;
      return true;
    }
      
    bool BoundingShape::init(const OdeHandle& odeHandle, const OsgHandle& osgHandle, const osg::Matrix& pose,
		      double scale, char mode){
      return readBBoxFile(filename, odeHandle, osgHandle, pose, scale, mode);
    }

    void BoundingShape::update(const osg::Matrix& m4x4){
      // update all primitives in list
      std::list<osg::Matrix>::iterator p=poses.begin();
      for(std::list<Primitive*>::iterator i=geoms.begin();i!=geoms.end();i++) {
	  (*i)->setPose(osg::Matrix::translate((*p).getTrans()) * m4x4);
	  std::cout << "updating boundingshape!" << std::endl;
	  p++;
      }
    }



  std::list<Primitive*> BoundingShape::getPrimitives() {
    return geoms;
  }

  std::list<osg::Matrix> BoundingShape::getPoses() {
    return poses;
  }

  bool BoundingShape::isActive(){
    return active;
  }

}
