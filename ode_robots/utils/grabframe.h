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
 *   Revision 1.6  2009-03-12 08:44:18  martius
 *   fixed video recording
 *
 *   Revision 1.5  2008/04/17 15:59:02  martius
 *   OSG2 port finished
 *
 *   Revision 1.4.4.1  2008/04/08 14:09:23  martius
 *   compiles and runs with OSG2.2. Juhu
 *
 *   Revision 1.4  2006/07/14 12:23:56  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.3.4.1  2006/02/22 15:27:13  martius
 *   class-ified and osg-ified
 *
 *   Revision 1.3  2005/11/09 13:31:51  martius
 *   GPL'ised
 *
 ***************************************************************************/
#ifndef __GRABFRAME_H
#define __GRABFRAME_H

#include <stdio.h>
#include <osg/Image>
#include <osg/Camera>

namespace lpzrobots{

  class VideoStream : public osg::Camera::DrawCallback {
  public:
    VideoStream(){ filename=0; opened=false; pause = false;}

    void open(const char* filename);
    void close();
    bool grabAndWriteFrame(const osg::Camera& camera);

    bool isOpen() const { return opened; }

    // DrawCallback interface
    virtual void operator() (const osg::Camera &) const ;

    bool pause;
  private:
    bool opened;
    char* filename;
    unsigned int w;
    unsigned int h;
    long int counter;
  };

}

#endif
