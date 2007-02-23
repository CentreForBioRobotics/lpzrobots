/**************************************************************************
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
 ***************************************************************************/


#ifndef __ARM_H
#define __ARM_H

#include <selforg/inspectable.h>
#include <list>
#include <vector>

#include <string.h>
#include <iostream>

#include <selforg/matrix.h>
#include "oderobot.h"
// include primitives (box, spheres, cylinders ...)
#include "primitive.h"
#include "oneaxisservo.h"
#include "joint.h"

#include<math.h>

#include "osgprimitive.h"

using namespace matrix;

namespace lpzrobots{

  /* Enumeration of different parts and joints */
  enum parts 
	{
	 	base,
		shoulder1,
		shoulder2,
		upperArm,
	 	foreArm,
		hand
	};

  typedef struct 
	{
		double body_mass;
		double body_height;
		double body_width;
		double body_depth;

		double shoulder_mass;
		double shoulder_radius;
		double joint_offset; // distance of shoulder components from each other	
		
		double upperarm_mass;
		double upperarm_radius;
		double upperarm_length;
		
		double forearm_mass;
		double forearm_radius;
		double forearm_length;

		double elevation_min;
		double elevation_max;
		double humeral_min;
		double humeral_max;
		double azimuthal_min;
		double azimuthal_max;
		double elbow_min;
		double elbow_max;

		double motorPower;
	  double damping;    // motor damping
		double servoFactor; // reduces servo angle constraints to servoFactor percent of hingeJoint angle constraints

		bool displayTarget; // true - display sphere at given targetPos
		double targetRadius;
		double* targetPos; // position of target sphere
		
  } ArmConf;

  class Arm : public OdeRobot, public Inspectable {
  public:
		  
		Arm(const OdeHandle& odeHandle, const OsgHandle& osgHandle, const ArmConf& conf, const std::string& name);

		static ArmConf getDefaultConf()
	  {
			ArmConf conf;		 
			
			conf.motorPower=2;//1.0;
			conf.damping=0.1;//1.0;
			conf.upperarm_radius = 0.05;//0.15; <- not beautiful
      conf.forearm_radius = 0.05;//0.1; <- not beautiful TODO universelle Anordnung!
			
			// body
			conf.body_mass = 1.0;
			conf.body_height = 5.0;
			conf.body_width = 2.0;
			conf.body_depth = 0.5;
			// shoulder
			conf.shoulder_mass=0.005;
			conf.shoulder_radius=0.03; // 0.1
			conf.joint_offset=0.005;
      // upper arm
			conf.upperarm_mass = 0.1; // 0.01
			conf.upperarm_length = 1.5;
			// forearm
			conf.forearm_mass = 0.1; // 0.01
      conf.forearm_length = 1.2;
			// stops at hinge joints 
			conf.elevation_min=-M_PI/2;
			conf.elevation_max=M_PI/2;
			conf.humeral_min=-M_PI/2;//-4*M_PI/10;//-2*M_PI/3;
			conf.humeral_max=M_PI/2;//4*M_PI/10;//2*M_PI/3;
			conf.azimuthal_min=-M_PI/3;//-3*M_PI/4;
			conf.azimuthal_max=M_PI/3;
			conf.elbow_min=-M_PI/2;
			conf.elbow_max=M_PI/2;
			conf.servoFactor=0.8;

			conf.displayTarget=false;
			conf.targetRadius=0.5;
			conf.targetPos=NULL;
			
      return conf;
		}			
			
    virtual ~Arm(){};

		virtual paramkey getName() const {return "Arm";};
		
		/** 
		 * sets the pose of the vehicle
     * @param pose desired 4x4 pose matrix
		 */
    virtual void place(const osg::Matrix& pose);

		/** 
		 * update the subcomponents
		 */
		virtual void update();

		/**
		 * returns actual sensorvalues
		 * @param sensors sensors scaled to [-1,1]
		 * @param sensornumber length of the sensor array
		 * @return number of actually written sensors
		 */
    virtual int getSensors(sensor* sensors, int sensornumber);

		/** 
		 * sets actual motorcommands
		 * @param motors motors scaled to [-1,1]
		 * @param motornumber length of the motor array
     */
    virtual void setMotors(const motor* motors, int motornumber);

		/** 
		 * returns number of sensors
		 */
		virtual int getSensorNumber()
		{
			return sensorno;
		};
				
		/** 
		 * returns number of motors
		 */
		virtual int getMotorNumber()
		{
			return motorno;
		};

		/**
		 * transform position in external coordinate space into position in shoulder centered coordinate space
		 * (second shoulder part assumed to be origin)
		 * e.g. used to create a proper target in distal learning
		 * @param list containing  x,y,z-coordinate
		 */
		void scaleShoulderCentered(double* pos)
		{
			// TODO ensure that not less than 3 values
			osg::Vec3 origin = objects[shoulder2]->getPosition();	
			pos[0]=(pos[0]-origin[0])*factorSensors;
			pos[1]=(pos[1]-origin[1])*factorSensors;
			pos[2]=(pos[2]-origin[2])*factorSensors;
		}
		
		/** 
		 * returns a vector with the positions of all segments of the robot
		 * @param poslist vector of positions (of all robot segments)
		 * @return length of the list
		 */
		virtual int getSegmentsPosition(std::vector<Position> &poslist);

		/**
		 * returns the position of the endeffector (hand)
		 * @param position vector position vector
		 */
		void getEndeffectorPosition(double* position);
		
		virtual bool collisionCallback(void *data, dGeomID o1, dGeomID o2);

		/** 
		 * this function is called in each timestep. It should perform robot-internal checks,
		 * like space-internal collision detection, sensor resets/update etc.
		 * @param globalData structure that contains global data from the simulation environment
		 */
		virtual void doInternalStuff(const GlobalData& globalData);

		/** 
		 * The list of all parameters with there value as allocated lists.
		 */
		paramlist getParamList() const;

		virtual paramval getParam(const paramkey& key) const;

    virtual bool setParam(const paramkey& key, paramval val);

    virtual Primitive* getMainObject() const;
		
		int getHitCounter()
		{
			return hitCount;
		}
	
		void resetHitCounter()
		{
			hitCount=0;
		}
		
	protected:
  	/**
		 * the main object of the robot, which is used for position and speed tracking 
		 */
		virtual Primitive* getMainPrimitive() const 
		{ 
			//return objects[hand]; 
			return objects[base]; 
		}

		/** 
		 * creates vehicle at desired pose
		 * @param pose 4x4 pose matrix
		 * @param snowmanmode snowman body
		 */
		virtual void create(const osg::Matrix& pose);

		/** 
		 * destroys vehicle and space
		 */
		virtual void destroy();

		static void mycallback(void *data, dGeomID o1, dGeomID o2);

		void hitTarget();
		
    double dBodyGetPositionAll ( dBodyID basis , int para );
		double dGeomGetPositionAll ( dGeomID basis , int para );

		void BodyCreate(int n, dMass m, dReal x, dReal y, dReal z, dReal qx, dReal qy, dReal qz, dReal qangle);

		// inspectable interface
		virtual std::list<Inspectable::iparamkey> getInternalParamNames() const;
		virtual std::list<Inspectable::iparamval> getInternalParams() const;
//		virtual std::list<ILayer> getStructuralLayers() const;
//		virtual std::list<IConnection> getStructuralConnections() const;

		
		ArmConf conf;    
		matrix::Matrix endeff;
		
		paramval factorSensors;
		paramval print;
		
		std::vector <Primitive*> objects; // Primitive* object[NUMParts]; 
		std::vector <Joint*> joints; // Joint* joint[NUMJoints]; 
		std::vector <HingeServo*> hingeServos;
	
		int hitCount;
		
		int sensorno;      // number of sensors
		int motorno;       // number of motors
		
		bool created;      // true if robot was created
	
		bool red;
		
		dSpaceID parentspace;
		
		int printed;
		
	};
}
#endif
