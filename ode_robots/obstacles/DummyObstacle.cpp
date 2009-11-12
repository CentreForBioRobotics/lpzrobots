/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
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
 *  DESCRIPTION                                                            *
 *  
 *                                                                         *
 *                                                                         *
 *  $Log$
 *  Revision 1.2  2009-11-12 16:15:29  guettler
 *  fixed creation bug
 *
 *  Revision 1.1  2009/11/12 16:05:11  guettler
 *  new DummyObstacle which uses a DummyPrimitive
 *										                       *
 *                                                                         *
 **************************************************************************/
#include "DummyObstacle.h"

#include "primitive.h"

namespace lpzrobots {

  DummyObstacle::DummyObstacle(const OdeHandle& odeHandle, const OsgHandle& osgHandle) : AbstractObstacle(odeHandle,osgHandle) {
    obst[0] = new DummyPrimitive();
  }

  DummyObstacle::~DummyObstacle() {
  }

  void DummyObstacle::setPose(const osg::Matrix& pose) {
  }

  /// return the "main" primitive of the obtactle. The meaning of "main" is arbitrary
  Primitive* DummyObstacle::getMainPrimitive() const {
    return obst[0];
  }

  /// overload this function to create the obstactle. All primitives should go into the list "obst"
  void DummyObstacle::create() {
  }

}
