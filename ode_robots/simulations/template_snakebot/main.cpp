#include <stdio.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>
#include <vector>

#include "component_to_robot.h"
#include "noisegenerator.h"
#include "simulation.h"
#include "agent.h"
#include "one2onewiring.h"
#include "playground.h"


#include "sinecontroller.h"
//#include "invertnchannelcontroller.h"
#include "invertmotornstep.h"

using namespace university_of_leipzig::robots;

ConfigList configs;
PlotMode plotMode = NoPlot;

//Startfunktion die am Anfang der Simulationsschleife, einmal ausgefuehrt wird
void start(const OdeHandle& odeHandle, GlobalData& global) 
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
  global.odeConfig.noise=0.1;
  /*  
  Playground* playground = new Playground(odeHandle);
  playground->setGeometry(7.0, 0.2, 1.5);
  playground->setPosition(0,0,0); // playground positionieren und generieren
  */

  // set up the vertex list for the knot 
  VertexList vl;
  university_of_leipzig::robots::Matrix<double> mat(4, 9);

  for(unsigned i = 0; i < 9; ++i)
    mat(0, i) = i;

  mat(1, 0) =  2.5;
  mat(1, 1) =  2.5;
  mat(1, 2) =  7.5;
  mat(1, 3) = 10.0;
  mat(1, 4) =  5.5;
  mat(1, 5) =  0.0;
  mat(1, 6) =  2.5;
  mat(1, 7) =  7.5;
  mat(1, 8) =  7.5;

  mat(2, 0) =  -5.0;
  mat(2, 1) =   0.0;
  mat(2, 2) =   2.5;
  mat(2, 3) =   0.0;
  mat(2, 4) =  -2.5;
  mat(2, 5) =   0.0;
  mat(2, 6) =  1.25;
  mat(2, 7) =   2.5;
  mat(2, 8) =  10.0;

  mat(3, 0) =  2.5;
  mat(3, 1) =  2.5;
  mat(3, 2) =  5.0;
  mat(3, 3) =  2.5;
  mat(3, 4) =  0.5;
  mat(3, 5) =  2.5;
  mat(3, 6) =  5.0;
  mat(3, 7) =  2.5;
  mat(3, 8) =  2.5;

  double fx = 0.2;
  double fy = 0.2;
  double fz = 0.2;
  for(unsigned i = 0; i < 9; ++i) {
    mat(1, i) *= fx;
    mat(2, i) *= fy;
    mat(3, i) *= fz;
  }


  CubicSpline<dReal> cs;
  cs.create(mat);

  double p = 0.0;
  while(p < 8.0) {
    Vector<dReal> v = cs.get_point(p);


    Vector3<dReal> v3(v(0), v(1), v(2));
    vl.insert(vl.end(), v3);
   

    p = cs.get_distant_point_parameter(p, 0.9);
  }
  
  // create a spiral/circle snake (or something)
  
    for(unsigned i = 0; i < 15; ++i) {
     vl.insert(vl.end(), Vector3<double>(i / 3 * cos(M_PI / 180 * i * 5), i / 3 * sin(M_PI / 180 * i * 5), i));
    //vl.insert(vl.end(), Vector3<double>(3 * cos(M_PI / 180 * i * 25), 3 * sin(M_PI / 180 * i * 25), 0.5));
    }
  
  
  // create a streched snake  
  /*
  for(unsigned i = 0; i < 5 ; ++i) {
    vl.insert(vl.end(), Vector3<double>(0.0, i * 0.7, 0.1));
    // vl.insert(vl.end(), Vector3<double>(i * 0.7, 0.0, 0.1));
  }
  */


  ODEHandle ode_handle(world , space , contactgroup);

  // set up a robot arm description
  RobotArmDescription desc;
  desc.p_ode_handle   = &ode_handle;
  desc.segment_radius = 0.05;
  desc.segment_mass   = 1.0;
  desc.p_vertex_list  = &vl;

  //  CCURobotArmComponent rac(desc);
  // IComponent *p_component = new Test();
  IComponent    *p_component = new CCURobotArmComponent(desc);
  AbstractRobot *p_robot     = new ComponentToRobot(p_component, ode_handle);


  // initialization
  //  global.odeConfig.noise=0.1;
  //  
  AbstractController *controller = new InvertMotorNStep(10);

  // AbstractController *controller = new SineController();
  AbstractWiring* wiring     = new One2OneWiring(new ColorUniformNoise());
  Agent* agent               = new Agent();


  agent->init(controller, p_robot, wiring);  
  global.agents.push_back(agent);
  
    configs.push_back(controller);
  showParams(configs);
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
 
