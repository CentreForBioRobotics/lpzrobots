/***************************************************************************
 *   Copyright (C) 2008 by mc   *
 *   mc@linux-6hav   *
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
 ***************************************************************************/
#ifndef SPHERICAL_ROBOT_SUB_WIDGET_H
#define SPHERICAL_ROBOT_SUB_WIDGET_H

#include <QFrame>
#include <QWidget>
#include <QPainter>
#include <QPixmap>



class SphericalRobotSubWidget: public QWidget {

Q_OBJECT

public:
SphericalRobotSubWidget(QWidget *parent = 0);
//const int* getParam();
void setArrowParam(int alpha, int len);
int alpha;
int len;

protected:
void paintEvent(QPaintEvent *);

private:
QPixmap pmap;
QPoint motor_arrow_part1[3];
QPoint motor_arrow_part2[4];
int ir_sensors[12];


};

#endif
