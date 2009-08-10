/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    frankguettler@gmx.de                                                 *
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
 *  windowstatistics provides cool stuff for displaying statistics on the  *
 *  graphics window.                                                       *
 *                                                                         *
 *   $Log$
 *   Revision 1.5  2009-08-10 07:45:54  guettler
 *   uses new BackCaller implementation
 *
 *   Revision 1.4  2008/01/17 09:55:55  der
 *   methods added for adding std::list<AbstractMeasure*> to the HUD
 *
 *   Revision 1.3  2007/12/06 10:02:49  der
 *   abstractground: returns now cornerpoints
 *   abstractobstacle: is now trackable
 *   hudstatistics: supports now AbstractmMeasure
 *
 *   Revision 1.2  2007/09/28 12:31:49  robot3
 *   The HUDSM is not anymore deduced from StatisticalTools, so the statistics
 *   can be updated independently from the HUD
 *   addPhysicsCallbackable and addGraphicsCallbackable now exists in Simulation
 *
 *   Revision 1.1  2007/09/28 10:24:05  robot3
 *   The WindowStatisticsManager is now called HUDStatisticsManager
 *
 *   Revision 1.4  2007/09/28 10:08:49  robot3
 *   fixed memory bugs, statistics are from now on aligned right
 *
 *   Revision 1.3  2007/09/28 09:15:25  robot3
 *   extended comments
 *
 *   Revision 1.2  2007/09/28 08:47:29  robot3
 *   corrected some memory bug (3 still remaining)
 *
 *   Revision 1.1  2007/09/27 10:48:13  robot3
 *   first version of the WSM
 *
 *                                                                         *
 ***************************************************************************/
#ifndef __HUD_STATISTICS_H
#define __HUD_STATISTICS_H

#import <selforg/statistictools.h>

/* forward declaration block */
namespace osgText {
class Text;
class Font;
}

namespace osg {
class Geode;
}

namespace lpzrobots {
class Color;
}
/* end of forward declaration */

namespace lpzrobots {

/**
 * manages all the stuff displaying statistics on the graphics window.
 * This is a experimental version, so do not to be afraid changing this crazy
 * code.
 *
 * This class uses the implementation of the class StatisticTools, which is
 * generalized to make nice statistics. Instead of passing the values to the
 * guilogger (INSPECTABLE interface), we simply diplay this values on the graphics
 * window.
 *
 * So how it works:
 * - overwriting the method getMeasure gives us the ability to create the needed text
 *   object, then storing it in a class named WindowStatistic
 *   (which is stored in the windowStatisticList).
 */
class HUDStatisticsManager : public Callbackable {

public:
  /**
   * creates the HUDStatisticsManager, normally done by class Base.
   * @param geode this is the graphical node at wich the text objects are hooked in.
   */
  HUDStatisticsManager(osg::Geode* geode, osgText::Font* font);

  virtual ~HUDStatisticsManager() {}

  	/**
	 * adds a variable to observe (on the window) and measure the value
	 * @param observedValue    the value to observe.
	 * @param measureName      the name of the measured value
	 * @param mode             the mode of measure
	 * @param stepSpan         in most cases the stepSpan is important to get
	 * the measured value of a number of steps, like AVG:
	 * if stepSpan = 0, AVG is calculated over all steps
	 * if stepSpan = n, AVG is calculated over the LAST n steps
	 * The same counts for all the other MeasureModes.
	 * @param additionalParam  is used for example for mode PEAK, the param is the limit value,
	 * all values minus limit are displayed, values below the limit are set to 0.
  	 * In CONV mode (test the convergence), this value is the epsilon criteria.
	 * @return the object StatisticMeasure. Use addMeasure(...) instead of getMeasure(...) to
  	 * obtain the value adress of the calculated statistic.
  	 * @see StatisticTools
  	 * @see StatisticMeasure
	 */
  virtual StatisticMeasure* getMeasure( double& observedValue, const char* measureName, MeasureMode mode, long stepSpan, double additionalParam =0);

  	/**
	 * adds a variable to observe (on the window) and measure the value
	 * @param observedValue    the value to observe.
	 * @param measureName      the name of the measured value
	 * @param mode             the mode of measure
	 * @param stepSpan         in most cases the stepSpan is important to get
	 * the measured value of a number of steps, like AVG:
	 * if stepSpan = 0, AVG is calculated over all steps
	 * if stepSpan = n, AVG is calculated over the LAST n steps
	 * The same counts for all the other MeasureModes.
	 * @param additionalParam  is used for example for mode PEAK, the param is the limit value,
	 * all values minus limit are displayed, values below the limit are set to 0.
  	 * In CONV mode (test the convergence), this value is the epsilon criteria.
	 * @return the object StatisticMeasure. Use addMeasure(...) instead of getMeasure(...) to
  	 * obtain the value adress of the calculated statistic.
  	 * @see StatisticTools
  	 * @see StatisticMeasure
	 */
  virtual double& addMeasure( double& observedValue, const char* measureName, MeasureMode mode, long stepSpan, double additionalParam =0);
  
    /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * @param measure the measure to add
   */
  virtual double& addMeasure(AbstractMeasure* measure);
  
  /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * With this method you can add a list of AbstractMeasures.
   * @param measureList the list of measures to add
   */
  virtual double& addMeasureList(std::list<AbstractMeasure*> measureList);

    /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * With this method you can add a list of AbstractMeasures.
   * @param measureList the list of measures to add
   */
  virtual double& addMeasureList(std::list<ComplexMeasure*> measureList);
  
  
      /**
   * You can add another abstract measure you like. in some cases (e.g. complex
   * measures) it is better to let the measure decide how it likes to be initialized
   * With this method you can add a list of AbstractMeasures.
   * @param measureList the list of measures to add
   */
  virtual double& addMeasureList(std::list<StatisticMeasure*> measureList);
  
  
  	/**
	 * starts the measure at a specific time. This is useful if there are
	 * values that have to be ignored at simulation start.
	 * @param step number of steps (normally simsteps) to wait for beginning the measures
	 */
  virtual void beginMeasureAt(long step) { statTool->beginMeasureAt(step);}

  /**
   * Tells you wether the measures have already been started.
   */
  virtual bool measureStarted() { return statTool->measureStarted(); }


	/**
	 * CALLBACKABLE INTERFACE
	 *
	 *	this method is invoked when a callback is done from the class where this
	 * class is for callback registered, it is overwritten
	 */
  virtual void doOnCallBack(BackCaller* source, BackCaller::CallbackableType type = BackCaller::DEFAULT_CALLBACKABLE_TYPE);


  virtual StatisticTools* getStatisticTools() { return statTool; }

protected:

  /**
   * Nested class WindowStatistic, which puts the measure and the graphics text together.
   */
  class WindowStatistic {
  public:

    WindowStatistic(AbstractMeasure* measure, osgText::Text* text) : measure(measure),
      text(text) {}

    virtual ~WindowStatistic() {}

    virtual AbstractMeasure* getMeasure() { return measure; }

    virtual osgText::Text* getText() { return text; }

  private:
    AbstractMeasure* measure;
    osgText::Text* text;
  };

/// the struct list which holds the measures and the appropiate text
  std::list<WindowStatistic*> windowStatisticList;

  StatisticTools* statTool;

  // position of first graphical text
  float xInitPosition;
  float yInitPosition;
  float zInitPosition;
  float yOffset;

  // graphical node
  osg::Geode* geode;

  // default text properties
  osgText::Font* font;
  Color* textColor;
  int fontsize;

};


}

#endif
