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
 *  Camera Manipulation by mouse and keyboard                              *
 *                                                                         *
 *   $Log$
 *   Revision 1.2  2006-07-14 12:23:34  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.4  2006/03/19 13:36:38  robot3
 *   race mode now works
 *
 *   Revision 1.1.2.3  2006/03/06 16:58:27  robot3
 *   minor changes
 *
 *   Revision 1.1.2.2  2006/03/03 12:08:51  robot3
 *   preparations made for new cameramanipulators
 *
 *   Revision 1.1.2.1  2006/02/01 10:24:34  robot3
 *   new camera manipulator added
 *
 *                                                                         *
 ***************************************************************************/
#ifndef __CAMERAMANIPULATORTV_H
#define __CAMERAMANIPULATORTV_H

#include "osgforwarddecl.h"
#include "cameramanipulator.h"

namespace lpzrobots {

  /**
     CameraManipulatorTV is a MatrixManipulator which provides Flying simulator-like
     updating of the camera position & orientation. 
     Left mouse button: Pan and tilt
     Right mouse button: forward and sideways
     Middle mouse button: up and sideways
  */

  class CameraManipulatorTV : public CameraManipulator {

  public:
    
    CameraManipulatorTV(osg::Node* node,GlobalData& global);
    
    /** returns the classname of the manipulator
	it's NECCESSARY to define this funtion, otherwise
	the new manipulator WON'T WORK! (but ask me not why)
     */
    virtual const char* className() const { return "TV-Camera"; }
  
  protected:
    
    virtual ~CameraManipulatorTV();
    
    /** This handles robot movements, so that the camera movemenent is right affected.
	should be overwritten by new cameramanipulator
    */
       virtual void calcMovementByAgent();
    
    /** Sets the right view and eye if the robot has changed.
	Is called from manageRobots();
	should be overwritten by new cameramanipulator
    */
        virtual void setHomeViewByAgent();
        virtual void setHomeEyeByAgent();
   
  };
}

#endif
