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
 *   Revision 1.9.4.6  2006-03-30 12:34:57  martius
 *   documentation updated
 *
 *   Revision 1.9.4.5  2006/01/10 15:10:24  martius
 *   fine tuning, fuer controllinterval 1
 *   still not transparent
 *
 *   Revision 1.9.4.4  2005/12/30 22:51:45  martius
 *   moved to osg
 *
 *   Revision 1.9.4.3  2005/11/16 11:26:53  martius
 *   moved to selforg
 *
 *   Revision 1.9.4.2  2005/11/15 12:29:27  martius
 *   new selforg structure and OdeAgent, OdeRobot ...
 *
 *   Revision 1.9.4.1  2005/11/14 17:37:18  martius
 *   moved to selforg
 *
 *   Revision 1.9  2005/11/09 13:24:42  martius
 *   added GPL
 *
 *                                                                 *
 ***************************************************************************/

#ifndef __SPHEREROBOT_H
#define __SPHEREROBOT_H

#include "oderobot.h"
#include "sliderservo.h"

namespace lpzrobots {

  class Primitive;
  class Joint;
  class SliderJoint;
  class SliderServo;  

  typedef struct {
  public:
    double diameter;
    double spheremass;
    double pendulardiameter;
    double pendularmass;
    double slidermass;
    double sliderrange;
  
    double force;      // forcefactor of the servo power (1 is usual)
    double hingeRange; //the angle (in rad) of the hinges that connect pendular with poles
  } SphererobotConf;


  /**
   *This is a class, which models a snake like robot. It consists of a number of equal elements, each linked 
   *by a joint. This class is based upon the class roboter by the same author.
   *@author Marcel Kretschmann
   *@version beta
   **/
  class Sphererobot : public OdeRobot
  {
  public:
    typedef enum objects { Base, Pendular, Pole1Bot, Pole2Bot, Pole3Bot, 
			   Pole1Top , Pole2Top, Pole3Top, Last};

  protected:
    const static int sensorno = 9;
  
    SphererobotConf conf;
    bool created;

  public:
    SliderServo* servo[3];
    Primitive* object[Last];
    SliderJoint* slider[3];
    Joint* joint[6];

  public:

    Sphererobot ( const OdeHandle& odeHandle, const OsgHandle& osgHandle, 
		  const SphererobotConf& conf, const char* name );
	
    virtual ~Sphererobot();
	
    static SphererobotConf getDefaultConf(){
      SphererobotConf c;
      c.diameter     = 1;
      c.spheremass   = 0.2;
      c.pendulardiameter = 0.2;
      c.pendularmass = 1.0;
      c.slidermass   = 0.005;
      c.sliderrange  = 0.1; // range of the slider from center in multiple of diameter [-range,range]
      c.force        = 1;
      c.hingeRange   = M_PI/180*30;
      return c;
    }

    /// update the subcomponents
    virtual void update();
	
    /** sets the pose of the vehicle
	@param pose desired 4x4 pose matrix
    */
    virtual void place(const osg::Matrix& pose);
  
    /**
     *This is the collision handling function for snake robots.
     *This overwrides the function collisionCallback of the class robot.
     *@param data
     *@param o1 first geometrical object, which has taken part in the collision
     *@param o2 second geometrical object, which has taken part in the collision
     *@return true if the collision was threated  by the robot, false if not
     **/
    virtual bool collisionCallback(void *data, dGeomID o1, dGeomID o2);
    /** this function is called in each timestep. It should perform robot-internal checks, 
	like space-internal collision detection, sensor resets/update etc.
	@param globalData structure that contains global data from the simulation environment
    */
    virtual void doInternalStuff(const GlobalData& globalData);
	
    /**
     *Writes the sensor values to an array in the memory.
     *@param sensors pointer to the array
     *@param sensornumber length of the sensor array
     *@return number of actually written sensors
     **/
    virtual int getSensors ( sensor* sensors, int sensornumber );
	
    /**
     *Reads the actual motor commands from an array, an sets all motors of the snake to this values.
     *It is an linear allocation.
     *@param motors pointer to the array, motor values are scaled to [-1,1] 
     *@param motornumber length of the motor array
     **/
    virtual void setMotors ( const motor* motors, int motornumber );
	
    /**
     *Returns the number of motors used by the snake.
     *@return number of motors
     **/
    virtual int getMotorNumber();
  
    /**
     *Returns the number of sensors used by the robot.
     *@return number of sensors
     **/
    virtual int getSensorNumber();
	
    /** returns a vector with the positions of all segments of the robot
    */
    virtual int getSegmentsPosition(vector<Position> &poslist);	

  protected:
    /** the main object of the robot, which is used for position and speed tracking */
    virtual Primitive* getMainPrimitive() const { return object[Base]; }

    /** creates vehicle at desired pose
	@param pose 4x4 pose matrix
    */
    virtual void create(const osg::Matrix& pose); 
    virtual void destroy(); 
    


  };

}

#endif
