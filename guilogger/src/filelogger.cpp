/***************************************************************************
 *   Copyright (C) 2005 by Dominic Schneider   *
 *   dominic@isyspc8   *
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

#include <stdio.h>
#include "filelogger.h"

FileLogger::FileLogger(QString pf)
{   prefix = pf;
    log= FALSE;
    instream=0;
}

FileLogger::~FileLogger(){
  if(instream) {
    fclose(instream);
  }  
}


void FileLogger::writeChannelNames(char *datablock)
{   int i=2;
    filename = prefix + (QDateTime::currentDateTime()).toString("yyyy-MMMM-ddd hh-mm-ss") + ".log";

    if(datablock == NULL) return;
    if(instream) {
      fclose(instream);
    }
    instream = fopen(filename.latin1(),"w+");
    while(datablock[++i] != '\0') 
      fprintf(instream, "%c", datablock[i]);  //um das #C am Anfang der Zeile nich in Datei zu schreiben
    fprintf(instream, "\n");
}


void FileLogger::writeChannelData(char *datablock)
{
    if(!log || !datablock || !instream) return;

    if(datablock[0]=='#' && datablock[1] == 'C')
    {   
      writeChannelNames(datablock);
      return;
    }
    fprintf(instream, "%s\n", datablock);
}
