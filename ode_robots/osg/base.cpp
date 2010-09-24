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
 *                                                                         *
 *   $Log$
 *   Revision 1.50  2010-09-24 09:00:04  martius
 *   fixed lighing bug! Juhu. The StateSet was shared such that the material
 *    was shared. In this way the last material was used for all primitives
 *
 *   Revision 1.49  2010/09/23 08:35:22  martius
 *   light generation improved
 *
 *   Revision 1.48  2010/05/28 13:48:30  martius
 *   initialized dummy to 0
 *
 *   Revision 1.47  2010/03/25 16:39:51  martius
 *   primitive has addForce/addTorque function
 *
 *   Revision 1.46  2010/03/17 09:33:16  martius
 *   removed memory leaks and some small bugs
 *   valgrind suppression file is updated
 *
 *   Revision 1.45  2010/03/16 15:47:46  martius
 *   osgHandle has now substructures osgConfig and osgScene
 *    that minimized amount of redundant data (this causes a lot of changes)
 *   Scenegraph is slightly changed. There is a world and a world_noshadow now.
 *    Main idea is to have a world without shadow all the time avaiable for the
 *    Robot cameras (since they do not see the right shadow for some reason)
 *   tidied up old files
 *
 *   Revision 1.44  2010/03/10 10:43:52  guettler
 *   SoftShadowMap uses now parameter shadowsize (was not set yet)
 *
 *   Revision 1.43  2010/03/07 22:39:08  guettler
 *   moved shadow to OsgHandle.shadowType (TODO: move it to OsgConfig)
 *
 *   Revision 1.42  2010/03/05 14:32:55  martius
 *   camera sensor added
 *   for that the scenegraph structure was changed into root, world, scene
 *   camera does not work with shadows
 *   works with newest version of ode (0.11)
 *
 *   Revision 1.41  2010/01/26 10:11:47  martius
 *   unbelievable, the createHUD problem was not solved. Here we go, hopefully...
 *
 *   Revision 1.40  2009/11/26 10:16:07  martius
 *   hopefully finally fixed the createHUD problem.
 *   the getHUD function only creates a HUD that is detached. That is why
 *   there is the new createHUDManager function
 *
 *   Revision 1.39  2009/09/08 16:26:30  fhesse
 *   typo corrected
 *
 *   Revision 1.38  2009/09/03 12:53:25  guettler
 *   reverted changes of revision 1.37:
 *   - getHUDSM is called without calling createHUDSM before
 *     if simulation is started with -nographics
 *   FIX: createHUDSM calls now getHUDSM (fix of HUDSM was not complete)
 *
 *   Revision 1.37  2009/09/03 11:22:54  martius
 *   fix of HUDSM was not complete, it was only in getHUD not in createHUD
 *   getHUD assumes now that the hUDStatisticsManager exists (no double creation code)
 *
 *   Revision 1.36  2009/09/03 10:23:31  guettler
 *   FIX: - hudsm did not updated: added StatisticTools instead
 *   of HUDSM to callback type Base::PHYSICS_CALLBACKABLE
 *
 *   Revision 1.35  2009/08/10 14:55:32  der
 *   shadowTexSize and shadow are integer
 *
 *   Revision 1.34  2009/08/10 07:54:32  guettler
 *   - uses new BackCaller implementation
 *   - bugfix: avoid crash if noGraphics when getting HUDSM
 *
 *   Revision 1.33  2009/08/07 13:27:46  martius
 *   makePhysicalScene to create phyiscal scene independent of graphical scene
 *     (to cope with new noGraphics implementation)
 *
 *   Revision 1.32  2009/07/30 12:09:12  guettler
 *   commented out unused variables
 *
 *   Revision 1.31  2009/07/29 14:19:49  jhoffmann
 *   Various bugfixing, remove memory leaks (with valgrind->memcheck / alleyoop)
 *
 *   Revision 1.30  2009/03/31 15:46:05  martius
 *   caption is a std::string and can be changed on-line
 *
 *   Revision 1.29  2009/03/27 09:06:24  guettler
 *   corrected encoding type to 8859-1
 *
 *   Revision 1.28  2009/03/27 06:21:31  guettler
 *   CTRL +S  changes now the shadow type in the simulation: cleaned up the code
 *
 *   Revision 1.27  2009/03/26 19:20:57  martius
 *   setUserLight  compability  with osg<2.6
 *
 *   Revision 1.26  2009/03/25 15:44:23  guettler
 *   ParallelSplitShadowMap: corrected light direction (using directional light), complete ground is now shadowed
 *
 *   Revision 1.25  2009/03/13 09:19:53  martius
 *   changed texture handling in osgprimitive
 *   new OsgBoxTex that supports custom texture repeats and so on
 *   Box uses osgBoxTex now. We also need osgSphereTex and so on.
 *   setTexture has to be called before init() of the primitive
 *
 *   Revision 1.24  2009/01/20 17:27:34  martius
 *   texture for background changeable from outside
 *
 *   Revision 1.23  2009/01/05 13:18:48  martius
 *   ambient light even more
 *
 *   Revision 1.22  2008/10/10 14:05:15  martius
 *   time display was wrong
 *
 *   Revision 1.21  2008/07/29 15:44:00  guettler
 *   removed forceFronCullFace option for pssm to obtain compatibiltity with OSG 2.6
 *
 *   Revision 1.20  2008/06/26 10:15:55  der
 *   changed far distance from 10 to 30 for parallel split shadow map
 *
 *   Revision 1.19  2008/05/05 09:35:35  guettler
 *   hud now displays if in pause mode
 *
 *   Revision 1.18  2008/04/30 13:13:20  guettler
 *   caption corrected
 *
 *   Revision 1.17  2008/04/23 07:17:16  martius
 *   makefiles cleaned
 *   new also true realtime factor displayed,
 *    warning if out of sync
 *   drawinterval in full speed is 10 frames, independent of the speed
 *
 *   Revision 1.16  2008/04/18 14:00:09  guettler
 *   cosmetic changes, added some printouts
 *
 *   Revision 1.15  2008/04/17 15:59:00  martius
 *   OSG2 port finished
 *
 *   Revision 1.14.2.7  2008/04/17 15:04:55  martius
 *   shadow is 5 on default, also improved cmd line parsing of shadow and texsize
 *
 *   Revision 1.14.2.6  2008/04/11 13:46:50  martius
 *   quickMP multithreading included
 *
 *   Revision 1.14.2.5  2008/04/11 10:41:35  martius
 *   config file added
 *
 *   Revision 1.14.2.4  2008/04/10 07:40:17  guettler
 *   Optimised parameters for the ShadowTechnique ParallelSplitShadowMap.
 *
 *   Revision 1.14.2.3  2008/04/09 14:25:35  martius
 *   shadow cmd line option
 *
 *   Revision 1.14.2.2  2008/04/09 13:57:59  guettler
 *   New ShadowTechnique added.
 *
 *   Revision 1.14.2.1  2008/04/09 10:18:41  martius
 *   fullscreen and window options done
 *   fonts on hud changed
 *
 *   Revision 1.14  2007/09/28 12:31:49  robot3
 *   The HUDSM is not anymore deduced from StatisticalTools, so the statistics
 *   can be updated independently from the HUD
 *   addPhysicsCallbackable and addGraphicsCallbackable now exists in Simulation
 *
 *   Revision 1.13  2007/09/28 10:24:04  robot3
 *   The WindowStatisticsManager is now called HUDStatisticsManager
 *
 *   Revision 1.12  2007/09/28 10:08:49  robot3
 *   fixed memory bugs, statistics are from now on aligned right
 *
 *   Revision 1.11  2007/09/27 10:47:04  robot3
 *   mathutils: moved abs to selforg/stl_adds.h
 *   simulation,base: added callbackable support,
 *   added WSM (HUDStatisticsManager) funtionality
 *
 *   Revision 1.10  2007/08/29 13:08:04  martius
 *   added HUD with time and caption
 *
 *   Revision 1.9  2007/04/05 15:10:36  martius
 *   different ground
 *
 *   Revision 1.8  2007/03/16 11:37:11  martius
 *   ground plane gets primitive to support substances
 *
 *   Revision 1.7  2007/02/21 14:26:18  martius
 *   increased ambient light
 *
 *   Revision 1.6  2006/09/20 15:30:47  martius
 *   shadowsize, light
 *
 *   Revision 1.5  2006/09/20 12:55:44  martius
 *   Light
 *
 *   Revision 1.4  2006/08/30 08:59:21  martius
 *   categories and collision mask used for static geoms to reduce number of collision checks
 *
 *   Revision 1.3  2006/08/04 15:05:42  martius
 *   documentation
 *
 *   Revision 1.2  2006/07/14 12:23:33  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.9  2006/06/29 16:35:56  robot3
 *   includes cleared up
 *
 *   Revision 1.1.2.8  2006/05/28 22:12:16  martius
 *   - noshadow cmdline flag
 *
 *   Revision 1.1.2.7  2006/05/18 11:42:51  robot3
 *   -shadowing the normal scene integrated (first version)
 *   -note that there is a bug that the shadow disappears
 *    after some time (e.g. 60 minutes)
 *
 *   Revision 1.1.2.6  2006/01/12 22:34:06  martius
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
#include <assert.h>
#include <osg/Node>
#include <osg/CameraNode>
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
#include <osg/PolygonOffset>
#include <osg/CullFace>
#include <osg/TexGenNode>
#include <osgText/Font>

#include <osgUtil/CullVisitor>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/AnimationPathManipulator>

#include "base.h"

#include "osgprimitive.h"
#include "primitive.h"

#include <selforg/callbackable.h>

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
//#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>

using namespace osg;

namespace lpzrobots {


  /********************************************************************
   * fragment shader for non textured objects (non-default, not used) *
   *******************************************************************/
  char fragmentShaderSource_noBaseTexture[] =
  "uniform sampler2DShadow shadowTexture; \n"
  "uniform vec2 ambientBias; \n"
  "\n"
  "void main(void) \n"
  "{ \n"
  "    ambientBias.x=0.8f; \n"
  "    ambientBias.y=0.4f; \n"
  "    gl_FragColor = gl_Color * (ambientBias.x + shadow2DProj( shadowTexture, gl_TexCoord[0] ) * ambientBias.y - 0.4f); \n"
  "}\n";


  /********************************************************************
   * fragment shader for textured objects (default, used)             *
   *******************************************************************/
  char fragmentShaderSource_withBaseTexture[] =
  "uniform sampler2D baseTexture; \n"
  "uniform sampler2DShadow shadowTexture; \n"
  "uniform vec2 ambientBias; \n"
  "\n"
  "void main(void) \n"
  "{ \n"
  "    vec4 color = gl_Color* texture2D( baseTexture, gl_TexCoord[0].xy ); \n"
  "    gl_FragColor = color * (ambientBias.x + shadow2DProj( shadowTexture, gl_TexCoord[1])  * ambientBias.y); \n"
  "}\n";

     Base::Base(const std::string& caption)
       : ground(0), caption(caption), groundTexture("Images/greenground.rgb"), 
         dummy(0), hud(0), timestats(0), captionline(0), statisticLine(0), 
         plane(0), hUDStatisticsManager(0), ReceivesShadowTraversalMask(0x1),
         CastsShadowTraversalMask(0x2), shadowTexSize(2048), useNVidia(1)
     {
     }


  Base::~Base(){
  }

  void Base::base_close(){
    if(plane) delete plane;
    //    if(dummy) dummy->unref(); // this happens automatically
    if(ground ){
      dGeomDestroy(ground);
    }
    if(hUDStatisticsManager) delete hUDStatisticsManager;
  }


  /** Shadow types: 1 - LightSpacePerspectiveShadowMap 
   * 2 - ShadowTextue 3 - ParallelSplitShadowMap
   * 4 - SoftShadowMap 5 - ShadowMap
   */
  osgShadow::ShadowedScene* Base::createShadowedScene(osg::Node* sceneToShadow, LightSource* lightSource, int shadowType)
{
  // some conf variables for ShadowVolume
  bool twoSided=false;
  bool twoPass=false;
  bool updateLightPosition = false;

  // some conf variables for ParallelSplitShadowMap
  int mapCount =3;
  bool debugColor=false;
  int minNearSplit=3;
  int maxFarDist=50;
  // int moveVCamFactor = 0;
  // double polyoffsetfactor = -0.02;
  // double polyoffsetunit = 1.0;
  // 20080728; guettler: commented out for OSG 2.6 compatibility

  // some conf variables for SoftShadowMap
  // make the shadow prenumba a little bit sharper then default (0.005)
  float softnessWidth = 0.002;

  osgShadow::ShadowedScene* shadowedScene = new osgShadow::ShadowedScene;

  shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
  shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
  shadowedScene->setNodeMask(CastsShadowTraversalMask | ReceivesShadowTraversalMask);

  // add ShadowTechnique

  switch(shadowType) {
//   case 1: /// LightSpacePerspectiveShadowMap
//     {
//       osg::ref_ptr<osgShadow::MinimalShadowMap> sm = 
//         new osgShadow::LightSpacePerspectiveShadowMapDB();
//       shadowedScene->setShadowTechnique( sm.get() );
//       float minLightMargin = 10.f;
//       float maxFarPlane = 50;
//       unsigned int baseTexUnit = 0;
//       unsigned int shadowTexUnit = 7;
//       sm->setMinLightMargin( minLightMargin );
//       sm->setMaxFarPlane( maxFarPlane );
//       sm->setTextureSize( osg::Vec2s((int)shadowTexSize,(int)shadowTexSize) );
//       sm->setShadowTextureCoordIndex( shadowTexUnit );
//       sm->setShadowTextureUnit( shadowTexUnit );
//       sm->setBaseTextureCoordIndex( baseTexUnit );
//       sm->setBaseTextureUnit( baseTexUnit );      

//     } 
  case 1: /// ShadowVolume
    {
      // hint to tell viewer to request stencil buffer when setting up windows
      osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

      osg::ref_ptr<osgShadow::ShadowVolume> sv = new osgShadow::ShadowVolume;
      sv->setDynamicShadowVolumes(updateLightPosition);
      if (twoSided)
        sv->setDrawMode(osgShadow::ShadowVolumeGeometry::STENCIL_TWO_SIDED);
      if (twoPass)
        sv->setDrawMode(osgShadow::ShadowVolumeGeometry::STENCIL_TWO_PASS);
      shadowedScene->setShadowTechnique(sv.get());
    }
    break;
  case 2: /// ShadowTexture
    {
      osg::ref_ptr<osgShadow::ShadowTexture> st = new osgShadow::ShadowTexture;
      shadowedScene->setShadowTechnique(st.get());
    }
    break;
  case 3: /// ParallelSplitShadowMap
    {
        osg::ref_ptr<osgShadow::ParallelSplitShadowMap> pssm = new osgShadow::ParallelSplitShadowMap(NULL,mapCount);

        pssm->setTextureResolution(shadowTexSize);

        if (debugColor)
          pssm->setDebugColorOn();

      	pssm->setMinNearDistanceForSplits(minNearSplit);

        pssm->setMaxFarDistance(maxFarDist);

        if ( maxFarDist > 0 ) {
        	int moveVCamFactor = 0;
            pssm->setMoveVCamBehindRCamFactor(moveVCamFactor);
        }
        
        if (useNVidia!=0)
          // pssm->setPolygonOffset(osg::Vec2(10.0f,20.0f));     
        //        pssm->setPolygonOffset(osg::Vec2(1.0f,4.0f)); 
/*        double polyoffsetfactor = pssm->getPolygonOffset().x();
        double polyoffsetunit   = pssm->getPolygonOffset().y();
        while (arguments.read("--PolyOffset-Factor", polyoffsetfactor));
        while (arguments.read("--PolyOffset-Unit", polyoffsetunit));
        pssm->setPolygonOffset(osg::Vec2(polyoffsetfactor,polyoffsetunit));*/


        shadowedScene->setShadowTechnique(pssm.get());

#if OPENSCENEGRAPH_MAJOR_VERSION == 2 &&  OPENSCENEGRAPH_MINOR_VERSION >= 6
        pssm->setUserLight(lightSource->getLight());
#endif
	/*
	osg::ref_ptr<osgShadow::ParallelSplitShadowMap> pssm = new osgShadow::ParallelSplitShadowMap(NULL,mapCount);

	if (debugColor)
	  pssm->setDebugColorOn();

	if (useNVidia!=0)
        pssm->setPolygonOffset(osg::Vec2(10.0f,20.0f)); //NVidea
	else
	  pssm->setPolygonOffset(osg::Vec2(polyoffsetfactor,polyoffsetunit)); //ATI Radeon

	// 20080728; guettler: commented out for OSG 2.6 compatibility
	      if (cullFaceFront)
	        pssm->forceFrontCullFace();

	shadowedScene->setShadowTechnique(pssm.get());*/
    }
    break;
  case 4: /// SoftShadowMap
    {
      osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
      sm->setSoftnessWidth(softnessWidth);
      sm->setTextureSize(osg::Vec2s((int)shadowTexSize,(int)shadowTexSize));
      shadowedScene->setShadowTechnique(sm.get());
    }
    break;
  case 5: /// ShadowMap
  default:
    {
      osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
      shadowedScene->setShadowTechnique(sm.get());
      sm->setTextureSize(osg::Vec2s((int)shadowTexSize,(int)shadowTexSize));
    }
    break;
  }
  shadowedScene->addChild(sceneToShadow);

  return shadowedScene;
}

  osg::Node* Base::createHUD()
  {
    osg::Geode* geode = new osg::Geode();

    // turn lighting off for the text and disable depth test to ensure its always ontop.
    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    osgText::Font* font = osgText::readFontFile("fonts/fudd.ttf");

    osg::Vec3 position(500.0f,9.0f,0.0f);
    Color textColor(0.2,0.2,0.0);
    int fontsize=12;

    {
      captionline = new  osgText::Text;
      geode->addDrawable( captionline );
      captionline->setCharacterSize(fontsize);
      captionline->setFont(font);
      captionline->setPosition(position);
      captionline->setColor(textColor);
      captionline->setAlignment(osgText::Text::RIGHT_BASE_LINE);
      captionline->setText(caption.c_str());
    }

    // timing
    position=osg::Vec3(12.0f,9.0f,0.0f);
    {
      timestats = new  osgText::Text;
      geode->addDrawable( timestats);
      timestats->setCharacterSize(fontsize);
      timestats->setFont(font);
      timestats->setPosition(position);
      timestats->setColor(textColor);
      setTimeStats(0,0,0,false);
    }

    {
      osg::Geometry* geom = new osg::Geometry;

      osg::Vec3Array* vertices = new osg::Vec3Array;
      double xMin=6;
      double xMax=506;
      double yMin=6;
      double yMax=22;
      double depth=-0.1;
      vertices->push_back(osg::Vec3(xMin,yMax,depth));
      vertices->push_back(osg::Vec3(xMin,yMin,depth));
      vertices->push_back(osg::Vec3(xMax,yMin,depth));
      vertices->push_back(osg::Vec3(xMax,yMax,depth));

      vertices->push_back(osg::Vec3());
      geom->setVertexArray(vertices);

      osg::Vec3Array* normals = new osg::Vec3Array;
      normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
      geom->setNormalArray(normals);
      geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

      osg::Vec4Array* colors = new osg::Vec4Array;
      colors->push_back(osg::Vec4(1.0f,1.0,0.8f,0.5f));
      geom->setColorArray(colors);
      geom->setColorBinding(osg::Geometry::BIND_OVERALL);

      geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

      osg::StateSet* stateset = geom->getOrCreateStateSet();
      stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
      //stateset->setAttribute(new osg::PolygonOffset(1.0f,1.0f),osg::StateAttribute::ON);
      stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

      geode->addDrawable(geom);

      // create HUDStatisticsManager and register it for being called back every step
      createHUDManager(geode,font);
    }

    osg::CameraNode* camera = new osg::CameraNode;

    // set the projection matrix
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0,512,0,384));

    // set the view matrix
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera->setRenderOrder(osg::CameraNode::POST_RENDER);

    camera->addChild(geode);

    return camera;
  }

  void  Base::createHUDManager(osg::Geode* geode, osgText::Font* font){
    hUDStatisticsManager = new HUDStatisticsManager(geode,font);
    this->addCallbackable(hUDStatisticsManager->getStatisticTools(), Base::PHYSICS_CALLBACKABLE);
    this->addCallbackable(hUDStatisticsManager, Base::GRAPHICS_CALLBACKABLE);
  }

  void Base::setTimeStats(double time, double realtimefactor,
			  double truerealtimefactor, bool pause){
    if(timestats){
      char buffer[100];
      int minutes = int(time)/60;
      int seconds = int(time)%60;
      if (pause) {
        sprintf(buffer,"Time: %02i:%02i  Speed: %.1fx (paused)",minutes,
                seconds,realtimefactor);
      } else if (realtimefactor>0){
	if(fabs(truerealtimefactor/realtimefactor-1)<0.15)
	  sprintf(buffer,"Time: %02i:%02i  Speed: %.1fx",minutes,
		  seconds,realtimefactor);
	else
	  sprintf(buffer,"Time: %02i:%02i  Speed: %.1fx(%.1fx!)",minutes,
		  seconds,truerealtimefactor, realtimefactor);
      } else
        sprintf(buffer,"Time: %02i:%02i  Speed: %.1fx (max)",minutes, seconds,truerealtimefactor);
      timestats->setText(buffer);
    }
  }

  void Base::setCaption(const std::string& caption) {
    this->caption = caption;
    if(captionline){
      captionline->setText(caption);
    }
  }

  void Base::makeScene(OsgScene* scene){
    scene->root = new Group; // master node containing scene but also hud and other stuff
    scene->world = new Group;// actual world with sky, ground, scene and so on.
    scene->world_noshadow = new Group;// like world but without shadow
    scene->scene = new Group;// actual scene with robots and stuff
    // the base and sky subgraphs go to set the earth sky of the
    // model and clear the color and depth buffer for us, by using
    // osg::Depth, and setting their bin numbers to less than 0,
    // to force them to draw before the rest of the scene.
    ClearNode* clearNode = new ClearNode;

    // use a transform to make the sky and base around with the eye point.
    scene->worldtransform = new osg::Transform;//MoveEarthySkyWithEyePointTransform;
    // add the transform to the earth sky.
    clearNode->addChild(scene->worldtransform);
    // transform's value isn't known until in the cull traversal so its bounding
    // volume can't be determined, therefore culling will be invalid,
    // so switch it off, this cause all our parents to switch culling
    // off as well. But don't worry culling will be back on once underneath
    // this node or any other branch above this transform.
    scene->worldtransform->setCullingActive(false);
    // add the sky and base layer.
    scene->world->addChild(clearNode);    

    // do the same clearnote and transform for world_noshadow
    ClearNode* clearNodeNS = new ClearNode;
    osg::Transform* transformNS = new osg::Transform;
    clearNodeNS->addChild(transformNS);
    transformNS->setCullingActive(false);

    // add the sky and base layer.
    scene->world_noshadow->addChild(clearNodeNS);    
    
  
    osg::Node* sky = makeSky();
    scene->worldtransform->addChild(sky); // bin number -2 so drawn first.
    transformNS->addChild(sky);          // bin number -2 so drawn first.

    scene->groundScene = makeGround();
    // add it  to the noshadow world 
    // for the shadow we have to distinguish between different modes
    transformNS->addChild(scene->groundScene);     
   

    // scene->lightSource = makeLights(scene->world->getOrCreateStateSet());
    // scene->worldtransform->addChild(scene->lightSource);    
    // transformNS->addChild(makeLights(transformNS->getOrCreateStateSet()));    
    makeLights(scene->worldtransform);
    makeLights(transformNS);
    
    
    int shadowType=(int)osgHandle.cfg->shadowType;
    if(shadowType){
      // create root of shadowedScene
      scene->shadowedSceneRoot = new osg::Group;
      scene->shadowedSceneRoot->addChild(scene->scene);
      scene->shadowedScene = createShadowedScene(scene->shadowedSceneRoot,
                                                 scene->lightSource,(int)osgHandle.cfg->shadowType);

      // 20090325; guettler: if using pssm (shadowtype 3), add also the ground to the shadowed scene
      if (shadowType==3)
      	scene->shadowedSceneRoot->addChild(scene->groundScene); // bin number -1 so draw second.      
      else 
        scene->worldtransform->addChild(scene->groundScene); // bin number -1 so draw second.
    

      scene->root->addChild(scene->world);
    }else {
      scene->root->addChild(scene->world_noshadow);
    }

    // add the shadowed scene to the world
    scene->worldtransform->addChild(scene->shadowedScene);    
    // scene->world->addChild(scene->shadowedScene);    
    // add the normal scene to the root
    //scene->world_noshadow->addChild(scene->scene);    
    transformNS->addChild(scene->scene);    
    
    dummy=new osg::Group; // we uses this hack to prevent the nodes from being deleted
    dummy->addChild(scene->world);
    dummy->addChild(scene->world_noshadow);


    hud = createHUD();
    if(hud) scene->root->addChild(hud);    
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

  void Base::makePhysicsScene(){
    // add ODE Ground here (physical plane)
    ground = dCreatePlane ( odeHandle.space , 0 , 0 , 1 , 0 );
    dGeomSetCategoryBits(ground,Primitive::Stat);
    dGeomSetCollideBits(ground,~Primitive::Stat);
    // assign a dummy primitive to the ground plane to have substance (material) support
    plane = new Plane();
    dGeomSetData(ground, (void*)plane);
    //    std::cout << "GROUND: " << ground << std::endl;
  }


  Node* Base::makeGround(){ // the old ground, is NOT used for shadowing!
    float ir = 1000.0f;
    float texscale =0.2;
    Vec3Array *coords = new Vec3Array(4);
    Vec2Array *tcoords = new Vec2Array(4);
    Vec4Array *colors = new Vec4Array(1);

    (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);

    (*coords)[0].set(-ir,-ir,0.0f);
    (*coords)[1].set( ir,-ir,0.0f);
    (*coords)[2].set( ir, ir,0.0f);
    (*coords)[3].set(-ir, ir,0.0f);
    (*tcoords)[0].set(-texscale*ir,-texscale*ir);
    (*tcoords)[1].set(-texscale*ir, texscale*ir);
    (*tcoords)[2].set( texscale*ir, texscale*ir);
    (*tcoords)[3].set( texscale*ir,-texscale*ir);

//     int i, c;
//     float theta;
//     float ir = 1000.0f;

//     Vec3Array *coords = new Vec3Array(19);
//     Vec2Array *tcoords = new Vec2Array(19);
//     Vec4Array *colors = new Vec4Array(1);

//     (*colors)[0].set(1.0f,1.0f,1.0f,1.0f);

//     c = 0;
//     (*coords)[c].set(0.0f,0.0f,0.0f);
//     (*tcoords)[c].set(0.0f,0.0f);

//     for( i = 0; i <= 18; i++ )
//       {
//         theta = osg::DegreesToRadians((float)i * 20.0);

//         (*coords)[c].set(ir * cosf( theta ), ir * sinf( theta ), -0.001f);
//         (*tcoords)[c].set((*coords)[c][0],(*coords)[c][1]);

//         c++;
//       }

    Geometry *geom = new Geometry;

    geom->setVertexArray( coords );

    geom->setTexCoordArray( 0, tcoords );

    geom->setColorArray( colors );
    geom->setColorBinding( Geometry::BIND_OVERALL );

    //    geom->addPrimitiveSet( new DrawArrays(PrimitiveSet::TRIANGLE_FAN,0,19) );
    geom->addPrimitiveSet( new DrawArrays(PrimitiveSet::TRIANGLE_FAN,0,4) );

    Texture2D *tex = new Texture2D;

    tex->setImage(osgDB::readImageFile(groundTexture));
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
    geode->setNodeMask(geode->getNodeMask() & ~CastsShadowTraversalMask);


    return geode;
  }


  void Base::makeLights(osg::Group* node)
  {

    // // create a default light
    osg::Light* myLight0 = new osg::Light();
    myLight0->setLightNum(1);
    osg::LightSource* lightS0 = new osg::LightSource();
    node->addChild(lightS0);
    lightS0->setLight(myLight0);
    lightS0->setStateSetModes(*(node->getOrCreateStateSet()),osg::StateAttribute::ON);

    //       /*
    // // create a spot light.
    // Light* light_0 = new Light;
    // light_0->setLightNum(0);
    // //    light_0->setPosition(Vec4(0.0f, 0.0f, 50.0f, 1.0f));
    // light_0->setPosition(Vec4(40.0f, 40.0f, 50.0f, 1.0f));
    // //    light_0->setAmbient(Vec4(0.25f, 0.25f, 0.25f, 1.0f));
    // light_0->setAmbient(Vec4(0.7f, 0.7f, 0.7f, 1.0f));  // Georg 21.07.2007 changed from 0.5 to 0.7
    // //light_0->setAmbient(Vec4(0.9f, 0.9f, 0.9f, 1.0f));  // Georg 05.01.2008 changed from 0.7 to 0.9
    // light_0->setDiffuse(Vec4(0.8f, 0.8f, 0.8f, 1.0f));
    // //    light_0->setDirection(Vec3(-1.0f, -1.0f, -1.2f));
    // light_0->setSpecular(Vec4(1.0f, 0.9f, 0.8f, 1.0f));

    // LightSource* light_source_0 = new LightSource;
    // light_source_0->setLight(light_0);
    // light_source_0->setLocalStateSetModes(StateAttribute::ON);
    // light_source_0->setStateSetModes(*stateset, StateAttribute::ON);

    // return light_source_0;
    // */

    // create a directional light (infinite distance place at 45 degrees)
    osg::Light* myLight1 = new osg::Light();
    myLight1->setLightNum(0); 
    myLight1->setPosition(osg::Vec4(1.0,1.0,1.0,0.0f));
    myLight1->setDirection(osg::Vec3(-1.0, -1.0, -1.0));
    myLight1->setAmbient(osg::Vec4(0.5f,0.5f,0.5f,1.0f));
    myLight1->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    //    myLight1->setSpecular(osg::Vec4(.0f,.0f,.0f,.0f));
    myLight1->setConstantAttenuation(1.0f);



    osg::LightSource* lightS1 = new osg::LightSource();
    lightS1->setLight(myLight1);
    // lightS->setLocalStateSetModes(osg::StateAttribute::ON);

    node->addChild(lightS1);
    lightS1->setStateSetModes(*(node->getOrCreateStateSet()),osg::StateAttribute::ON);

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


  void Base::changeShadowTechnique()
  {
    OsgScene* scene = osgHandle.scene;
    std::string shadowName;
    int shadowType = ++(osgHandle.cfg->shadowType);
    switch (shadowType) {
    case 6:
      shadowType=0; // max shadowtype at the moment: 5
    case 0:
      scene->root->removeChild(scene->world);
      scene->root->addChild(scene->world_noshadow);
      shadowName = std::string("NoShadow");
      break;
    case 1:
    case 2:
      shadowType=3; // temporarily disable volume shadows (1) and ShadowTextue (2)
    case 3:
      scene->root->removeChild(scene->world_noshadow);
      scene->root->addChild(scene->world);
      if(scene->shadowedScene) {
        scene->world->removeChild(scene->shadowedScene);
      }
      scene->shadowedSceneRoot = new osg::Group;
      scene->shadowedSceneRoot->addChild(scene->groundScene);
      scene->shadowedSceneRoot->addChild(scene->scene);
                 
      scene->shadowedScene = createShadowedScene(scene->shadowedSceneRoot,
                                                 scene->lightSource, 
                                                 shadowType);
      scene->world->addChild(scene->shadowedScene);
      // 20090325; guettler: if using pssm (shadowtype 3), add also the ground to the shadowed scene
      scene->worldtransform->removeChild(scene->groundScene);
      shadowName = std::string("ParallelSplitShadowMap");
      break;
    case 4:
      scene->world->removeChild(scene->shadowedScene);
      scene->shadowedScene = createShadowedScene(scene->scene,scene->lightSource, shadowType);
      scene->world->addChild(scene->shadowedScene);
      scene->worldtransform->addChild(scene->groundScene); // bin number -1 so draw second.
      scene->shadowedSceneRoot->removeChild(scene->groundScene);
      shadowName = std::string("SoftShadowMap");
      break;
    case 5:
      scene->world->removeChild(scene->shadowedScene);
      scene->shadowedScene = createShadowedScene(scene->scene,scene->lightSource, shadowType);
      // add the shadowed scene to the root
      scene->world->addChild(scene->shadowedScene);
      shadowName = std::string("ShadowMap (simple)");
      break; 
    default:
      shadowName = std::string("NoShadow");
      break;
    }
    printf("Changed shadowType to %i (%s)\n",shadowType,shadowName.c_str());
    osgHandle.cfg->shadowType=shadowType;
  }

// Helper
int Base::contains(char **list, int len,  const char *str) {
  for(int i=0; i<len; i++) {
    if(strcmp(list[i],str) == 0)
      return i+1;
  }
  return 0;
}


HUDStatisticsManager* Base::getHUDSM()
{
  if (hUDStatisticsManager==0)
  {
    // create HUDStatisticsManager and register it for being called back every step
    // but do not display if the system is initialised with -nographics
    createHUDManager(new osg::Geode(),osgText::readFontFile("fonts/fudd.ttf"));
  }
  return hUDStatisticsManager;
}


}
