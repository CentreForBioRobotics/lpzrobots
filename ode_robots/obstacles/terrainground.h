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
 *   Revision 1.10  2010-07-02 05:44:29  martius
 *   comments improved
 *
 *   Revision 1.9  2008/09/16 14:49:46  martius
 *   use cmath instead of math.h
 *
 *   Revision 1.8  2006/07/14 12:23:33  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.7.4.4  2006/06/16 22:27:26  martius
 *   getMainPrimtive
 *
 *   Revision 1.7.4.3  2006/05/29 18:55:46  martius
 *   moved from meshground to terrainground as it was in former times
 *
 *   Revision 1.1.2.1  2006/05/28 22:14:56  martius
 *   heightfield included
 *
 *
 *                                                                 *
 ***************************************************************************/
#ifndef __TERRAINGROUND_H
#define __TERRAINGROUND_H

#include <stdio.h>
#include <cmath>

#include "heightfieldprimitive.h"
#include "abstractobstacle.h"
 
namespace lpzrobots {

  /** Class provides an terrain based on HeightFields. 
      Can be loaded from image or from HeightFieldFiles
  */
  class TerrainGround : public AbstractObstacle {
  public:
    
  
    /** Constructor
	@param odeHandle 
	@param osgHandle
	@param filename name of the file to load. 
	If ending is .ppm then it is considered as a bitmap height file.
	The coding mode is used to decode the heights. 
	Otherwise it is consider to be a OSG HeightFieldFile
	@param texture image filename for the texture
	@param x_size size in x direction in world coordinates
	@param y_size size in y direction in world coordinates
	@param height height in world coordinates
	@param coding Coding mode, see OSGHeightField (this is an own class, not in OSG)
    */
    TerrainGround(const OdeHandle& odeHandle, const OsgHandle& osgHandle, 
	       const std::string& filename, const std::string& texture, 
	       double x_size, double y_size, double height,
	       OSGHeightField::CodingMode coding = OSGHeightField::Red);

    virtual ~TerrainGround(){}

    /**
     * updates the position of the geoms  ( not nessary for static objects)
     */
    virtual void update(){ };

    virtual void setPose(const osg::Matrix& pose);


    virtual Primitive* getMainPrimitive() const { return 0; }

  protected:
    virtual void create();
    virtual void destroy();    

  protected:
    std::string filename;
    std::string texture;
    HeightField* heightfield;
    double x_size;
    double y_size;
    double height;
    OSGHeightField::CodingMode coding;
  };
}

#endif
