/***************************************************************************
 *  callback class for motion blur                                         *
 *  the param persistence determines the level of motion blur              *
 *                                                                         *
 ***************************************************************************/
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
 *   Revision 1.4  2008-09-11 15:24:01  martius
 *   motioncallback resurrected
 *   noContact substance
 *   use slider center of the connecting objects for slider drawing
 *
 *   Revision 1.2  2006/07/14 12:23:35  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.1  2006/04/27 16:19:03  robot3
 *   motionblurcallback for the simulation
 *
 *                                                                         *
 ***************************************************************************/

#include "motionblurcallback.h"


namespace lpzrobots {

  void MotionBlurOperation::operator () (osg::Object* object)
  {
    osg::GraphicsContext* gc = dynamic_cast<osg::GraphicsContext*>(object);
    if (!gc) return;
    
    double t = gc->getState()->getFrameStamp()->getSimulationTime();
    
    if (!cleared_)
      {
	// clear the accumulation buffer
	glClearColor(0, 0, 0, 0);
	glClear(GL_ACCUM_BUFFER_BIT);
	cleared_ = true;
	t0_ = t;
      }
    
    double dt = fabs(t - t0_);
    t0_ = t;
    
    // compute the blur factor
    double s = powf(0.2, dt / persistence_);
    
    // scale, accumulate and return
    glAccum(GL_MULT, s);
    glAccum(GL_ACCUM, 1 - s);
    glAccum(GL_RETURN, 1.0f);
  }
  
}
