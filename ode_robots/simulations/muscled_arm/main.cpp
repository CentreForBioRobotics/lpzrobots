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
 *   Revision 1.1.4.2  2005-11-24 16:16:40  fhesse
 *   moved from main branch
 *
 *   Revision 1.2  2005/11/17 16:28:40  fhesse
 *   initial version
 *
 *   Revision 1.1  2005/11/11 15:35:54  fhesse
 *   preinitial version
 *
 *   Revision 1.14  2005/11/09 13:41:25  martius
 *   GPL'ised
 *
 ***************************************************************************/
#include <stdio.h>

// include drawstuff library
#include <drawstuff/drawstuff.h>

// include ode library
#include <ode/ode.h>

// include noisegenerator (used for adding noise to sensorvalues)
#include <selforg/noisegenerator.h>

// include simulation environmet stuff
#include "simulation.h"

// include agent (class for holding a robot, a controller and a wiring)
#include "odeagent.h"

// used wiring
#include <selforg/one2onewiring.h>

// used robot
#include "nimm4.h"
#include "arm2segm.h"
#include "muscledarm.h"

// used arena
#include "playground.h"
#include "sphere.h"

// used controller
#include <selforg/invertnchannelcontroller.h>
#include <selforg/invertmotornstep.h>
#include <selforg/sinecontroller.h>
#include <selforg/invertnchannelcontroller.h>
// plotoptions is a list of possible online output, 
// if the list is empty no online gnuplot windows and no logging to file occurs.
// The list is modified with commandline options, see main() at the bottom of this file
list<PlotOption> plotoptions;

// starting function (executed once at the beginning of the simulation loop)
void start(const OdeHandle& odeHandle, GlobalData& global) 
{
  dsPrint ( "\nWelcome to the virtual ODE - robot simulator of the Robot Group Leipzig\n" );
  dsPrint ( "------------------------------------------------------------------------\n" );
  dsPrint ( "Press Ctrl-C for an basic commandline interface.\n\n" );

  // initial camera position and viewpoint
  float KameraXYZ[3]= {0.340f,-0.0779f,1.51f};
  float KameraViewXYZ[3] = {179.90f,-97.69f,0.0000f};;
  dsSetViewpoint ( KameraXYZ , KameraViewXYZ );

  // quality in which spheres are drawn
  dsSetSphereQuality (2); 

  // initialization
  // - set noise to 0.1
  // - register file chess.ppm as a texture called chessTexture (used for the wheels)
  global.odeConfig.noise=0.1;
  int chessTexture = dsRegisterTexture("chess.ppm");

  // use Playground as boundary:
  // - create pointer to playground (odeHandle contains things like world and space the 
  //   playground should be created in; odeHandle is generated in simulation.cpp)
  // - setting geometry for each wall of playground: 
  //   setGeometry(double length, double width, double	height)
  // - setting initial position of the playground: setPosition(double x, double y, double z)
  // - push playground in the global list of obstacles(globla list comes from simulation.cpp)
  Playground* playground = new Playground(odeHandle);
  playground->setGeometry(7.0, 0.2, 1.5);
  playground->setPosition(0,0,0); // playground positionieren und generieren
  global.obstacles.push_back(playground);


  Sphere* sphere =new Sphere(odeHandle);
  sphere->setGeometry(0.2,0,0);
  sphere->setPosition(0,1,0); 
  global.obstacles.push_back(sphere);
  
  MuscledArmConf  conf = MuscledArm::getDefaultConf();
  conf.drawSphere=true;
  conf.strained=false;
  conf.jointAngleSensors=false;
  conf.jointAngleRateSensors=false;
  conf.MuscleLengthSensors=true;
  MuscledArm* arm = new MuscledArm(odeHandle, conf);
  arm->setParam("damping",20);
  arm->setParam("factorMotors",5);
  arm->place(Position(0,0,0));
  global.configs.push_back(arm);

  // create pointer to controller
  // push controller in global list of configurables
  //  InvertMotorNStep(int buffersize, double cInit = 0.1, bool useS = false, bool someInternalParams = true);

  InvertMotorNStepConf cc = InvertMotorNStep::getDefaultConf();
  cc.buffersize=30;
  AbstractController *controller = new InvertMotorNStep(cc);
  //AbstractController *controller = new   SineController();
  global.configs.push_back(controller);
  
  // create pointer to one2onewiring
  One2OneWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.1));

  // create pointer to agent
  // initialize pointer with controller, robot and wiring
  // push agent in globel list of agents
  OdeAgent* agent = new OdeAgent(plotoptions);
  agent->init(controller, arm, wiring);
  //  agent->setTrackOptions(TrackRobot(true, false, false,50));
  global.agents.push_back(agent);

  // switch off gravity
  global.odeConfig.setParam("gravity",0);
  global.odeConfig.setParam("realtimefactor",0);

  // show (print to console) params of all objects in configurable list 
  showParams(global.configs);
}

// executed once at the end of the simulation loop
void end(GlobalData& global){
  // clear obstacles list
  for(ObstacleList::iterator i=global.obstacles.begin(); i != global.obstacles.end(); i++){
    delete (*i);
  }
  global.obstacles.clear();
  
  // clear agents list
  for(OdeAgentList::iterator i=global.agents.begin(); i != global.agents.end(); i++){
    delete (*i)->getRobot();
    delete (*i)->getController();
    delete (*i);
  }
  global.agents.clear();
}


// this function is called if the user pressed Ctrl-C
void config(GlobalData& global){
  changeParams(global.configs);
}

// print command line options
void printUsage(const char* progname){
  printf("Usage: %s [-g] [-l]\n\t-g\tuse guilogger\n\t-l\tuse guilogger with logfile\n", progname);
}

int main (int argc, char **argv)
{ 
  // start with online windows (default: start without plotting and logging)
  if(contains(argv, argc, "-g")) plotoptions.push_back(PlotOption(GuiLogger));
  
  // start with online windows and logging to file
  if(contains(argv, argc, "-l")) plotoptions.push_back(PlotOption(GuiLogger_File));
  
  // display help
  if(contains(argv, argc, "-h")) printUsage(argv[0]);


  // initialise the simulation and provide the start, end, and config-function
  simulation_init(&start, &end, &config);
 
  // start the simulation (returns, if the user closes the simulation)
  simulation_start(argc, argv);
  
  // tidy up
  simulation_close();  
  return 0;
}

/*
 [Simulation Environment:  odeconfig.h,v - 1.12 ][1804289383]
 noise=              0.100000
 simstepsize=        0.010000
 realtimefactor=     0.000000
 drawinterval=       50.000000
 controlinterval=    5.000000
 gravity=            0.000000
 [muscledarm.cpp,v - 1.2 ][2031419189]
 factorMotors=       0.100000
 factorSensors=      4.000000
 damping=            1.000000
 [invertmotornstep.cpp,v - 1.19 ][240828263]
 epsA=               0.001299
 epsC=               0.000000
 adaptrate=          0.010000
 nomupdate=          0.010000
 desens=             0.000000
 s4delay=            1.000000
 s4avg=              5.000000
 steps=              4.000000
 zetaupdate=         0.000000
 logaE=              0.000000
 rootE=              0.000000
 relativeE=          0.000000
 factorB=            0.200000
 noiseB=             0.001000
*/

/*
[Simulation Environment:  odeconfig.h,v - 1.12 ][1804289383]
 noise=              0.000000
 simstepsize=        0.010000
 realtimefactor=     0.000000
 drawinterval=       50.000000
 controlinterval=    5.000000
 gravity=            0.000000
[ muscledarm.cpp,v - 1.2 ][715246296]
 factorMotors=       0.000000
 factorSensors=      10.000000
 damping=            10.000000
[ sinecontroller.hpp,v - 1.4 ][394186711]
 sinerate=           400.000000
 phaseshift=         1.000000
*/



/*

Use random number seed: 1132245941

[Simulation Environment:  odeconfig.h,v - 1.12 ][1804289383]
 noise=              0.100000
 simstepsize=        0.010000
 realtimefactor=     0.000000
 drawinterval=       50.000000
 controlinterval=    5.000000
 gravity=            0.000000
[ muscledarm.cpp,v - 1.3 ][1577223465]
 factorMotors=       2.000000
 factorSensors=      4.000000
 damping=            20.000000
[ invertmotornstep.cpp,v - 1.19 ][686205722]
 epsA=               0.005501
 epsC=               0.000000
 adaptrate=          0.010000
 nomupdate=          0.010000
 desens=             0.000000
 s4delay=            1.000000
 s4avg=              1.000000
 steps=              3.000000
 zetaupdate=         0.000000
 logaE=              0.000000
 rootE=              0.000000
 relativeE=          0.000000
 factorB=            0.200000
 noiseB=             0.001000




*/

