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
 *  base.h provides osg stuff for basic environment with sky and so on.    *
 *                                                                         *
 *   $Log$
 *   Revision 1.5  2007-09-27 10:47:04  robot3
 *   mathutils: moved abs to selforg/stl_adds.h
 *   simulation,base: added callbackable support,
 *   added WSM (WindowStatisticsManager) funtionality
 *
 *   Revision 1.4  2007/08/29 13:07:48  martius
 *   added HUD
 *
 *   Revision 1.3  2006/09/20 15:30:47  martius
 *   shadowsize, light
 *
 *   Revision 1.2  2006/07/14 12:23:33  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.7  2006/06/29 16:35:56  robot3
 *   includes cleared up
 *
 *   Revision 1.1.2.6  2006/05/28 22:14:56  martius
 *   heightfield included
 *
 *   Revision 1.1.2.5  2006/05/18 11:45:51  robot3
 *   -shadowing the normal scene integrated (first version)
 *   -note that there is a bug that the shadow disappears
 *    after some time (e.g. 60 minutes)
 *
 *   Revision 1.1.2.4  2006/01/31 15:45:02  martius
 *   virtual destructor
 *
 *   Revision 1.1.2.3  2006/01/12 14:21:00  martius
 *   drawmode, material
 *
 *   Revision 1.1.2.2  2005/12/09 16:54:16  martius
 *   camera is woring now
 *
 *   Revision 1.1.2.1  2005/12/06 17:40:59  martius
 *   base class for simulation
 *
 *                                                                 *
 ***************************************************************************/
#ifndef __BASE_H
#define __BASE_H

#include<ode/ode.h>
#include<osg/Transform>
#include <osgText/Text>

#include "osghandle.h"
#include "odehandle.h"

#include "windowstatistics.h"

class osg::Node;
class Callbackable;

namespace lpzrobots {

  class MoveEarthySkyWithEyePointTransform : public osg::Transform {
    public:
      /** Get the transformation matrix which moves from local coords to world coords.*/
      virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;

      /** Get the transformation matrix which moves from world coords to local coords.*/
      virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;
  };

  class Base {
  public:
    Base(const char* caption=0)
      : caption(caption){
      timestats=0; hud=0;
    }

    virtual osg::Group* makeScene();
    virtual osg::Node* makeSky();
    virtual osg::Node* makeGround();
    virtual osg::Node* createHUD();
    virtual osg::LightSource* makeLights(osg::StateSet* stateset);
    virtual osg::Group* createShadowedScene(osg::Node* shadowed,
					    osg::Vec3 posOfLight,
					    unsigned int unit);


    virtual void setCaption(const char* caption) {
      this->caption = caption;
    }

  /** adds an Callbackable object for getting a callback every step.
   * note that the object are not called back in this class. This must
   * be done in the deduced class (here: Simulation).
   */
    virtual void addCallbackable(Callbackable* callbackable);

    virtual WindowStatisticsManager* getWSM() { return this->windowStatisticsManager; }

    virtual ~Base();

  protected:
    virtual void setTimeStats(double time, double realtimefactor);

    dGeomID ground;

    osg::Group* root;
    osg::Node* hud;
    osgText::Text* timestats;
	osgText::Text* statisticLine;

    OsgHandle osgHandle;
    // ODE globals
    OdeHandle odeHandle;
    const char* caption;

    bool useShadow;
    unsigned int shadowTexSize;

    /// this manager provides methods for displaying statistics on the graphical window!
    WindowStatisticsManager* windowStatisticsManager;

    std::list<Callbackable*> callbackables;
  };
}

#endif
