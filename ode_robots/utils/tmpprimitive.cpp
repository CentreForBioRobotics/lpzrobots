/***************************************************************************
 *   Copyright (C) 2005-2011 LpzRobots development team                    *
 *    Georg Martius  <georg dot martius at web dot de>                     *
 *    Frank Guettler <guettler at informatik dot uni-leipzig dot de        *
 *    Frank Hesse    <frank at nld dot ds dot mpg dot de>                  *
 *    Ralf Der       <ralfder at mis dot mpg dot de>                       *
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
 ***************************************************************************/

#include "tmpprimitive.h"
#include "primitive.h"
#include "osgprimitive.h"

namespace lpzrobots {

  // ////////////////// TmpPrimitive //////////////////// //
  
  TmpPrimitive::TmpPrimitive(Primitive* p, char mode, double mass, const Pose& pose, 
                             const Color& color)
    : item(p), mode(mode), mass(mass), pose(pose), color(color), initialized(false) 
  {
    useColorName=false;
    if(!item) 
      item = new Sphere(0.1);    
  }
  
  TmpPrimitive::TmpPrimitive(Primitive* p, char mode, double mass, const Pose& pose, 
               const std::string& colorname, float alpha)
    : item(p), mode(mode), mass(mass), pose(pose), 
      colorname(colorname), alpha(alpha), initialized(false) 
  {
    useColorName=true;
    if(!item) 
      item = new Sphere(0.1);
  }
    
  void TmpPrimitive::init(const OdeHandle& odeHandle, const OsgHandle& osgHandle){
    Color mcolor(color);
    if(useColorName){
      mcolor = osgHandle.getColor(colorname);
      mcolor.alpha() = alpha;
    }
    item->init(odeHandle, mass, osgHandle.changeColor(mcolor), mode);
    item->setPose(pose);      
    initialized=true;
  }
          
  void TmpPrimitive::deleteObject(){
    if(item) delete item;
    item=0;    
  }


  // ////////////////// TmpDisplayItem //////////////////// //

  TmpDisplayItem::TmpDisplayItem(OSGPrimitive* p, const Pose& pose,
                                 const Color& color)
    : item(p), pose(pose), color(color), initialized(false) 
  {
    useColorName=false;
    if(!item) 
      item = new OSGSphere(0.1);
  }

  TmpDisplayItem::TmpDisplayItem(OSGPrimitive* p, const Pose& pose,
                                 const std::string& colorname, float alpha)
    : item(p), pose(pose), colorname(colorname), alpha(alpha),
      initialized(false) 
  {
    useColorName=true;
    if(!item) 
      item = new OSGSphere(0.1);
  }
    
  void TmpDisplayItem::init(const OdeHandle& odeHandle, const OsgHandle& osgHandle){
    Color mcolor(color);
    if(useColorName){
      mcolor = osgHandle.getColor(colorname);
      mcolor.alpha() = alpha;
    }
    item->init(osgHandle.changeColor(mcolor));   
    item->setMatrix(pose);      
    initialized=true;
  }
          
  void TmpDisplayItem::deleteObject(){
    if(item) delete item;
    item=0;
  }

}

