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
 *   Revision 1.27  2011-06-03 13:42:48  martius
 *   oderobot has objects and joints, store and restore works automatically
 *   removed showConfigs and changed deprecated odeagent calls
 *
 *   Revision 1.26  2011/06/01 22:02:56  martius
 *   getAllPrimitives changed to vector return type
 *   inspectables infolines are printed without name again (for guilogger)
 *
 *   Revision 1.25  2011/05/30 13:56:42  martius
 *   clean up: moved old code to oldstuff
 *   configable changed: notifyOnChanges is now used
 *    getParam,setParam, getParamList is not to be overloaded anymore
 *
 *   Revision 1.24  2011/04/28 09:42:27  martius
 *   texture changed
 *
 *   Revision 1.23  2010/12/17 17:00:26  martius
 *   odeagent has new constructor (old is marked as deprecated) -> log files have again
 *    important information about simulation
 *   addsensorstorobotadapater copies configurables
 *   torquesensors still in debug mode
 *   primitives support explicit decelleration (useful for rolling friction)
 *   hurling snake has rolling friction
 *
 *   Revision 1.22  2010/06/28 14:47:50  martius
 *   internal collisions are now switched on again
 *   joints do not ignore collision of connected pairs here
 *   frictionGround effects substances->works again
 *
 *   Revision 1.21  2010/01/26 09:53:06  martius
 *   changed to new collision model
 *
 *   Revision 1.20  2009/03/13 09:19:53  martius
 *   changed texture handling in osgprimitive
 *   new OsgBoxTex that supports custom texture repeats and so on
 *   Box uses osgBoxTex now. We also need osgSphereTex and so on.
 *   setTexture has to be called before init() of the primitive
 *
 *   Revision 1.19  2008/05/07 16:45:51  martius
 *   code cosmetics and documentation
 *
 *   Revision 1.18  2007/11/07 13:21:16  martius
 *   doInternal stuff changed signature
 *
 *   Revision 1.17  2007/09/06 18:47:59  martius
 *   createNewSimpleSpace used
 *
 *   Revision 1.16  2007/06/28 11:25:36  fhesse
 *   dBodyAddRelForce() as comment in  setMotors to check later
 *
 *   Revision 1.15  2006/10/20 14:25:07  martius
 *   *** empty log message ***
 *
 *   Revision 1.14  2006/09/21 22:09:58  martius
 *   collision for mesh
 *
 *   Revision 1.13  2006/09/21 16:15:40  der
 *   different friction because of terrain
 *
 *   Revision 1.12  2006/08/11 15:44:53  martius
 *   *** empty log message ***
 *
 *   Revision 1.11  2006/07/14 15:13:45  fhesse
 *   minor changes
 *
 *   Revision 1.9.4.7  2006/06/29 16:39:55  robot3
 *   -you can now see bounding shapes if you type ./start -drawboundings
 *   -includes cleared up
 *   -abstractobstacle and abstractground have now .cpp-files
 *
 *   Revision 1.9.4.6  2006/06/25 17:00:32  martius
 *   Id
 *
 *   Revision 1.9.4.5  2006/06/25 16:57:13  martius
 *   abstractrobot is configureable
 *   name and revision
 *
 *   Revision 1.9.4.4  2005/12/30 22:54:38  martius
 *   removed parentspace!
 *
 *   Revision 1.9.4.3  2005/12/21 17:34:59  martius
 *   moved to osg
 *
 *   Revision 1.9.4.2  2005/11/15 12:29:26  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.9.4.1  2005/11/14 17:37:17  martius
 *   moved to selforg
 *
 *   Revision 1.9  2005/11/09 13:26:31  martius
 *   added factorSensors
 *
 *   Revision 1.8  2005/10/06 17:14:24  martius
 *   switched to stl lists
 *
 *   Revision 1.7  2005/09/22 12:24:37  martius
 *   removed global variables
 *   OdeHandle and GlobalData are used instead
 *   sensor prepared
 *
 *   Revision 1.6  2005/08/31 17:18:15  fhesse
 *   setTextures added, Mass is now sphere (not box anymore)
 *
 *   Revision 1.5  2005/08/29 06:41:22  martius
 *   kosmetik
 *
 *   Revision 1.4  2005/08/03 20:35:28  martius
 *   *** empty log message ***
 *
 *   Revision 1.3  2005/07/27 13:23:09  martius
 *   new color and position construction
 *
 *   Revision 1.2  2005/07/26 17:04:21  martius
 *   lives in its own space now
 *
 *   Revision 1.1  2005/07/21 12:17:04  fhesse
 *   new hurling snake, to do add collision space, clean up, comment
 *
 *         
 *                                                                 *
 ***************************************************************************/

#include "hurlingsnake.h"
#include "osgprimitive.h"

using namespace std;

namespace lpzrobots {

  /**
   * Constructor
   * @param w world in which robot should be created
   * @param s space in which robot should be created
   * @param c contactgroup for collision treatment
   */
  HurlingSnake::HurlingSnake(const OdeHandle& odeHandle, const OsgHandle& osgHandle, 
			     const std::string& name)
    : OdeRobot(odeHandle, osgHandle, name, "$Id$"), oldp(0,0,0){
    factorSensor=20.0;
    frictionGround=0.3;

    addParameterDef("factorForce", &factorForce,3.0, "factor of motor output");
    addParameterDef("factorSensor", &factorSensor,20.0, "factor of velocity as sensor value");
    addParameterDef("frictionGround", &frictionGround,0.3, "friction coefficient for ground");
    addParameterDef("frictionRoll", &frictionRoll,0.0, "friction coefficient for rolling");
    addParameterDef("place", &placedummy,0, "place the robot at 0,0");


    /* Parameter: 
       gravity= -0.5
       factorForce=3.0;
       factorSensor=20.0;
       frictionGround=0.3;
       InvertMotorNStep():
       controller->setParam("steps", 2);
       controller->setParam("adaptrate", 0.001);
       controller->setParam("nomupdate", 0.001);

       gravity= -1
       factorForce=5.0;
       factorSensor=20.0;
       frictionGround=0.3;
       InvertMotorNStep():
       controller->setParam("steps", 2);
       controller->setParam("adaptrate", 0.001);
       controller->setParam("nomupdate", 0.001);

       
       
    */
    created=false;

    this->osgHandle.color=Color(1,1,0);

    NUM= 10;		/* number of spheres */

    //    SIDE= 0.2;		/* side length of a box */
    MASS= 1.0;		/* mass of a sphere*/
    RADIUS= 0.1732f;	/* sphere radius */

    sensorno = 2;
    motorno  = 2;

  };

  HurlingSnake::~HurlingSnake(){
    destroy();
  }

 
  void HurlingSnake::update(){
    assert(created); // robot must exist
  
    for (int i=0; i<NUM; i++) { 
      objects[i]->update();
    }
    for (int i=0; i < NUM-1; i++) { 
      joints[i]->update();
    }
  }

  void HurlingSnake::place(const osg::Matrix& pose){
    // lift the snake about its radius
    osg::Matrix p2;
    p2 = pose * osg::Matrix::translate(osg::Vec3(0, 0, RADIUS)); 
    create(p2);    
  };


  void HurlingSnake::doInternalStuff(GlobalData& global){
    // decellerate
    for (int i=0; i<NUM; i++) { 
      objects[i]->decellerate(0,frictionRoll);
    }    
  }
  

  /** returns actual sensorvalues
      @param sensors sensors scaled to [-1,1] 
      @param sensornumber length of the sensor array
      @return number of actually written sensors
  */
  int HurlingSnake::getSensors(sensor* sensors, int sensornumber){
    int len = (sensornumber < sensorno)? sensornumber : sensorno;
  
    Pos p(objects[NUM-1]->getPosition());      //read actual position
    Pos s = (p - oldp)*factorSensor;
    
    sensors[0]=s.x();
    sensors[1]=s.y();
    oldp=p;
    return len;
  }


  /** sets actual motorcommands
      @param motors motors scaled to [-1,1] 
      @param motornumber length of the motor array
  */
  void HurlingSnake::setMotors(const motor* motors, int motornumber){    
    //  dBodyAddForce (objects[NUM-1].body,motors[0]*factorForce,motors[1]*factorForce,motors[2]*factorForce);
    // force vector in global frame of reference
    dBodyAddForce (objects[NUM-1]->getBody(),motors[0]*factorForce,motors[1]*factorForce,0);
    // force vector is applied relative to the body's own frame of reference
    //dBodyAddRelForce (objects[NUM-1]->getBody(),motors[0]*factorForce,motors[1]*factorForce,0);
  }


  /** returns number of sensors
   */
  int HurlingSnake::getSensorNumber(){
    return sensorno;
  }

  /** returns number of motors
   */
  int HurlingSnake::getMotorNumber(){
    return motorno;
  }


  /** returns a vector with the positions of all segments of the robot
      @param vector of positions (of all robot segments) 
      @return length of the list
  */
  int HurlingSnake::getSegmentsPosition(std::vector<Position> &poslist){
    Position pos;
    for (int i=0; i<NUM; i++){
      Pos p = objects[i]->getPosition();
      poslist.push_back(p.toPosition());
    }   
    return NUM;
  }



  void HurlingSnake::create(const osg::Matrix& pose){   
    if (created){
      destroy();
    }
    // create vehicle space and add it to parentspace
    odeHandle.createNewSimpleSpace(parentspace,false);
    odeHandle.substance.roughness=frictionGround;

    joints.resize(NUM-1);
    objects.resize(NUM);

    for (int i=0; i<NUM; i++) {
      objects[i] = new Sphere(RADIUS);      
      objects[i]->setTexture("Images/cross_stripes.rgb");	      
      //objects[i]->setTexture("Images/wood.rgb");	      
      if (i==NUM-1){
	OsgHandle osgHandle_head = osgHandle;
	osgHandle_head.color = Color(1, 0, 0);
	objects[i]->init(odeHandle, MASS, osgHandle_head);
      } else {
	objects[i]->init(odeHandle, MASS, osgHandle);
      }
      objects[i]->setPose(osg::Matrix::translate(i*RADIUS*2*1.1, 0, 0+0.03) * pose);      
    }
    oldp = objects[NUM-1]->getPosition();
    for (int i=0; i<(NUM-1); i++) {
      Pos p1(objects[i]->getPosition());
      Pos p2(objects[i+1]->getPosition());
      joints[i] = new BallJoint(objects[i],objects[i+1], (p1+p2)/2);
      joints[i]->init(odeHandle, osgHandle, true, RADIUS/10, false);
    }

    created=true;
  }


  /** destroys robot
   */
  void HurlingSnake::destroy(){
    if (created){
      cleanup();
      odeHandle.deleteSpace();
    }
    created=false;
  }


  void HurlingSnake::notifyOnChange(const paramkey& key){
    if(key == "frictionGround") {
      // change substances      
      for (int i=0; i<NUM; i++) {        
        objects[i]->substance.roughness=frictionGround;        
      }
    }
    else if(key == "place") {
      OdeRobot::place(Pos(0,0,3)) ; 
    }
  }


}
