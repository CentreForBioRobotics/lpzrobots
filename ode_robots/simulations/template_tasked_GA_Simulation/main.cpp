/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
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
 *   DESCRIPTION:                                                          *
 *   template for simulations which are parallized. This may be useful     *
 *   for analysis which is using different simulation parameters for       *
 *   several simulations.                                                  *
 *   This simulation is indented to be used with multicore processors,     *
 *   e.g. touchwood (Uni Leipzig) with up to 16 processors.                *
 *                                                                         *
 *   This template is not much useful for clusters, where typically        *
 *   simulations are batch processed and therefore the simulation jobs are *
 *   distributed on the cluster nodes. But there could be a gain of        *
 *   performance if a single node contains more than one cpu and the       *
 *   batch processing algorithm does not distribute more than one job to   *
 *   one cluster node.                                                     *
 *   Remember that the use of parallel threads in one simulation should be *
 *   disabled to avoid thread locks. So the steps of ODE, OSG and          *
 *   controller are executed sequential (guaranteed by TaskedSimulation).  *
 *
 *   PLEASE read and study the other template simulations if you are not   *
 *   familiar with lpzrobots! The comments in this file are limited to     *
 *   the parallelism stuff.                                                *
 *
 *   $Log$
 *   Revision 1.2  2009-09-16 10:26:11  guettler
 *   removed global variable numberIndividuals (now in simTaskHandle)
 *
 *   Revision 1.1  2009/08/24 11:09:55  robot12
 *   new template for tasked simulations use the ga_tools added
 *
 *   Revision 1.2  2009/08/21 09:49:08  robot12
 *   (guettler) support for tasked simulations.
 *   - use the simulation template_taskedSimulations.
 *   - merged (not completely) from lpzrobots_tasked.
 *   - graphics is supported, but only for one simulation of a pool
 *
 *   Revision 1.1.2.1  2009/08/11 16:00:17  guettler
 *   - support for tasked simulations, does not yet work with graphics
 *   - in development state
 *
 *   Revision 1.2  2009/07/02 10:05:59  guettler
 *   added example erasing an agent after one cycle and creating new ones
 *                                                                         *
 *   Revision 1.1  2009/04/23 14:17:34  guettler
 *   new: simulation cycles, first simple implementation, use the
 *   additional method bool restart() for starting new cycles,
 *   template simulation can be found in template_cycledSimulation
 *   (originally taken from template_onerobot)
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>

// include all necessary stuff
#include <ode/ode.h>
#include <selforg/noisegenerator.h>
#include <ode_robots/odeagent.h>
#include <selforg/one2onewiring.h>
#include <ode_robots/nimm2.h>
#include <ode_robots/playground.h>
#include <selforg/invertmotorspace.h>
//#include <selforg/derivativewiring.h>
#include <selforg/invertmotornstep.h>

// include some needed files for parallel task handling
// class TaskedSimulation, holds the SimulationTaskHandle and additional info (like taskId)
#include <ode_robots/taskedsimulation.h>
// class SimulationTask encapsulates one simulation as a single task
#include <ode_robots/simulationtask.h>
// holds all data needed by handling the tasks, additionally there can be put more data.
#include <ode_robots/simulationtaskhandle.h>
// manages the handling of the tasks, including the parallel loop.
#include <ode_robots/simulationtasksupervisor.h>

// used ga_tools
#include <ga_tools/SingletonGenAlgAPI.h>
#include <ga_tools/Generation.h>
#include <ga_tools/Individual.h>
#include <ga_tools/Gen.h>
#include <ga_tools/TemplateValue.h>
// only for deleting
#include <ga_tools/ValueMutationStrategy.h>
#include <ga_tools/StandartMutationFactorStrategy.h>
#include <ga_tools/DoubleRandomStrategy.h>

#include "TemplateCycledGaSimulationFitnessStrategy.h"

#include <selforg/trackablemeasure.h>
#include <selforg/statistictools.h>

// fetch all the stuff of lpzrobots into scope
using namespace lpzrobots;


/**
 * our fitness strategy
 */
TemplateCycledGaSimulationFitnessStrategy* fitnessStr; // the fitness strategy

// create your own SimulationTaskHandle
struct ThisSimulationTaskHandle : public SimulationTaskHandle {
    std::vector<Individual*>* individuals;
    int numberIndividuals;

    /**
     * our fitness strategy
     */
    TemplateCycledGaSimulationFitnessStrategy* fitnessStr; // the fitness strategy
};

/**
 * Just create your own simulation, it's up to you.
 *
 * It's essential that your simulation is deduced from
 * TaskedSimulation instead of Simulation.
 * With this little change you have access to the
 * taskId and the global simTaskHandle.
 */
class ThisSim : public TaskedSimulation {
  public:

    OdeRobot* vehicle;
    OdeAgent* agent;

    /**
     * starting function (executed once at the beginning of the simulation loop/first cycle)
     * This function contains the additional parameters simTaskHandle and taskId, with these
     * you have access to your global data.
     */
    void start(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global,
        SimulationTaskHandle& sTHandle, int taskId) {
      ThisSimulationTaskHandle& handle = static_cast<ThisSimulationTaskHandle&>(sTHandle);
      m_individual = (*handle.individuals)[taskId];

      // So we are now ready to start the algorithm!
      // But without the simulation we have no fun with the algorithm. ;) The only we just need is the simulation!
      // Also we must create the robots and agents for the simulation:
      createBots(global, handle, taskId);

      // First: position(x,y,z) second: view(alpha,beta,gamma)
      // gamma=0;
      // alpha == horizontal angle
      // beta == vertical angle
      setCameraHomePos(Pos(37.3816, 23.0469, 200.818), Pos(0., -90.0, 0.));
      // TODO: disable camera tracking (static (CameraManipulator) instead of CameraManipulatorTV)

      // initialisation
      // - set noise to 0.05
      global.odeConfig.noise = 0.05;
      // set realtimefactor to maximum
      global.odeConfig.setParam("realtimefactor", 0);
    }

    /**
     * restart() is called at the second and all following starts of the cylce
     * The end of a cycle is determined by (simulation_time_reached==true)
     * @param the odeHandle
     * @param the osgHandle
     * @param globalData
     * @return if the simulation should be restarted; this is false by default
     */
    virtual bool restart(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global,
        SimulationTaskHandle& sTHandle, int taskId) {
      ThisSimulationTaskHandle& handle = static_cast<ThisSimulationTaskHandle&>(sTHandle);

      //read the result and give it back.
      handle.fitnessStr->m_storage.push_back(&m_trackableEntropy->getValueAddress());

      return false; // don't restart, just quit
      // see template_cycledSimulation for more info about usage
    }

    /** optional additional callback function which is called every simulation step.
     Called between physical simulation step and drawing.
     @param draw indicates that objects are drawn in this timestep
     @param pause always false (only called of simulation is running)
     @param control indicates that robots have been controlled this timestep
     */
    virtual void addCallback(GlobalData& globalData, bool draw, bool pause, bool control,
        SimulationTaskHandle& sTHandle, int taskId) {
      ThisSimulationTaskHandle* simTaskHandle = static_cast<ThisSimulationTaskHandle*> (&sTHandle);
      // for demonstration: set simsteps for one cycle to 60.000/currentCycle (10min/currentCycle)
      // if simulation_time_reached is set to true, the simulation cycle is finished
      if (globalData.sim_step >= (60000 / this->currentCycle)) {
        simulation_time_reached = true;
      }
    }

    // add own key handling stuff here, just insert some case values
    virtual bool command(const OdeHandle&, const OsgHandle&, GlobalData& globalData, int key, bool down,
        SimulationTaskHandle& sTHandle, int taskI) {
      ThisSimulationTaskHandle* simTaskHandle = static_cast<ThisSimulationTaskHandle*> (&sTHandle);
      if (down) { // only when key is pressed, not when released
        switch ((char) key) {
          default:
            return false;
            break;
        }
      }
      return false;
    }

  private:
    Individual* m_individual;
    TrackableMeasure* m_trackableEntropy;

    /**
     * This function creates the robots and agents for one simulation.
     * @param global
     */
    void createBots(GlobalData& global, ThisSimulationTaskHandle& sTHandle, int taskId) {
      OdeRobot * vehicle; // the robot
      OdeAgent * agent; // the agent
      Playground * playground; // the playground for the created robot

      // Next we need a playground for the robot:
      // Use Playground as boundary:
      // - create pointer to playground (odeHandle contains things like world and space the
      //   playground should be created in; odeHandle is generated in simulation.cpp)
      // - setting geometry for each wall of playground:
      //   setGeometry(double length, double width, double  height)
      // - setting initial position of the playground: setPosition(double x, double y, double z)
      // - push playground in the global list of obstacles(global list comes from simulation.cpp)
      playground = new Playground(odeHandle, osgHandle, osg::Vec3(18, 0.2, 0.5));
      playground->setPosition(osg::Vec3((double) (taskId % (int) sqrt(sTHandle.numberIndividuals)) * 19.0, 19.0 * (double) (taskId
          / (int) sqrt(sTHandle.numberIndividuals)), 0.05)); // position and generate playground
      // register playground in obstacles list
      global.obstacles.push_back(playground);

      // Use Nimm2 vehicle as robot:
      // - get default configuration for nimm2
      // - activate bumpers, cigar mode of the nimm2 robot
      // - create pointer to nimm2 (with odeHandle, osg Handle and configuration)
      // - place robot
      Nimm2Conf c = Nimm2::getDefaultConf();
      c.force = 4;
      c.bumper = true;
      c.cigarMode = true;
      vehicle = new Nimm2(odeHandle, osgHandle, c, ("Nimm2" + m_individual->getName()).c_str());
      vehicle->place(Pos((double) (taskId % (int) sqrt(sTHandle.numberIndividuals)) * 19.0, 19.0 * (double) (taskId / (int) sqrt(
          sTHandle.numberIndividuals)), 0.0));

      // Read the gene values and create the neuron matrix.
      // The genes have a value of type IValue. We use only double values so we took for this interface
      // a TemplateValue<double> which is type of an IValue (see create prototypes in start()).
      // So we only need to cast them! Than we can read it!
      matrix::Matrix init(2, 2);
      double v1, v2, v3, v4;
      TemplateValue<double>* value = dynamic_cast<TemplateValue<double>*> (m_individual->getGen(0)->getValue());
      value != 0 ? v1 = value->getValue() : v1 = 0.0;
      value = dynamic_cast<TemplateValue<double>*> (m_individual->getGen(1)->getValue());
      value != 0 ? v2 = value->getValue() : v2 = 0.0;
      value = dynamic_cast<TemplateValue<double>*> (m_individual->getGen(2)->getValue());
      value != 0 ? v3 = value->getValue() : v3 = 0.0;
      value = dynamic_cast<TemplateValue<double>*> (m_individual->getGen(3)->getValue());
      value != 0 ? v4 = value->getValue() : v4 = 0.0;
      // set the matrix values
      init.val(0, 0) = v1;
      init.val(0, 1) = v2;
      init.val(1, 0) = v3;
      init.val(1, 1) = v4;

      // Create pointer to controller:
      // Push controller in global list of configurables.
      // Use the neuron matrix for the controller.
      InvertMotorNStepConf confMotorNStep = InvertMotorNStep::getDefaultConf();
      confMotorNStep.initialC = init;
      InvertMotorNStep *controller = new InvertMotorNStep(confMotorNStep);
      global.configs.push_back(controller);

      // create pointer to one2onewiring
      One2OneWiring* wiring = new One2OneWiring(new ColorUniformNoise(0.1));

      // create pointer to agent
      // initialise pointer with controller, robot and wiring
      // push agent in global list of agents
      agent = new OdeAgent(plotoptions);
      agent->init(controller, vehicle, wiring);
      global.agents.push_back(agent);

      // create measure for the agent
      // and connect the measure with the fitness strategy
      std::list<Trackable*> trackableList;
      trackableList.push_back(vehicle);
      m_trackableEntropy = new TrackableMeasure(trackableList,
          ("E Nimm2 of " + m_individual->getName()).c_str(), ENTSLOW, playground->getCornerPointsXY(), X | Y, 18);
    }

};

/**
 * Defines a method to construct a ThisSim. This method is needed by the
 * SimulationTask, provided through the SimulationTaskSupervisor.
 * If you like to get the singleton instance of SimulationTaskSupervisor, you have
 * to pass as argument an instance of the ThisSimulationBuilder.
 */
class ThisSimCreator : public TaskedSimulationCreator {
  public:
    virtual TaskedSimulation* buildTaskedSimulationInstance() {
      return new ThisSim();
    }
};

int main(int argc, char **argv) {
  int numberIndividuals = 96; // we have 12 processors available
  // ga_tool initialising
  // First we need some variables.
  RandGen random; // a random generator
  IFitnessStrategy* invertedFitnessStr; // the inverted fitness strategy
  IGenerationSizeStrategy* gSStr; // a generation size strategy
  ISelectStrategy* selStr; // a select strategy
  GenPrototype* pro1; // the 4 prototypes for the genes 2 Sensors - 2 Engines ==> 4 neuron connections
  GenPrototype* pro2;
  GenPrototype* pro3;
  GenPrototype* pro4;
  TemplateCycledGaSimulationFitnessStrategy* fitnessStr; // the fitness strategy
  IMutationFactorStrategy* mutFaStr;
  IMutationStrategy* mutStr;
  IRandomStrategy* randomStr;

  //this 3 PlotOptions are needed for some measures. They will bring us some data on the screen and save all to a log file.
  PlotOption opt1(GuiLogger); // a plot Option for the generation measure to guilogger
  PlotOption opt2(File); // a plot Option for the generation measure to file
  PlotOption optGen(File); // a plot Option for gene measure to file

  // Next we need the general strategies for the algorithm.
  // - a GenerationSizeStrategy: Here we take a fixed size strategy. This means every generation has the size of "numberIndividuals"
  // - a SelectStrategy: Here we take a tournament strategy which tests 2 individuals. The better one will win.
  gSStr = SingletonGenAlgAPI::getInstance()->createFixGenerationSizeStrategy((int) ((numberIndividuals
      - (numberIndividuals / 10)) / 2));
  SingletonGenAlgAPI::getInstance()->setGenerationSizeStrategy(gSStr);
  selStr = SingletonGenAlgAPI::getInstance()->createTournamentSelectStrategy(&random);
  SingletonGenAlgAPI::getInstance()->setSelectStrategy(selStr);

  // After this we need the fitness strategy.
  // Here we need our own strategy! But our strategy will be higher if the individual are better.
  // So we need a inverted fitness strategy because the genetic algorithm will optimise again zero.
  // More details on this strategies can be found in the belonging header files.
  fitnessStr = new TemplateCycledGaSimulationFitnessStrategy();
  invertedFitnessStr = SingletonGenAlgAPI::getInstance()->createInvertedFitnessStrategy(fitnessStr);
  SingletonGenAlgAPI::getInstance()->setFitnessStrategy(invertedFitnessStr);

  // Now its time to create all needed stuff for the genes.
  // - mutation strategy for the prototypes
  // - random strategy for the prototypes
  // - and the 4 prototypes for the genes:
  mutFaStr = SingletonGenAlgAPI::getInstance()->createStandartMutationFactorStrategy();
  // The second value means the mutation probability in 1/1000. Normal is a value lower than max. 5%.
  // But we have so few individuals, that we need a higher mutation (33,3%).
  mutStr = SingletonGenAlgAPI::getInstance()->createValueMutationStrategy(mutFaStr, 50);
  // The last parameters ensure that the created genes lay inside the interval from -100 to +100.
  randomStr = SingletonGenAlgAPI::getInstance()->createDoubleRandomStrategy(&random, -100.0, 200.0, 0.0);
  // The prototypes need a name, a random strategy to create random genes and a mutation strategy to mutate existing genes.
  pro1 = SingletonGenAlgAPI::getInstance()->createPrototype("P1", randomStr, mutStr);
  pro2 = SingletonGenAlgAPI::getInstance()->createPrototype("P2", randomStr, mutStr);
  pro3 = SingletonGenAlgAPI::getInstance()->createPrototype("P3", randomStr, mutStr);
  pro4 = SingletonGenAlgAPI::getInstance()->createPrototype("P4", randomStr, mutStr);
  SingletonGenAlgAPI::getInstance()->insertGenPrototype(pro1);
  SingletonGenAlgAPI::getInstance()->insertGenPrototype(pro2);
  SingletonGenAlgAPI::getInstance()->insertGenPrototype(pro3);
  SingletonGenAlgAPI::getInstance()->insertGenPrototype(pro4);

  // At last we create all interesting measures (PlotOptions).
  opt1.setName("opt1");
  opt2.setName("opt2");
  SingletonGenAlgAPI::getInstance()->enableMeasure(opt1);
  SingletonGenAlgAPI::getInstance()->enableMeasure(opt2);
  optGen.setName("optGen");
  SingletonGenAlgAPI::getInstance()->enableGenContextMeasure(optGen);

  // Prepare the first generation:
  // We can use "run" for a automatically run or we must control all ourself like here!
  // So we must prepare the first generation, for this the algorithm must know how many individuals he should create,
  // how much will die on the end and if he should make an automatically update of the statistic values.
  // The automatically update isn't possible because before we need a run of the simulation, so we make it later ourself (param false)!
  SingletonGenAlgAPI::getInstance()->prepare((int) ((numberIndividuals - (numberIndividuals / 10)) / 2),
      numberIndividuals - (((int) ((numberIndividuals - (numberIndividuals / 10)) / 2)) * 2), &random, false);

  // 1. create your own deduced SimulationTaskHandle
  ThisSimulationTaskHandle simTaskHandle;
  // 2. create your ThisSimCreator
  ThisSimCreator simCreator;
  // 3. set simTaskHandle and simCreator
  SimulationTaskSupervisor::setSimTaskHandle(simTaskHandle);
  SimulationTaskSupervisor::setTaskedSimCreator(simCreator);
  // 4. add needed data to your simTaskHandle
  simTaskHandle.fitnessStr = fitnessStr;


  // 100 generation iterating
  for (int x = 0; x < 10; x++) {
    // 4. add needed data to your simTaskHandle
    simTaskHandle.individuals = const_cast<std::vector<Individual*>*>(&SingletonGenAlgAPI::getInstance()->getEngine()->getActualGeneration()->getAllIndividual());
    simTaskHandle.numberIndividuals = numberIndividuals;

    // 5. create the SimulationTasks
    // just add another task pool and run this ones
    char buffer[15];
    sprintf(buffer, "taskpool %i", x);
    SimulationTaskSupervisor::getInstance()->createSimTasks(numberIndividuals);
    SimulationTaskSupervisor::getInstance()->setSimTaskNameSuffix(buffer);
    SimulationTaskSupervisor::getInstance()->runSimTasks(&argc, argv);

    RandGen random; // a random generator

    // Step in the algorithm:
    // - update the statistical values inside the genetic algorithm
    // - make a step in the measure
    // - select the individual which will be killed by use of their statistical values.
    // - and generate new individuals
    SingletonGenAlgAPI::getInstance()->update();
    SingletonGenAlgAPI::getInstance()->measureStep(x + 1);
    SingletonGenAlgAPI::getInstance()->select();
    SingletonGenAlgAPI::getInstance()->crossover(&random);
  }
}

