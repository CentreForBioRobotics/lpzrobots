/***************************************************************************
 *   Copyright (C) 2005-2011 LpzRobots development team                    *
 *    Georg Martius  <georg dot martius at web dot de>                     *
 *    Frank Guettler <guettler at informatik dot uni-leipzig dot de        *
+ *    Frank Hesse    <frank at nld dot ds dot mpg dot de>                  *
 *    Ralf Der       <ralfder at mis dot mpg dot de>                       *
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
#ifndef __OPERATORS_H
#define __OPERATORS_H

#include "operator.h"
#include "axis.h"

namespace lpzrobots {
  /**
     An Operator for limiting the orientation of the main primitive of a robot.
   */
  class LimitOrientationOperator : public Operator {
  public:
    LimitOrientationOperator(const Axis& robotAxis, const Axis& globalAxis, 
                             double maxAngle, double force)
      : Operator("LimitOrientationOperator","1.0"), 
        robotAxis(robotAxis), globalAxis(globalAxis), 
        maxAngle(maxAngle), force(force), currentforce(force) {
    }

    virtual ManipType observe(OdeAgent* agent, GlobalData& global, ManipDescr& descr);
  protected:
    Axis robotAxis;
    Axis globalAxis;
    double maxAngle;

    double force;
    double currentforce;
  };


  struct LiftUpOperatorConf {
    bool   resetForceIfLifted;  ///< force is reseted as soon as robot has reached height
    bool   increaseForce;       ///< increase force (until robot has reached height)
    bool   propControl;         ///< if true then applied force is scaled with distance

    bool   intervalMode;        ///< if true then the operator works only in intervals 
    double duration;            ///< duration of operation in interval-mode
    double interval;            ///< interval between restart of operation ( > duration)
                               
    double height;              ///< height to which it is lifted
    double force;               ///< initial force
    double visualHeight;        ///< height above the robot main object for the visual sphere
  };

  /**
     An Operator for lifting up a robot from time to time.
   */
  class LiftUpOperator : public Operator {
  public:
    LiftUpOperator(const LiftUpOperatorConf conf = getDefaultConf())
      : Operator("LiftUpOperator","0.8"), conf(conf)
    {
      currentforce = conf.force;
      addParameter("force",    &this->conf.force,   0, 100, "lift up force");
      addParameter("height",   &this->conf.height,  0, 100, "lift up height");
      if(conf.intervalMode){
        addParameter("interval", &this->conf.interval,  0, 1000, "interval of operation");
        addParameter("duration", &this->conf.duration,  0, 1000, "duration of lifting within interval");
      }
    }

    static LiftUpOperatorConf getDefaultConf(){
      LiftUpOperatorConf c;
      c.resetForceIfLifted = true;
      c.increaseForce      = true; 
      c.intervalMode       = false; 
      c.propControl        = true; 
      c.duration           = 1; 
      c.interval           = 10;      
      c.height             = 1; 
      c.force              = 1; 
      c.visualHeight       = 0.5; 
      return c;
    }

    virtual ManipType observe(OdeAgent* agent, GlobalData& global, ManipDescr& descr);
  protected:
    LiftUpOperatorConf conf;

    double currentforce;
  };


  /**
     An Operator for pulling the main primitive of a robot towards a point
   */
  class PullToPointOperator : public Operator {
  public:
    /// defines which dimensions should be effected
    enum Dimensions { X = 1, Y = 2, Z = 4, XY = X | Y, XZ = X | Z, YZ = Y | Z, 
                      XYZ = X | Y | Z };

    PullToPointOperator(const Pos& point, double force,
                        bool showPoint, Dimensions dim = XYZ, 
                        double minDist = 0, double damp = 0)
      : Operator("PullToPointOperator","1.0"), 
        point(point), force(force), showPoint(showPoint), dim(dim),
        minDist(minDist), damp(damp)
    {
      addParameter("force",    &this->force,   0, 100, "pull to point force");
      addParameter("damp",     &this->damp,   0, 1,   "pull to point damping");
    }

    virtual ManipType observe(OdeAgent* agent, GlobalData& global, ManipDescr& descr);

  protected:
    Pos point;    
    double force;
    bool showPoint;
    Dimensions dim;
    double minDist;
    double damp;
  };


}

#endif
