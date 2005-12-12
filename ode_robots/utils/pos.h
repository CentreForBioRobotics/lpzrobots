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
 *   Revision 1.1.2.2  2005-12-12 23:42:14  martius
 *   Pos is a class againt to have customisable Constructors
 *
 *   Revision 1.1.2.1  2005/12/11 23:35:08  martius
 *   *** empty log message ***
 *
 *
 ***************************************************************************/
#ifndef __POS_H
#define __POS_H

#include <osg/Vec3>
#include <ode/ode.h>

namespace lpzrobots{

  class Pos : public osg::Vec3 {
  public:
    Pos () : osg::Vec3 () {};
    Pos (float x, float y, float z) : osg::Vec3(x, y, z) {}
    Pos (const dReal v[3]) : osg::Vec3(v[0], v[1], v[2]) {}
  };

}

#endif
