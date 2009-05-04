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
 *   Informative Beschreibung der Klasse                                   *
 *                                                                         *
 *   $Log$
 *   Revision 1.2  2009-05-04 09:06:00  robot12
 *   some implements... Part7
 *
 *   Revision 1.1  2009/04/30 11:51:25  robot12
 *   some implements... new classes
 *
 *
 *
 ***************************************************************************/

#ifndef VALUEMUTATIONSTRATEGIE_H_
#define VALUEMUTATIONSTRATEGIE_H_

#include "types.h"

#include "Gen.h"
#include "Individual.h"
#include "SingletonGenFactory.h"
#include "IMutationFactorStrategie.h"
#include "GenContext.h"
#include "IMutationStrategie.h"
#include "IValue.h"

class ValueMutationStrategie : public IMutationStrategie {
public:
	ValueMutationStrategie(IMutationFactorStrategie* strategie, int mutationProbability);
	virtual ~ValueMutationStrategie();

	virtual Gen* mutate(Gen* gen, GenContext* context, Individual* individual, SingletonGenFactory* factory);
	virtual int getMutationProbability(void);

protected:
	IMutationFactorStrategie* m_strategie;
	int m_mutationProbability;

private:
	ValueMutationStrategie();
};

#endif /* VALUEMUTATIONSTRATEGIE_H_ */
