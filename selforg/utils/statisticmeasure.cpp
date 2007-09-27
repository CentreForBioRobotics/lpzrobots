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
 *   Revision 1.2  2007-09-27 10:49:39  robot3
 *   removed some minor bugs,
 *   added CONVergence test
 *   changed little things for support of the new WSM
 *
 *   Revision 1.1  2007/05/07 21:01:31  robot3
 *   statistictools is a class for easy visualization of measurements of observed values
 *   it is possible to add the observed value itself with mode ID
 *
 *                                                                         *
 ***************************************************************************/

#include "statisticmeasure.h"
#include <iostream>
#include "stl_adds.h"
#include "assert.h"


StatisticMeasure::StatisticMeasure(double& observedValue, char* measureName, MeasureMode mode, long stepSpan, double additionalParam) : name(measureName), observedValue(observedValue), mode(mode), stepSpan(stepSpan), additionalParam(additionalParam) {
	value=0;
	actualStep=0;
	oldestStepIndex=0;
	if (stepSpan>0) {
		this->valueHistory = (double*) malloc(sizeof(double) * stepSpan);
		// set all values to 0
		for (int i=0;i<stepSpan;i++)
			this->valueHistory[i]=0;
	}
	/// use this section for defining individual constructor commands
	switch(mode){
		case CONV:
			if (stepSpan==0) {
				std::cout << "ERROR: The stepspan in addMeasure(observedValue, CONV, stepSpan, epsilon)"
					<< std::endl << "       must not be <1!" << std::endl;
				std::cout << "Program terminated. Please correct this error in main.cpp (or wherever) first." << std::endl;
				exit(-1);
			}
			this->stepsReached=0;
			break;
		default: // not defined
			break;
	}
}

void StatisticMeasure::step() {
	// update the variable value from observedValue
	switch(mode){
		case ID: // return observed value itself
			value=observedValue;
			break;
		case AVG:
			value=calculateAverageValue();
			break;
		case PEAK:
			if (observedValue>additionalParam)
				value=observedValue-additionalParam;
			else
				value=0;
			break;
		case MAX:
			if (observedValue>value) // TODO: include history
				value=observedValue;
			break;
		case MIN:
			if (observedValue<value) // TODO: include history
				value=observedValue;
			break;
		case SUM:
			value=calculateSumValue();
			break;
		case CONV:
			value=testConvergence();
			break;
		default: // not defined
			break;
	}
	if (stepSpan>0) {
		// store new value in history, the position is oldestStepIndex
		valueHistory[oldestStepIndex]=observedValue;
		if (oldestStepIndex==(stepSpan-1))
			oldestStepIndex=0;
		else
			oldestStepIndex++;
	}
	actualStep++;
}

double StatisticMeasure::testConvergence() {
	if (std::abs(observedValue-valueHistory[actualStep])<additionalParam) {
		stepsReached++;
		if (stepsReached==stepSpan)
			return 1.0;
	} else
		stepsReached=0;
	return 0.0;
}


double StatisticMeasure::calculateSumValue() {
	double newsum=this->value;
	if (stepSpan==0) {
		value+=observedValue;
	} else {
		// first: subtrate oldestVal from value
		newsum -= valueHistory[oldestStepIndex];
		// add observedValue to value
		newsum += observedValue;
	}
	return newsum;
}


double StatisticMeasure::calculateAverageValue() {
	double newavg=this->value;
	if (stepSpan==0) {
		// use update rule described in diploma thesis of Frank Guettler

	} else {
		// first: subtrate 1/stepSpan*oldestVal from value
		newavg -= valueHistory[oldestStepIndex]/stepSpan;
		// add 1/stepSpan*observedValue to value
		newavg += observedValue/stepSpan;
	}
	return newavg;
}

