/************************************************************************/
/*shpererobot.h								*/
/*Schlangenkonstrukt fuer das ODE-Robotersystem des Authors		*/
/*@author Marcel Kretschmann						*/
/*@version alpha 0.1							*/
/*									*/
/************************************************************************/
#ifndef __SPHEREROBOTTest_H
#define __SPHEREROBOTTest_H


#include "sphererobot.h"
#include "sliderservo.h"
#include "roboter.h"

/**
 *This is a class, which models a snake like robot. It consists of a number of equal elements, each linked 
 *by a joint. This class is based upon the class roboter by the same author.
 *@author Marcel Kretschmann
 *@version beta
 **/
class SphererobotTest : public Roboter
{
 public:
  typedef enum objects { Base, Pendular, Pole1Bot, Pole2Bot, Pole3Bot, 
			 Pole1Top , Pole2Top, Pole3Top};

private:
  
  //std::vector<dJointID> skyJoints; // for fixing segment 0 in the sky
  dSpaceID sphererobot_space;
  
  vector<PID*> motorliste2;
  
public:
  SliderServo* servo;
protected:
  SphererobotConf conf;
  double lastJointPos[3];

public:

  //SphererobotTest ();

  /**
   *constructor
   *@param startRoboterID ID, which should be managed clearly
   *@author Marcel Kretschmann
   *@version beta
   **/ 
  SphererobotTest ( int startRoboterID , const ODEHandle& odeHandle, 
		const SphererobotConf& conf );
	
  /**
   *Destruktor
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual ~SphererobotTest();
	
  static SphererobotConf getStandartConf(){
    SphererobotConf c;
    c.diameter     = 1;
    c.spheremass   = 0.001;
    c.pendulardiameter = 0.2;
    c.pendularmass = 0.3;
    c.slidermass   = 0.0001;
    c.sliderrange  = 0.1; // range of the slider from center in multiple of diameter [-range,range]
    c.hingeRange   = M_PI/180*45;
    return c;
  }

  /**
   *Draws the geometrical objects of the robot.
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual void draw();
	
  /**Sets the sphere to position pos, sets color to c, and creates sphere if necessary.
   *This overwrides the function place of the class robot.
   *@param pos desired position of the snake in struct Position
   *@param c desired color for the snake in struct Color
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual void place (Position pos, Color *c = 0);
	
  static void mycallback(void *data, dGeomID o1, dGeomID o2);
  
  /**
   *This is the collision handling function for snake robots.
   *This overwrides the function collisionCallback of the class robot.
   *@param data
   *@param o1 first geometrical object, which has taken part in the collision
   *@param o2 second geometrical object, which has taken part in the collision
   *@return true if the collision was threated  by the robot, false if not
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual bool collisionCallback(void *data, dGeomID o1, dGeomID o2);
	
  /**
   *Writes the sensor values to an array in the memory.
   *@param sensor* pointer to the array
   *@param sensornumber length of the sensor array
   *@return number of actually written sensors
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual int getSensors ( sensor* sensors, int sensornumber );
	
  /**
   *Reads the actual motor commands from an array, an sets all motors of the snake to this values.
   *It is an linear allocation.
   *@param motors pointer to the array, motor values are scaled to [-1,1] 
   *@param motornumber length of the motor array
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual void setMotors ( const motor* motors, int motornumber );
	
  /**
   *Returns the number of motors used by the snake.
   *@return number of motors
   *@author Marcel Kretschmann
   *@version final
   **/
  virtual int getMotorNumber();
  
  /**
   *Returns the number of sensors used by the robot.
   *@return number of sensors
   *@author Marcel Kretschmann
   *@version final
   **/
  virtual int getSensorNumber();
	
  /**
   *Updates the sensorarray.
   *This overwrides the function sensoraktualisierung of the class robot
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual void sensoraktualisierung ( );
	
  /**
   *Returns the position of the snake. Here the position of the snake is the position of the first element of the snake.
   *@return Position (x,y,z)
   *@author Marcel Kretschmann
   *@version final
   **/
  virtual Position getPosition ();
	
  /**
   *Prints some internal robot parameters. Actualy it prints all sensor data of one callculation step.
   *@author Marcel Kretschmann
   *@version beta
   **/
  virtual void getStatus ();

};

#endif
