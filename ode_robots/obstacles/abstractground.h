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
 *   Revision 1.1.2.2  2006-05-23 13:37:45  robot3
 *   -fixed some creating bugs
 *   -setColor,setTexture and createGround must be
 *    called before setPosition now
 *
 *   Revision 1.1.2.1  2006/05/19 08:41:27  robot3
 *   Class AbstractGround contains now basic routines like
 *   creating the groundPlane, setPose and so on
 *
 *                                                                         *
 *                                                                         *
 ***************************************************************************/
#ifndef __ABSTRACTGROUND_H
#define __ABSTRACTGROUND_H

//#include <math.h>
#include <vector>
#include <osg/Matrix>

#include "primitive.h"
#include "abstractobstacle.h"

namespace lpzrobots {

  // abstract class for any playground
  class AbstractGround : public AbstractObstacle {



public:
  

  AbstractGround(const OdeHandle& odeHandle, const OsgHandle& osgHandle, bool createGround=true):
    AbstractObstacle::AbstractObstacle(odeHandle, osgHandle), creategroundPlane(createGround) {
    ground_length=10.0f;
    wallTextureFileName="Images/wall.rgb";
  };
  
  virtual ~AbstractGround(){
    destroy();
  }

  virtual void update(){
    if (obstacle_exists){
      for (unsigned int i=0; i<obst.size(); i++){
	if(obst[i]) obst[i]->update();
      }
      if (groundPlane) groundPlane->update();
    }
  };
  

  virtual void setPose(const osg::Matrix& pose){
    this->pose = pose;
    recreate();
  };

  virtual void createGround(bool create) {
    creategroundPlane=create;
    if (obstacle_exists) {
      std::cout << "ERROR: createGround(bool create)  has no effect AFTER setPosition(osg::Vec3) !!!" 
		<< std::endl;
      std::cout << "Program terminated. Please correct this error in main.cpp first." << std::endl;
      exit(-1);
    }
  }

  /**
   * sets the obstacle color
   * @param color values in RGBA
   */
  virtual void setColor(const Color& color) {
    osgHandle.color = color;
    if (obstacle_exists) {
      std::cout << "ERROR: setColor(const Color& color) has no effect AFTER setPosition(osg::Vec3) !!!"
		<< std::endl;
      std::cout << "Program terminated. Please correct this error in main.cpp first." << std::endl;
      exit(-1);
    }
  };


  virtual void setTexture(const std::string& filename){
    wallTextureFileName=filename;
    if (obstacle_exists) {
      std::cout << "ERROR: "
		<< "setTexture(const std::sting& filename) has no effect AFTER setPosition(osg::Vec3) !!!"
		<< std::endl;
      std::cout << "Program terminated. Please correct this error in main.cpp first." << std::endl;
      exit(-1);
    }
  }


protected:

    vector<Primitive*> obst; //obstacles
    Box* groundPlane; // the groundplane
    bool creategroundPlane;
    double ground_length;
    string wallTextureFileName;

    virtual void recreate() {
      if (obstacle_exists){
	destroy();
      }
      create();
      createGround();
      obstacle_exists=true;
    }

    virtual void create()=0;

    virtual void createGround() {
      if (creategroundPlane) {
	// now create the plane in the middle
	groundPlane = new Box(ground_length,ground_length, 0.1f);
	groundPlane->init(odeHandle, 0, osgHandle,
			  Primitive::Geom | Primitive::Draw);
	groundPlane->setPose(osg::Matrix::translate(0.0f,0.0f,-0.05f) * pose);
	groundPlane->getOSGPrimitive()->setColor(Color(1.0f,1.0f,1.0f));
	groundPlane->getOSGPrimitive()->setTexture("Images/greenground.rgb",true,true);
      }
    }


  virtual void destroy(){
    for(unsigned int i=0; i < obst.size(); i++){
      if(obst[i]) delete(obst[i]);
    }
    obst.clear();
    if (groundPlane) delete(groundPlane);
    groundPlane=0;
    obstacle_exists=false;
  };
  

};

}

#endif
