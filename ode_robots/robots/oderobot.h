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
 *   Revision 1.1.2.1  2005-11-14 14:43:52  martius
 *   moved from abstractrobot to oderobot
 *
 *   Revision 1.11  2005/10/06 17:14:24  martius
 *   switched to stl lists
 *
 *   Revision 1.10  2005/09/22 12:24:36  martius
 *   removed global variables
 *   OdeHandle and GlobalData are used instead
 *   sensor prepared
 *
 *   Revision 1.9  2005/09/22 07:30:53  martius
 *   moved color and position into extra modules
 *
 *   Revision 1.8  2005/09/12 00:10:44  martius
 *   position operators are const
 *
 *   Revision 1.7  2005/08/30 16:53:53  martius
 *   Position struct has toArray and operators
 *
 *   Revision 1.6  2005/08/29 06:40:35  martius
 *   added virtual destructor
 *
 *   Revision 1.5  2005/08/22 20:32:45  martius
 *   robot has a name
 *
 *   Revision 1.4  2005/07/27 13:22:16  martius
 *   position and color have constructors
 *   ODEHandle
 *
 *   Revision 1.3  2005/07/18 14:47:41  martius
 *   world, space, contactgroup are not pointers anymore.
 *
 *   Revision 1.2  2005/07/07 09:27:11  martius
 *   isGeomInObjectList added
 *
 *   Revision 1.1  2005/06/15 14:20:04  martius
 *   moved into robots
 *                                                                 *
 ***************************************************************************/
#ifndef __ODEROBOT_H
#define __ODEROBOT_H

#include <vector>
using namespace std;
 
#include "abstractrobot.h"
#include "odehandle.h"
#include "globaldata.h"
#include "color.h"

typedef struct
{
  dBodyID body;
  dGeomID geom;
} Object;

/**
 * Abstract class  for ODE robot 
 * 
 */
class OdeRobot : public AbstractRobot {
public:

  /**
   * Constructor
   * @param odehandle structure with all global ODE variables
   */
  OdeRobot(const OdeHandle& odehandle, const char* name="OdeRobot")
    : AbstractRobot(name) {
    world=odehandle.world;
    space=odehandle.space;
    contactgroup=odehandle.jointGroup;
  };

  virtual ~OdeRobot(){}


  /// draws the robot
  virtual void draw() = 0;

/** sets the vehicle to position pos, sets color to c, and creates robot if necessary
    @params pos desired position of the robot in struct Position
    @param c desired color for the robot in struct Color. Might be NULL!
*/
  virtual void place(Position pos , Color *c = 0) = 0;

  /** checks for internal collisions and treats them. 
   *  In case of a treatment return true (collision will be ignored by other objects and the default routine)
   *  else false (collision is passed to other objects and (if not treated) to the default routine).
   */
  virtual bool collisionCallback(void *data, dGeomID o1, dGeomID o2){
    return false;
  }

  /** this function is called in each timestep. It should perform robot-internal checks, 
      like space-internal collision detection, sensor resets/update etc.
      @param GlobalData structure that contains global data from the simulation environment
   */
  virtual void doInternalStuff(const GlobalData& globalData) = 0;

  /** sets color of the robot
      @param col Color struct with desired Color
   */
  virtual void setColor(Color col){
    color=col;
  };


protected:
  static bool isGeomInObjectList(Object* os, int len, dGeomID geom){  
    for(int i=0; i < len; i++){
      if(geom == os[i].geom) return true;
    }
    return false;
  }

protected:
  
  dSpaceID space;
  dWorldID world;
  dJointGroupID contactgroup;

  Color color;
};

#endif
 
