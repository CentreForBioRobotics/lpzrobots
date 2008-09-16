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
 *   Revision 1.8  2008-09-16 15:37:29  martius
 *   added randomgen
 *
 *   Revision 1.7  2008/04/28 11:14:54  guettler
 *   removed include "abstractrobot.h" (not needed)
 *
 *   Revision 1.6  2008/04/17 14:54:45  martius
 *   randomGen added, which is a random generator with long period and an
 *    internal state. Each Agent has an instance and passed it to the controller
 *    and the wiring. This is good for
 *   a) repeatability on agent basis,
 *   b) parallel execution as done in ode_robots
 *
 *   Revision 1.5  2007/11/07 13:40:24  martius
 *   sensors and motors also added as type here (not very nice anyway)
 *
 *   Revision 1.4  2006/12/21 11:44:17  martius
 *   commenting style for doxygen //< -> ///<
 *   FOREACH and FOREACHC are macros for collection iteration
 *
 *   Revision 1.3  2006/07/20 17:14:36  martius
 *   removed std namespace from matrix.h
 *   storable interface
 *   abstract model and invertablemodel as superclasses for networks
 *
 *   Revision 1.2  2006/07/14 12:24:02  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.1  2005/11/16 11:24:27  martius
 *   moved to selforg
 *
 *   Revision 1.8  2005/11/09 13:55:44  martius
 *   *** empty log message ***
 *
 *   Revision 1.7  2005/10/24 09:52:36  fhesse
 *   comments in doxygen
 *
 *   Revision 1.6  2005/10/06 17:11:36  martius
 *   switched to stl lists
 *
 *   Revision 1.5  2005/08/03 20:34:58  martius
 *   use if Inspectable interface
 *
 *   Revision 1.4  2005/07/21 15:14:47  martius
 *   wireSensors and wireMotors get constant fields
 *
 *   Revision 1.3  2005/07/18 14:44:27  martius
 *   noise moved into wiring
 *
 *   Revision 1.2  2005/07/18 10:14:45  martius
 *   noise is added here
 *
 *   Revision 1.1  2005/07/14 15:57:53  fhesse
 *   now agent contains controller, robot and wiring, plotting ability included, therefore plotagent can be removed; ono2onewiring replaces one2oneagent
 *                                            *
 *                                                                         *
 ***************************************************************************/
#ifndef __ABSTRACTWIRING_H
#define __ABSTRACTWIRING_H

//#include "abstractrobot.h"
#include "noisegenerator.h"
#include "inspectable.h"
#include "randomgenerator.h"

/** Abstract wiring-object between controller and robot.
 *  Implements wiring of robot sensors to inputs of the controller and
 *  controller outputs to robot motors.
 */
class AbstractWiring : public Inspectable {
public:
  typedef double sensor;
  typedef double motor;

  /** constructor
   *  @param noise NoiseGenerator that is used for adding noise to sensor values
   */
  AbstractWiring(NoiseGenerator* noise){
    rsensornumber = 0;
    rmotornumber  = 0;
    csensornumber = 0;
    cmotornumber  = 0;
    noiseGenerator = noise;
  }

  /** destructor
   */
  virtual ~AbstractWiring(){
    if(noiseGenerator) delete noiseGenerator;
  }

  /** Initializes the  number of sensors and motors from robot
   *  (to be precise the internal parameters rsensornumber and rmotornumber!),
   *  calculates the number of sensors and motors on controller side.
   *  Must be overloaded to calculate and provide the appropriate numbers
   *  controllersensornumber (csensornumber), controllermotornumber (cmotornumber),
   *  robotsensornumber (rsensornumber) and robotmotornumber (rmotornumber),
   *  @param randGen pointer to random generator, if not given then a new one is created
   *  @return returns false on error, otherwise true
   */
  virtual bool init(int robotsensornumber, int robotmotornumber, RandGen* randGen=0) = 0;

  /** Realizes wiring from robot sensors to controller sensors.
   *   Must be overloaded in order to implement the appropriate mapping.
   *   @param rsensors pointer to array of sensorvalues from robot
   *   @param rsensornumber number of sensors from robot
   *   @param csensors pointer to array of sensorvalues for controller
   *   @param csensornumber number of sensors to controller
   *   @param noise size of the noise added to the sensors
   *  @return returns false on error, otherwise true
   */
  virtual bool wireSensors(const sensor* rsensors, int rsensornumber,
			   sensor* csensors, int csensornumber,
			   double noise) = 0;

  /** Realizes wiring from controller motor outputs to robot motors.
   *   Must be overloaded in order to implement the appropriate mapping.
   *   @param rmotors pointer to array of motorvalues for robot
   *   @param rmotornumber number of robot motors
   *   @param cmotors pointer to array of motorvalues from controller
   *   @param cmotornumber number of motorvalues from controller
   *   @return returns false if error, else true
   */
  virtual bool wireMotors(motor* rmotors, int rmotornumber,
			  const motor* cmotors, int cmotornumber)  = 0;



  /** Returns the number of sensors on robot side.
   */
  virtual int getRobotSensornumber(){return rsensornumber;}

  /** Returns the number of motors on robot side.
   */
  virtual int getRobotMotornumber() {return rmotornumber;}

  /** Returns the number of sensors on controller side.
   */
  virtual int getControllerSensornumber(){return csensornumber;}

  /** Returns the number of motors on controller side.
   */
  virtual int getControllerMotornumber() {return cmotornumber;}

  /** Returns the list of the names of all internal parameters.
   */
  virtual std::list<iparamkey> getInternalParamNames() const { return std::list<iparamkey>(); };

  /** Returns a list of the values of all internal parameters
      (in the order given by getInternalParamNames()).
   */
  virtual std::list<iparamval>  getInternalParams() const { return std::list<iparamval>(); };


protected:
  /// number of sensors at robot side
  int rsensornumber;

  /// number of motors at robot side
  int rmotornumber;

  /// number of sensors at controller side
  int csensornumber;

  /// number of motors at controller side
  int cmotornumber;

  /// noise generator
  NoiseGenerator* noiseGenerator;

};

#endif
