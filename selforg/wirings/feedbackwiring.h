/***************************************************************************
 *   Copyright (C) 2007 by Robot Group Leipzig                             *
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
 *   Revision 1.1  2007-11-28 10:30:56  martius
 *   wiring with feedback connections
 *
 *                                            *
 *                                                                         *
 ***************************************************************************/
#ifndef __FEEDBACKWIRING_H
#define __FEEDBACKWIRING_H

#include "abstractwiring.h"

/** Implements essentionally a one to one wiring with feedback connections. 
    The feedback connections from output to input is parameterised
     with a feedback strength.   
    It is possible to generate virtual motors for context sensors.
    
    In order to change the feedback strength use the following code 
     after initialisation of the agent/wiredcontroller
    \code
    matrix::Matrix rs = wiring->getFeedbackRatio();
    double c=ratio;
    rs.toMapP(&c,constant);
    wiring->setFeedbackRatio(rs);
    \endcode
 */
class FeedbackWiring :public AbstractWiring{
public:
  typedef enum {Motor=1, Context=2, All=3} Mode;

  /** constructor
      @param noise NoiseGenerator that is used for adding noise to sensor values  
      @param mode Motor|Context|All: Motor: motor outputs send feedback;
           Context: virtual motor outputs for each context sensor with feedback
      @param feedbackratio default ratio used to feed back the output to the input,
         meaning \f[ x_t = 0.1*x_t + 0.9*y_{t-1} \f]
   */
  FeedbackWiring(NoiseGenerator* noise, Mode mode = Context,double feedbackratio=0.9);
  virtual ~FeedbackWiring();

  virtual bool init(int robotsensornumber, int robotmotornumber);

  virtual bool wireSensors(const sensor* rsensors, int rsensornumber, 
			   sensor* csensors, int csensornumber,
			   double noise);

  virtual bool wireMotors(motor* rmotors, int rmotornumber,
			  const motor* cmotors, int cmotornumber);

  virtual std::list<iparamkey> getInternalParamNames() const;
  virtual std::list<iparamval> getInternalParams() const;

  /// return the feedback ratio vector 
  virtual matrix::Matrix getFeedbackRatio() const;
  /** sets the feedback ratio vector. 
      The size of the vector must be at least as large as feedbackratio*/
  virtual void setFeedbackRatio(const matrix::Matrix&);

protected:  

  Mode mode;
  double defaultfeedbackratio; 
  matrix::Matrix feedbackratio;
  /// array that stored the values of the motors     
  motor *motors;        
  int vmotornumber;

  /// for storing the noise values
  sensor* noisevals;

  bool initialised;
};

#endif
