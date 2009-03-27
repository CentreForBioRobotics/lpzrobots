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
 *   Revision 1.1  2009-03-27 06:16:57  guettler
 *   support for gcc 4.3 compatibility (has to be checked), StatisticTools moves from utils to statistictools
 *
 *   Revision 1.10  2008/05/01 22:03:56  martius
 *   build system expanded to allow system wide installation
 *   that implies  <ode_robots/> for headers in simulations
 *
 *   Revision 1.9  2008/04/29 09:56:21  guettler
 *   -debug printouts removed
 *
 *   Revision 1.8  2008/04/29 08:51:54  guettler
 *   -cosmetic changes of StatisticTools
 *   -StatisticTools now uses new function addInspectableValue of the
 *   interface Inspectable, not overloading getInternalParams and
 *   getInternalParamNames anymore
 *
 *   Revision 1.7  2008/01/17 09:59:27  der
 *   complexmeasure: preparations made for predictive information,
 *   fixed a minor bug
 *   statisticmeasure, statistictools: added support for adding
 *   std::list<AbstractMeasure*> to StatisticTools, some minor
 *   improvements
 *
 *   Revision 1.6  2008/01/14 09:09:23  der
 *   added stepSize. An abstractmeasure can now be calculated every stepSize
 *   steps.
 *
 *   Revision 1.5  2007/12/06 10:18:10  der
 *   AbstractMeasure is now a abstract type for Measures,
 *   StatisticTools now supports AbstractMeasures,
 *   StatisticalMeasure, ComplexMeasure  now derived from
 *   AbstractMeasure,
 *   ComplexMeasure provides support for calculation e.g. entropy,
 *   uses Discretisizer,
 *   Discretisizer is a stand-alone class for support of discretisizing values
 *   TrackableMeasure derived from ComplexMeasure and provides support for calculating complex measures for Trackable objects
 *
 *   Revision 1.4  2007/09/28 09:15:25  robot3
 *   extended comments
 *
 *   Revision 1.3  2007/09/28 08:48:21  robot3
 *   corrected some minor bugs, files are still in develop status
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
#include "statistictools.h"
#include "statisticmeasure.h"
#include "complexmeasure.h"


void StatisticTools::doOnCallBack() {
    // update all statistic measures
    if (beginMeasureCounter>0)
        beginMeasureCounter--;
    else
        for (std::list<AbstractMeasure*>::iterator i=activeMeasures.begin();i!=activeMeasures.end();i++) {
          if (((*i)->getActualStep())%((*i)->getStepSize())==0)
            (*i)->step();
        }
}

double& StatisticTools::addMeasure(double& observedValue, const char* measureName, MeasureMode mode, long stepSpan, double additionalParam) {
    StatisticMeasure* newMeasure = this->getMeasure(observedValue,measureName,mode,stepSpan,additionalParam);
    return  newMeasure->getValueAddress();
}

StatisticMeasure* StatisticTools::getMeasure(double& observedValue, const char* measureName, MeasureMode mode, long stepSpan, double additionalParam) {
    StatisticMeasure* newMeasure = new StatisticMeasure(observedValue, measureName, mode, stepSpan, additionalParam);
    this->activeMeasures.push_back(newMeasure);
  addInspectableValue(newMeasure->getName(),&newMeasure->getValueAddress());
  return newMeasure;
}

double& StatisticTools::addMeasure(AbstractMeasure* measure) {
  this->activeMeasures.push_back(measure);
  addInspectableValue(measure->getName(),&measure->getValueAddress());
  return  measure->getValueAddress();
}

double& StatisticTools::addMeasureList(std::list<AbstractMeasure*> measureList) {
  FOREACH(std::list<AbstractMeasure*>,measureList,measure) {
    addMeasure(*measure);
  }
  return measureList.front()->getValueAddress();
}

double& StatisticTools::addMeasureList(std::list<ComplexMeasure*> measureList) {
  FOREACH(std::list<ComplexMeasure*>,measureList,measure) {
    addMeasure(*measure);
  }
  return measureList.front()->getValueAddress();
}

double& StatisticTools::addMeasureList(std::list<StatisticMeasure*> measureList) {
  FOREACH(std::list<StatisticMeasure*>,measureList,measure) {
    addMeasure(*measure);
  }
  return measureList.front()->getValueAddress();
}



void StatisticTools::beginMeasureAt(long step) {
    this->beginMeasureCounter=step;
}
