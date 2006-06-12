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
 *   Revision 1.1.2.6  2006-06-12 13:37:55  robot3
 *   added missing const OsgHandle->changeAlpha(const float& alpha);
 *
 *   Revision 1.1.2.5  2006/01/12 14:39:06  martius
 *   transparent stateset
 *
 *   Revision 1.1.2.4  2005/12/29 16:48:06  martius
 *   changeColor
 *
 *   Revision 1.1.2.3  2005/12/22 14:09:56  martius
 *   different tesselhints for different level of detail
 *
 *   Revision 1.1.2.2  2005/12/13 18:12:20  martius
 *   some utils
 *
 *   Revision 1.1.2.1  2005/12/06 16:18:02  martius
 *   handle class for OpenSceneGraph
 *
 *
 ***************************************************************************/
#ifndef __OSGHANDLE_H
#define __OSGHANDLE_H

#include "osgforwarddecl.h"
#include "color.h"

namespace lpzrobots {

/** Data structure for accessing the ODE */
class OsgHandle
{
public:
  OsgHandle( );
  OsgHandle( osg::Group* scene, osg::TessellationHints* tesselhints[3], 
	     osg::StateSet* normalState, osg::StateSet* transparentState, 
	     const Color& color);

  osg::Group* scene;  
  osg::TessellationHints* tesselhints[3];  
  osg::StateSet* normalState;  
  osg::StateSet* transparentState;  
  Color color;

  OsgHandle changeColor(const Color& color) const;
  OsgHandle changeAlpha(const float& alpha) const;
  
};

}

#endif

