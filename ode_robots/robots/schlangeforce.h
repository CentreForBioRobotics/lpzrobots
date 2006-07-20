/************************************************************************/
/*schlangeforce.h			                            	*/
/*Snake with torque added to joints                     		*/
/*									*/
/************************************************************************/
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
 *   Revision 1.15  2006-07-20 17:19:44  martius
 *   removed using namespace std from matrix.h
 *
 *   Revision 1.14  2006/07/14 12:23:41  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.13.4.6  2006/06/25 16:57:15  martius
 *   abstractrobot is configureable
 *   name and revision
 *
 *   Revision 1.13.4.5  2006/03/30 12:34:56  martius
 *   documentation updated
 *
 *   Revision 1.13.4.4  2006/02/01 18:33:40  martius
 *   use Axis type for Joint axis. very important, since otherwise Vec3 * pose is not the right direction vector anymore
 *
 *   Revision 1.13.4.3  2006/01/04 17:04:41  fhesse
 *   comments originating from old file removed
 *
 *   Revision 1.13.4.2  2006/01/04 14:46:00  fhesse
 *   inherits from Schlange; moved to osg
 *
 *   Revision 1.13.4.1  2005/11/14 17:37:18  martius
 *   moved to selforg
 *
 *   Revision 1.13  2005/11/14 12:48:43  martius
 *   *** empty log message ***
 *
 *   Revision 1.12  2005/11/09 13:24:42  martius
 *   added GPL
 *
 *                                                                 *
 ***************************************************************************/
#ifndef __SCHLANGEFORCE_H
#define __SCHLANGEFORCE_H

#include "schlange.h"

namespace lpzrobots {

  /**
   * This is a class, which models a snake like robot. 
   * It consists of a number of equal elements, each linked 
   * by a joint powered by torques added to joints
   **/
  class SchlangeForce: public Schlange
    {
    public:
      SchlangeForce ( const OdeHandle& odeHandle, const OsgHandle& osgHandle,
		      const SchlangeConf& conf, const std::string& name);

    
      virtual ~SchlangeForce();
	
      /**
       *Reads the actual motor commands from an array, 
       *an sets all motors of the snake to this values.
       *It is an linear allocation.
       *@param motors pointer to the array, motor values are scaled to [-1,1] 
       *@param motornumber length of the motor array
       **/
      virtual void setMotors ( const motor* motors, int motornumber );

      /**
       *Writes the sensor values to an array in the memory.
       *@param sensors pointer to the array
       *@param sensornumber length of the sensor array
       *@return number of actually written sensors
       **/
      virtual int getSensors ( sensor* sensors, int sensornumber );
	
      /** returns number of sensors
       */
      virtual int getSensorNumber() { assert(created); return joints.size() * 2; }

      /** returns number of motors
       */
      virtual int getMotorNumber(){ assert(created); return joints.size() * 2; }

    private:
      virtual void create(const osg::Matrix& pose);
      virtual void destroy();
    };

}

#endif
