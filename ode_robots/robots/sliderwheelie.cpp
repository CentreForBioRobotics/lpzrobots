/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    frankguettler@gmx.de                                                 *
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
 *   Revision 1.4  2006-09-20 12:56:17  martius
 *   Snakes have CreateSegment
 *
 *   Revision 1.3  2006/07/20 17:19:44  martius
 *   removed using namespace std from matrix.h
 *
 *   Revision 1.2  2006/07/14 12:23:42  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.4  2006/06/26 20:33:50  robot5
 *   Joint adjustments.
 *
 *   Revision 1.1.2.3  2006/06/25 21:57:20  martius
 *   abstractrobot is configureable
 *   name and revision
 *
 *   Revision 1.1.2.2  2006/06/20 07:18:29  robot3
 *   -added cvs log
 *   -changed some behaviour of wheelie
 *
 *
 ***************************************************************************/

#include "sliderwheelie.h"
using namespace std;

namespace lpzrobots {

  SliderWheelie::SliderWheelie(const OdeHandle& odeHandle, const OsgHandle& osgHandle,
			     const SliderWheelieConf& conf, const std::string& name) 
    : DefaultSliderWheelie(odeHandle, osgHandle, conf, name, "$Id$")
  {
  }
	
  SliderWheelie::~SliderWheelie() {
  }
	

  /**
   *Reads the actual motor commands from an array, and sets all motors (forces) of the snake to this values.
   *It is a linear allocation.
   *@param motors pointer to the array, motor values are scaled to [-1,1] 
   *@param motornumber length of the motor array
   **/
  void SliderWheelie::setMotors(const motor* motors, int motornumber) {
   assert(created);
   unsigned int len = min(motornumber, getMotorNumber())/2;
   // controller output as torques 
   for(unsigned int i=0; (i<len) && (i<hingeServos.size()); i++) {
    hingeServos[i]->set(motors[i]);
   }
   for(unsigned int i=0; (i<len) && (i<sliderServos.size()); i++) {
    sliderServos[i]->set(motors[i]);
   }
  }

  /**
   *Writes the sensor values to an array in the memory.
   *@param sensor* pointer to the arrays

   *@param sensornumber length of the sensor array
   *@return number of actually written sensors
   **/
  int SliderWheelie::getSensors(sensor* sensors, int sensornumber) {
   assert(created);
   unsigned int len=min(sensornumber, getSensorNumber());
   // get the hingeServos
   for(unsigned int n=0; (n<len) && (n<hingeServos.size()); n++) {
    sensors[n] = hingeServos[n]->get();
   }
   for(unsigned int n=0; (n<len) && (n<sliderServos.size()); n++) {
    sensors[n] = sliderServos[n]->get();
   }
   return len;
  }


  /** creates vehicle at desired position 
      @param pos struct Position with desired position
  */
  void SliderWheelie::create(const osg::Matrix& pose) {
   DefaultSliderWheelie::create(pose);
   //*****************joint definition***********
   for(int n=0; n<conf.segmNumber-1; n++) {
    const Pos& p1(objects[n]->getPosition());
    const Pos& p2(objects[n+1]->getPosition());

    if(n%2==1) {
     HingeJoint* j = new HingeJoint(objects[n], objects[n+1],
 					   (p1 + p2)/2,
					   Axis(0,1,0)*pose);
     j->init(odeHandle, osgHandle, false, conf.segmDia/2*1.02);

     // setting stops at hinge joints
     j->setParam(dParamLoStop, -conf.jointLimit);
     j->setParam(dParamHiStop,  conf.jointLimit);
     joints.push_back(j);

     HingeServo* servo = new HingeServo(j, -conf.jointLimit, conf.jointLimit, conf.motorPower);
     hingeServos.push_back(servo);
    } else {

     SliderJoint* j = new SliderJoint(objects[n], objects[n+1],
 					   (p1 + p2)/2,
					   Axis(cos(6.28*n/conf.segmNumber+1.96),0,sin(6.28*n/conf.segmNumber+1.96))*pose);
     j->init(odeHandle, osgHandle, false, conf.segmDia/2*1.02);

     // setting stops at slider joints
     j->setParam(dParamLoStop, -conf.segmLength*conf.sliderLength*0.2);
     j->setParam(dParamHiStop,  conf.segmLength*conf.sliderLength*0.8);
     joints.push_back(j);

     SliderServo* servo = new SliderServo(j, 
					  -conf.segmLength*conf.sliderLength*0.15, 
					  conf.segmLength*conf.sliderLength*0.75, 
					  conf.motorPower);
     sliderServos.push_back(servo);

    }
   }

   // connecting first and last segment
   const Pos& p1(objects[conf.segmNumber-1]->getPosition());
   const Pos& p2(objects[0]->getPosition());

   HingeJoint* j = new HingeJoint(objects[conf.segmNumber-1], objects[0],
				  (p1 + p2)/2,
 				  Axis(0,1,0)*pose);
   j->init(odeHandle, osgHandle, false, conf.segmDia/2*1.02);

   // setting stops at hinge joints
   j->setParam(dParamLoStop, -conf.jointLimit);
   j->setParam(dParamHiStop,  conf.jointLimit);
   joints.push_back(j);

   HingeServo* servo = new HingeServo(j, -conf.jointLimit, conf.jointLimit, conf.motorPower);
   hingeServos.push_back(servo);

  }

  bool SliderWheelie::setParam(const paramkey& key, paramval val) {
   bool rv = DefaultSliderWheelie::setParam(key, val);
   for(vector<HingeServo*>::iterator i=hingeServos.begin(); i!=hingeServos.end(); i++) {
     if(*i) (*i)->setPower(conf.motorPower);
   }
   for(vector<SliderServo*>::iterator i=sliderServos.begin(); i!=sliderServos.end(); i++) {
     if(*i) (*i)->setPower(conf.motorPower);
   }
   return rv;
  }

  /** destroys vehicle and space
   */
  void SliderWheelie::destroy() {
   if(created) {
    DefaultSliderWheelie::destroy();  
    for (vector<HingeServo*>::iterator i = hingeServos.begin(); i!= hingeServos.end(); i++) {
     if(*i) delete *i;
    }
    for (vector<SliderServo*>::iterator i = sliderServos.begin(); i!= sliderServos.end(); i++) {
     if(*i) delete *i;
    }
    hingeServos.clear();
    sliderServos.clear();
   }
  }
}
