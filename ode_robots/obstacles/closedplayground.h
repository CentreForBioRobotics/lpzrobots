/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
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
 *   Revision 1.2  2005-09-22 12:24:36  martius
 *   removed global variables
 *   OdeHandle and GlobalData are used instead
 *   sensor prepared
 *
 *   Revision 1.1  2005/07/21 12:01:54  robot8
 *   adding a new obstacle, which is similar to playground, but closed to all directions
 *
 *   Revision 1.6  2005/07/18 14:52:33  martius
 *   world and space are not pointers anymore.
 *
 *   Revision 1.5  2005/07/07 10:24:23  martius
 *   avoid internal collisions
 *
 *   Revision 1.4  2005/06/15 14:22:11  martius
 *   GPL included
 *                                                                 *
 ***************************************************************************/
#ifndef __CLOSEDPLAYGROUND_H
#define __CLOSEDPLAYGROUND_H


#include <stdio.h>
#include <math.h>

#include "abstractobstacle.h"
#include <drawstuff/drawstuff.h>


//Fixme: playground creates collisions with ground and itself
class ClosedPlayground : public AbstractObstacle {

  double length, width, height;
  double base_x, base_y, base_z;

  dGeomID obst1; //Obstacle1
  dGeomID obst2; //Obstacle2
  dGeomID obst3; //Obstacle3
  dGeomID obst4; //Obstacle4
  dGeomID obst5; //Obstacle4

  bool obstacle_exists;

 public:
  
  ClosedPlayground(const OdeHandle& odehandle):
    AbstractObstacle::AbstractObstacle(odehandle){

    base_x=0.0;
    base_y=0.0;
    base_z=0.0;
	
    length=7.0;
    width=0.2;
    height=0.5;

    obstacle_exists=false;
    
    setColor(0,1,0);

  };

  /**
   * draws the obstacle (4 boxes for the playground)
   */
  virtual void draw(){
    double box[3];
    //dsSetTexture (DS_WOOD);    
    dsSetColor (color.r, color.g, color.b);

    box[0] = width; box[1] = length; box[2] = height;
    dsDrawBox ( dGeomGetPosition ( obst1 ) , dGeomGetRotation ( obst1 ) , box );
    dsDrawBox ( dGeomGetPosition ( obst2 ) , dGeomGetRotation ( obst2 ) , box );
    box[0] = length; box[1] = width; box[2] = height;
    dsDrawBox ( dGeomGetPosition ( obst3 ) , dGeomGetRotation ( obst3 ) , box );
    dsDrawBox ( dGeomGetPosition ( obst4 ) , dGeomGetRotation ( obst4 ) , box );
    box[0] = length; box[1] = length; box[2] = width;
    dsDrawBox ( dGeomGetPosition ( obst5 ) , dGeomGetRotation ( obst5 ) , box );
    
  };
  
  
  virtual void setPosition(double x, double y, double z){
    base_x=x;
    base_y=y;
    base_z=z;
    if (obstacle_exists){
      destroy();
    }
    create();
  };

  virtual void getPosition(double& x, double& y, double& z){
    x=base_x;
    y=base_y;
    z=base_z;
  }
  
  virtual void setGeometry(double length_, double width_, double height_){
    length=length_;
    width=width_;
    height =height_;
  };

  virtual void setColor(double r, double g, double b){
    color.r=r;
    color.g=g;
    color.b=b;
  };

 protected:
  virtual void create(){
    obst1 = dCreateBox ( space, width , length-0.01 , height);
    dGeomSetPosition ( obst1, base_x - (length/2 + width/2), base_y, height/2 +base_z);
	
    obst2 = dCreateBox ( space, width, length-0.01, height );
    dGeomSetPosition ( obst2, base_x + (length/2 +width/2), base_y, height/2 +base_z);
	
    obst3 = dCreateBox ( space, length-0.01, width, height );
    dGeomSetPosition ( obst3, base_x, base_y-(length/2 +width/2), height/2 +base_z);
	
    obst4 = dCreateBox ( space, length-0.01, width, height );
    dGeomSetPosition ( obst4, base_x, base_y+(length/2 +width/2), height/2 +base_z);
    
    obst5 = dCreateBox ( space, length-0.01, length-0.01, width );
    dGeomSetPosition ( obst5, base_x, base_y , height +base_z + width/2);

    obstacle_exists=true;
    // printf("Obst: %i,%i,%i,%i\n",obst1,obst2,obst3,obst4);
  };


  virtual void destroy(){
    dGeomDestroy( obst1 );
    dGeomDestroy( obst2 );
    dGeomDestroy( obst3 );
    dGeomDestroy( obst4 );
    dGeomDestroy( obst5 );
    
    obstacle_exists=false;
  };

};

#endif
