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
 *   Revision 1.17  2006-08-04 16:25:14  martius
 *   bugfixing
 *
 *   Revision 1.16  2006/07/14 12:23:52  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.15.4.6  2006/05/15 13:11:29  robot3
 *   -handling of starting guilogger moved to simulation.cpp
 *    (is in internal simulation routine now)
 *   -CTRL-F now toggles logging to the file (controller stuff) on/off
 *   -CTRL-G now restarts the GuiLogger
 *
 *   Revision 1.15.4.5  2006/02/08 16:15:11  martius
 *   testing with invertcontroller
 *
 *   Revision 1.15.4.4  2006/01/10 21:46:02  martius
 *   moved to osg
 *
 *   Revision 1.15.4.3  2005/12/06 17:38:19  martius
 *   *** empty log message ***
 *
 *   Revision 1.15.4.2  2005/11/16 11:27:38  martius
 *   invertmotornstep has configuration
 *
 *   Revision 1.15.4.1  2005/11/15 12:30:04  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.15  2005/11/14 12:50:21  martius
 *   *** empty log message ***
 *
 *   Revision 1.14  2005/11/09 13:41:25  martius
 *   GPL'ised
 *
 ***************************************************************************/
#include "simulation.h"

#include "odeagent.h"
#include "octaplayground.h"

#include <selforg/noisegenerator.h>
#include <selforg/one2onewiring.h>
#include <selforg/selectiveone2onewiring.h>
#include <selforg/derivativewiring.h>

#include <selforg/sinecontroller.h>
#include <selforg/invertmotornstep.h>
#include <selforg/invertmotorspace.h>
#include <selforg/invertnchannelcontroller.h>

#include "shortcircuit.h"

// fetch all the stuff of lpzrobots into scope
using namespace lpzrobots;

int channels;
int t=0;
double omega = 0.05;


SineWhiteNoise* sineNoise;

class ThisSim : public Simulation {

public:
  // starting function (executed once at the beginning of the simulation loop)
  void start(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global) 
  {
    setCameraHomePos(Pos(5.2728, 7.2112, 3.31768), Pos(140.539, -13.1456, 0));

    // initialization
    global.odeConfig.setParam("noise",0.05);
    global.odeConfig.setParam("realtimefactor",0);
    global.odeConfig.setParam("drawinterval", 500);


    OdeRobot* robot = new ShortCircuit(odeHandle, osgHandle, channels, channels);  
    //  OdeRobot* robot = new Nimm2(odeHandle);  
    InvertMotorNStepConf cc = InvertMotorNStep::getDefaultConf();
    cc.cInit=0.1;
    cc.cNonDiag=0.0;
    AbstractController *controller = new InvertMotorNStep(cc);  
    //AbstractController *controller = new InvertNChannelController(40);  
    //AbstractController *controller = new InvertMotorSpace(10,1);  
    //controller->setParam("nomupdate",0.001);
    controller->setParam("adaptrate",0.000);
    controller->setParam("epsA",0.6);
    controller->setParam("epsC",0.6);
    controller->setParam("factorB",0.1);
    controller->setParam("steps",1);
    //  AbstractController *controller = new InvertNChannelController(10);  
    //AbstractController *controller = new SineController();
    
    OdeAgent* agent = new OdeAgent(plotoptions);
    
    // sineNoise = new SineWhiteNoise(omega,2,M_PI/2);
    // One2OneWiring* wiring = new One2OneWiring(sineNoise, true);
    One2OneWiring* wiring = new One2OneWiring(new WhiteUniformNoise(), true);
    
    //AbstractWiring* wiring = new SelectiveOne2OneWiring(sineNoise, &select_firsthalf);
    // DerivativeWiringConf c = DerivativeWiring::getDefaultConf();
    //   c.useId=true;
    //   c.useFirstD=true;
    //   c.derivativeScale=20;
    //   c.blindMotorSets=0;
    //   AbstractWiring* wiring = new DerivativeWiring(c, new ColorUniformNoise(0.1));
    agent->init(controller, robot, wiring);
    global.agents.push_back(agent);
    
    global.configs.push_back(controller);
    
    showParams(global.configs);
  }



  void command(const OdeHandle& odeHandle, GlobalData& global, int key){
    switch (key){
    case '>': omega+=0.05;
      break;
    case '<': omega-=0.05;
      break;
    case '.': omega+=0.005;
      break;
    case ',': omega-=0.005;
      break;
    case 'r': omega=0.05;
      break;
    case 'n': omega=0;
      break;
    }
    fprintf(stderr, "Omega: %g\n", omega);
    sineNoise->setOmega(omega);
  }

  // note: this is the normal signature (look above)
  // add own key handling stuff here, just insert some case values
//   virtual bool command(const OdeHandle&, const OsgHandle&, GlobalData& globalData, int key, bool down)
//   {
//     if (down) { // only when key is pressed, not when released
//       switch ( (char) key )
// 	{
// 	default:
// 	  return false;
// 	  break;
// 	}
//     }
//     return false;
//   }

  
};

void printUsage(const char* progname){
  printf("Usage: %s numchannels [-g] [-f]\n\tnumchannels\tnumber of channels\n\
\t-g\t\tuse guilogger\n\t-f\t\tuse guilogger with logfile", progname);
}

int main (int argc, char **argv)
{  
  if(argc <= 1){
    printUsage(argv[0]);  
    return -1;
  }
  channels = std::max(1,atoi(argv[1]));
  ThisSim sim;
  return sim.run(argc, argv) ? 0 : 1;
}
 
/*
Mit 
invertmotornstep/motorspace
  epsC=epsA=0.6
  noise=0.05
Es scheint wichtig zu sein, dass das Modell mitlernt.

Bei mehreren steps muss epsC runtergedreht werden.

*/

