#include <stdio.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>

#include "simulation.h"
#include "one2oneagent.h"
#include "playground.h"

#include "invertnchannelcontroller.h"

#include "schlangeforce.h"

ConfigList configs;

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
  dsSetSphereQuality (2); //Qualitaet in der Sphaeren gezeichnet werden

  // initialization
  simulationConfig.noise=0.1;

  Playground* playground = new Playground(&world, &space);
  playground->setGeometry(70.0, 0.2, 1.5);
  playground->setPosition(0,0,0); // playground positionieren und generieren
  obstacles.push_back(playground);

  //****************
  SchlangeForce* schlange1 = new SchlangeForce ( 1 , &world , &space , &contactgroup , 0 , 0 , 0.25 , 4 , 0.5 , 0.2 , 0 , 0.1 , 2 , 10 , anglerate);
  Position p = {0,0,0};
  Color col = {0,0.5,0.8};
  schlange1->place(p,&col);
  AbstractController *controller = new InvertNChannelController(10,true);  
  
  One2OneAgent* agent = new One2OneAgent(/*NoPlot*/GuiLogger);
  agent->init(controller, schlange1);
  agents.push_back(agent);

  
  //****************  
  configs.push_back(&simulationConfig);
  //****************  
  
  configs.push_back(controller);
  configs.push_back(schlange1);
  //************
  
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


int main (int argc, char **argv)
{  
  // initialise the simulation and provide the start, end, and config-function
  simulation_init(&start, &end, &config);
  // start the simulation (returns, if the user closes the simulation)
  simulation_start(argc, argv);
  simulation_close();  // tidy up.
  return 0;
}
 
