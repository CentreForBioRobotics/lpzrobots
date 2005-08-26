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
 *   Revision 1.1  2005-08-26 09:34:35  robot2
 *   ppm image lib
 *
 *   Revision 1.1  2005/08/02 13:18:33  fhesse
 *   function for drawing geoms
 *                                                                 *
 *                                                                         *
 ***************************************************************************/
#ifndef __IMAGEPPM_H
#define __IMAGEPPM_H


class ImagePPM {

private:
  int image_width, image_height;
  unsigned char *image_data;

public:
  ImagePPM ();  
  ~ImagePPM();
  int loadImage(char *filename); // load from PPM file
  int width()           { return image_width;  }
  int height()          { return image_height; }
  unsigned char *data() { return image_data;   }

};

#endif
