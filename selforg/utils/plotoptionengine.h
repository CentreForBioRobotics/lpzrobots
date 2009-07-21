/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
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
 *  DESCRIPTION                                                            *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *  $Log$
 *  Revision 1.3  2009-07-21 08:50:16  robot12
 *  finish of the split
 *  to do: add some comments....
 *
 *  Revision 1.2  2009/06/30 14:20:49  robot12
 *  finishing the gen API and add some comments
 *
 *  Revision 1.1  2009/06/02 09:55:24  robot12
 *  Splitting of WiredController and PlotOption into WiredController : public PlotOptionEngine and
 *  PlotOption (used by ga_tools). Further refactorings needed.
 *										   *
 *                                                                         *
 **************************************************************************/

#ifndef PLOTOPTIONENGINE_H_
#define PLOTOPTIONENGINE_H_

#include <list>

#include "plotoption.h"

class Inspectable;

/*
 *
 */
class PlotOptionEngine
{
public:
  PlotOptionEngine(const PlotOption& plotOption);
  PlotOptionEngine(const std::list<PlotOption>& plotOptions);

  virtual
  ~PlotOptionEngine();

  /** adds the PlotOptions to the list of plotoptions
      If a plotoption with the same Mode exists, then the old one is deleted first
   */
  virtual PlotOption addPlotOption(PlotOption& plotoption);

  /** removes the PlotOptions with the given type
      @return true if sucessful, false otherwise
   */
  virtual bool removePlotOption(PlotMode mode);


  /** adds an inspectable object for logging. Must be called before addPlotOption and before init!
   */
  virtual void addInspectable(const Inspectable* inspectable);


  /**
     write comment to output streams (PlotOptions). For instance changes in parameters.
  */
  virtual void writePlotComment(const char* cmt);

  /** Performs an step of the PlotOptionEngine
       @param time (optional) current simulation time (used for logging)
   */
   void step(double time=-1);


protected:
  std::list<PlotOption> plotOptions;
  std::list<const Inspectable* > inspectables;


  long int t;

  virtual void plot(double time);

  virtual void plotNames();

};

#endif /* PLOTOPTIONENGINE_H_ */
