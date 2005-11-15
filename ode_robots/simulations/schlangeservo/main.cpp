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
 *   Revision 1.2.4.1  2005-11-15 12:29:59  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.2  2005/11/09 13:41:25  martius
 *   GPL'ised
 *
 ***************************************************************************/
// 1127216427

#include <stdio.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>

#include <selforg/noisegenerator.h>

#include "simulation.h"
#include "odeagent.h"
#include <selforg/one2onewiring.h>
#include <selforg/derivativewiring.h>
#include "playground.h"
#include "sphere.h"

#include <selforg/invertnchannelcontroller.h>
#include <selforg/invertmotorspace.h>
#include <selforg/invertmotornstep.h>
#include <selforg/sinecontroller.h>

#include "schlangeservo.h"


ConfigList configs;
list<PlotOption> plotoptions;

// Funktion die die Steuerung des Roboters uebernimmt
bool StepRobot()
{
  for(OdeAgentList::iterator i=agents.begin(); i != agents.end(); i++){
    (*i)->step(simulationConfig.noise);
  }
  return true;
}

//Startfunktion die am Anfang der Simulationsschleife, einmal ausgefuehrt wird
void start() 
{
  dsPrint ( "\nWelcome to the virtual ODE - robot simulator of the Robot Group Leipzig\n" );
  dsPrint ( "------------------------------------------------------------------------\n" );
  dsPrint ( "Press Ctrl-C for an basic commandline interface.\n\n" );


  //Anfangskameraposition und Punkt auf den die Kamera blickt
  float KameraXYZ[3]= {1.64f, 3.61f, 13.2f};
  float KameraViewXYZ[3] = {-90.0f,-60.6f,0.0000f};

  dsSetViewpoint ( KameraXYZ , KameraViewXYZ );
  dsSetSphereQuality (2); //Qualitaet in der Sphaeren gezeichnet werden

  // initialization
  configs.push_back(&simulationConfig);

  // create playground
//   Playground* playground = new Playground(world, space);
//   playground->setGeometry(15.0, 0.2, 1.5);
//   playground->setPosition(0,0,0); // playground positionieren und generieren
//   obstacles.push_back(playground);

  // create spheres
//   Sphere* sphere;
//   for (int i=-3; i<3; i++){
//     sphere = new Sphere(world, space);
//     sphere->setPosition(i*0.5,i*0.5,1.0); //positionieren und generieren
//     obstacles.push_back(sphere);
//   }


  //****************/
  SchlangeServoConf conf = SchlangeServo::getDefaultConf();
  conf.servoPower=0.3;
  SchlangeServo* schlange1 = 
    new SchlangeServo ( ODEHandle(world , space , contactgroup), conf, "S1");
  Color col(0,0.5,0.8);
  schlange1->place(Position(2,0,6),&col); 

  //AbstractController *controller = new InvertNChannelController(100/*,true*/);  
  //  AbstractController *controller = new InvertMotorSpace(100/*,true*/);  
  AbstractController *controller = new InvertMotorNStep(50);  
  //AbstractController *controller = new SineController();  
  
   AbstractWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.1));
//   DerivativeWiringConf c = DerivativeWiring::getDefaultConf();
//   c.useId=true;
//   c.useFirstD=true;
//   // c.useSecondD=true;
//   c.derivativeScale=10;
//   AbstractWiring* wiring = new DerivativeWiring(c, new ColorUniformNoise(0.1));
  OdeAgent* agent = new OdeAgent(plotoptions);
  agent->init(controller, schlange1, wiring);
  agents.push_back(agent);
  configs.push_back(controller);
  configs.push_back(schlange1);
  
 
  simulationConfig.setParam("noise",0.1);
  simulationConfig.setParam("simstepsize",0.005);
  simulationConfig.setParam("drawinterval",1);
  simulationConfig.setParam("controlinterval",1);
  simulationConfig.setParam("gravity",0.0); // do not use 'simulationConfig.gravity=0.0;', 
                                            // because world is already initialized and 
                                            // dWorldSetGravity will not be called when 
                                            // you only set the value  


   controller->setParam("epsC",0.001);
//   // controller->setParam("desens",0.0);
//   controller->setParam("s4delay",1.0);
//   controller->setParam("s4avg",1.0);
   controller->setParam("epsA",0.01);
//   controller->setParam("factorB",0.0);
//   controller->setParam("zetaupdate",0.1);

  schlange1->setParam("gamma",/*0.0000*/ 0.0);
  schlange1->setParam("frictionGround",0.1);
  schlange1->setParam("factorForce", /*0.0005*/3);
  schlange1->setParam("factorSensors", /*20.0 */5);
  

  showParams(configs);
}

void end(){
  for(ObstacleList::iterator i=obstacles.begin(); i != obstacles.end(); i++){
    delete (*i);
  }
  obstacles.clear();
  for(OdeAgentList::iterator i=agents.begin(); i != agents.end(); i++){
    delete (*i)->getRobot();
    delete (*i)->getController();
    delete (*i);
  }
  agents.clear();
}


// this function is called if the user pressed Ctrl-C
void config(){
  changeParams(configs);
}

void printUsage(const char* progname){
  printf("Usage: %s [-g] [-l]\n\t-g\tuse guilogger\n\t-l\tuse guilogger with logfile", progname);
}

int main (int argc, char **argv)
{  
  if(contains(argv, argc, "-g")) plotoptions.push_back(PlotOption(GuiLogger));
  if(contains(argv, argc, "-l")) plotoptions.push_back(PlotOption(GuiLogger_File));
  if(contains(argv, argc, "-h")) printUsage(argv[0]);

  // initialise the simulation and provide the start, end, and config-function
  simulation_init(&start, &end, &config);
  // start the simulation (returns, if the user closes the simulation)
  simulation_start(argc, argv);
  simulation_close();  // tidy up.
  return 0;
}
 
