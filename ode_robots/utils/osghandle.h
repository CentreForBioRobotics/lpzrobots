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
 *   Revision 1.1.2.1  2005-12-06 16:18:02  martius
 *   handle class for OpenSceneGraph
 *
 *
 ***************************************************************************/
#ifndef __OSGHANDLE_H
#define __OSGHANDLE_H

//class osg::Group;
//class osg::TesselHints;
#include <osg/Group>
#include <osg/ShapeDrawable>
#include "color.h"

namespace lpzrobots {

/** Data structure for accessing the ODE */
class OsgHandle
{
public:
  OsgHandle( ) { }
  OsgHandle( osg::Group* scene, osg::TessellationHints* tesselhints, const Color& color){
    this->scene = scene;
    this->tesselhints = tesselhints;
    this->color = color;
  }

  osg::Group* scene;  
  osg::TessellationHints* tesselhints;  
  Color color;
};

}

#endif

