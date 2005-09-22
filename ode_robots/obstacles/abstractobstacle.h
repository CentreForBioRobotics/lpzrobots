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
 *   Revision 1.7  2005-09-22 12:24:36  martius
 *   removed global variables
 *   OdeHandle and GlobalData are used instead
 *   sensor prepared
 *
 *   Revision 1.6  2005/09/13 13:20:12  martius
 *   initialised color
 *
 *   Revision 1.5  2005/08/29 06:32:25  martius
 *   added virtual destructor
 *
 *   Revision 1.4  2005/07/18 14:52:33  martius
 *   world and space are not pointers anymore.
 *
 *   Revision 1.3  2005/06/15 14:22:11  martius
 *   GPL included
 *                                                                 *
 ***************************************************************************/
#ifndef __ABSTRACTOBSTACLE_H
#define __ABSTRACTOBSTACLE_H

#include <abstractrobot.h>
#include <ode/ode.h>

#include "odehandle.h"
#include "color.h"

/**
 *  Abstract class (interface) for obstacles
 */
class AbstractObstacle{

 public:
  /**
   * Constructor
   * @param w world in which obstacle should be created
   * @param s space in which obstacle should be created
   */
  AbstractObstacle(const OdeHandle& odehandle): color(0.5,0.5,0.5) {
    world=odehandle.world;
    space=odehandle.space;
  };

  virtual ~AbstractObstacle(){}
  
  /**
   * draws the obstacle
   */
  virtual void draw() = 0;
  
  /**
   * sets position of the obstacle and creates/recreates obstacle if necessary
   */
  virtual void setPosition(double x, double y, double z) = 0;

  /**
   * gives actual position of the obstacle
   */
  virtual void getPosition(double& x, double& y, double& z) = 0;
  
  /**
   * sets geometry parameters for the obstacle
   */
  virtual void setGeometry(double length, double width, double height) = 0;

  virtual void setColor(double r, double g, double b)=0;

 protected:

  dSpaceID space;
  dWorldID world;

  Color color;

};

#endif
