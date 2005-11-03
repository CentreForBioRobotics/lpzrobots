#include <stdio.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>

#include "simulation.h"
#include "grabframe.h"

#include "noisegenerator.h"
#include "agent.h"
#include "one2onewiring.h"
#include "playground.h"

#include "invertmotornstep.h"
#include "sinecontroller.h"
#include "noisegenerator.h"

#include "sphererobotarms.h"

PlotMode plotMode = NoPlot;
AbstractController *controller;
SphererobotArms* sphere1;

void start(const OdeHandle& odeHandle, GlobalData& global) 
{
  dsPrint ( "\nWelcome to the virtual ODE - robot simulator of the Robot Group Leipzig\n" );
  dsPrint ( "------------------------------------------------------------------------\n" );
  dsPrint ( "Press Ctrl-C for an basic commandline interface.\n\n" );

  //Anfangskameraposition und Punkt auf den die Kamera blickt
  float KameraXYZ[3]= {2.1640f,-1.3079f,1.7600f};
  float KameraViewXYZ[3] = {125.5000f,-17.0000f,0.0000f};;
  dsSetViewpoint ( KameraXYZ , KameraViewXYZ );
  dsSetSphereQuality (3); //Qualitaet in der Sphaeren gezeichnet werden
  dWorldSetERP(odeHandle.world, 0.9);

  // initialization
  global.odeConfig.setParam("noise",0.1);
  //  global.odeConfig.setParam("gravity",-10);
  global.odeConfig.setParam("controlinterval",1);

    
  Playground* playground = new Playground(odeHandle);
  playground->setGeometry(100, 0.2, 1);
  playground->setPosition(0,0,0); // playground positionieren und generieren
  global.obstacles.push_back(playground);
    
  //****************
  SphererobotArmsConf conf = SphererobotArms::getStandartConf();  

  sphere1 = new SphererobotArms ( odeHandle, conf);

  Color col(0,0.5,0.8);
  sphere1->place ( Position ( 0 , 0 , 1 ), &col );
  controller = new InvertMotorNStep(10);  
  //controller = new SineController();  
  //  controller->setParam("sineRate", 50);  
  //  controller->setParam("phaseShift", 0.7);

  One2OneWiring* wiring = new One2OneWiring ( new ColorUniformNoise() );
  Agent* agent = new Agent ( plotMode );
  agent->init ( controller , sphere1 , wiring );
  global.agents.push_back ( agent );
  global.configs.push_back ( controller );
      
  showParams(global.configs);
}

void end(GlobalData& global){
  for(ObstacleList::iterator i=global.obstacles.begin(); i != global.obstacles.end(); i++){
    delete (*i);
  }
  global.obstacles.clear();
  for(AgentList::iterator i=global.agents.begin(); i != global.agents.end(); i++){
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

void printUsage(const char* progname){
  printf("Usage: %s [-g] [-l]\n\t-g\tuse guilogger\n\t-l\tuse guilogger with logfile", progname);
  exit(0);
}


//Funktion die eingegebene Befehle/kommandos verarbeitet
void command (const OdeHandle&, GlobalData& globalData, int cmd)
{
  //dsPrint ( "Eingabe erfolgt %d (`%c')\n" , cmd , cmd );
  switch ( (char) cmd )
    {
    case 'y' : dBodyAddForce ( sphere1->object[SphererobotArms::Base].body , 10 ,0 , 0 ); break;
    case 'Y' : dBodyAddForce ( sphere1->object[SphererobotArms::Base].body , -10 , 0 , 0 ); break;
    case 'x' : dBodyAddTorque ( sphere1->object[SphererobotArms::Base].body , 0 , 0 , 3 ); break;
    case 'X' : dBodyAddTorque ( sphere1->object[SphererobotArms::Base].body , 0 , 0 , -3 ); break;
    case 'S' : controller->setParam("sineRate", controller->getParam("sineRate")*1.2); 
      printf("sineRate : %g\n", controller->getParam("sineRate"));
      break;
    case 's' : controller->setParam("sineRate", controller->getParam("sineRate")/1.2); 
      printf("sineRate : %g\n", controller->getParam("sineRate"));
      break;
//     case 'P' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->KP+=1; 
//       printf("KP : %g\n", sphere1->servo[0]->KP); break;
//     case 'p' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->KP-=1; 
//       printf("KP : %g\n", sphere1->servo[0]->KP); break;
//     case 'D' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->KD+=0.01; 
//       printf("KD : %g\n", sphere1->servo[0]->KD); break;
//     case 'd' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->KD-=0.01; 
//       printf("KD : %g\n", sphere1->servo[0]->KD); break;
//     case 'I' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->KI+=0.01; 
//       printf("KI : %g\n", sphere1->servo[0]->KI); break;
//     case 'i' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->KI-=0.01; 
//       printf("KI : %g\n", sphere1->servo[0]->KI); break;
//     case 'A' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->alpha*=1.01; 
//       printf("KI : %g\n", sphere1->servo[0]->alpha); break;
//     case 'a' : for(int i=0; i<sphere1->getMotorNumber(); i++) sphere1->servo[i]->alpha*=0.99; 
//       printf("KI : %g\n", sphere1->servo[0]->alpha); break;
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

 
