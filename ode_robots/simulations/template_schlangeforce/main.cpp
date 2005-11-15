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
 *   Revision 1.18.4.1  2005-11-15 12:30:11  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.18  2005/11/09 14:27:45  fhesse
 *   schlangeforce active, schlangeservo commented out
 *
 *   Revision 1.17  2005/11/09 13:41:25  martius
 *   GPL'ised
 *
 ***************************************************************************/
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
#include <selforg/sinecontroller.h>

#include "schlangeservo.h"
#include "schlangeforce.h"
#include "nimm2.h"

ConfigList configs;
list<PlotOption> plotoptions;

//Startfunktion die am Anfang der Simulationsschleife, einmal ausgefuehrt wird
void start(const OdeHandle& odeHandle, GlobalData& global) 
{
  dsPrint ( "\nWelcome to the virtual ODE - robot simulator of the Robot Group Leipzig\n" );
  dsPrint ( "------------------------------------------------------------------------\n" );
  dsPrint ( "Press Ctrl-C for an basic commandline interface.\n\n" );

  global.odeConfig.setParam("gravity",-9.0); // do not use 'global.odeConfig.gravity=0.0;', 
                                            // because world is already initialized and 
                                            // dWorldSetGravity will not be called when 
                                            // you only set the value  

  //Anfangskameraposition und Punkt auf den die Kamera blickt
  //float KameraXYZ[3]= {0.276f,7.12f,1.78f};
  //float KameraViewXYZ[3] = {-88.0f,-5.5f,0.0000f};
  float KameraXYZ[3]= {5.0f, 7.0f, 7.0f};
  float KameraViewXYZ[3] = {-85.0f,-12.0f,0.0000f};

  dsSetViewpoint ( KameraXYZ , KameraViewXYZ );
  dsSetSphereQuality (2); //Qualitaet in der Sphaeren gezeichnet werden

  // initialization
  

  Playground* playground = new Playground(odeHandle);
  playground->setGeometry(15.0, 0.2, 1.5);
  playground->setPosition(0,0,0); // playground positionieren und generieren
  global.obstacles.push_back(playground);

  Sphere* sphere;
  for (int i=-3; i<3; i++){
    sphere = new Sphere(odeHandle);
    sphere->setPosition(i*0.5,i*0.5,1.0); //positionieren und generieren
    global.obstacles.push_back(sphere);
  }

  


  SchlangenConf con = SchlangeForce::getDefaultConf();
  SchlangeForce*schlange3 = new SchlangeForce(0, odeHandle, con);
  Position p3(1,1,0);
  Color col3 (1,1,0);
  schlange3->place(p3,&col3);
  AbstractController *controller3 = new InvertNChannelController(10,true);  
  
  One2OneWiring* wiring3 = new One2OneWiring(new ColorUniformNoise(0.1));
  OdeAgent* agent3 = new OdeAgent(NoPlot/*GuiLogger*/);
  agent3->init(controller3, schlange3, wiring3);
  global.agents.push_back(agent3);
  configs.push_back(controller3);
  configs.push_back(schlange3);



  /* comment this in to add a SchlangeServo to the simulation*/
  /*
  SchlangeServoConf conf = SchlangeServo::getDefaultConf();
  conf.servoPower=10;
  conf.jointLimit=M_PI/2;
  SchlangeServo* schlange1 = 
    new SchlangeServo ( odeHandle, conf, "S1");
  Color col(0,0.5,0.8);
  schlange1->place(Position(2,0,6),&col); 


  //schlange1->fixInSky();
  AbstractController *controller = new SineController();  
  
  // AbstractWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.1));
  DerivativeWiringConf c = DerivativeWiring::getDefaultConf();
  c.useId=true;
  c.useFirstD=true;
  // c.useSecondD=true;
  c.derivativeScale=10;
  AbstractWiring* wiring = new DerivativeWiring(c, new ColorUniformNoise(0.1));
  OdeAgent* agent = new OdeAgent(plotoptions);
  agent->init(controller, schlange1, wiring);
  global.agents.push_back(agent);
  configs.push_back(controller);
  configs.push_back(schlange1);
  
 
  global.odeConfig.setParam("noise",0.1);
  global.odeConfig.setParam("simstepsize",0.005);
  global.odeConfig.setParam("drawinterval",1);
  global.odeConfig.setParam("controlinterval",1);

   controller->setParam("epsC",0.001);
   controller->setParam("epsA",0.01);

  schlange1->setParam("gamma", 0.0);
  schlange1->setParam("frictionGround",0.1);
  schlange1->setParam("factorForce", 3);
  schlange1->setParam("factorSensors", 5);
  */
  
  
  
  showParams(configs);
}

void end(GlobalData& global){
  for(ObstacleList::iterator i=global.obstacles.begin(); i != global.obstacles.end(); i++){
    delete (*i);
  }
  global.obstacles.clear();
  for(OdeAgentList::iterator i=global.agents.begin(); i != global.agents.end(); i++){
    delete (*i)->getRobot();
    delete (*i)->getController();
    delete (*i);
  }
  global.agents.clear();
}


// this function is called if the user pressed Ctrl-C
void config(GlobalData& global){
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
 
