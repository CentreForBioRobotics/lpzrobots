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
 *   Revision 1.1.2.7  2006-06-29 16:39:55  robot3
 *   -you can now see bounding shapes if you type ./start -drawboundings
 *   -includes cleared up
 *   -abstractobstacle and abstractground have now .cpp-files
 *
 *   Revision 1.1.2.6  2006/06/26 08:25:03  robot3
 *   fixed ground texture bug
 *
 *   Revision 1.1.2.5  2006/06/22 12:25:45  der
 *   added setGroundTexture and setGroundColor
 *
 *   Revision 1.1.2.4  2006/06/16 22:27:26  martius
 *   getMainPrimtive
 *
 *   Revision 1.1.2.3  2006/05/23 14:13:41  der
 *   fixed initialization bug
 *
 *   Revision 1.1.2.2  2006/05/23 13:37:45  robot3
 *   -fixed some creating bugs
 *   -setColor,setTexture and createGround must be
 *    called before setPosition now
 *
 *   Revision 1.1.2.1  2006/05/19 08:41:27  robot3
 *   Class AbstractGround contains now basic routines like
 *   creating the groundPlane, setPose and so on
 *
 *                                                                         *
 *                                                                         *
 ***************************************************************************/
#ifndef __ABSTRACTGROUND_H
#define __ABSTRACTGROUND_H

#include <vector>
#include "abstractobstacle.h"

namespace lpzrobots {
  
  class Box; // Primitive Box

  // abstract class for any playground
  class AbstractGround : public AbstractObstacle {

public:
  
    AbstractGround(const OdeHandle& odeHandle, const OsgHandle& osgHandle, bool createGround=true);
  
    virtual ~AbstractGround();

    virtual void update();
  

    virtual void setPose(const osg::Matrix& pose);

    virtual void createGround(bool create);

    /**
     * sets the obstacle color
     * @param color values in RGBA
     */
    virtual void setColor(const Color& color);


    virtual void setTexture(const std::string& filename);

    virtual Primitive* getMainPrimitive() const;

    virtual void setGroundTexture(const std::string& filename);

    /**
     * sets the ground color
     * should be called before setPosition()
     * @param color values in RGBA
     */
    virtual void setGroundColor(const Color& color);

  protected:

    std::vector<Primitive*> obst; //obstacles
    Box* groundPlane; // the groundplane
    bool creategroundPlane;
    double ground_length;
    std::string wallTextureFileName;
    Color groundColor;
    std::string groundTextureFileName;

    virtual void recreate();

    virtual void create()=0;

    virtual void createGround();

    virtual void destroy();

  };

}

#endif
