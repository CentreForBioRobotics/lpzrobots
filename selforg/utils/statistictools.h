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
 ***************************************************************************
 *                                                                         *
 *  DESCRIPTION                                                            *
 *                                                                         *
 *   $Log$
 *   Revision 1.10  2008-04-29 09:56:21  guettler
 *   -debug printouts removed
 *
 *   Revision 1.9  2008/04/29 08:51:54  guettler
 *   -cosmetic changes of StatisticTools
 *   -StatisticTools now uses new function addInspectableValue of the
 *   interface Inspectable, not overloading getInternalParams and
 *   getInternalParamNames anymore
 *
 *   Revision 1.8  2008/01/17 09:59:27  der
 *   complexmeasure: preparations made for predictive information,
 *   fixed a minor bug
 *   statisticmeasure, statistictools: added support for adding
 *   std::list<AbstractMeasure*> to StatisticTools, some minor
 *   improvements
 *
 *   Revision 1.7  2007/12/06 10:18:10  der
 *   AbstractMeasure is now a abstract type for Measures,
 *   StatisticTools now supports AbstractMeasures,
 *   StatisticalMeasure, ComplexMeasure  now derived from
 *   AbstractMeasure,
 *   ComplexMeasure provides support for calculation e.g. entropy,
 *   uses Discretisizer,
 *   Discretisizer is a stand-alone class for support of discretisizing values
 *   TrackableMeasure derived from ComplexMeasure and provides support for calculating complex measures for Trackable objects
 *
 *   Revision 1.6  2007/10/01 13:27:47  robot3
 *   documentation
 *
 *   Revision 1.5  2007/09/28 10:08:49  robot3
 *   fixed memory bugs, statistics are from now on aligned right
 *
 *   Revision 1.4  2007/09/28 08:48:21  robot3
 *   corrected some minor bugs, files are still in develop status
 *
 *   Revision 1.3  2007/09/27 10:49:39  robot3
 *   removed some minor bugs,
 *   added CONVergence test
 *   changed little things for support of the new WSM
 *
 *   Revision 1.2  2007/05/08 10:18:15  der
 *   added a function for starting the measure after a given time.
 *   made some tests
 *
 *   Revision 1.1  2007/05/07 21:01:32  robot3
 *   statistictools is a class for easy visualization of measurements of observed values
 *   it is possible to add the observed value itself with mode ID
 *
 *                                                                         *
 ***************************************************************************/
#ifndef _STATISTIC_TOOLS_H
#define _STATISTIC_TOOLS_H

#include "inspectable.h"
#include "callbackable.h"
#include "measuremodes.h"


// begin forward declarations
class AbstractMeasure;
class StatisticMeasure;
class ComplexMeasure;
// end forward declarations

class StatisticTools : public Inspectable, public Callbackable {

public:
  StatisticTools() : beginMeasureCounter(0) { }

	/**
	 * adds a variable to observe and measure the value
	 * @param observedValue    the value to observe.
	 * @param measureName      the name of the measured value
	 * @param mode             the mode of measure
	 * @param stepSpan         in most cases the stepSpan is important to get
	 * the measured value of a number of steps, like AVG:
	 * if stepSpan = 0, AVG is calculated over all steps
	 * if stepSpan = n, AVG is calculated over the LAST n steps
	 * The same counts for all the other MeasureModes.
	 * @param additionalParam  is used for example for mode PEAK, the param is the limit value,
	 * all values minus limit are displayed, values below the limit are set to 0.
  	 * In CONV mode (test the convergence), this value is the epsilon criteria.
	 * @return measured value as adress. So it is possible to measure this value again
	 */
  virtual double& addMeasure(double& observedValue, const char* measureName, MeasureMode mode, long stepSpan, double additionalParam=0);

  /**
   * Same as the method above, but instead of getting the calculated value back (the adress), you get
   * the StatisticMeasure itself
   */
  virtual StatisticMeasure* getMeasure(double& observedValue,const char* measureName, MeasureMode mode, long stepSpan, double additionalParam=0);


  /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * @param measure the measure to add
   * @return the address value of the measure
   */
  virtual double& addMeasure(AbstractMeasure* measure);

  /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * With this method you can add a list of AbstractMeasures.
   * @param measureList the list of measures to add
   * @return the address value of the first measure
   */
  virtual double& addMeasureList(std::list<AbstractMeasure*> measureList);


    /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * With this method you can add a list of AbstractMeasures.
   * @param measureList the list of measures to add
   * @return the address value of the first measure
   */
  virtual double& addMeasureList(std::list<ComplexMeasure*> measureList);


    /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * With this method you can add a list of AbstractMeasures.
   * @param measureList the list of measures to add
   * @return the address value of the first measure
   */
  virtual double& addMeasureList(std::list<StatisticMeasure*> measureList);



	/**
	 * starts the measure at a specific time. This is useful if there are
	 * values that have to be ignored at simulation start.
	 * @param step number of steps (normally simsteps) to wait for beginning the measures
	 */
	virtual void beginMeasureAt(long step);

  /**
   * Tells you wether the measures have already been started.
   * @return true if measures have already been started, otherwise false
   */
  virtual bool measureStarted() { return (beginMeasureCounter==0?true:false); }


	/**
	 * CALLBACKABLE INTERFACE
	 *
	 *	this method is invoked when a callback is done from the class where this
	 * class is for callback registered
	 */
	virtual void doOnCallBack();


protected:
	std::list<AbstractMeasure*> activeMeasures;
	long beginMeasureCounter;
};

#endif
