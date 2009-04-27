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
 *   Revision 1.4  2009-04-27 10:59:33  robot12
 *   some implements
 *
 *
 ***************************************************************************/

#ifndef SINGLETONINDIVIDUALFACTORY_H_
#define SINGLETONINDIVIDUALFACTORY_H_

#include "types.h"s

#include "Individual.h"
#include "SingletonGenFactory.h"
#include "SingletonGenEngine.h"
#include "Generation.h"
#include "Gen.h"
#include <selforg/utils/randomgenerator.h>

class SingletonIndividualFactory {
public:
	inline static SingletonIndividualFactory* getInstance(void) {
		if(m_factory==0)m_factory = new SingletonIndividualFactory;
		return m_factory;
	}

	inline static void destroyFactory(void) {delete m_factory; m_factory=NULL;}

	// 2 Methodes to create an Individual
	Individual* createIndividual(std::string name)const;													// random
	Individual* createIndividual(Individual* individual1, Individual* individual2, RandGen* random)const;	// recombinate

private:
	static SingletonIndividualFactory* m_factory = 0;
	static int m_number = 0;

	SingletonIndividualFactory();
	virtual ~SingletonIndividualFactory();
};

#endif /* SINGLETONINDIVIDUALFACTORY_H_ */
