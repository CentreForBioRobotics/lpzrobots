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
 *  DESCRIPTION                                                            *
 *                                                                         *
 *   $Log$
 *   Revision 1.1.2.6  2006-01-12 22:34:06  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.5  2006/01/12 14:21:00  martius
 *   drawmode, material
 *
 *   Revision 1.1.2.4  2005/12/13 18:11:13  martius
 *   transform primitive added, some joints stuff done, forward declaration
 *
 *   Revision 1.1.2.3  2005/12/11 23:35:08  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.2  2005/12/09 16:54:16  martius
 *   camera is woring now
 *
 *   Revision 1.1.2.1  2005/12/06 17:40:59  martius
 *   base class for simulation
 *
 *                                                                 *
 ***************************************************************************/

#include <iostream>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/Depth>
#include <osg/StateSet>
#include <osg/ClearNode>
#include <osg/Transform>
#include <osg/MatrixTransform>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/ShapeDrawable>

#include <osgUtil/CullVisitor>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/AnimationPathManipulator>

#include "rendertotexturecallback.h"
#include "base.h"
#include "primitive.h"

using namespace osg;

namespace lpzrobots {


  Group* Base::makeScene(){
    // no database loaded so automatically create Ed Levin Park..
    root = new Group;
    // the base and sky subgraphs go to set the earth sky of the
    // model and clear the color and depth buffer for us, by using
    // osg::Depth, and setting their bin numbers to less than 0,
    // to force them to draw before the rest of the scene.
    ClearNode* clearNode = new ClearNode;
    
    // use a transform to make the sky and base around with the eye point.
    osg::Transform* transform = new osg::Transform;//MoveEarthySkyWithEyePointTransform;

    // add the transform to the earth sky.
    clearNode->addChild(transform);

    root->addChild(clearNode);

    Group* group = new Group; // create an extra group for the normal scene
    
    root->addChild(group);

    // transform's value isn't knowm until in the cull traversal so its bounding
    // volume can't be determined, therefore culling will be invalid,
    // so switch it off, this cause all our paresnts to switch culling
    // off as well. But don't worry culling will be back on once underneath
    // this node or any other branch above this transform.
    transform->setCullingActive(false);

    // add the sky and base layer.
    transform->addChild(makeSky());  // bin number -2 so drawn first.
    transform->addChild(makeGround()); // bin number -1 so draw second.      
    
    LightSource* lightSource = makeLights(root->getOrCreateStateSet());
    transform->addChild(lightSource);
    
    // This should bring real shadows, need to be fixed somehow
    // ref_ptr<Texture2D> texture = new Texture2D;
//     texture->setInternalFormat(GL_DEPTH_COMPONENT);
//     texture->setShadowComparison(true);
//     texture->setShadowTextureMode(Texture::LUMINANCE);
    
//     ref_ptr<TexGen> texGen = new TexGen;
//     texGen->setMode(TexGen::EYE_LINEAR);
//     group->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get(), StateAttribute::ON);
//     group->getOrCreateStateSet()->setTextureAttributeAndModes(0, texGen.get(), StateAttribute::ON);
    
//     root->setCullCallback(new RenderToTextureCallback(group, texture.get(), lightSource, texGen.get()));
    
    return group; 
  }

  Node* Base::makeSky() {
    // taken from osghangglider example
    int i, j;
    float lev[] = {-5.0, -1.0, 2.0, 12.0, 30.0, 60.0, 90.0  };
    float cc[][4] =
      {
        { 0.0, 0.0, 0.15 },
        { 0.0, 0.0, 0.15 },
        { 0.4, 0.4, 0.7 },
        { 0.2, 0.2, 0.6 },
        { 0.1, 0.1, 0.6 },
        { 0.1, 0.1, 0.6 },
        { 0.1, 0.1, 0.6 },
      };
    float x, y, z;
    float alpha, theta;
    float radius = 1000.0f;
    int nlev = sizeof( lev )/sizeof(float);

    Geometry *geom = new Geometry;

    Vec3Array& coords = *(new Vec3Array(19*nlev));
    Vec4Array& colors = *(new Vec4Array(19*nlev));
    Vec2Array& tcoords = *(new Vec2Array(19*nlev));
    
    
    int ci = 0;

    for( i = 0; i < nlev; i++ )
      {
        for( j = 0; j <= 18; j++ )
	  {
            alpha = osg::DegreesToRadians(lev[i]);
            theta = osg::DegreesToRadians((float)(j*20));

            x = radius * cosf( alpha ) * cosf( theta );
            y = radius * cosf( alpha ) * -sinf( theta );
            z = radius * sinf( alpha );

            coords[ci][0] = x;
            coords[ci][1] = y;
            coords[ci][2] = z;

            colors[ci][0] = cc[i][0];
            colors[ci][1] = cc[i][1];
            colors[ci][2] = cc[i][2];
            colors[ci][3] = 1.0;

            tcoords[ci][0] = (float)j/18.0;
            tcoords[ci][1] = (float)i/(float)(nlev-1);

            ci++;
	  }


      }

    for( i = 0; i < nlev-1; i++ )
      {
        DrawElementsUShort* drawElements = new DrawElementsUShort(PrimitiveSet::TRIANGLE_STRIP);
        drawElements->reserve(38);

        for( j = 0; j <= 18; j++ )
	  {
            drawElements->push_back((i+1)*19+j);
            drawElements->push_back((i+0)*19+j);
	  }

        geom->addPrimitiveSet(drawElements);
      }
    
    geom->setVertexArray( &coords );
    geom->setTexCoordArray( 0, &tcoords );

    geom->setColorArray( &colors );
    geom->setColorBinding( Geometry::BIND_PER_VERTEX );


    Texture2D *tex = new Texture2D;
    tex->setImage(osgDB::readImageFile("Images/white.rgb"));

    StateSet *dstate = new StateSet;

    dstate->setTextureAttributeAndModes(0, tex, StateAttribute::OFF );
    dstate->setTextureAttribute(0, new TexEnv );
    dstate->setMode( GL_LIGHTING, StateAttribute::OFF );
    dstate->setMode( GL_CULL_FACE, StateAttribute::ON );
    

    // clear the depth to the far plane.
    osg::Depth* depth = new osg::Depth;
    depth->setFunction(osg::Depth::ALWAYS);
    depth->setRange(1.0,1.0);   
    dstate->setAttributeAndModes(depth,StateAttribute::ON );

    dstate->setRenderBinDetails(-2,"RenderBin");

    geom->setStateSet( dstate );

    Geode *geode = new Geode;
    geode->addDrawable( geom );

    geode->setName( "Sky" );

    return geode;
  }

  Node* Base::makeGround(){
    int i, c;
    float theta;
    float ir = 1000.0f;

    Vec3Array *coords = new Vec3Array(19);
    Vec2Array *tcoords = new Vec2Array(19);
    Vec4Array *colors = new Vec4Array(1);

    (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);

    c = 0;
    (*coords)[c].set(0.0f,0.0f,0.0f);
    (*tcoords)[c].set(0.0f,0.0f);
    
    for( i = 0; i <= 18; i++ )
      {
        theta = osg::DegreesToRadians((float)i * 20.0);

        (*coords)[c].set(ir * cosf( theta ), ir * sinf( theta ), 0.0f);
        (*tcoords)[c].set((*coords)[c][0],(*coords)[c][1]);

        c++;
      }

    Geometry *geom = new Geometry;

    geom->setVertexArray( coords );

    geom->setTexCoordArray( 0, tcoords );

    geom->setColorArray( colors );
    geom->setColorBinding( Geometry::BIND_OVERALL );

    geom->addPrimitiveSet( new DrawArrays(PrimitiveSet::TRIANGLE_FAN,0,19) );

    Texture2D *tex = new Texture2D;

    tex->setImage(osgDB::readImageFile("Images/ground.rgb"));
    tex->setWrap( Texture2D::WRAP_S, Texture2D::REPEAT );
    tex->setWrap( Texture2D::WRAP_T, Texture2D::REPEAT );

    StateSet *dstate = new StateSet;
    dstate->setMode( GL_LIGHTING, StateAttribute::OFF );
    dstate->setTextureAttributeAndModes(0, tex, StateAttribute::ON );

    dstate->setTextureAttribute(0, new TexEnv );

//     // clear the depth to the far plane.
//     osg::Depth* depth = new osg::Depth;
//     depth->setFunction(osg::Depth::ALWAYS);
//     depth->setRange(1.0,1.0);   
//     dstate->setAttributeAndModes(depth,StateAttribute::ON );

    dstate->setRenderBinDetails(-1,"RenderBin");


    geom->setStateSet( dstate );

    Geode *geode = new Geode;
    geode->addDrawable( geom );
    geode->setName( "Ground" );

    // add ODE Ground here
    ground = dCreatePlane ( odeHandle.space , 0 , 0 , 1 , 0 );

    return geode;
  }


LightSource* Base::makeLights(StateSet* stateset)
{
  // create a spot light.
  Light* light_0 = new Light;
  light_0->setLightNum(0);
  light_0->setPosition(Vec4(10.0, 0, 20.0, 1.0f));
  light_0->setDirection(Vec3(-0.5, 0, -1.0));
  light_0->setAmbient(Vec4(0.6f, 0.6f, 0.6f, 1.0f));
  light_0->setDiffuse(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
  //light_0->setSpotCutoff(60.0f);
  //  light_0->setSpotExponent(2.0f);

  LightSource* light_source_0 = new LightSource;	
  light_source_0->setLight(light_0);
  light_source_0->setLocalStateSetModes(StateAttribute::ON);   
  
  light_source_0->setStateSetModes(*stateset, StateAttribute::ON);
  
  return light_source_0;
}


/********************************************************************************/

  bool MoveEarthySkyWithEyePointTransform::computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const 
  {
    osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
    if (cv)
      {
	osg::Vec3 eyePointLocal = cv->getEyeLocal();
	matrix.preMult(osg::Matrix::translate(eyePointLocal.x(),eyePointLocal.y(),0.0f));
      }
    return true;
  }

  bool MoveEarthySkyWithEyePointTransform::computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
  {
    std::cout<<"computing transform"<<std::endl;
    
    osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
    if (cv)
      {
	osg::Vec3 eyePointLocal = cv->getEyeLocal();
	matrix.postMult(osg::Matrix::translate(-eyePointLocal.x(),-eyePointLocal.y(),0.0f));
      }
    return true;
  }
  

}



