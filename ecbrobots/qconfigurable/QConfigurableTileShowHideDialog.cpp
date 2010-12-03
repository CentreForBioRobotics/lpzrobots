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
 *   Revision 1.2  2010-12-03 11:11:53  wrabe
 *   - now handled paramVal, paramInt and paramBool, all the params are displayed
 *     as ConfigurableTiles witch can be show and hide seperatly or arranged by user
 *     (showHideDialog reacheble by contextMenu (right click an the Widget containing
 *     the tiles ), arrange the Tiles is can done by drag and drop (there is no history or
 *     storage implementet yet))
 *
 *   Revision 1.1  2010/11/30 17:07:06  wrabe
 *   - new class QConfigurableTileShowHideDialog
 *   - try to introduce user-arrangeable QConfigurationTiles (current work, not finished)
 *
 *                                                                         *
 ***************************************************************************/

#include "QConfigurableTileShowHideDialog.h"
#include <QDialogButtonBox>
#include <QRegion>
#include <QMessageBox>
#include <QLinkedList>

namespace lpzrobots {
  
  QConfigurableTileShowHideDialog::QConfigurableTileShowHideDialog(QMap<QString, QAbstractConfigurableTileWidget*> configLineWidgetMap,
      QGridLayout *parentGridLayout) :
    configLineWidgetMap(configLineWidgetMap), parentGridLayout(parentGridLayout) {

    setFixedSize(200, 400);
    setLayout(new QVBoxLayout());

    cbFrame_ypos = 0;
    cbFrame = new QFrame();
    cbFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout* grid = new QGridLayout(cbFrame);

    int row = 0;
    checkBoxConfiguableShowHideList.clear();
    foreach(QAbstractConfigurableTileWidget* configLineWidget, configLineWidgetMap)
      {
        QCheckBox* cb = new QCheckBox();
        cb->setText(configLineWidget->getConfigurableName());
        //        cb->setMinimumSize(170, 15);
        if (!configLineWidget->isHidden())
          cb->setCheckState(Qt::Checked);
        else cb->setCheckState(Qt::Unchecked);
        grid->addWidget(cb, row++, 0, Qt::AlignTop);
        checkBoxConfiguableShowHideList.append(cb);
      }
    //cbFrame->setMinimumSize(180, row * 30);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(false);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setWidget(cbFrame);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(sl_dialogAccept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(sl_dialogReject()));

    layout()->addWidget(scrollArea);
    layout()->addWidget(buttonBox);
  }
  
  QConfigurableTileShowHideDialog::~QConfigurableTileShowHideDialog() {
    delete (cbFrame);
    delete (scrollArea);

  }

  void QConfigurableTileShowHideDialog::sl_dialogAccept() {
    // 1. generate a list of ConfigurableTiles that currently are visible and also should be visible in future
    // 2. generate a list of ConfigurableTiles that was not vilible until now, but should be visible
    int index_newbies = 0;
    int index_visible_max = 0;
    QMap<int, QAbstractConfigurableTileWidget*> configurableTile_Visibles;
    QMap<int, QAbstractConfigurableTileWidget*> configurableTile_Newbies;
    foreach(QCheckBox* cb, checkBoxConfiguableShowHideList)
      {
        QAbstractConfigurableTileWidget* configurableTile = configLineWidgetMap[cb->text()];
        if (configurableTile == 0) continue;

        if (cb->checkState() == Qt::Checked){
          if (configurableTile->isVisible()){
            configurableTile_Visibles.insert(configurableTile->getTileIndex(), configurableTile);
            if (index_visible_max < configurableTile->getTileIndex()) index_visible_max = configurableTile->getTileIndex();
          }else{
            configurableTile_Newbies.insert(index_newbies++, configurableTile);
          }
        }
        configurableTile->hide();
      }

    int newTileIndex = 0;
    foreach(QAbstractConfigurableTileWidget* configurableTile, configurableTile_Visibles)
      {
        configurableTile->setTileIndex(newTileIndex++);
        configurableTile->show();
      }

    foreach(QAbstractConfigurableTileWidget* configurableTile, configurableTile_Newbies)
      {
        configurableTile->setTileIndex(newTileIndex++);
        configurableTile->show();
      }

    this->close();
  }

  void QConfigurableTileShowHideDialog::sl_dialogReject() {
    this->close();
  }

}
