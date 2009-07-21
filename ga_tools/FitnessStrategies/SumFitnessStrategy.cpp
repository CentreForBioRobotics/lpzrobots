/***************************************************************************
 *   Copyright (C) 2005-2009 by Robot Group Leipzig                        *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
 *    jhoffmann@informatik.uni-leipzig.de                                  *
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
 ***************************************************************************
 *                                                                         *
 *   This is a example implementation for IFitnessStrategy. It only calc.  *
 *   the Sum of all double gens of the individual.                         *
 *                                                                         *
 *   $Log$
 *   Revision 1.4  2009-07-21 08:37:59  robot12
 *   add some comments
 *
 *   Revision 1.3  2009/06/15 13:58:37  robot12
 *   3 new fitness strategys and IFitnessStrategy and SumFitnessStragegy with comments.
 *
 *   Revision 1.2  2009/05/06 13:28:23  robot12
 *   some implements... Finish
 *
 *   Revision 1.1  2009/05/04 15:27:56  robot12
 *   rename of some files and moving files to other positions
 *    - SingletonGenAlgAPI has one error!!! --> is not ready now
 *
 *   Revision 1.2  2009/05/04 09:06:00  robot12
 *   some implements... Part7
 *
 *   Revision 1.1  2009/04/30 11:51:26  robot12
 *   some implements... new classes
 *
 *
 *
 ***************************************************************************/

#include "SumFitnessStrategy.h"

#include "Individual.h"
#include "Gen.h"
#include "IValue.h"
#include "TemplateValue.h"
#include "IFitnessStrategy.h"

// if it isn't a double gen add one to the sum.
#define STANDART_FACTOR_FOR_UNKNOWN_DATA_TYP 1.0

SumFitnessStrategy::SumFitnessStrategy() {
	// nothing
}

SumFitnessStrategy::~SumFitnessStrategy() {
	// nothing
}

double SumFitnessStrategy::getFitness(const Individual* individual) {
	double sum = 0.0;						//the sum and on the end the resulting fitness value
	int num = individual->getSize();		//number of gens inside the individual
	Gen* gen;								//the actual gen
	IValue* value;							//the value of the gen
	TemplateValue<double>* tValue;			//the casted value of the gen

	for(int x=0; x<num; x++) {
		gen = individual->getGen(x);		//become the gen from the individual
		value = gen->getValue();			//become the value of the gen
		tValue = dynamic_cast<TemplateValue<double>* >(value);	//cast the value to double gen
		if(tValue == 0) { //UNKNOWN DATA TYP	//test the value if it is really a double gen
			sum += STANDART_FACTOR_FOR_UNKNOWN_DATA_TYP;
		}
		else {
			sum += tValue->getValue();		// add the value
		}
	}

	return sum;		//return the result
}
