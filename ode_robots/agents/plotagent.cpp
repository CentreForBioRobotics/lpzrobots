/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
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
 *                                                                         *
 *   $Log$
 *   Revision 1.8  2005-07-14 13:34:33  fhesse
 *   added parameter for delay in guilogger init
 *   -d 15 (not to long, but long enough to allow the gnuplot windows to be drawn)
 *
 *   Revision 1.7  2005/07/08 10:14:51  martius
 *   derivative agent works fine
 *   guilogger logmode controlable
 *
 *   Revision 1.6  2005/07/06 16:05:54  martius
 *   changed to noisegenerator
 *
 *   Revision 1.5  2005/06/27 16:04:58  fhesse
 *   -m added to guilogger call (required by new guilogger version)
 *
 *   Revision 1.4  2005/06/21 15:30:33  martius
 *   sensornumber taken from controller!
 *
 *   Revision 1.3  2005/06/17 10:48:58  martius
 *   Catched Signal SIGPIPE (in case Logger dies)
 *   Logging mode as enum (GuiLogger, NoPlot)
 *
 *   Revision 1.2  2005/06/15 14:02:47  martius
 *   revised and basicly tested
 *                                                                 *
 ***************************************************************************/
#include "plotagent.h"
#include <signal.h>
#include "printInternals.h"
#include "printInternals.cpp" // TODO: avoid this kind of hack

PlotAgent::PlotAgent(PlotMode plotmode/*=GuiLogger*/)
  : Agent(), plotmode(plotmode) 
{ 
  pipe=0;
  numberInternalParameters=0;
}

bool PlotAgent::OpenGui(){
  // this prevents the simulation to terminate if the child (guilogger) closes
  // or if we fail to open it.
  signal(SIGPIPE,SIG_IGN); 
  // TODO: get the guilogger call from some  config
  if(plotmode == GuiLogger_File){
    pipe=popen("guilogger -m pipe -l -d 15 > /dev/null","w");
  }else{
    pipe=popen("guilogger -m pipe -d 15 -l > /dev/null","w");
  }
  if(pipe==0){
    fprintf(stderr, "%s:%i: could not open guilogger!\n", __FILE__, __LINE__);    
    return false;
  }else return true;
}

void PlotAgent::CloseGui(){
    pclose(pipe);
    pipe=0;
}

bool PlotAgent::init(AbstractController* controller, AbstractRobot* robot){
  if(!Agent::init(controller, robot)) return false;  
  if(plotmode != NoPlot){
    if(!OpenGui()) return false;
    int s = controller->getSensorNumber();
    int m = robot->getMotorNumber();
    numberInternalParameters = printInternalParameterNames(pipe, s,m,controller);
  }
  return true;
}

void PlotAgent::plot(const sensor* x, int sensornumber, const motor* y, int motornumber){
  if(!controller || !x || !y || plotmode==NoPlot || !pipe) return;
  if(sensornumber!=controller->getSensorNumber()) {
    fprintf(stderr, "%s:%i: Given sensor number does not match the one from robot!\n", 
	    __FILE__, __LINE__);
  }
  if(motornumber!=robot->getMotorNumber()) {
    fprintf(stderr, "%s:%i: Given motor number does not match the one from robot!\n", 
	    __FILE__, __LINE__);
  }

   printInternalParameters(pipe, x, sensornumber, y, motornumber, 
 			  numberInternalParameters, controller);
 
};
