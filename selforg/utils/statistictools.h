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
 *   Revision 1.2  2007-05-08 10:18:15  der
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

class StatisticMeasure;


/** measure modes of statistical types.
 */
enum MeasureMode {
 	/// returns the value to observe itself
 	ID,
	/// returns the average value
	AVG,
	/// returns the median value
	MED,
	/// returns the minimum value
	MIN,
	/// returns the maximum value
	MAX,
	/// returns only values above defined limit
 	PEAK,
	/// returns the sum of the value generated over time (or stepSpan)
 	SUM
};






class StatisticTools : public Inspectable, public Callbackable {

public:
	StatisticTools() { beginMeasureCounter=0; }

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
	 * @return measured value as adress. So it is possible to measure this value again
	 */
	virtual double& addMeasure(double& observedValue, char* measureName, MeasureMode mode, long stepSpan, double additionalParam=0);


	/**
	 * starts the measure at a specific time. This is useful if there are
	 * values that have to be ignored at simulation start.
	 */
	virtual void beginMeasureAt(long step);


	/**
	 * CALLBACKABLE INTERFACE
	 *
	 *	this method is invoked when a callback is done from the class where this
	 * class is for callback registered
	 */
	virtual void doOnCallBack();

	/**
	 * INSPECTABLE INTERFACE
	 */
	virtual std::list<iparamkey> getInternalParamNames() const;

	virtual std::list<iparamval> getInternalParams() const;

private:
	std::list<StatisticMeasure*> activeMeasures;
	long beginMeasureCounter;
};

#endif
