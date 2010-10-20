/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de
 *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    guettler@informatik.uni-leipzig.de                                   *
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
 *                                                                         *
 *                                                                         *
 *  $Log$
 *  Revision 1.4  2010-10-20 13:20:21  martius
 *  bugfixing: Tiptool caused crash
 *
 *  Revision 1.3  2010/10/08 07:41:00  martius
 *  modified matrixviz.pro and removed selforg/stl_adds from GroupPlotChannel
 *
 *  Revision 1.2  2009/10/02 15:25:40  robot14
 *  filters, main app - not finished yet
 *
 *  Revision 1.1  2009/08/13 13:14:04  robot14
 *  first version
 *
 *  Revision 1.1  2009/04/17 14:17:33  guettler
 *  New PlotChannels and Filters for matrices
 *										   *
 *                                                                         *
 **************************************************************************/
#include "GroupPlotChannel.h"
#include "tools/stl_adds.h"
#include <cassert>
#include <iostream>

using namespace std;

GroupPlotChannel::GroupPlotChannel(string name) : AbstractPlotChannel(name) {
  // TODO Auto-generated constructor stub

}

GroupPlotChannel::~GroupPlotChannel() {
  // TODO Auto-generated destructor stub
}

void GroupPlotChannel::addPlotChannel(AbstractPlotChannel* channelToAdd) {
  this->channelsOfGroup.push_back(channelToAdd);
}

list<double> GroupPlotChannel::getValues() {
  list<double> valueList;
  FOREACHC(list<AbstractPlotChannel*>, channelsOfGroup, channelIt) {
    //list<double> valueList;
    valueList.push_back((*channelIt)->getValue());
    //return valueList;
  }
  return valueList;
}

list<AbstractPlotChannel*> GroupPlotChannel::getChannelsOfGroup() {
  return channelsOfGroup;
}

AbstractPlotChannel* GroupPlotChannel::at(int pos){
  if(pos>=channelsOfGroup.size()){
    std::cerr << " error: GroupPlotChannel::at " << pos << "/" << channelsOfGroup.size() << std::endl;  
    return 0;                                                                                                
  }

  //  assert(pos < channelsOfGroup.size());

  AbstractPlotChannel* channel = 0;
  int it = 0;
  FOREACHC(list<AbstractPlotChannel*>, channelsOfGroup, channelIt) {
    if(it == pos) channel = *channelIt;
    it++;
  }
//  for( const_iterator i = channelsOfGroup.begin(); i !=  channelsOfGroup.end(); i++){
//    if(it == pos) cannel = *i;
//    it++;
//  }
  return channel;

}


