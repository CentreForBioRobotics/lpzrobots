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
 *   Revision 1.6  2006-08-31 07:34:13  robot8
 *   -temporary disabling a part oif the replication function
 *
 *   Revision 1.5  2006/08/21 11:50:59  robot8
 *   -added some commemts
 *   -update of atomcomponent
 *
 *   Revision 1.4  2006/07/25 11:29:56  robot8
 *   -test-update of atomcomponent (not working yet)
 *   -added backward reference feature for component softlinks
 *   -removed recusive destruction for fissed component-subtrees
 *
 *   Revision 1.3  2006/07/18 09:23:22  robot8
 *   -atomcomponent update
 *   -coloring simulation
 *   -one bug left: softlink removal from higher branches of the tree could not removed
 *
 *   Revision 1.1.2.4  2006/07/14 13:01:41  robot8
 *   -atomcomponent update
 *   -one bug left: softlink removal from higher branches of the tree could not removed
 *
 *   Revision 1.1.2.3  2006/07/07 07:40:16  robot8
 *   -some bugs removed to components and robots
 *   -some smaller functions implemented in component system
 *   -atomcomponents are functional, but still could not replicate
 *
 *   Revision 1.1.2.2  2006/06/16 12:07:39  robot8
 *   -component update
 *   -atomcomponents now could use fusion and fission
 *   -but still fission does not work all correctly
 *
 *   Revision 1.1.2.1  2006/06/12 12:59:08  robot8
 *   -some corrections to component system (for example now connections are only pushed as pointers to the connection vector)
 *   -created the evolution simulation for evolutionary evolution algorithms with atom like structures
 *
 *
 ***************************************************************************/

#include "simulation.h"

#include "atomodeagent.h"
#include "octaplayground.h"
#include "passivesphere.h"

#include <selforg/invertnchannelcontroller.h>
#include <selforg/invertmotorspace.h>
#include <selforg/invertmotornstep.h>
#include <selforg/sinecontroller.h>
#include <selforg/noisegenerator.h>
#include <selforg/one2onewiring.h>
#include <selforg/derivativewiring.h>

#include "sphererobot.h"
#include "schlangeservo.h"
#include "sphererobot3masses.h"

#include "atomcomponent.h"

#include <vector.h>


//physical definition part
#define REACTIONROOMWIDTH 1
#define REACTIONROOMLENGTH 1
//both density values ahve to be bigger than 1 so, that the atoms do not colide at the beginning
#define ATOMDENSITY 6
 //this is the space between two atoms, bigger means less robots and smaller more atoms
#define ROBOTDENSITY 10 //this is the space between two robots-atoms, bigger means less robots and smaler more robot-atoms
#define ROBOTHEIGHT 1.5
#define SHIFTBETWEENATOMSANDROBOTS_X 0.1
#define SHIFTBETWEENATOMSANDROBOTS_Y 0.1

//controller parameters


//evolution definition part
#define MINFITTNESS 1

// fetch all the stuff of lpzrobots into scope
using namespace lpzrobots;

class Evolution : public Simulation 
{

public:
    vector <Component*> components;
    vector <repSlider> replicationSlider;

public:

  // starting function (executed once at the beginning of the simulation loop)
  void start(const OdeHandle& odeHandle, const OsgHandle& osgHandle, GlobalData& global) 
  {

    setCameraHomePos ( Pos ( 0, 0, 7), Pos ( -0.0828247 , -89.9146 , 0) );
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
    OctaPlayground* playground = new OctaPlayground(odeHandle, osgHandle, osg::Vec3( REACTIONROOMWIDTH , 0.2 , REACTIONROOMLENGTH ) , 4 );
    playground->setPosition(osg::Vec3(0,0,-0.1)); // playground positionieren und generieren
    global.obstacles.push_back(playground);

    //****************

    ComponentConf cConf = Component::getDefaultConf ();
    cConf.max_force = 5;

    AtomConf aConf = AtomComponent::getDefaultAtomConf ();
    aConf.core_radius = 0.05;
    aConf.shell_radius = 0.1;
    aConf.mass = 1;
    aConf.max_bindings = 4;
    aConf.binding_energy = 0.1;
    aConf.min_fission_energy = 3;

//adding the controller for the component-connections

    InvertMotorNStepConf cc = InvertMotorNStep::getDefaultConf();

    cc.cInit=2;
    AbstractController* controller;
    DerivativeWiringConf c = DerivativeWiring::getDefaultConf ();
    DerivativeWiring* wiring;
    AtomOdeAgent* agent;

    OsgHandle osgHandle_2 = osgHandle.changeColor ( Color ( 0 , 156/255.0 , 2 ) );


    for ( double x = -REACTIONROOMWIDTH/2; x <= REACTIONROOMWIDTH/2; x += ROBOTDENSITY*(2*aConf.shell_radius) )
	for ( double y = -REACTIONROOMLENGTH/2; y <= REACTIONROOMLENGTH/2; y += ROBOTDENSITY*(2*aConf.shell_radius) +1)
	{
	    //atom
	    components.push_back ( new AtomComponent ( odeHandle , osgHandle_2 , cConf , aConf ) );
	    components.back ()->place ( Pos( x + SHIFTBETWEENATOMSANDROBOTS_X , y + SHIFTBETWEENATOMSANDROBOTS_Y , aConf.shell_radius*2 + ROBOTHEIGHT )); 

	    ((AtomComponent*) components.back ())->atomconf.leadingatom = true;
	    //controller
	    controller = new InvertMotorNStep ( cc );
	    controller->setParam ("adaptrate", 0.005);
	    controller->setParam ("epsC", 0.005);
	    controller->setParam ("epsA", 0.001);
	    controller->setParam ("rootE", 3);
	    controller->setParam ("steps", 2);
	    controller->setParam ("s4avg", 5);
	    controller->setParam ("factorB",0);
//	    controller = new SineController ( 18 );
	    //wiring
	    wiring = new DerivativeWiring ( c , new ColorUniformNoise () );   
	    //agent
	    agent = new AtomOdeAgent ( plotoptions );
	    agent->init ( controller , components.back () , wiring );
	    global.agents.push_back ( agent );
	    global.configs.push_back ( controller );
	}
    
//unbound atoms

    osgHandle_2 = osgHandle.changeColor ( Color ( 2 , 156/255.0 , 0 ) );

    for ( double x = -REACTIONROOMWIDTH/2.0; x <= REACTIONROOMWIDTH/2.0; x += ATOMDENSITY*(2*aConf.shell_radius) )
	for ( double y = -REACTIONROOMLENGTH/2.0; y <= REACTIONROOMLENGTH/2.0; y += ATOMDENSITY*(2*aConf.shell_radius) )
	{
	    components.push_back ( new AtomComponent ( odeHandle , osgHandle_2 , cConf , aConf ) );
	    components.back ()->place ( Pos( x , y , 2*aConf.shell_radius ) ); 
	}

    showParams(global.configs);
    replicationSlider.empty ();

  } 

    void addCallback ( GlobalData& globalData , bool draw , bool pause )
	{
	    for ( unsigned int n = 0; n < components.size (); n++ )
		components[n]->update (); //not realy perfect, because the atoms belonging to robots are drawn an additional time by the agents

	    //only if there are replication sliders allocated at the moment
//	    cout<<&replicationSlider<<"\n";
//	    cout<<replicationSlider.size ()<<"\n";
	    for ( unsigned int n = 0; n < replicationSlider.size (); n++ )
	    {
		cout<<"addCallback "<<replicationSlider.size ()<<"\n";
		replicationSlider[n].slider->update (); //not realy perfect, because the atoms belonging to robots are drawn an additional time by the agents
		replicationSlider[n].bindingcounter--;

		if ( replicationSlider[n].startComponent->getDistanceToComponent ( replicationSlider[n].endComponent ) < 0.3 || replicationSlider[n].bindingcounter == 0 )
		{
		    //adding the new controllers for the two structures that do not have one up till now, that means the ones that have now leading atom
		    if ( ((AtomComponent*) replicationSlider[n].startComponent)->atomconf.leadingatom == false )
		    {
			((AtomComponent*) replicationSlider[n].startComponent)->makeComponentStructureRoot (); //to be shure that the structure stays correct when we create a new controller
			InvertMotorNStepConf cc = InvertMotorNStep::getDefaultConf();
			cc.cInit=2;
			AbstractController* controller;
			DerivativeWiringConf c = DerivativeWiring::getDefaultConf ();
			DerivativeWiring* wiring;
			AtomOdeAgent* agent;

			//controller
			controller = new InvertMotorNStep ( cc );
			controller->setParam ("adaptrate", 0.005);
			controller->setParam ("epsC", 0.005);
			controller->setParam ("epsA", 0.001);
			controller->setParam ("rootE", 3);
			controller->setParam ("steps", 2);
			controller->setParam ("s4avg", 5);
			controller->setParam ("factorB",0);
			//wiring
			wiring = new DerivativeWiring ( c , new ColorUniformNoise () );   
			//agent
			agent = new AtomOdeAgent ( plotoptions );
			agent->init ( controller ,  replicationSlider[n].startComponent , wiring );
			globalData.agents.push_back ( agent );
			globalData.configs.push_back ( controller );

			((AtomComponent*) replicationSlider[n].startComponent)->atomconf.leadingatom = true;
		    }
		    
		    ((AtomComponent*) replicationSlider[n].startComponent->originComponent)->enableStructureFusionRecursive ();
		    ((AtomComponent*) replicationSlider[n].endComponent->originComponent)->enableStructureFusionRecursive ();

		    ((AtomComponent*) replicationSlider[n].startComponent)->fusion ( (AtomComponent*) replicationSlider[n].endComponent );
		    ((AtomComponent::connectionAddition*) replicationSlider[n].startComponent->connection.back().data)->binding_strength = replicationSlider[n].startingdistance;

		    delete replicationSlider[n].slider;
		    replicationSlider.erase ( replicationSlider.begin () + n );
		}
	    }



	    /**********************************************************************************************/

	    //	    evolution loop
	    //      calculate fitness

/*
	    
	    for ( unsigned int n = 0; n < globalData.agents.size (); n++ )
	    {
		if ( globalData.agents[n]->getController () < MINFITTNESS ) //is there a param for E?
		{
		    
		    ((AtomComponent*) globalData.agents[n]->getRobot ())->atomconf.leadingatom = false;

		    //deleting the controller from global configs
		    vector <Configurable*>::iterator eraseiterator = globalData.configs.begin ();
		    for ( int m = 0; ((InvertMotorNStep*) globalData.configs.at ( m )) == globalData.agents[n]->getController (); m++ )
			eraseiterator++;

		    globalData.configs.erase ( eraseiterator );

		    delete ( globalData.agents[n] );
		}
		
	    }
*/
	    //      delete Controller of selected component-trees -> these robots will die, number depends on global fitness Setings of the Simulation
	    //      physical structures are still within the simulation, so they could colide and be bound to other structures, maybe destruction of the structure? should be a parameter of the simulation

	    






	    /**********************************************************************************************/

	}


  // add own key handling stuff here, just insert some case values
  virtual bool command(const OdeHandle&, const OsgHandle&, GlobalData& globalData, int key, bool down)
  {
    if (down) { // only when key is pressed, not when released

		ComponentConf compConf = Component::getDefaultConf ();
		compConf.max_force = 5;

		AtomConf atConf = AtomComponent::getDefaultAtomConf ();
		atConf.core_radius = 0.05;
		atConf.shell_radius = 0.1;
		atConf.mass = 1;
		atConf.max_bindings = 4;
		atConf.binding_energy = 0.3;
		atConf.min_fission_energy = 2;
		
		int n = 3;

      switch ( (char) key )
	{
	    case 'Q':
		cout<<"atom added\n";

		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 0 , 0 , 0 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( -REACTIONROOMWIDTH/2.0 , REACTIONROOMLENGTH/2.0 , atConf.shell_radius + n )); 
		break;
	    case 'W':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 0 , 0 , 1 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( 0 , REACTIONROOMLENGTH/2.0 , atConf.shell_radius + n )); 
		break;
	    case 'E':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 0 ,  1 , 0 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( REACTIONROOMWIDTH/2.0 , REACTIONROOMLENGTH/2.0 , atConf.shell_radius + n )); 
		break;
	    case 'A':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 0 , 1 , 1 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( -REACTIONROOMWIDTH/2.0 , 0 , atConf.shell_radius + n )); 
		break;
	    case 'S':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 1 , 0 , 0 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( 0 , 0 , atConf.shell_radius + n )); 
		break;
	    case 'D':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 1 , 0 , 1 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( REACTIONROOMWIDTH/2.0 , 0 , atConf.shell_radius + n )); 
		break;
	    case 'Y':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 1 , 1 , 0 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( -REACTIONROOMWIDTH/2.0 , -REACTIONROOMLENGTH/2.0 , atConf.shell_radius + n )); 
		break;
	    case 'X':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 1 , 1 , 1 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( 0 , -REACTIONROOMLENGTH/2.0 , atConf.shell_radius + n )); 
		break;
	    case 'C':
		cout<<"atom added\n";
		components.push_back ( new AtomComponent ( odeHandle , osgHandle.changeColor ( Color ( 0.5 , 0.5 , 0.5 ) ) , compConf , atConf ) );
		components.back ()->place ( Pos( REACTIONROOMWIDTH/2.0 , -REACTIONROOMLENGTH/2.0 , atConf.shell_radius + n )); 
		break;
	    case 'R':
		cout<<"KEY REPLICATION\n";
		cout<<"replicationSlider-Adress: "<<&replicationSlider<<"\n";
		((AtomComponent*)components[0])->replication ( (AtomComponent*) components[1] /*, &replicationSlider*/ );
		cout<<&replicationSlider<<"\n";
		cout<<replicationSlider.size ()<<"\n";

		cout<<"END OF KEY REPLICATION\n";
		break;
	    case 'T':
		cout<<"KEY RESTRUCTURE TEST\n";
		cout<<"before\n";
		printComponentInfo ( components.back () );

		((AtomComponent*)components.back())->makeComponentStructureRoot ();
		
		cout<<"after\n";
		printComponentInfo ( components.back () );
		cout<<"END RESTRUCTURE TEST\n";
		break;


	    case 'I':
		cout<<"PRINTING INFORMATION\n";
		for ( unsigned int n = 0; n < globalData.agents.size (); n++ )
		{
		    cout<<"################################################Robot##"<<n<<"################################################\n";
		    printComponentInfo ( (Component*) globalData.agents[n]->getRobot() );
		}
		break;
	    case 'L':
		    cout<<"#####################################Complete Component List#############################################\n";
		for ( unsigned int n = 0; n < components.size (); n++ )
		{
		    cout<<"*******************Component**"<<components[n]<<"******************\n";   
		    cout<<"origin component: "<<((AtomComponent*) components[n] )->originComponent<<"\n";
		    cout<<"direct origin component: "<<((AtomComponent*) components[n] )->directOriginComponent<<"\n";
		    cout<<"leading atom: "<<((AtomComponent*) components[n] )->atomconf.leadingatom<<"\n";
		    cout<<"number of Subcomponents: "<<((AtomComponent*) components[n] )->getNumberSubcomponents ()<<"\n";
		    cout<<"number of all Subcomponents: "<<((AtomComponent*) components[n])->getNumberSubcomponentsAll ()<<"\n";
		    
		    for ( unsigned int m = 0; m < ((AtomComponent*) components[n])->connection.size (); m++ )
		    {
			cout<<"------------Connection-"<<m<<"------------\n";
			cout<<"softlink: "<<((AtomComponent*) components[n])->connection[m].softlink<<"\n";
			cout<<"data: "<<((AtomComponent*) components[n])->connection[m].data<<"\n";
			cout<<"subcomponent: "<<((AtomComponent*) components[n])->connection[m].subcomponent<<"\n";
		    }

		    for ( unsigned int m = 0; m < (components[n])->backwardreference.size (); m++ )
		    {
			cout<<"------------Backwardreference-"<<m<<"------------\n";
			cout<<"component: "<<components[n]->backwardreference[m]<<"\n";
			
		    }

		}
		break;
	    case 'M' :
		cout<<"#####################################connection vector capacity analysis#############################################\n";
		for ( unsigned int n = 0; n < components.size(); n++)
		    cout<<"Component "<<n<<" holds "<<components[n]->connection.size ()<<" and can hold "<<components[n]->connection.capacity ()<<" connections\n";
		break;

		break;


	    default:
		return false;
		break;

	}
    }
    return false;
  }

    void printComponentInfo ( Component* comp )
	{
	    cout<<"*******************Component**"<<comp<<"******************\n";   
	    cout<<"origin component: "<<((AtomComponent*) comp )->originComponent<<"\n";
	    cout<<"direct origin component: "<<((AtomComponent*) comp )->directOriginComponent<<"\n";
	    cout<<"leading atom: "<<((AtomComponent*) comp )->atomconf.leadingatom<<"\n";
	    cout<<"number of Subcomponents: "<<((AtomComponent*) comp)->getNumberSubcomponents ()<<"\n";
	    cout<<"number of all Subcomponents: "<<((AtomComponent*) comp)->getNumberSubcomponentsAll ()<<"\n";
	    
	    for ( unsigned int m = 0; m < ((AtomComponent*) comp)->connection.size (); m++ )
	    {
		cout<<"------------Connection-"<<m<<"------------\n";
		cout<<"softlink: "<<((AtomComponent*) comp)->connection[m].softlink<<"\n";
		cout<<"data: "<<((AtomComponent*) comp)->connection[m].data<<"\n";
		cout<<"subcomponent: "<<((AtomComponent*) comp)->connection[m].subcomponent<<"\n";


	    }

	    for ( unsigned int m = 0; m < ((AtomComponent*) comp)->backwardreference.size (); m++ )
	    {
		cout<<"------------Backwardreference-"<<m<<"------------\n";
		cout<<"component: "<<comp->backwardreference[m]<<"\n";

	    }

//	    cout<<"*********************************************************\n";   
	    
	    for ( unsigned int m = 0; m < ((AtomComponent*) comp)->connection.size (); m++ )
	    {
		if ( !comp->connection[m].softlink )
		    printComponentInfo ( comp->connection[m].subcomponent );

	    }

	}


  /**
   *fitness calculation
   **/
  double calculateFitness ()
  {
      return 0;
  }
  

};



int main (int argc, char **argv)
{ 
  Evolution sim;
  // run simulation
  return sim.run(argc, argv) ? 0 : 1;
}
