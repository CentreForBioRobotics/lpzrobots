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
 *   This is an adapter class to add sensors to existing robots            *
 *    without modifiing them.                                              *
 *                                                                         *
 *   $Log$
 *   Revision 1.2  2007-11-07 13:20:25  martius
 *   also motors can be added
 *
 *   Revision 1.1  2007/08/24 11:48:56  martius
 *   initial
 *
 *                                                                 *
 ***************************************************************************/
#ifndef __ADDSENSORS2ROBOTADAPTER__
#define __ADDSENSORS2ROBOTADAPTER__

#include "oderobot.h"
#include "sensor.h"
#include "motor.h"

namespace lpzrobots {

  /** Robot Adapter to add sensors to robots
  */
  class AddSensors2RobotAdapter : public OdeRobot {
  public:
  
    /**
     * constructor of adapter
     * @param robot robot the wrap and plug sensors in
     * @param senors list of sensors
     */
    AddSensors2RobotAdapter( const OdeHandle& odeHandle, const OsgHandle& osgHandle, 
			     OdeRobot* robot, 
			     const std::list<Sensor*>& sensors = std::list<Sensor*>(),
			     const std::list<Motor*>& motors  = std::list<Motor*>(), 
			     bool sensors_before_rest = false);

    virtual ~AddSensors2RobotAdapter();
    
    /// adds a sensor to the robot. Must be called before placement of the robot, otherwise it has no affect
    virtual void addSensor(Sensor* sensor);

    /// adds a motor to the robot. Must be called before placement of the robot, otherwise it has no affect
    virtual void addMotor(Motor* motor);

    virtual void update(){ robot->update(); }

    virtual void place(const osg::Matrix& pose);

    virtual bool collisionCallback(void *data, dGeomID o1, dGeomID o2){
      return robot->collisionCallback(data,o1,o2);
    }

    virtual int getSensorNumber();
    virtual int getSensors(sensor* sensors_, int sensornumber);

    virtual int getMotorNumber();
    virtual void setMotors(const motor* motors_, int motornumber);

    void doInternalStuff(GlobalData& globalData);

    virtual Primitive* getMainPrimitive() const { return robot->getMainPrimitive();}
      
  protected:
    OdeRobot* robot;
    std::list<Sensor*> sensors;
    std::list<Motor*> motors;
    bool sensors_before_rest;
  };

}

#endif
