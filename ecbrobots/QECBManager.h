/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
 *    jhoffmann@informatik.uni-leipzig.de                                  *
 *    wolfgang.rabe@01019freenet.de                                        *
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
 *   Revision 1.2  2010-11-26 12:22:37  guettler
 *   - Configurable interface now allows to set bounds of paramval and paramint
 *     * setting bounds for paramval and paramint is highly recommended (for QConfigurable (Qt GUI).
 *   - bugfixes
 *   - current development state of QConfigurable (Qt GUI)
 *
 *   Revision 1.1  2010/11/10 09:32:00  guettler
 *   - port to Qt part 1
 *                                                *
 *                                                                         *
 ***************************************************************************/

#ifndef __QECBMANAGER_H_
#define __QECBMANAGER_H_

#include <QObject>

#include <selforg/configurable.h>

#include "QGlobalData.h"
#include "ecbagent.h"

#include "QECBCommunicator.h"

namespace lpzrobots {

  // forward declaration begin
  class QECBCommunicator;
  // forward declaration end

  class QECBManager : public QObject {

    Q_OBJECT

    public:

      QECBManager(int argc, char** argv);

      virtual ~QECBManager();

      /**
       * This initalizes and starts the ECBManager. Do not overload it.
       */
      void initialize();

      virtual QGlobalData& getGlobalData();

      enum EVENT {
        EVENT_START_LOOP,
        EVENT_RESTART_LOOP,
        EVENT_PAUSE_LOOP,
        EVENT_STOP_LOOP,
        EVENT_START_GUILOGGER
      };

    public slots:

      void sl_GUIEventHandler(int);

      void sl_textLog(QString log);  // forwarded to QECBRobotsWindow, using sig_textLog(QString log)

    signals:

      void sig_communicationStateChanged(QECBCommunicator::ECBCommunicationState);
      void sig_textLog(QString log);

    protected:
      bool simulation_time_reached;

      /**
       * Use this function to define the robots, controller, wiring of
       * the agents.
       * @param global The struct which should contain all neccessary objects
       * like Agents
       * @return true if all is ok!
       */
      virtual bool start(QGlobalData& global) = 0;

      /** optional additional callback function which is called every
       * simulation step.
       * To use this method, just overload it.
       * @param globalData The struct which contains all neccessary objects
       * like Agents
       * @param paused indicates that simulation is paused
       * @param control indicates that robots have been controlled this timestep (default: true)
       */
      void addCallback(QGlobalData& globalData, bool pause, bool control) {
      }
      ;

      /** add own key handling stuff here, just insert some case values
       * To use this method, just overload it
       * @param globalData The struct which contains all neccessary objects
       * like Agents
       * @param key The key number which is pressed
       * @return true if this method could handle the key,
       * otherwise return false
       */
      virtual bool command(QGlobalData& globalData, int key) {
        return false;
      }
      ;

      // Helper
      int contains(char **list, int len, const char *str) {
        for (int i = 0; i < len; i++) {
          if (strcmp(list[i], str) == 0)
            return i + 1;
        }
        return 0;
      }

    private:
      QGlobalData globalData;
      bool commInitialized;

      int argc;
      char** argv;


      /**
       * Destroys all created ECBs, Agents and Controller.
       * After that you can call the start() function again.
       */
      void cleanup();



      virtual void handleStartParameters();

      virtual void startLoop();
      virtual void stopLoop();

  };

} // namespace lpzrobots

#endif /* __QECBMANAGER_H_ */
