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
 ***************************************************************************
 *                                                                         *
 *   This file provides basic primitives for openscenegraph usage          *
 *   with ODE.                                                             *
 *                                                                         *
 *                                                                         *
 *   $Log$
 *   Revision 1.1.2.8  2005-12-15 17:03:42  martius
 *   cameramanupulator setPose is working
 *   joints have setter and getter parameters
 *   Primitives are not longer inherited from OSGPrimitive, moreover
 *   they aggregate them
 *
 *   Revision 1.1.2.7  2005/12/14 15:36:45  martius
 *   joints are visible now
 *
 *   Revision 1.1.2.6  2005/12/13 18:11:13  martius
 *   transform primitive added, some joints stuff done, forward declaration
 *
 *   Revision 1.1.2.5  2005/12/12 23:42:53  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.4  2005/12/11 23:35:08  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.3  2005/12/09 16:54:16  martius
 *   camera is woring now
 *
 *   Revision 1.1.2.2  2005/12/06 17:38:15  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.1  2005/12/06 10:13:24  martius
 *   openscenegraph integration started
 *
 *                                                                 *
 *                                                                         *
 ***************************************************************************/

#include <assert.h>

#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
//#include <osg/Texture>
//#include <osg/TexGen>
//#include <osg/PolygonOffset>
//#include <osg/Light>
//#include <osg/LightSource>
//#include <osg/Material>

#include "osgprimitive.h"

namespace lpzrobots {

  using namespace osg;

  OSGPrimitive::OSGPrimitive(){  }

  OSGPrimitive::~OSGPrimitive(){    
    Node::ParentList l = transform->getParents();
    for(Node::ParentList::iterator i = l.begin(); i != l.end(); i++){
      (*i)->removeChild(transform.get());  
    }
  }


  /******************************************************************************/
  void OSGPrimitive::setMatrix(const Matrix& m4x4){
    assert(!transform == false);
    transform->setMatrix(m4x4);
  }

  Group* OSGPrimitive::getGroup() { 
    return transform.get(); 
  }

  Transform* OSGPrimitive::getTransform() { 
    return transform.get(); 
  }

  void OSGPrimitive::setTexture(const std::string& filename){
    osg::Group* grp = getGroup();    
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
    texture->setImage(osgDB::readImageFile(filename));
    osg::StateSet* stateset = grp->getOrCreateStateSet();
    stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
  }

  void OSGPrimitive::setColor(const Color& color){
    if(shape.valid())
      shape->setColor(color);
  }

  /******************************************************************************/
  OSGDummy::OSGDummy(){}
  
  void OSGDummy::init(const OsgHandle& osgHandle){
  }
  
  void OSGDummy::setMatrix( const osg::Matrix& m4x4 ) {
  }
  
  Group* OSGDummy::getGroup() { 
    return 0;
  }

  void OSGDummy::setTexture(const std::string& filename) {
    
  }

  void OSGDummy::setColor(const Color& color) {

  }
  
  Transform* OSGDummy::getTransform() {
    return 0;
  }

  /******************************************************************************/
  OSGPlane::OSGPlane() {
  }

  void OSGPlane::init(const OsgHandle& osgHandle){
    assert(osgHandle.scene && osgHandle.tesselhints);
    geode = new Geode;  
    transform = new MatrixTransform;
    transform->addChild(geode.get());
    osgHandle.scene->addChild(transform.get());
  
    //  shape = new ShapeDrawable(new InfinitePlane(), osgHandle.tesselhints);
    shape = new ShapeDrawable(new Box(Vec3(0.0f, 0.0f, 0.0f), 
				      50, 50, 0.01), osgHandle.tesselhints); // TODO add larger values here
    shape->setColor(osgHandle.color);
    geode->addDrawable(shape.get());
  }


  /******************************************************************************/
  OSGBox::OSGBox(float lengthX, float lengthY, float lengthZ)
    : lengthX(lengthX), lengthY(lengthY), lengthZ(lengthZ) {
  }

  void OSGBox::init(const OsgHandle& osgHandle){
    assert(osgHandle.scene && osgHandle.tesselhints);
    geode = new Geode;  
    transform = new MatrixTransform;
    transform->addChild(geode.get());
    osgHandle.scene->addChild(transform.get());

    shape = new ShapeDrawable(new Box(Vec3(0.0f, 0.0f, 0.0f), 
				      lengthX, lengthY, lengthZ), osgHandle.tesselhints);
    shape->setColor(osgHandle.color);
    geode->addDrawable(shape.get());
  }

  /******************************************************************************/
  OSGSphere::OSGSphere(float radius)
    : radius(radius) {
  }

  void OSGSphere::init(const OsgHandle& osgHandle){
    assert(osgHandle.scene && osgHandle.tesselhints);

    geode = new Geode;  
    transform = new MatrixTransform;
    transform->addChild(geode.get());
    osgHandle.scene->addChild(transform.get());

    shape = new ShapeDrawable(new Sphere(Vec3(0.0f, 0.0f, 0.0f), radius), osgHandle.tesselhints);
    shape->setColor(osgHandle.color);
    geode->addDrawable(shape.get());
  }

  /******************************************************************************/
  OSGCapsule::OSGCapsule(float radius, float height)
    : radius(radius), height(height) {
  }

  void OSGCapsule::init(const OsgHandle& osgHandle){
    assert(osgHandle.scene && osgHandle.tesselhints);

    geode = new Geode;  
    transform = new MatrixTransform;
    transform->addChild(geode.get());
    osgHandle.scene->addChild(transform.get());

    shape = new ShapeDrawable(new Capsule(Vec3(0.0f, 0.0f, 0.0f), 
					  radius, height), osgHandle.tesselhints);
    shape->setColor(osgHandle.color);
    geode->addDrawable(shape.get());
  }

  /******************************************************************************/
  OSGCylinder::OSGCylinder(float radius, float height)
    : radius(radius), height(height) {
  }

  void OSGCylinder::init(const OsgHandle& osgHandle){
    assert(osgHandle.scene && osgHandle.tesselhints);

    geode = new Geode;  
    transform = new MatrixTransform;
    transform->addChild(geode.get());
    osgHandle.scene->addChild(transform.get());

    shape = new ShapeDrawable(new Cylinder(Vec3(0.0f, 0.0f, 0.0f), 
					   radius, height), osgHandle.tesselhints);
    shape->setColor(osgHandle.color);
    geode->addDrawable(shape.get());
  }

}

