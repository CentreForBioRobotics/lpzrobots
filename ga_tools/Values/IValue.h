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
 *   Revision 1.2  2009-05-06 13:28:22  robot12
 *   some implements... Finish
 *
 *   Revision 1.1  2009/05/04 15:27:57  robot12
 *   rename of some files and moving files to other positions
 *    - SingletonGenAlgAPI has one error!!! --> is not ready now
 *
 *   Revision 1.6  2009/05/04 09:20:52  robot12
 *   some implements.. Finish --> first compile
 *
 *   Revision 1.5  2009/04/29 14:32:28  robot12
 *   some implements... Part4
 *
 *   Revision 1.4  2009/04/24 11:26:07  robot12
 *   some implements
 *
 *   Revision 1.3  2009/04/23 15:17:42  jhoffmann
 *   inserted copyright-template, corrected includes, doxygen style templated
 *
 ***************************************************************************/
#ifndef IVALUE_H_
#define IVALUE_H_


#include <selforg/inspectable.h>

/**
 * Technische Beschreibung der Klasse
 */
class IValue : public Inspectable
{
public:

  /**
   * Beschreibung des Konstruktors
   */
  IValue();


  /**
   * Beschreibung des Destruktors
   */
  virtual ~IValue();

  virtual IValue* operator*(const IValue&)const = 0;
};

#endif /* IVALUE_H_ */
