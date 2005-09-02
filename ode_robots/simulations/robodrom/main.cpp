#include <stdio.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>

#include "noisegenerator.h"
#include "simulation.h"
#include "agent.h"
#include "one2onewiring.h"
#include "forcedsphere.h"
#include "sphererobotarms.h"
#include "playground.h"
#include "terrainground.h"

#include "invertmotorspace.h"
#include "invertmotornstep.h"
#include "sinecontroller.h"


ConfigList configs;
PlotMode plotMode = NoPlot;

// Funktion die die Steuerung des Roboters uebernimmt
bool StepRobot()
{

  for(AgentList::iterator i=agents.begin(); i != agents.end(); i++){
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
  //float KameraXYZ[3]= {2.1640f,-1.3079f,1.7600f};
  float KameraXYZ[3]= {-14.1362f,7.5110f,17.3500f};
  //float KameraViewXYZ[3] = {125.5000f,-17.0000f,0.0000f};
  float KameraViewXYZ[3] = {-36.0000f,-50.0000f,0.0000f};
  dsSetViewpoint ( KameraXYZ , KameraViewXYZ );
  dsSetSphereQuality (2); //Qualitaet in der Sphaeren gezeichnet werden

  // initialization
  simulationConfig.noise=0.1;
    
  Playground* playground = new Playground(world, space);
  playground->setGeometry(20.0, 0.2, 5.0);
  playground->setPosition(0,0,0); // playground positionieren und generieren
  obstacles.push_back(playground);
  
  Terrainground *terrainground = new Terrainground(world, space, 20.0, 1.3, "terrains/dip3_128.ppm");
  //terrain_bumpInDip128.ppm
  terrainground->setPosition(-10,-10,1);
  obstacles.push_back(terrainground);
  
  configs.push_back(&simulationConfig);


  SphererobotConf conf = SphererobotArms::getStandartConf();  
  conf.diameter=2;
  conf.spheremass=1;
  SphererobotArms* sphere1 = new SphererobotArms ( ODEHandle(world , space , contactgroup), conf);
  
  Color col(0,0.5,0.8);
  sphere1->place ( Position ( 2 , 0 , 5 ) , &col );
  //AbstractController *controller = new InvertNChannelController(10);  
  AbstractController *controller = new InvertMotorNStep(10);
  controller->setParam("factorB", 0.1);
  controller->setParam("steps", 3);
  
  AbstractWiring* wiring = new One2OneWiring ( new ColorUniformNoise() );
  Agent* agent = new Agent ( plotMode );
  agent->init ( controller , sphere1 , wiring );
  agents.push_back ( agent );
  configs.push_back ( controller );


//   Forcedsphere* sphere = new Forcedsphere(world, space, contactgroup,1.0, 10);
//   sphere->place(Position( 0, 1, 6));
//   //  AbstractController *controller = new SineController();
//   AbstractController *controller = new InvertMotorSpace(10);
//   One2OneWiring* wiring = new One2OneWiring( new ColorUniformNoise() );
//   Agent* agent = new Agent ( plotMode );
//   agent->init ( controller , sphere , wiring );
//   agents.push_back ( agent );
//   configs.push_back ( controller );

  showParams(configs);
}

void end(){
   for(ObstacleList::iterator i=obstacles.begin(); i != obstacles.end(); i++){
     delete (*i);
   }
   obstacles.clear();
   
   for(AgentList::iterator i=agents.begin(); i != agents.end(); i++){
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
  exit(0);
}



int main (int argc, char **argv)
{  
  if(contains(argv, argc, "-g")) plotMode = GuiLogger;
  if(contains(argv, argc, "-l")) plotMode = GuiLogger_File;
  if(contains(argv, argc, "-h")) printUsage(argv[0]);

  // initialise the simulation and provide the start, end, and config-function
  simulation_init(&start, &end, &config);
  // start the simulation (returns, if the user closes the simulation)
  simulation_start(argc, argv);
  simulation_close();  // tidy up.
  return 0;
}
 
