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
 ***************************************************************************
 *                                                                         *
 *  base.h provides osg stuff for basic environment with sky and so on.    *
 *                                                                         *
 *   $Log$
 *   Revision 1.1.2.2  2005-12-09 16:54:16  martius
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

#include "osghandle.h"
#include "odehandle.h"

class osg::Node;
class osg::Node;


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
    virtual osg::Group* makeScene();
    virtual osg::Node* makeSky();
    virtual osg::Node* makeGround();

  protected:
    dGeomID ground;

    osg::Group* root;

    OsgHandle osgHandle;
    // ODE globals
    OdeHandle odeHandle;
  };
}

#endif
