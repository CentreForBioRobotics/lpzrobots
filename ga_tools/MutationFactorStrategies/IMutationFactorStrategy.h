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
 *   This is a interface for a strategy which is part of the mutation      *
 *   strategy. It calculate the factor for the mutation. To this time it is*
 *   only used by ValueMutationStrategy.                                *
 *                                                                         *
 *   $Log$
 *   Revision 1.3  2009-07-21 08:37:59  robot12
 *   add some comments
 *
 *   Revision 1.2  2009/06/17 11:11:06  robot12
 *   finishing the mutationfactorstrategy and add some comments.
 *
 *   Revision 1.1  2009/05/04 15:27:55  robot12
 *   rename of some files and moving files to other positions
 *    - SingletonGenAlgAPI has one error!!! --> is not ready now
 *
 *   Revision 1.2  2009/04/28 13:23:55  robot12
 *   some implements... Part2
 *
 *   Revision 1.1  2009/04/27 10:59:34  robot12
 *   some implements
 *
 *
 ***************************************************************************/

#ifndef IMUTATIONFACTORSTRATEGY_H_
#define IMUTATIONFACTORSTRATEGY_H_

//includes
#include <vector>

//forward declarations
class Gen;
class IValue;

/**
 * This is a interface for a strategy, which is used by ValueMutationStrategy
 */
class IMutationFactorStrategy {
public:
	/**
	 * default constructor
	 */
	IMutationFactorStrategy();

	/**
	 * default destructor
	 */
	virtual ~IMutationFactorStrategy();

	/**
	 * this abstract function should later calculate the new mutation factor.
	 * It use for this a set gens. Most the gens which for one prototyp in one generation are.
	 * @param gene (vector<Gen*>) this set of gens
	 * @return (IValue*) the mutation factor
	 */
	virtual IValue* calcMutationFactor(const std::vector<Gen*>& gene) = 0;
};

#endif /* IMUTATIONFACTORSTRATEGY_H_ */
