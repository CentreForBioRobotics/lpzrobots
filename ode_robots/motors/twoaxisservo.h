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
 *   Revision 1.1  2007-02-12 13:28:20  martius
 *   twoaxisservo and some minor changes
 *
 *
 *
 *                                                                 *
 ***************************************************************************/
#ifndef __SERVO2_H
#define __SERVO2_H

#include "joint.h"
#include "pid.h"

namespace lpzrobots {

  /** general servo motor for 2 axis joints
   */
  class TwoAxisServo {
  public:
    /** min and max values are understood as travel bounds. Min should be less than 0.*/
  
    TwoAxisServo(TwoAxisJoint* joint, double min1, double max1, double power1, 
		 double min2, double max2, double power2, 
		 double damp=0.3, double integration=2.0)
      : joint(joint), min1(min1), max1(max1), min2(min2), max2(max2), 
	pid1(power1, integration, damp),
	pid2(power2, integration, damp) { 
      assert(joint); 
      assert(min1<max1); assert(min2<max2);
      assert(min1 <= 0); assert(min2 <= 0);
      assert(max1 >= 0); assert(max2 >= 0);
      assert(power1 >=0 && power2 >=0 && damp >=0 && integration >=0);
    }
    virtual ~TwoAxisServo(){}

    /** sets the set point of the servo. 
	Position must be between -1 and 1. It is scaled to fit into min, max
    */
    virtual void set(double pos1, double pos2){
      if(pos1 > 0){
	pos1 *= max1; 
      }else{
	pos1 *= -min1;
      }
      pid1.setTargetPosition(pos1);  
      double force1 = pid1.stepWithD(joint->getPosition1(), joint->getPosition1Rate());
      if(pos2 > 0){
	pos2 *= max2; 
      }else{
	pos2 *= -min2;
      }
      pid2.setTargetPosition(pos2);  
      double force2 = pid2.stepWithD(joint->getPosition2(), joint->getPosition2Rate());
      joint->addForces(force1, force2);
    }

    /** returns the position of the servo (joint) of 1. axis in ranges [-1, 1] (scaled by min1, max1)*/
    virtual double get1(){
      double pos =  joint->getPosition1();
      if(pos > 0){
	pos /= max1; 
      }else{
	pos /= -min1;
      }
      return pos;    
    }

    /** returns the position of the servo (joint) of 2. axis in ranges [-1, 1] (scaled by min2, max2)*/
    virtual double get2(){
      double pos =  joint->getPosition2();
      if(pos > 0){
	pos /= max2; 
      }else{
	pos /= -min2;
      }
      return pos;    
    }

    /** returns the positions of the joint in ranges [-1, 1] (scaled by min, max)*/
    void get(double& p1, double& p2){
      p1=get1();
      p2=get2();
    }


    /** adjusts the power of the servo*/
    virtual void setPower(double power1, double power2) { 
      pid1.KP = power1;
      pid2.KP = power2;
    };
    /** returns the power of the servo*/
    virtual double& power1() { 
      return pid1.KP;
    };
    /** returns the power of the servo*/
    virtual double& power2() { 
      return pid2.KP;
    };

    /** returns the damping of the servo*/
    virtual double& damping() { 
      return pid1.KD;
    };
    /** returns the damping of the servo*/
    virtual double& offsetCanceling() { 
      return pid1.KI;
    };
  
  private:
    TwoAxisJoint* joint;
    double min1;
    double max1;
    double min2;
    double max2;
    PID pid1;
    PID pid2;
  };

  typedef TwoAxisServo UniversalServo;
}
#endif
