/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    frankguettler@gmx.de                                                 *
 *    mai00bvz@studserv.uni-leipzig.de                                     *
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
 *   Revision 1.1  2007-01-18 15:40:15  robot3
 *   special version for master thesis
 *
 *   Revision 1.19  2007/01/18 14:42:15  robot3
 *   changed to barrel2nd
 *
 *   Revision 1.18  2006/12/21 11:43:05  martius
 *   commenting style for doxygen //< -> ///<
 *   new sensors for spherical robots
 *
 *   Revision 1.17  2006/08/11 15:46:34  martius
 *   *** empty log message ***
 *
 *   Revision 1.16  2006/08/04 15:07:46  martius
 *   documentation
 *
 *   Revision 1.15  2006/07/14 12:23:53  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.14.4.16  2006/06/25 17:01:56  martius
 *   remove old simulations
 *   robots get names
 *
 *   Revision 1.14.4.15  2006/05/18 11:55:56  robot3
 *   made playground smaller (for shadowing issues)
 *
 *   Revision 1.14.4.14  2006/05/15 13:11:30  robot3
 *   -handling of starting guilogger moved to simulation.cpp
 *    (is in internal simulation routine now)
 *   -CTRL-F now toggles logging to the file (controller stuff) on/off
 *   -CTRL-G now restarts the GuiLogger
 *
 *   Revision 1.14.4.13  2006/04/25 09:06:16  robot3
 *   *** empty log message ***
 *
 *   Revision 1.14.4.12  2006/03/31 12:14:49  fhesse
 *   orange color for nimm robot
 *
 *   Revision 1.14.4.11  2006/03/31 11:27:53  fhesse
 *   documentation updated
 *   one sphere removed (todo  fix problem with sphere placing)
 *
 *   Revision 1.14.4.10  2006/01/12 15:17:46  martius
 *   *** empty log message ***
 *
 *   Revision 1.14.4.9  2005/12/29 15:55:33  martius
 *   end is obsolete
 *
 *   Revision 1.14.4.8  2005/12/29 15:47:12  martius
 *   changed to real Sim class
 *
 *   Revision 1.14.4.7  2005/12/14 15:37:25  martius
 *   *** empty log message ***
 *
 *   Revision 1.14.4.6  2005/12/13 18:12:09  martius
 *   switched to nimm2
 *
 *   Revision 1.14.4.5  2005/12/11 23:35:08  martius
 *   *** empty log message ***
 *
 *   Revision 1.14.4.4  2005/12/09 16:53:17  martius
 *   camera is working now
 *
 *   Revision 1.14.4.3  2005/12/06 10:13:25  martius
 *   openscenegraph integration started
 *
 *   Revision 1.14.4.2  2005/11/24 16:19:12  fhesse
 *   include corrected
 *
 *   Revision 1.14.4.1  2005/11/15 12:30:07  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.14  2005/11/09 13:41:25  martius
 *   GPL'ised
 *
 ***************************************************************************/
#include <stdio.h>

// include ode library
#include <ode/ode.h>

// include noisegenerator (used for adding noise to sensorvalues)
#include <selforg/noisegenerator.h>

// include simulation environment stuff
#include "simulation.h"

// include agent (class for holding a robot, a controller and a wiring)
#include "odeagent.h"

// used wiring
#include <selforg/one2onewiring.h>
#include <selforg/derivativewiring.h>

// used robot
#include "barrel2masses2nd.h"
//#include "sphererobot3masses.h"


// used arena
#include "playground.h"
// used passive spheres
#include "passivesphere.h"

// used controller
//#include <selforg/invertnchannelcontroller.h>
//#include <selforg/invertmotorspace.h>
//#include <selforg/sinecontroller.h>
#include <selforg/invertmotornstep.h>

// fetch all the stuff of lpzrobots into scope
using namespace lpzrobots;


class ThisSim : public Simulation {
public:

  // starting function (executed once at the beginning of the simulation loop)
  void start(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global) 
  {
    // first: position(x,y,z) second: view(alpha,beta,gamma)
    // gamma=0;
    // alpha == horizontal angle
    // beta == vertical angle
    setCameraHomePos(Pos(5.2728, 7.2112, 3.31768), Pos(140.539, -13.1456, 0));
    // initialization
    // - set noise to 0.1
    global.odeConfig.noise=0.05;
    //  global.odeConfig.setParam("gravity", 0);

    // use Playground as boundary:
    // - create pointer to playground (odeHandle contains things like world and space the 
    //   playground should be created in; odeHandle is generated in simulation.cpp)
    // - setting geometry for each wall of playground: 
    //   setGeometry(double length, double width, double	height)
    // - setting initial position of the playground: setPosition(double x, double y, double z)
    // - push playground in the global list of obstacles(globla list comes from simulation.cpp)
    
    // odeHandle and osgHandle are global references
    // vec3 == length, width, height
    Playground* playground = new Playground(odeHandle, osgHandle, osg::Vec3(10, 0.2, 0.5));
    playground->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
    // register playground in obstacles list
    global.obstacles.push_back(playground);

    // add passive spheres as obstacles
    // - create pointer to sphere (with odehandle, osghandle and 
    //   optional parameters radius and mass,where the latter is not used here) )
    // - set Pose(Position) of sphere 
    // - set a texture for the sphere
    // - add sphere to list of obstacles
//    for (int i=0; 0<= 1/*2*/; i+=2){
//      PassiveSphere* s1 = new PassiveSphere(odeHandle, osgHandle, 0.5);
//      s1->setPosition(osg::Vec3(-4.5+i*4.5,0,0));
//      s1->setTexture("Images/dusty.rgb");
//      global.obstacles.push_back(s1);
//    }

    // use Nimm2 vehicle as robot:
    // - get default configuration for nimm2
    // - activate bumpers, cigar mode and infrared front sensors of the nimm2 robot
    // - create pointer to nimm2 (with odeHandle, osg Handle and configuration)
    // - place robot
//     Nimm2Conf c = Nimm2::getDefaultConf();
//     c.force   = 4;
//     c.bumper  = true;
//     c.cigarMode  = true;
     //     c.irFront = true;
//     OdeRobot* vehicle = new Nimm2(odeHandle, osgHandle, c, "Nimm2");    
//     vehicle->place(Pos(2,0,0));
     
     // use Nimm4 vehicle as robot:
     // - create pointer to nimm4 (with odeHandle and osg Handle and possible other settings, see nimm4.h)
     // - place robot
     //OdeRobot* vehiInvertMotorSpacecle = new Nimm4(odeHandle, osgHandle);
     //vehicle->place(Pos(0,2,0));
     
     
    /* * * * BARRELS * * * */
    for(int i=0; i< 1; i++){
      //****************
      Sphererobot3MassesConf conf = Barrel2Masses2nd::getDefaultConf();
      conf.motorsensor=true;
//      conf.addSensor(new AxisOrientationSensor(AxisOrientationSensor::ZProjection, Sensor::X | Sensor::Y));
//      conf.addSensor(new SpeedSensor(10, SpeedSensor::Translational, Sensor::X ));
      conf.irAxis1=true;
      conf.irAxis2=true;
      conf.irAxis3=true;
      conf.spheremass   = 0.3;
      conf.drawIRs=true;
      conf.diameter     = 1;
      conf.pendularmass  = 1.0;
      conf.pendularrange  = 0.25; // range of the slider from center in multiple of diameter [-range,range]
      conf.irsensorscale=1.5;
      conf.irCharacter=1;  

      // osgHandle.changeColor(Color(0.0,0.0,1.0) -- aendert color der barrel
        Barrel2Masses2nd* myBarrel = new Barrel2Masses2nd ( odeHandle, osgHandle.changeColor(Color(0.0,0.0,1.0)), 
				    conf, "Barrel2nd"); //, 0.5); 
      // Matrixangabe -- rotate liefert fertige matrix zurueck
      myBarrel->place ( osg::Matrix::rotate(M_PI/2, 1,0,0));


      InvertMotorNStepConf cc = InvertMotorNStep::getDefaultConf();
//      cc.cInit=0.5;
      //    cc.useS=true;
        InvertMotorNStep*  controller = new InvertMotorNStep(cc);    
      //controller = new FFNNController("models/barrel/controller/nonoise.cx1-10.net", 10, true);
//      controller->setParam("steps", 2);    
      //    controller->setParam("adaptrate", 0.001);    
//      controller->setParam("adaptrate", 0.0);    
//    controller->setParam("nomupdate", 0.005);    
//    controller->setParam("epsC", 0.03);    
//    controller->setParam("epsA", 0.05);    
      // controller->setParam("epsC", 0.001);    
      // controller->setParam("epsA", 0.001);    
      //    controller->setParam("rootE", 1);    
      //    controller->setParam("logaE", 2);    
//    controller->setParam("rootE", 3);    
//    controller->setParam("logaE", 0);    
      //     controller = new SineController();  
//    controller->setParam("sinerate", 15);  
//    controller->setParam("phaseshift", 0.45);

// derivative == ableitung, also benutze zusaetzlich abgeleitete sensoren
//       DerivativeWiringConf dc = DerivativeWiring::getDefaultConf();
//       dc.useId=true;
//       dc.useFirstD=false;
//       AbstractWiring* wiring = new DerivativeWiring(dc,new ColorUniformNoise());

// selektiert bestimmte sensoren und motoren
//    AbstractWiring* wiring = new SelectiveOne2OneWiring(new ColorUniformNoise(), new select_from_to(0,1));

      // zusaetzlich das rauschen, gibt weisses und color...
      One2OneWiring* wiring = new One2OneWiring ( new ColorUniformNoise() );   

      OdeAgent* agent = new OdeAgent ( plotoptions );

//      OdeAgent* agent = new OdeAgent ( PlotOption(File, Robot, 1) );
      agent->init ( controller , myBarrel , wiring );
      //  agent->setTrackOptions(TrackRobot(true, false, false, "ZSens_Ring10_11", 50));
      global.agents.push_back ( agent );
      global.configs.push_back ( controller );
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
 