#include <stdio.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>

#include "simulation.h"
#include "noisegenerator.h"
#include "agent.h"
#include "one2onewiring.h"
#include "playground.h"

#include "invertnchannelcontroller.h"
#include "sinecontroller.h"
#include "noisegenerator.h"

#include "sphererobot.h"

ConfigList configs;
PlotMode plotMode = NoPlot;

Sphererobot* sphere1;

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
  float KameraXYZ[3]= {2.1640f,-1.3079f,1.7600f};
  float KameraViewXYZ[3] = {125.5000f,-17.0000f,0.0000f};;
  dsSetViewpoint ( KameraXYZ , KameraViewXYZ );
  dsSetSphereQuality (3); //Qualitaet in der Sphaeren gezeichnet werden

  // initialization
  simulationConfig.noise=0.1;
  configs.push_back(&simulationConfig);
  
  Playground* playground = new Playground(world, space);
  playground->setGeometry(7, 0.2, 1.5);
  playground->setPosition(0,0,0); // playground positionieren und generieren
  obstacles.push_back(playground);
    
  
  dWorldSetERP ( world , 1 );
  //****************
  SphererobotConf conf = Sphererobot::getStandartConf();  
  sphere1 = new Sphererobot ( 1 , ODEHandle(world , space , contactgroup), conf);
  Color col(0,0.5,0.8);
  sphere1->place ( Position ( 0 , 0 , 0 ) , &col );
  //AbstractController *controller = new InvertNChannelController(10);  
  AbstractController *controller = new SineController();  
  
  One2OneWiring* wiring = new One2OneWiring ( new ColorUniformNoise() );
  Agent* agent = new Agent ( plotMode );
  agent->init ( controller , sphere1 , wiring );
  agents.push_back ( agent );
  configs.push_back ( controller );
      
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

//Funktion die eingegebene Befehle/kommandos verarbeitet
void command (int cmd)
{
	//dsPrint ( "Eingabe erfolgt %d (`%c')\n" , cmd , cmd );
	switch ( (char) cmd )
	{
		case 'y' : dBodyAddForce ( sphere1->getObjektAt ( 3 ).body , 0 , 0 , 10 ); break;
		case 'a' : dBodyAddForce ( sphere1->getObjektAt ( 3 ).body , 0 , 0 , -10 ); break;
		case 'x' : dBodyAddForce ( sphere1->getObjektAt ( 5 ).body , 0 , 0 , 10 ); break;
		case 'c' : dBodyAddForce ( sphere1->getObjektAt ( 7 ).body , 0 , 0 , 10 ); break;
	}
}

int main (int argc, char **argv)
{  
  if(contains(argv, argc, "-g")) plotMode = GuiLogger;
  if(contains(argv, argc, "-l")) plotMode = GuiLogger_File;
  if(contains(argv, argc, "-h")) printUsage(argv[0]);

  // initialise the simulation and provide the start, end, and config-function
  simulation_init(&start, &end, &config, &command , 0 , 0 );
  // start the simulation (returns, if the user closes the simulation)
  simulation_start(argc, argv);
  simulation_close();  // tidy up.
  return 0;
}

 
