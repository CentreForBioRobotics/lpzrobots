/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    frankguettler@gmx.de                                                 *
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
 *   Revision 1.1  2006-07-20 17:16:05  martius
 *   simulation for complex model networks
 *
 *   Revision 1.15  2006/07/14 12:23:53  martius
 *   selforg becomes HEAD
 *
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

// used robot
#include "nimm2.h"
#include "nimm4.h"

// used arena
#include "playground.h"
// used passive spheres
#include "passivesphere.h"

// used controller
//#include <selforg/invertnchannelcontroller.h>
#include <selforg/invertmotorbigmodel.h>
#include <selforg/multilayerffnn.h>

// fetch all the stuff of lpzrobots into scope
using namespace lpzrobots;


class ThisSim : public Simulation {
public:

  // starting function (executed once at the beginning of the simulation loop)
  void start(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global) 
  {
    setCameraHomePos(Pos(5.2728, 7.2112, 3.31768), Pos(140.539, -13.1456, 0));
    // initialization
    // - set noise to 0.1
    // - register file chess.ppm as a texture called chessTexture (used for the wheels)
    global.odeConfig.noise=0.1;
    //  global.odeConfig.setParam("gravity", 0);
    //  int chessTexture = dsRegisterTexture("chess.ppm");

    // use Playground as boundary:
    // - create pointer to playground (odeHandle contains things like world and space the 
    //   playground should be created in; odeHandle is generated in simulation.cpp)
    // - setting geometry for each wall of playground: 
    //   setGeometry(double length, double width, double	height)
    // - setting initial position of the playground: setPosition(double x, double y, double z)
    // - push playground in the global list of obstacles(globla list comes from simulation.cpp)
    Playground* playground = new Playground(odeHandle, osgHandle, osg::Vec3(10, 0.2, 0.5));
    playground->setPosition(osg::Vec3(0,0,0)); // playground positionieren und generieren
    global.obstacles.push_back(playground);

    // add passive spheres as obstacles
    // - create pointer to sphere (with odehandle, osghandle and 
    //   optional parameters radius and mass,where the latter is not used here) )
    // - set Pose(Position) of sphere 
    // - set a texture for the sphere
    // - add sphere to list of obstacles
    for (int i=0; i<= 1/*2*/; i+=2){
      PassiveSphere* s1 = new PassiveSphere(odeHandle, osgHandle, 0.5);
      s1->setPosition(osg::Vec3(-4.5+i*4.5,0,0));
      s1->setTexture("Images/dusty.rgb");
      global.obstacles.push_back(s1);
    }

    // set color for nimm robot
    OsgHandle osgHandle_orange = osgHandle.changeColor(Color(2, 156/255.0, 0));

    // use Nimm2 vehicle as robot:
    // - get default configuration for nimm2
    // - activate bumpers, cigar mode and infrared front sensors of the nimm2 robot
    // - create pointer to nimm2 (with odeHandle, osg Handle and configuration)
    // - place robot
    Nimm2Conf c = Nimm2::getDefaultConf();
    c.bumper  = true;
    c.cigarMode  = false;
    c.irFront = false;
    OdeRobot* vehicle = new Nimm2(odeHandle, osgHandle_orange, c, "Nimm2");    
    vehicle->place(Pos(2,0,0));
    
    // use Nimm4 vehicle as robot:
    // - create pointer to nimm4 (with odeHandle and osg Handle and possible other settings, see nimm4.h)
    // - place robot
    //OdeRobot* vehiInvertMotorSpacecle = new Nimm4(odeHandle, osgHandle);
    //vehicle->place(Pos(0,2,0));
    
    // create pointer to controller
    // push controller in global list of configurables
    //  AbstractController *controller = new InvertNChannelController(10);  
    //AbstractController *controller = new InvertMotorSpace(10);  
    InvertMotorBigModelConf cc = InvertMotorBigModel::getDefaultConf();

    std::vector<Layer> layers;
    //    layers.push_back(Layer(3, 0.5 , FeedForwardNN::tanh,FeedForwardNN::dtanh));
    layers.push_back(Layer(3,0.5));
    layers.push_back(Layer(2,0.5));
    MultiLayerFFNN* net = new MultiLayerFFNN(0.05, layers);
    cc.model = net;
    cc.modelInit = 0.5;
    AbstractController *controller = new InvertMotorBigModel(cc);  
    controller->setParam("adaptrate",0);
    global.configs.push_back(controller);
    
    // create pointer to one2onewiring
    One2OneWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.1));
    
    // create pointer to agent
    // initialize pointer with controller, robot and wiring
    // push agent in globel list of agents
    OdeAgent* agent = new OdeAgent(plotoptions);
    agent->init(controller, vehicle, wiring);
    global.agents.push_back(agent);
  
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
 
