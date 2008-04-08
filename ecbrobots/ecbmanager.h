/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
 *    marcoeckert@web.de                                                   *
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
 *   Revision 1.1  2008-04-08 08:14:30  guettler
 *   Initial revision
 *
 *                                                                         *
 ***************************************************************************/
#ifndef __ECBMANAGER_H
#define __ECBMANAGER_H

#include <selforg/configurable.h>

#include "globaldata.h"
#include "ecbagent.h"

// forward declaration begin
class ECBCommunicator;
// forward declaration end

class ECBManager : public Configurable{

public:

  ECBManager();

  virtual ~ECBManager();

  /**
   * This starts the ECBManager. Do not overload it.
   * @return
   */
  bool run(int argc, char** argv);


  /// CONFIGURABLE INTERFACE

  virtual paramval getParam(const paramkey& key) const;

  virtual bool setParam(const paramkey& key, paramval val);

  virtual paramlist getParamList() const;

protected:
  bool simulation_time_reached;

    /**
   * Use this function to define the robots, controller, wiring of
   * the agents.
   * @param global The struct which should contain all neccessary objects
   * like Agents
   * @return true if all is ok!
   */
  virtual bool start(GlobalData& global) = 0;

   /** optional additional callback function which is called every
    * simulation step.
    * To use this method, just overload it.
    * @param globalData The struct which contains all neccessary objects
    * like Agents
    * @param pause indicates that simulation is paused
    * @param control indicates that robots have been controlled this timestep (default: true)
    */
  void addCallback ( GlobalData& globalData,bool pause, bool control ) {};


    /** add own key handling stuff here, just insert some case values
     * To use this method, just overload it
     * @param globalData The struct which contains all neccessary objects
     * like Agents
     * @param key The key number which is pressed
     * @param down tells if the key is pressed down, if down=false,
     * the key is
     * @return true if this method could handle the key,
     * otherwise return false
     */
  virtual bool command ( GlobalData& globalData, int key, bool down ) { return false; };

  // Helper
  int contains(char **list, int len,  const char *str){
    for(int i=0; i<len; i++){
      if(strcmp(list[i],str) == 0) return i+1;
    }
    return 0;
  }


private:
  ECBCommunicator* comm;
  GlobalData globalData;

  virtual void handleStartParameters ( int argc, char** argv );

  virtual bool loop();

};




#endif
