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
 *   Revision 1.2  2007-09-12 14:25:44  fhesse
 *   collisionCallback() emtied
 *   comments added
 *   started cleaning up
 *
 *   Revision 1.1  2007/09/12 11:23:11  fhesse
 *   moved from simulation/hand to here (to oderobots/robot)
 *
 *   Revision 1.4  2007/07/05 11:20:02  robot6
 *   hingeservo.h substituted by oneaxismotor.h (includes Hingeservo)
 *   "hand" added in Makefile
 *
 *   Revision 1.3  2007/05/07 09:07:21  robot3
 *   intended region for converting the code from nonreadable to human readable
 *
 *   Revision 1.2  2006/09/21 16:15:57  der
 *   *** empty log message ***
 *
 *   Revision 1.6.4.9  2006/06/25 16:57:13  martius
 *   abstractrobot is configureable
 *   name and revision
 *
 *   Revision 1.6.4.8  2006/03/30 12:34:56  martius
 *   documentation updated
 *
 *   Revision 1.6.4.7  2006/03/30 08:43:05  fhesse
 *   getTracePrimitive removed
 *
 *   Revision 1.6.4.6  2006/03/28 14:20:28  fhesse
 *   getTracePrimitive() added
 *
 *   Revision 1.6.4.5  2005/12/30 22:54:38  martius
 *   removed parentspace!
 *
 *   Revision 1.6.4.4  2005/12/21 17:35:09  martius
 *   moved to osg
 *
 *   Revision 1.6.4.3  2005/11/16 11:26:52  martius
 *   moved to selforg
 *
 *   Revision 1.6.4.2  2005/11/15 12:29:26  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.6.4.1  2005/11/14 17:37:17  martius
 *   moved to selforg
 *
 *   Revision 1.6  2005/11/09 13:26:21  martius
 *   added factorSensor
 *
 *   Revision 1.5  2005/10/06 17:14:24  martius
 *   switched to stl lists
 *
 *   Revision 1.4  2005/09/22 12:24:37  martius
 *   removed global variables
 *   OdeHandle and GlobalData are used instead
 *   sensor prepared
 *
 *   Revision 1.3  2005/08/31 17:18:15  fhesse
 *   setTextures added, Mass is now sphere (not box anymore)
 *
 *   Revision 1.2  2005/07/26 17:04:21  martius
 *   lives in its own space now
 *
 *   Revision 1.1  2005/07/21 12:17:04  fhesse
 *   new hurling snake, todo: add collision space, clean up, comment
 *
 *         
 *                                                                 *
 ***************************************************************************/
#ifndef __HAND_H
#define __HAND_H

//#include  <drawstuff/drawstuff.h>
#include "oderobot.h"
#include <selforg/configurable.h>
#include "primitive.h"
#include "joint.h"
#include "angularmotor.h"
#include "oneaxisservo.h"
//"hingeservo.h"

#include "primitive.h"
#include "osgforwarddecl.h"
#include "axis.h"

#include "irsensor.h"
#include "raysensorbank.h"

namespace lpzrobots {

  enum Motor_type
    {
      With_servo_motor,
      Without_servo_motor
    };

  enum IrSensor_Type{
    irDrawAll,
    irBack,
    irSide,
    irFront
  };

  enum Draw_Part_of_Ir_Sensor
    {
      Draw_All,
      Draw_just_Sensor,
      Draw_just_Ray,
      Draw_Nothing
    };
  // struct containing geom and body for each beam (= box, (cappped)cylinder, sphere)
  typedef struct {
    public:

double velocity;
    double power;       // used when non-servo motor is used
    double servo_motor_Power;  // used when servo motor is used
    double invert;
    double jointLimit1;
    double jointLimit2;
    double x;
    double y;
    double z;
    bool show_contacts;
    double thumb_angle;
    enum Motor_type set_typ_of_motor;
    double factorSensor;
    double finger_winkel;
    bool fix_palm_joint;
    bool one_finger_as_one_motor;


    //---------------InfrarRedSensor--------------------------  
    //  enum IrSensor_Type set_irsensor_type;
    double irRange;
    int number_of_ir_sensors;
    bool ir_sensor_used;
    enum Draw_Part_of_Ir_Sensor Draw_part_of_ir_sensor;
  } HandConf;

  enum GripMode{
    lateral,
    precision
  };

  /**
   * Artificial Hand 
   * 
   */
  class Hand : public OdeRobot{
  public:

    /**
     * constructor of hand
     * @param odeHandle data structure for accessing ODE
     * @param osgHandle ata structure for accessing OSG
     * @param conf configuration of robot
     */
    Hand(const OdeHandle& odeHandle, const OsgHandle& osgHandle, const HandConf& conf, const std::string& name);

    static HandConf getDefaultConf()
      {
	HandConf conf;
	conf.velocity = 0.2;
	conf.power = 5;
	conf.servo_motor_Power = 0.1;
	conf.invert = 1; 
	conf.x =0;
	conf.y =0;
	conf.z =2;	
	conf.show_contacts = true;
	conf.jointLimit1 = M_PI/2;
	conf.jointLimit2 = 2*M_PI;
	conf.set_typ_of_motor = Without_servo_motor;
	conf.thumb_angle=0;
	//conf.set_irsensor_type=irDrawAll;    
	conf.irRange = 2;
	conf.ir_sensor_used=true;
	conf.number_of_ir_sensors = 0;
	conf.Draw_part_of_ir_sensor=Draw_just_Ray;
	conf.factorSensor=2.0;
	conf.finger_winkel=M_PI/2;
	conf.fix_palm_joint=true;
	conf.one_finger_as_one_motor=false;
	return conf;
      }

  
    /** 
     * update the subcomponents
     */
    virtual void update();

    /** 
     * sets the pose of the vehicle
     * @param pose desired 4x4 pose matrix
     */
    virtual void place(const osg::Matrix& pose);

    /** 
     * checks for internal collisions and treats them. 
     * In case of a treatment return true (collision will be ignored by other objects and the default routine)
     * else false (collision is passed to other objects and (if not treated) to the default routine).
     */
    virtual bool collisionCallback(void *data, dGeomID o1, dGeomID o2);

    /** this function is called in each timestep. It should perform robot-internal checks, 
	like space-internal collision detection, sensor resets/update etc.
	@param globalData structure that contains global data from the simulation environment
    */
    virtual void doInternalStuff(const GlobalData& globalData);
  

    /** returns actual sensorvalues
	@param sensors sensors scaled to [-1,1] 
	@param sensornumber length of the sensor array
	@return number of actually written sensors
    */
    virtual int getSensors(sensor* sensors, int sensornumber);

    /** sets actual motorcommands
	@param motors motors scaled to [-1,1] 
	@param motornumber length of the motor array
    */
    virtual void setMotors(const motor* motors, int motornumber);

    /** returns number of sensors
     */
    virtual int getSensorNumber();

    /** returns number of motors
     */
    virtual int getMotorNumber();

    /** returns a vector with the positions of all segments of the robot
	@param poslist vector of positions (of all robot segments) 
	@return length of the list
    */
    //  virtual int getSegmentsPosition(vector<Position> &poslist);
  
    /** 
     * The list of all parameters with their value as allocated lists.       
     */
    virtual paramlist getParamList() const;
  
    /** 
     * Returns the value of the given parameter.       
     * @param key name of the parameter
     */
    virtual paramval getParam(const paramkey& key) const;

    /** 
     * Sets the values of the given parameter.       
     * @param key name of the parameter
     * @param val value to which the parameter will be set
     */
    virtual bool setParam(const paramkey& key, paramval val);


  protected:
    /** 
     * Returns the palm as the main object of the robot, 
     * which is used for position and speed tracking.
     */
    virtual Primitive* getMainPrimitive() const {
      if(!objects.empty()){
	return (objects[1]);
      }else return 0;
    }


  private:

    /** 
     * creates the hand at the desired pose
     * @param pose 4x4 pose matrix
     */
    virtual void create(const osg::Matrix& pose); 

    /** 
     * destroys robot and space
     */
    virtual void destroy();

    static void mycallback(void *data, dGeomID o1, dGeomID o2);
       
    /** true if robot was created */
    bool created;      

  protected:

    /** configuration of hand */
    HandConf conf;

    /** vector containing Primitives */
    std::vector <Primitive*> objects;
    /** vector containing OSGPrimitives */
    std::vector <OSGPrimitive*> osg_objects;
    /** vector containing Primitivesinfrared sensors */
    std::vector <IRSensor*> ir_sensors;

    /** true if contact joint is created  */
    bool contact_joint_created;

    //std::vector <HingeServo*> servos;
    //objects.reserve(number_beams);

    /** vector of the joints used in hand */
    std::vector <Joint*> joints;

    /** vector of the angular motors */
    std::vector <AngularMotor*> frictionmotors;

    /** vector of the used hinge servos*/
    std::vector <HingeServo*> servos;

    /**  a collection of ir sensors */
    RaySensorBank irSensorBank; 


    Primitive* p;
    Joint* j;

    /** space containing the hand */
    dSpaceID hand_space;     

    //Beam beam[number_beams]; // array of elements (rectangle and cylinders)
    //dJointID joint[number_joints];  // array containg "normal" joints for connecting the elementsconf
    //dJointID fix_joint[number_fix_joints]; //joints for keeping index, middle, ring and little finger together to achieve mor prosthetic like motion

    /** motorjoint for actuating the forearm_palm joint (ball joint) */
    AngularMotor* palm_motor_joint;

    /** motorjoint for actuating the palm_thumb joint (ball joint) */
    AngularMotor* thumb_motor_joint;


    /** for handling lateral and precision grip modes */
    GripMode gripmode; 



    /** initial position of robot */
    Position initial_pos;    

    //    int NUM;	   /* number of beats */
    //    double MASS;	   /* mass of a beats */
    //    double RADIUS;   /* sphere radius */

    // Joint* joint[10];
    // Primitive* object[10];

    Pos oldp;

    int sensorno;
    int motorno;
    int sensor_number;
    paramval factorForce;
    paramval frictionGround;
    
    double velocity;
 
  };

}

#endif
 
