
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
 *   Revision 1.6  2006-09-21 22:10:46  martius
 *   make opt fixed
 *
 *   Revision 1.5  2006/09/21 16:15:58  der
 *   *** empty log message ***
 *
 *   Revision 1.4  2006/09/11 12:01:31  martius
 *   *** empty log message ***
 *
 *   Revision 1.3  2006/07/14 14:36:43  martius
 *   cleanall target
 *   no unnecessary rebuild
 *
 *   Revision 1.2  2006/07/14 12:23:49  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.1  2006/07/13 13:18:20  der
 *   simulation with multiple snakes
 *
 *   Revision 1.2.4.3  2006/02/24 14:43:51  martius
 *   keys
 *
 *   Revision 1.2.4.2  2005/12/29 16:44:54  martius
 *   moved to osg
 *
 *   Revision 1.2.4.1  2005/11/15 12:29:59  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.2  2005/11/09 13:41:25  martius
 *   GPL'ised
 *
 ***************************************************************************/

#include <selforg/noisegenerator.h>

#include "simulation.h"
#include "odeagent.h"
#include <selforg/one2onewiring.h>
#include <selforg/derivativewiring.h>
#include "playground.h"
#include "terrainground.h"
#include "octaplayground.h"
#include "passivesphere.h"
#include "passivebox.h"

//#include <selforg/deprivation.h>
#include <selforg/invertnchannelcontroller.h>
//#include <selforg/invertmotorspace.h>
#include <selforg/invertmotornstep.h>
#include <selforg/sinecontroller.h>
#include <selforg/dercontroller.h>

#include "sliderwheelie.h"
#include "schlangeservo.h"
#include "schlangeservo2.h"
#include "sphererobot3masses.h"
#include "plattfussschlange.h"
#include "hurlingsnake.h"
//#include "forcedsphere.h"
#include "nimm2.h"


using namespace lpzrobots;

class ThisSim : public Simulation {
public:
	

  /// start() is called at the start and should create all the object (obstacles, agents...).
  virtual void start(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global){
    setCameraHomePos(Pos(15.2297, -16.7329, 11.8299),  Pos(45.3984, -29.7745, 0));

    global.odeConfig.setParam("controlinterval",2);
    global.odeConfig.setParam("gravity", -2.); 

    /*     Playground* playground = new Playground(odeHandle, osgHandle, 
	   osg::Vec3(120, 0.2, 5.5),0.9);
	   playground->setColor(Color(1,0.2,0,0.1));
	   playground->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
	   global.obstacles.push_back(playground);
    */
    /////////Neuer Playground klein innen 
    //    double diam=1.2; //internaldiameter=.9*diam, offset=1.0*internaldiameter; 

    //   OctaPlayground* playground2 = new OctaPlayground(odeHandle, osgHandle, osg::Vec3(/*Diameter.8*/1*diam, 0.2,/*Height*/ 4), 12,false);
    //      playground2->setTexture("Images/whitemetal_farbig.rgb");
    //       playground2->setColor(Color(0.4,0.4,0.4,0.2));
    //       playground2->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
    //      global.obstacles.push_back(playground2);



    //   OctaPlayground* playground = new OctaPlayground(odeHandle, osgHandle, osg::Vec3(/*Diameter=*/0.8*internaldiameter, 0.7, /*Height=*/1), 12,false);
    //     playground->setColor(Color(1,0.2,0,0.9));
    //     playground->setPosition(osg::Vec3(offset,0,0)); // playground positionieren und generieren
    //      global.obstacles.push_back(playground);

    // OctaPlayground* playground3 = new OctaPlayground(odeHandle, osgHandle, osg::Vec3(/*Diameter=*/0.8*internaldiameter, 0.7, /*Height=*/.5), 12,false );
    //     playground3->setColor(Color(1,0.2,0,0.9));
    //     playground3->setPosition(osg::Vec3(-offset,0,0)); // playground positionieren und generieren
    //      global.obstacles.push_back(playground3);
    //     controller->setParam("noiseB",0.0);

  

    // OctaPlayground* playground3 = new OctaPlayground(odeHandle, osgHandle, osg::Vec3(/*Diameter*/4*diam, 5*diam,/*Height*/ 2), 12,false);
    // playground3->setColor(Color(.0,0.2,1.0,0.1));
    //playground3->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
    //global.obstacles.push_back(playground2);


    //      OctaPlayground* playground4 = new OctaPlayground(odeHandle, osgHandle, osg::Vec3(/*Diameter*/10.5  *diam,.1,/*Height*/ 4), 12,true); //false heisst ohne Schatten 
    //  // playground4->setTexture("Images/really_white.rgb");
    //  playground4->setColor(Color(.2,.2,.2,0.2));
    //  playground4->setGroundTexture("Images/really_white.rgb");
    //  playground4->setGroundColor(Color(255.0f/255.0f,200.0f/255.0f,21.0f/255.0f));
    //  playground4->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
    //     global.obstacles.push_back(playground4);

    /////////Neuer Playground Ende

    Playground* playground = new Playground(odeHandle, osgHandle, 
					    osg::Vec3(20, 0.2, 8.5),1,true);
    playground->setColor(Color(1,0.2,0,0.1));
    playground->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
    global.obstacles.push_back(playground);
    

    double height = 0.3; //1.5;
    TerrainGround* terrainground = 
      new TerrainGround(odeHandle, osgHandle.changeColor(Color(1.0f,194.0/255.0,41.0/255.0)),
			"terrains/zoo_landscape1.ppm",
			""/*"Images/dusty.rgb" "terrains/macrospheresTex_256.ppm"*/, 
			20, 20, height, OSGHeightField::Red);
    terrainground->setPose(osg::Matrix::translate(0, 0, 0.1));
    global.obstacles.push_back(terrainground);

    height += 0.1;

    // Creation of passive boxes
    //****** PASSIVE BOXES **********/
    for(int i=0; i<1; i++){
      PassiveBox* b = 
	new  PassiveBox(odeHandle, 
			osgHandle.changeColor(Color(184 / 255.0, 233 / 255.0, 237 / 255.0)), 
			osg::Vec3(1.0, 1.0, 1.0), 3);
      b->setPosition(Pos(i*0.3-2, i*0.5-2, height)); 
      b->setTexture("Images/dusty.rgb");
      global.obstacles.push_back(b);    
    }
    // Creation of passive spheres
    //****** PASSIVE SPHERES **********/
    for(int i=0; i<3; i++){
      PassiveSphere* s = 
	new PassiveSphere(odeHandle, 
			  osgHandle.changeColor(Color(184 / 255.0, 233 / 255.0, 237 / 255.0)),
			  /*Diameter=*/.4+i*0.1, /*Mass=*/.8);
      s->setPosition(Pos(i-2, -i+2, height)); 
      s->setTexture("Images/dusty.rgb");
      global.obstacles.push_back(s);    
    }

    // Creation of spherical robots: 
    //****** SPHERICALS IR **********/
    for(int i=0; i<0; i++){
      OdeRobot* sphere1;
      //Sphererobot3MassesConf conf = Sphererobot3Masses::getDefaultConf(); 
      Sphererobot3MassesConf conf = Sphererobot3Masses::getDefaultConf();  
      conf.diameter=1.4;
      conf.spheremass = 2; 
      conf.axisZsensor=true;
      conf.axisXYZsensor=false;
      conf.irAxis1=true;
      conf.irAxis2=true;
      conf.irAxis3=true;
      conf.pendularrange=0.35;
      conf.irCharacter=0.5;
      sphere1 = new Sphererobot3Masses ( odeHandle, osgHandle.changeColor(Color(1.0,0.0,0.0)), 
       					 conf, "Sphere1", 0.5); 
//      sphere1 = new ForcedSphere(odeHandle, osgHandle.changeColor(Color(1.0,0.0,0.0))
//				 , ForcedSphere::getDefaultConf(), "FSphere");
    
      // sphere1->place ( Pos(-3,1/2,3+2*i));
      sphere1->place ( Pos(5, i*2, height));
      AbstractController* controller = new DerController();
      //AbstractController* controller = new InvertMotorNStep();
      //      controller->setParam("steps", 2);    
      controller->setParam("steps", 1);    
      controller->setParam("adaptrate", 0.0);  
      controller->setParam("nomupdate", 0.0);    
      controller->setParam("epsC", 0.1);    
      controller->setParam("epsA", 0.1);    
      controller->setParam("rootE", 3);     
      controller->setParam("factorB", 0.0); 
      controller->setParam("teacher", 0.0);  
      
      DerivativeWiringConf c = DerivativeWiring::getDefaultConf();
      c.useId = false;
      c.useFirstD = true;
      DerivativeWiring* wiring = new DerivativeWiring ( c , new ColorUniformNoise() );
      
      
      // One2OneWiring* wiring = new One2OneWiring ( new ColorUniformNoise() );
      OdeAgent* agent = new OdeAgent ( plotoptions );
      agent->init ( controller , sphere1 , wiring );
      //  agent->setTrackOptions(TrackRobot(true, false, false, "ZSens_Ring10_11", 50));
      global.agents.push_back ( agent );
      global.configs.push_back ( controller );    
    }

    // Creation of spherical robots: 
    //****** SPHERICALS XYZ **********/
    for(int i=0; i<2; i++){
      OdeRobot* sphere1;
      //Sphererobot3MassesConf conf = Sphererobot3Masses::getDefaultConf(); 
      Sphererobot3MassesConf conf = Sphererobot3Masses::getDefaultConf();  
      conf.diameter=1.4;
      conf.spheremass = 2; 
      conf.axisZsensor=false;
      conf.axisXYZsensor=true;
      conf.pendularrange=0.35;
      sphere1 = new Sphererobot3Masses ( odeHandle, osgHandle.changeColor(Color(i==1, 0.0, i==0)), 
       					 conf, "SphereXYZ", 0.4); 
    
      // sphere1->place ( Pos(-3,1/2,3+2*i));
      if(i==0){
	sphere1->place ( Pos(0, -8, height));
      }else{
	sphere1->place ( Pos(-i*2-4, 5, height));
      }
      //AbstractController* controller = new DerController();
      AbstractController* controller = new InvertMotorNStep();
      controller->setParam("steps", 2);    
      // controller->setParam("steps", 1);    
      controller->setParam("adaptrate", 0.0);  
      controller->setParam("nomupdate", 0.0);    
      controller->setParam("epsC", 0.1);    
      controller->setParam("epsA", 0.1);    
      controller->setParam("rootE", 3);     
      controller->setParam("factorB", 0.0); 
      controller->setParam("teacher", 0.0);  
            
      One2OneWiring* wiring = new One2OneWiring ( new ColorUniformNoise() );
      OdeAgent* agent = new OdeAgent ( plotoptions );
      agent->init ( controller , sphere1 , wiring );
      global.agents.push_back ( agent );
      global.configs.push_back ( controller );    
    }


    //creation of flatfoot  snakes 
    //****** PLATTFUSS **********/
    for(int i=0; i<2; i++){

      //****************/
      SchlangeConf conf = Schlange::getDefaultConf();
      conf.segmMass   = .2;
      conf.segmLength=.4;
      conf.segmDia=.1;
      conf.motorPower=.3;
      conf.segmNumber = 5+2*i;//-i/2; 
      // conf.jointLimit=conf.jointLimit*3;
      conf.jointLimit=conf.jointLimit*2.0;
      conf.frictionGround=0.2;// +((double)i)/100;
      conf.frictionJoint=0.1;
      PlattfussSchlange* schlange1; 
      if (i==0) {
	schlange1 = 
	  //new SchlangeServo2 ( odeHandle, osgHandle.changeColor(Color(0.8, 0.3, 0.5)),
	  new PlattfussSchlange ( odeHandle, osgHandle.changeColor(Color(1.0, 1.0, 1.0)),
				  conf, "Plattfuss 1");
      } else {
	schlange1 = 
	  //new SchlangeServo2 ( odeHandle, osgHandle.changeColor(Color(0.8, 0.3, 0.5)),
	  new PlattfussSchlange ( odeHandle, osgHandle.changeColor(Color(0.8, 0.4, .3)),
				  conf, "Plattfuss 2");
      }
      //Positionieren und rotieren 
      schlange1->place(// osg::Matrix::rotate(M_PI/2, 0, 1, 0)*
		       // osg::Matrix::translate(-.7+0.7*i,0,(i+1)*(.2+conf.segmNumber)/2.0/*+2*/));
		       osg::Matrix::translate(-7,(i*2),height+1));
      schlange1->setTexture("Images/whitemetal_farbig_small.rgb");
      if (i==0) {
	schlange1->setHeadColor(Color(1.0,0,0));
      } else {
	schlange1->setHeadColor(Color(0,1.0,0));
      }
 

      //AbstractController *controller = new InvertNChannelController(100/*,true*/);  
      //      AbstractController *controller = new InvertMotorNStep(); 
      AbstractController *controller = new DerController(); 
      //AbstractController *controller = new SineController();  
  
      //  AbstractWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.05)); //Only this line for one2Onewiring
      DerivativeWiringConf c = DerivativeWiring::getDefaultConf();
      c.useId = false;
      c.useFirstD = true;
      DerivativeWiring* wiring = new DerivativeWiring ( c , new ColorUniformNoise(0.1) );
      
      OdeAgent* agent = new OdeAgent(plotoptions);
      agent->init(controller, schlange1, wiring);
      global.agents.push_back(agent);
      global.configs.push_back(controller);
      global.configs.push_back(schlange1);
   
      controller->setParam("steps",1);
      controller->setParam("epsC",0.1);
      controller->setParam("epsA",0.1);
      controller->setParam("adaptrate",0.0);//0.005);
      controller->setParam("rootE",3);
      controller->setParam("logaE",0);

      // controller->setParam("desens",0.0);
      controller->setParam("s4delay",1.0);
      controller->setParam("s4avg",1.0);
    
      controller->setParam("factorB",0.0); 
      controller->setParam("noiseB",0.0);

      controller->setParam("frictionjoint",0.01);
      controller->setParam("teacher", 0.0); 
    
    }//creation of snakes End


    //****** SNAKES **********/
    //creation of normal   snakes 
    for(int i=0; i<1; i++){

      //****************/
      SchlangeConf conf = Schlange::getDefaultConf();
      conf.segmMass   = .2;
      conf.segmLength=.8;
      conf.segmDia=.2;
      conf.motorPower=.5;
      conf.segmNumber = 10+2*i;//-i/2; 
      // conf.jointLimit=conf.jointLimit*3;
      conf.jointLimit=conf.jointLimit*2.0;
      conf.frictionGround=0.2;// +((double)i)/100;
      conf.frictionJoint=0.1;
      //PlattfussSchlange* schlange1; 
      SchlangeServo2* schlange1; 
      if (i==0) {
	schlange1 = 
	  new SchlangeServo2 ( odeHandle, osgHandle.changeColor(Color(0.8, 0.3, 0.5)),
			       //  new PlattfussSchlange ( odeHandle, osgHandle.changeColor(Color(1.0, 1.0, 1.0)),
			       conf, "S1");
      } else {
	schlange1 = 
	  new SchlangeServo2 ( odeHandle, osgHandle.changeColor(Color(0.8, 0.3, 0.5)),
			       // new PlattfussSchlange ( odeHandle, osgHandle.changeColor(Color(0.8, 0.4, .3)),
			       conf, "S2");
      }
      //Positionieren und rotieren 
      schlange1->place(// osg::Matrix::rotate(M_PI/2, 0, 1, 0)*
		       // osg::Matrix::translate(-.7+0.7*i,0,(i+1)*(.2+conf.segmNumber)/2.0/*+2*/));
		       osg::Matrix::translate(5-i,2 + i*2,height));
      schlange1->setTexture("Images/whitemetal_farbig_small.rgb");
      if (i==0) {
	schlange1->setHeadColor(Color(1.0,0,0));
      } else {
	schlange1->setHeadColor(Color(0,1.0,0));
      }
 

      //      AbstractController *controller = new InvertMotorNStep(); 
      AbstractController *controller = new DerController(); 
      //AbstractController *controller = new SineController();  
  
      //  AbstractWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.05)); //Only this line for one2Onewiring
      DerivativeWiringConf c = DerivativeWiring::getDefaultConf();
      c.useId = false;
      c.useFirstD = true;
      DerivativeWiring* wiring = new DerivativeWiring ( c , new ColorUniformNoise() );
      
      OdeAgent* agent = new OdeAgent(plotoptions);
      agent->init(controller, schlange1, wiring);
      global.agents.push_back(agent);
      global.configs.push_back(controller);
      global.configs.push_back(schlange1);
  
 
      controller->setParam("steps",1);
      controller->setParam("epsC",0.1);
      controller->setParam("epsA",0.1);
      controller->setParam("adaptrate",0.0);//0.005);
      controller->setParam("rootE",3);
      controller->setParam("logaE",0);

      // controller->setParam("desens",0.0);
      controller->setParam("s4delay",1.0);
      controller->setParam("s4avg",1.0);
    
      controller->setParam("factorB",0.0); 
      controller->setParam("noiseB",0.0);

      controller->setParam("frictionjoint",0.01);
      controller->setParam("teacher", 0.0); 
    
    }//creation of snakes End


    //****** H U R L I N G **********/
    for(int r=0; r < 2; r++) {
      HurlingSnake* snake;
      Color c;    
      if (r==0) c=Color(0.8, 0.8, 0);
      if (r==1) c=Color(0,   0.8, 0);
      snake = new HurlingSnake(odeHandle, osgHandle.changeColor(c), "HurlingSnake_" + std::itos(r));
      ((OdeRobot*) snake)->place(Pos(r*3+3,r*2-5, height));
      snake->setParam("factorForce",12);
      snake->setParam("frictionGround",0.2);

      InvertMotorNStepConf invertnconf2 = InvertMotorNStep::getDefaultConf();
      invertnconf2.cInit=1.2;
      AbstractController *controller = new InvertMotorNStep(invertnconf2);
      controller->setParam("steps", 2);
      controller->setParam("epsA", 0.15);
      controller->setParam("epsC", 0.04);
      controller->setParam("adaptrate",  0.000); //0.001);
      controller->setParam("nomupdate",  0.000); //0.001);
      controller->setParam("factorB", 0);
	
      AbstractWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.05));
      OdeAgent* agent = new OdeAgent( plotoptions );
      agent->init(controller, snake, wiring);
      global.configs.push_back(controller);
      global.agents.push_back(agent);     
    }


    //****** CIGARS **********/
    for(int i=0; i < 2; i++) {
      
      Nimm2Conf conf = Nimm2::getDefaultConf();
      conf.speed=10;
      conf.wheelSize=1.2;
      conf.force=1.0;
      conf.bumper=true;
      conf.cigarMode=true;

      OdeRobot* nimm2 = new Nimm2(odeHandle, osgHandle, conf, "Cigar" + std::itos(i));
      //      nimm2->setColor(Color(1.0,1.0,0));
      
      //Positionieren und rotieren 
      nimm2->place( osg::Matrix::translate(i*1-4 , i*2-6, height));

      AbstractController* controller = new InvertMotorNStep();  
      //	  controller = new InvertMotorSpace(10);  
      controller->setParam("adaptrate", 0.000);
      //    controller->setParam("nomupdate", 0.0005);
      controller->setParam("epsC", 0.05);
      controller->setParam("epsA", 0.01);
      controller->setParam("epsC", 0.05);
      controller->setParam("rootE", 0);
      controller->setParam("steps", 2);
      controller->setParam("s4avg", 5);
      controller->setParam("s4del", 5);


      AbstractWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.1));
      OdeAgent* agent = new OdeAgent( plotoptions );
      agent->init(controller, nimm2, wiring);
      global.configs.push_back(controller);
      global.agents.push_back(agent);     
    }

    //****** CIGARS **********/
    for(int i=0; i < 1; i++) {      
      SliderWheelieConf mySliderWheelieConf = SliderWheelie::getDefaultConf();
      /******* S L I D E R - w H E E L I E *********/
      mySliderWheelieConf.segmNumber=8;
      mySliderWheelieConf.jointLimit=M_PI/4;
      mySliderWheelieConf.motorPower=0.4;
      mySliderWheelieConf.frictionGround=0.8;
      mySliderWheelieConf.sliderLength=1;
      mySliderWheelieConf.segmLength=0.4;
      OdeRobot* robot = new SliderWheelie(odeHandle, osgHandle, 
					  mySliderWheelieConf, "sliderWheelie1");
      robot->place(Pos(3,-3, height)); 
      InvertMotorNStepConf sliderinvertnconf = InvertMotorNStep::getDefaultConf();
      sliderinvertnconf.cInit=1;
      AbstractController* controller = new InvertMotorNStep(sliderinvertnconf);    
      //slidercontroller = new SineController();
      controller->setParam("steps",2);
      controller->setParam("factorB",0);
      
      DerivativeWiringConf c = DerivativeWiring::getDefaultConf();
      c.useId = false;
      c.useFirstD = true;
      AbstractWiring* wiring = new DerivativeWiring ( c , new ColorUniformNoise(0.1) );
      //     sliderwiring = new One2OneWiring(new ColorUniformNoise(0.1));
      OdeAgent* agent = new OdeAgent(plotoptions);
      agent->init(controller, robot, wiring);
      global.agents.push_back(agent);
      global.configs.push_back(controller);
      global.configs.push_back(robot);        
    }
    
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


};

int main (int argc, char **argv)
{  
  ThisSim sim;
  return sim.run(argc, argv) ? 0 : 1;
}


 
  
