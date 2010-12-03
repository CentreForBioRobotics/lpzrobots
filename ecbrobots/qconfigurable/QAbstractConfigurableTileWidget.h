/***************************************************************************
 *   Copyright (C) 2010 by                                                 *
 *   Research Network for Self-Organization of Robot Behavior              *
 *    guettler@informatik.uni-leipzig.de                                   *
 *    wrabe@informatik.uni-leipzig.de                                      *
 *    Georg.Martius@mis.mpg.de                                             *
 *    ralfder@mis.mpg.de                                                   *
 *    frank@nld.ds.mpg.de                                                  *
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
 *   Revision 1.1  2010-12-03 11:11:41  wrabe
 *   - replace of the ConfigurableLineWidgets by ConfigurableTileWidgets
 *   - (final rename from lines to tiles)
 *   - for history look at the ConfigurableLineWidget-classes
 *   - now handled paramVal, paramInt and paramBool, all the params are displayed
 *     as ConfigurableTiles witch can be show and hide seperatly or arranged by user
 *     (showHideDialog reacheble by contextMenu (right click an the Widget containing
 *     the tiles ), arrange the Tiles is can done by drag and drop (there is no history or
 *     storage implementet yet))
 *
 *   Revision 1.3  2010/11/30 17:07:06  wrabe
 *   - new class QConfigurableTileShowHideDialog
 *   - try to introduce user-arrangeable QConfigurationTiles (current work, not finished)
 *
 *   Revision 1.2  2010/11/28 20:33:44  wrabe
 *   - current state of work: only paramval´s
 *   - construct a configurable as a tile containing a QSlider to change the value by drag with mouse as well as a QSpinBox to change the configurable by typing new values (mouse-scrolls are also supported)
 *   - minimum and maximum boundaries can´t be changed will be so far, only a change- dialog-dummy is reacable over the context-menu
 *
 *   Revision 1.1  2010/11/26 12:22:36  guettler
 *   - Configurable interface now allows to set bounds of paramval and paramint
 *     * setting bounds for paramval and paramint is highly recommended (for QConfigurable (Qt GUI).
 *   - bugfixes
 *   - current development state of QConfigurable (Qt GUI)
 *
 *                                                                         *
 ***************************************************************************/

#ifndef __QCONFIGURABLETILEWIDGET_H_
#define __QCONFIGURABLETILEWIDGET_H_

#include "selforg/configurable.h"
#include <QWidget>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QFrame>
#include <QLabel>

namespace lpzrobots {
  
  class QAbstractConfigurableTileWidget : public QFrame {

    Q_OBJECT


    public:
      QAbstractConfigurableTileWidget(Configurable* config, Configurable::paramkey& key);
      virtual ~QAbstractConfigurableTileWidget();
      virtual void setName(QString name) {}
      virtual QString getName() {return QString(key.c_str());}
      virtual void toDummy(bool set){}
      virtual bool contains(QPoint pos);

      virtual void setTileIndex(int index) {
        tileIndex = index;
      }
      virtual int getTileIndex() {
        return tileIndex;
      }

      virtual QString getConfigurableName();
      static QSize widgetSize;

    protected:
      Configurable* config;
      Configurable::paramkey key;
      int tileIndex;

      //      QLabel* setAndCreateMinBoundLabel(QString minBoundString);
      //      QLabel* setAndCreateMaxBoundLabel(QString maxBoundString);
      //      QSlider* setAndCreateSlider(int minBound, int maxBound, int steps);
      //      QDoubleSpinBox* setAndCreateDoubleSpinBox(double val, int minBound, int maxBound);


  };

}

#endif /* __QCONFIGURABLETILEWIDGET_H_ */
