/***************************************************************************
 *   Copyright (C) 2005-2009 by Robot Group Leipzig                        *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
 *    jhoffmann@informatik.uni-leipzig.de                                  *
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
 *   This class adds functionality to the inspectable interface. So it is  *
 *   possible to change the inspectable object to become the next value.   *
 *                                                                         *
 *   The values of the inspectable will not longer be generate by only one *
 *   class.                                                                *
 *                                                                         *
 *   $Log$
 *   Revision 1.1  2009-06-29 13:37:05  robot12
 *   add the new class inspectableproxy
 *
 *
 ***************************************************************************/

#include "inspectableproxy.h"
#include <string>

InspectableProxy::InspectableProxy() {
	// nothing
}

InspectableProxy::InspectableProxy(const std::list<Inspectable*>& list) {
	//add all parameters of the inspectable in the own list.
	for(std::list<Inspectable*>::const_iterator iter = list.begin(); iter!=list.end(); iter++) {
		m_list.push_back(*iter);
		std::list<std::string> names = (*iter)->getInternalParamNames();
		std::list<double*> values = (*iter)->getInternalParamsPtr();
		std::list<std::string>::iterator namesIter = names.begin();
		std::list<double*>::iterator valuesIter = values.begin();
		unsigned int num = names.size();

		for(unsigned int i = 0; i < num; i++) {
			addInspectableValue(*namesIter,*valuesIter);
			namesIter++;
			valuesIter++;
		}
	}
}

InspectableProxy::~InspectableProxy() {
	// nothing
}

bool InspectableProxy::replaceList(const std::list<Inspectable*>& list) {
	//needs the same parameter count
	if(m_list.size() != list.size())
		return false;

	//clear all
	m_list.clear();
	mapOfValues.clear();
	mapOfMatrices.clear();

	//add the new parameters
	for(std::list<Inspectable*>::const_iterator iter = list.begin(); iter!=list.end(); iter++) {
		m_list.push_back(*iter);
		std::list<std::string> names = (*iter)->getInternalParamNames();
		std::list<double*> values = (*iter)->getInternalParamsPtr();
		std::list<std::string>::iterator namesIter = names.begin();
		std::list<double*>::iterator valuesIter = values.begin();
		unsigned int num = names.size();

		for(unsigned int i = 0; i < num; i++) {
			addInspectableValue(*namesIter,*valuesIter);
			namesIter++;
			valuesIter++;
		}
	}

	return true;
}
