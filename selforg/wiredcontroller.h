/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    joergweide84@aol.com (robot12)                                       *
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
 *   The Idea of this class is a controller wrapped in the wiring          *
 *   with all the plotoptions and so on, just without robot. It            *
 *   seams to be a much better architecture for connecting the controller  *
 *   to a hardware robot or other platforms.                               *
 *                                                                         *
 *                                                                         *
 *   $Log$
 *   Revision 1.10  2009-07-21 09:10:22  robot12
 *   add some comments
 *
 *   Revision 1.9  2009/06/02 09:55:24  robot12
 *   Splitting of WiredController and PlotOption into WiredController : public PlotOptionEngine and
 *   PlotOption (used by ga_tools). Further refactorings needed.
 *
 *   Revision 1.8  2009/05/11 17:08:01  martius
 *   flushing optimized
 *
 *   Revision 1.7  2009/03/25 11:55:32  robot1
 *   changed minor handling of PlotOptions
 *
 *   Revision 1.6  2008/08/12 11:50:00  guettler
 *   plug and play update, added some features for the ECBRobotGUI
 *
 *   Revision 1.5  2008/08/01 14:42:04  guettler
 *   we try the trip to hell! make selforg AVR compatible...good luck (first changes)
 *
 *   Revision 1.4  2008/05/07 16:45:52  martius
 *   code cosmetics and documentation
 *
 *   Revision 1.3  2008/05/02 17:20:04  martius
 *   *** empty log message ***
 *
 *   Revision 1.2  2008/04/17 14:54:35  martius
 *   randomGen added, which is a random generator with long period and an
 *    internal state. Each Agent has an instance and passed it to the controller
 *    and the wiring. This is good for
 *   a) repeatability on agent basis,
 *   b) parallel execution as done in ode_robots
 *
 *   Revision 1.1  2007/11/06 15:14:41  martius
 *   new class that composes controller and wiring
 *
 *                                                                 *
 ***************************************************************************/
/*
** Started on  Mon Oct 22 10:50:47 2007 Georg Martius
** Last update Mon Oct 22 10:50:47 2007 Georg Martius
*/
#ifndef   	WIREDCONTROLLER_H_
#define   	WIREDCONTROLLER_H_

#include "plotoptionengine.h"

#include <stdio.h>
#include <list>
#include <utility>
#include <string>

#include "types.h"
#include "randomgenerator.h"


class AbstractController;
class AbstractWiring;
class Configurable;
class Inspectable;
class Callbackable;
class WiredController;

/** The WiredController contains a controller and a wiring, which
    connects the controller with the robot.
    Additionally there are some ways to keep track of internal information.
    You have the possibility to keep track of sensor values,
     motor values and internal parameters of the controller with PlotOptions.
    The name PlotOptions is a bit missleaded, it should be "OutputOptions",
     however you can write the data into a file or send it to
     visualisation tools like guilogger or neuronviz.
 */
class WiredController : public PlotOptionEngine{
public:
  /** constructor. PlotOption as output setting.
      noisefactor is used to set the relative noise strength of this agent
   */
  WiredController(const PlotOption& plotOption = PlotOption(NoPlot), double noisefactor = 1);
  /** constructor. A list of PlotOption can given.
      noisefactor is used to set the relative noise strength of this agent
   */
  WiredController(const std::list<PlotOption>& plotOptions, double noisefactor = 1);

  /** destructor
   */
  virtual ~WiredController();

  /** initializes the object with the given controller and wiring
      and initializes the output options
      It is also possible to provide a random seed,
       if not given (0) rand() is used to create one
  */
  bool init(AbstractController* controller, AbstractWiring* wiring,
	    int robotsensornumber, int robotmotornumber, RandGen* randGen=0);

  /** Performs an step of the controller, which includes
      pushing sensor values through the wiring,
      controller step,
      pushing controller outputs (= motorcommands) back through the wiring
      @param sensors sensors inputs scaled to [-1,1]
      @param sensornumber length of the sensor array
      @param motors motors outputs. MUST have enough space for motor values!
      @param motornumber length of the provided motor array

      @param noise Noise strength.
      @param time (optional) current simulation time (used for logging)
  */
  void step(const sensor* sensors, int sensornumber,
                    motor* motors, int motornumber,
		    double noise, double time=-1);

  /** adds the PlotOptions to the list of plotoptions
      If a plotoption with the same Mode exists, then the old one is deleted first
   */
  virtual PlotOption addPlotOption(PlotOption& plotoption);

  /** Returns a pointer to the controller.
   */
  virtual AbstractController* getController() { return controller;}

  /** Returns a pointer to the wiring.
   */
  virtual AbstractWiring* getWiring() { return wiring;}


  /** adds an Callbackable object for getting a callback every step.
   */
  virtual void addCallbackable(Callbackable* callbackable);

protected:
  /**
   * Plots controller sensor- and motorvalues and internal controller parameters.
   * @param rx actual sensorvalues from robot (used for generation of motorcommand in actual timestep)
   * @param rsensornumber length of rx
   * @param cx actual sensorvalues which are passed to controller (used for generation of motorcommand in actual timestep)
   * @param csensornumber length of cx
   * @param y actual motorcommand (generated in the actual timestep)
   * @param motornumber length of y
   * @param time simulation time
   */
  virtual void plot(const sensor* rx, int rsensornumber, const sensor* cx, int csensornumber,
		    const motor* y, int motornumber, double time);


  AbstractController* controller;
  AbstractWiring* wiring;

  /// number of sensors of robot
  int rsensornumber;
  /// number of motors of robot
  int rmotornumber;
  /// number of sensors of comntroller
  int csensornumber;
  /// number of motors of comntroller
  int cmotornumber;

  /// factor that is  muliplied with noise stength
  double noisefactor;

  motor  *cmotors;
  sensor *csensors;

  void internInit();

 protected:
  bool initialised;

  std::list<Callbackable* > callbackables;

};

#endif 	    /* !WIREDCONTROLLER_H_ */
