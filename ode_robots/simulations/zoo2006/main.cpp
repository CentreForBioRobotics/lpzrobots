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
 *   Revision 1.1  2006-09-20 12:56:40  martius
 *   *** empty log message ***
 *
 *
 ***************************************************************************/
#include "simulation.h"

#include "odeagent.h"
#include "playground.h"
#include "terrainground.h"

#include "passivesphere.h"
#include "passivebox.h"
#include "passivecapsule.h"

#include <selforg/invertnchannelcontroller.h>
#include <selforg/invertmotornstep.h>
#include <selforg/invertmotorspace.h>
#include <selforg/sinecontroller.h>
#include <selforg/noisegenerator.h>
#include <selforg/one2onewiring.h>

#include <osg/Light>
#include <osg/LightSource>


#include "hurlingsnake.h"
#include "schlangeservo2.h"
#include "caterpillar.h"
#include "nimm2.h"
#include "nimm4.h"
#include "sphererobot3masses.h"

// fetch all the stuff of lpzrobots into scope
using namespace lpzrobots;
using namespace osg;

class ThisSim : public Simulation {
public:

  // starting function (executed once at the beginning of the simulation loop)
  void start(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global) 
  {
    setCameraHomePos(Pos(-19.15, 13.9, 6.9),  Pos(-126.1, -17.6, 0));

    // initialization
    // - set noise to 0.01
    global.odeConfig.setParam("noise",0.05);
    global.odeConfig.setParam("controlinterval",1);

    double height = 1.5;
    
    Playground* playground = 
      new Playground(odeHandle, osgHandle,osg::Vec3(20, 0.2, height+1.5));
    playground->setColor(Color(0.88f,0.4f,0.26f,0.2f));
    playground->setTexture("Images/really_white.rgb");
    playground->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
    global.obstacles.push_back(playground);
    
    TerrainGround* terrainground = 
      new TerrainGround(odeHandle, osgHandle.changeColor(Color(1.0f,194.0/255.0,41.0/255.0)),
			"terrains/zoo_landscape2.ppm",
			//"", 
			"Images/dusty.rgb", 
			// "terrains/terrain_bumpInDip128.ppm", 
			20, 20, height, OSGHeightField::Red);
    terrainground->setPose(osg::Matrix::translate(0, 0, 0.1));
    global.obstacles.push_back(terrainground);

    InvertMotorNStepConf invertnconf = InvertMotorNStep::getDefaultConf();


//     for(int i=0; i<5; i++){
//       PassiveSphere* s = 
// 	new PassiveSphere(odeHandle, 
// 			  osgHandle.changeColor(Color(184 / 255.0, 233 / 255.0, 237 / 255.0)), 0.2);
//       s->setPosition(Pos(i*0.5-2, i*0.5, height)); 
//       s->setTexture("Images/dusty.rgb");
//       global.obstacles.push_back(s);    
//     }

//     for(int i=0; i<5; i++){
//       PassiveBox* b = 
// 	new PassiveBox(odeHandle, 
// 			  osgHandle, osg::Vec3(0.2+i*0.1,0.2+i*0.1,0.2+i*0.1));
//       b->setPosition(Pos(i*0.5-5, i*0.5, height)); 
//       b->setColor(Color(1.0f,0.2f,0.2f,0.5f));
//       b->setTexture("Images/light_chess.rgb");
//       global.obstacles.push_back(b);    
//     }

//     for(int i=0; i<5; i++){
//       PassiveCapsule* c = 
// 	new PassiveCapsule(odeHandle, osgHandle, 0.2f, 0.3f, 0.3f);
//       c->setPosition(Pos(i-1, -i, height)); 
//       c->setColor(Color(0.2f,0.2f,1.0f,0.5f));
//       c->setTexture("Images/light_chess.rgb");
//       global.obstacles.push_back(c);    
//     }
        
    OdeAgent* agent;
    AbstractWiring* wiring;
    OdeRobot* robot;
    AbstractController *controller;
    
//     //******* R A U P E  *********/
//     CaterPillar* myCaterPillar;
//     CaterPillarConf myCaterPillarConf = DefaultCaterPillar::getDefaultConf();
//     myCaterPillarConf.segmNumber=3;
//     myCaterPillarConf.jointLimit=M_PI/3;
//     myCaterPillarConf.motorPower=0.2;
//     myCaterPillarConf.frictionGround=0.01;
//     myCaterPillarConf.frictionJoint=0.01;
//     myCaterPillar =
//       new CaterPillar ( odeHandle, osgHandle.changeColor(Color(1.0f,0.0,0.0)), 
// 			myCaterPillarConf, "Raupe");
//     ((OdeRobot*) myCaterPillar)->place(Pos(-5,-5,height)); 
    
//      invertnconf.cInit=2.0;
//      controller = new InvertMotorSpace(15);
//     wiring = new One2OneWiring(new ColorUniformNoise(0.1));
//     agent = new OdeAgent( plotoptions );
//     agent->init(controller, myCaterPillar, wiring);
//     global.agents.push_back(agent);
//     global.configs.push_back(controller);
//     global.configs.push_back(myCaterPillar);   
//     myCaterPillar->setParam("gamma",/*gb");
//     global.obstacles.push_back(s)0.0000*/ 0.0);
  

    //******* S C H L A N G E  (Long)  *********/
    SchlangeServo2* snake;
    SchlangeConf snakeConf = SchlangeServo2::getDefaultConf();
    snakeConf.segmNumber=4;
    snakeConf.frictionGround=0.01;

    snake = new SchlangeServo2 ( odeHandle, osgHandle, snakeConf, "SchlangeLong" );
    ((OdeRobot*) snake)->place(Pos(4,4,height)); 
    controller = new InvertMotorNStep(invertnconf);     
    wiring = new One2OneWiring(new ColorUniformNoise(0.1));
    agent = new OdeAgent( std::list<PlotOption>() );
    agent->init(controller, snake, wiring);
    global.agents.push_back(agent);
    global.configs.push_back(controller);
    global.configs.push_back(snake);   
 

//     //******* N I M M  2 *********/
//     Nimm2Conf nimm2conf = Nimm2::getDefaultConf();
//     nimm2conf.size = 1.6;
//     for(int r=0; r < 1; r++) { 
//       robot = new Nimm2(odeHandle, osgHandle, nimm2conf, "Nimm2_" + std::itos(r));
//       robot->place(Pos ((r-1)*5, 5, height));
//       //    controller = new InvertMotorNStep(10);   
//       controller = new InvertMotorSpace(15);   
//       controller->setParam("s4avg",10);
//       //    controller->setParam("factorB",0); // not needed here and it does some harm on the behaviour
//       wiring = new One2OneWiring(new ColorUniformNoise(0.1));
//       agent = new OdeAgent( std::list<PlotOption>() );
//       agent->init(controller, robot, wiring);
//       global.configs.push_back(controller);
//       global.agents.push_back(agent);        
//     }
    
    //******* N I M M  4 *********/
    for(int r=0; r < 2; r++) {
      robot = new Nimm4(odeHandle, osgHandle, "Nimm4_" + std::itos(r));
      robot->place(Pos((r-1)*5, -3, height));
      controller = new InvertMotorSpace(20);
      controller->setParam("s4avg",10); 
      controller->setParam("factorB",0); // not needed here and it does some harm on the behaviour
      wiring = new One2OneWiring(new ColorUniformNoise(0.1));
      agent = new OdeAgent( std::list<PlotOption>() );
      agent->init(controller, robot, wiring);
      global.agents.push_back(agent);        
    }

    //****** H U R L I N G **********/
    InvertMotorNStepConf invertnconf2 = InvertMotorNStep::getDefaultConf();
    for(int r=0; r < 3; r++) {
      HurlingSnake* snake;
      Color c;    
      if (r==0) c=Color(0.8, 0.8, 0);
      if (r==1) c=Color(0,   0.8, 0);
      snake = new HurlingSnake(odeHandle, osgHandle.changeColor(c), "HurlingSnake_" + std::itos(r));
      ((OdeRobot*) snake)->place(Pos(r*5-4,-6, height));
      invertnconf2.cInit=1.5;
      controller = new InvertMotorNStep(invertnconf2);
      controller->setParam("steps", 2);
      controller->setParam("epsA", 0.15);
      controller->setParam("epsC", 0.04);
      controller->setParam("adaptrate",  0.000); //0.001);
      controller->setParam("nomupdate",  0.000); //0.001);
      controller->setParam("factorB", 0);
    
      // deriveconf = DerivativeWiring::getDefaultConf();
      //     deriveconf.blindMotorSets=0;
      //     deriveconf.useId = true;
      //     deriveconf.useFirstD = true;
      //     deriveconf.derivativeScale = 50;
      //     wiring = new DerivativeWiring(deriveconf, new ColorUniformNoise(0.1));
      wiring = new One2OneWiring(new ColorUniformNoise(0.05));
      agent = new OdeAgent( plotoptions );
      agent->init(controller, snake, wiring);
			       global.configs.push_back(controller);
			       global.agents.push_back(agent);     
    }

    //****** S P H E R E **********/
    Sphererobot3MassesConf conf = Sphererobot3Masses::getDefaultConf();  
    Sphererobot3Masses* sphere1 = 
      new Sphererobot3Masses ( odeHandle, osgHandle.changeColor(Color(1.0,0.0,0)), 
				       conf, "Sphere1", 0.2); 
    ((OdeRobot*)sphere1)->place ( Pos( 0 , 0 , height ));
    controller = new InvertMotorSpace(15);
    controller->setParam("sinerate", 40);  
    controller->setParam("phaseshift", 0.0);
    One2OneWiring* wiring2 = new One2OneWiring ( new ColorUniformNoise() );
    agent = new OdeAgent ( std::list<PlotOption>() );
    agent->init ( controller , sphere1 , wiring2 );
    global.agents.push_back ( agent );
    global.configs.push_back ( controller );

    
    showParams(global.configs);
  }
  
  // add own key handling stuff here, just insert some case values
  virtual bool command(const OdeHandle&, const OsgHandle&, GlobalData& globalData, int key, bool down)
  {
    if (down) { // only when key is pressed, not when released
      switch ( (char) key )
	{
	default:
	  return false;
	  break;
	}
    }
    return false;
  }

  virtual osg::LightSource* makeLights(osg::StateSet* stateset)
  {
    // create a spot light.
    Light* light_0 = new Light;
    light_0->setLightNum(0);
    light_0->setPosition(Vec4(40.0f, 40.0f, 50.0f, 1.0f));
    // note that the blue component doesn't work!!! (bug in OSG???) 
    //   Really? It works with me! (Georg)
    light_0->setAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.0f)); 
    light_0->setDiffuse(Vec4(0.8f, 0.8f, 0.8f, 1.0f)); 
    //    light_0->setDirection(Vec3(-1.0f, -1.0f, -1.2f)); 
    light_0->setSpecular(Vec4(1.0f, 0.9f, 0.8f, 1.0f)); 

    LightSource* light_source_0 = new LightSource;	
    light_source_0->setLight(light_0);
    light_source_0->setLocalStateSetModes(StateAttribute::ON);   
    light_source_0->setStateSetModes(*stateset, StateAttribute::ON);
  
    return light_source_0;
  }

  
};

int main (int argc, char **argv)
{ 
  ThisSim sim;
  // run simulation
  return sim.run(argc, argv) ? 0 : 1;
}
 
