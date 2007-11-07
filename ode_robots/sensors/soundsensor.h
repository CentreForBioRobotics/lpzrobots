/***************************************************************************
 *   Copyright (C) 2007 by Robot Group Leipzig                             *
 *    georg@nld.ds.mpg.de                                                  *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *                                                                         *
 ** Started on  Mon Oct 15 16:48:03 2007 Georg Martius *
 ** Last update Mon Oct 15 16:48:03 2007 Georg Martius *
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
 *   Revision 1.1  2007-11-07 13:22:59  martius
 *   new sound sensor
 *
 *
 *                                                                 *
 ***************************************************************************/
#ifndef   	SOUNDSENSOR_H_
# define   	SOUNDSENSOR_H_

#include "sensor.h"

namespace lpzrobots {

  /** Sound sensor
      with possible direction and frequency detection and also
      distance dependence
  */
  class SoundSensor: public Sensor {
  public:  
    typedef enum Measure { Segments, Angle, AngleVel };

    /** @param dim Up-axis of the robot (sometimes it is not Z)	
    */
    SoundSensor(Dimensions dim = Z, Measure measure = Angle, 
		int segments=1, int levels=1, float maxDistance=1000);
    virtual ~SoundSensor();
  
    virtual void init(Primitive* own){ this->own = own;}
  
    virtual bool sense(const GlobalData& globaldata);

    virtual int getSensorNumber() const;

    virtual std::list<sensor> get() const;

    virtual int get(sensor* sensors, int length) const;

  private:
    short dim; ///< the axis in which the sensor is selective around
    Measure measure; ///< how to measure
    int segments;
    int levels;
    float maxDistance;
    
    double* val;
    double* oldangle;

    Primitive* own;

  };

  
  

}

#endif 	    /* !SOUNDSENSOR_H_ */
