/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
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
 *   $Log$
 *   Revision 1.8  2009-08-05 22:47:10  martius
 *   added support for integer variables and
 *    proper display for bool and int
 *
 *   Revision 1.7  2009/08/05 08:36:22  guettler
 *   added support for boolean variables
 *
 *   Revision 1.6  2009/03/27 06:16:57  guettler
 *   support for gcc 4.3 compatibility (has to be checked), StatisticTools moves from utils to statistictools
 *
 *   Revision 1.5  2008/11/14 09:16:15  martius
 *   small things
 *
 *   Revision 1.4  2008/08/14 08:06:02  robot1
 *   fixed compile bug
 *
 *   Revision 1.3  2008/08/01 14:42:03  guettler
 *   we try the trip to hell! make selforg AVR compatible...good luck (first changes)
 *
 *   Revision 1.2  2008/04/29 07:41:40  guettler
 *   added GPL and revision tag
 *
 *   Revision 1.1  2008/04/29 07:39:54  guettler
 *   interfaces moved to selforg/utils directory
 *
 *   Revision 1.7  2008/04/11 14:12:30  martius
 *   comments paramter in storeCfg
 *
 *   Revision 1.6  2007/06/08 15:44:32  martius
 *   termination string -> more robust parsing
 *
 *   Revision 1.5  2007/03/26 13:13:47  martius
 *   store and restore with params
 *
 *   Revision 1.4  2007/01/24 14:38:35  martius
 *   new implementation with a map from key to reference.
 *   addParameter and addParameterDef are used for registration of a parameter
 *   the old style is still supported
 *
 *   Revision 1.3  2006/07/14 12:23:57  martius
 *   selforg becomes HEAD
 *
 *   Revision 1.1.2.7  2006/06/25 21:56:07  martius
 *   configureable has name and revision
 *
 *   Revision 1.1.2.6  2006/02/08 16:17:40  martius
 *   no namespace using
 *
 *   Revision 1.1.2.5  2006/01/18 16:48:10  martius
 *   configurables can be stored and reloaded
 *
 *   Revision 1.1.2.4  2006/01/18 10:45:32  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.3  2006/01/18 10:44:49  martius
 *   (re)storeCfg
 *
 *   Revision 1.1.2.2  2006/01/16 17:40:13  martius
 *   parsing works
 *
 *   Revision 1.1.2.1  2006/01/16 17:27:17  martius
 *   *** empty log message ***
 *
 *                                                                 *
 ***************************************************************************/
#ifndef __CONFIGURABLE_H
#define __CONFIGURABLE_H

#include <iostream>
#include <cstdlib>

#include <list>
#include <utility>
#include <string>
#include <map>
#include "stl_adds.h"

/**
 Abstact class for configurable objects. Sort of Hashmap interface. Parameters are double values

 The Configurator is a (planned) external tool that can be used for changing the values of configurable objects.

 * Protocoll for Configurator:
 \code
 To Configurator (BNF notation):
 Conf         := <Comp>*
 Comp         := <CompName> <Pair>*
 CompName     := [<string>][<int>]<newline>
 Pair         := <alphanum>=<double><newline>
 \endcode
 the remaining tags are self explanatory

 Example
 \code
 [Component name which can contain spaces and digits and .,- ][ID1]
 key1 = value1
 key2 = value2
 .
 .
 [Other Component name which can contain spaces and digits and .,- ][ID2]
 key3 = value3
 \endcode

 Answer: (from Communicator to Simulation environment)\n
 1. On change of a parameter:
 \code
 [ID] key=newvalue
 \endcode
 or
 \code
 key=newvalue
 \endcode
 the latter one means that the parameter is changed in all components

 2. Request of the description as defined above.
 \code
 #Something I don\'t care
 \endcode
 */

class Configurable
{
  public:

    typedef std::string paramkey;
    typedef double paramval;
    typedef std::list<std::pair<paramkey, paramval> > paramlist;
    typedef std::map<paramkey, paramval*> parammap;

    // params of type bool
    typedef bool parambool;
    typedef std::list<std::pair<paramkey, parambool> > paramboollist;
    typedef std::map<paramkey, parambool*> paramboolmap;

    // params of type int
    typedef int paramint;
    typedef std::list<std::pair<paramkey, paramint> > paramintlist;
    typedef std::map<paramkey, paramint*> paramintmap;

    /// nice predicate function for finding by ID
    struct matchId : public std::unary_function<Configurable*, bool>
    {
        matchId(int id) :
          id(id)
        {
        }
        int id;
        bool operator()(Configurable* c)
        {
          return c->id == id;
        }
    };

    Configurable()
    {
      id = rand();
    }
    /// intialise with name and revision (use "$ID$")
    Configurable(const std::string& name, const std::string& revision) :
      name(name), revision(revision)
    {
      id = rand();
    }

//   Configurable(const Configurable& copy)
//     :id(copy.id), name(copy.name), revision(revision), 
//      mapOfValues(mapOfValues), mapOfBoolean(mapOfBoolean), mapOfInteger(mapOfInteger)
//   {  
//   }
  

    virtual ~Configurable()
    {
    }

    /// return the id of the configurable objects, which is created by random on initialisation
    int getId() const {
      return id;
    }

    /// return the name of the object
    virtual paramkey getName() const {
      return name;
    }

    /// returns the revision of the object
    virtual paramkey getRevision() const {
      return revision;
    }

    /// stes the name of the object
    virtual void setName(const paramkey& name){
      this->name = name;
    }
    /// sets the revision Hint: {  return "$ID$"; }
    virtual void setRevision(const paramkey& revision) {
      this->revision = revision;
    }

    /** returns the value of the requested parameter
     or 0 (+ error message to stderr) if unknown.
     */
    virtual paramval getParam(const paramkey& key) const;

    /** sets the value of the given parameter
     or does nothing if unknown.
     */
    virtual bool setParam(const paramkey& key, paramval val);

    /** The list of all parameters with there value as allocated lists.
	Note that these are only parameters that are managed manually (with setParam, getParam)
	@see getAllParamNames()	
     @return list of key-value pairs
     */
    virtual paramlist getParamList() const {
      return paramlist(); // return an empty list
    }

    /// returns all names that are configureable
    virtual std::list<paramkey> getAllParamNames();

    /**
     This is the new style for adding configurable parameters. Just call this function
     for each parameter and you are done.
     If you need to do some special treatment for setting (or getting) of the parameter
     you can handle this by overloading getParam and setParam
     */
    virtual void addParameter(const paramkey& key, paramval* val) {
      mapOfValues[key] = val;
    }

    /**
     See addParameter(const paramkey& key, paramval* val) but for bool values
     */
    virtual void addParameter(const paramkey& key, parambool* val) {
      mapOfBoolean[key] = val;
    }

    /**
     See addParameter(const paramkey& key, paramval* val) but for int values
     */
    virtual void addParameter(const paramkey& key, paramint* val) {
      mapOfInteger[key] = val;
    }

    /**
     This function is only provided for convenience. It does the same as addParameter but set the
     variable to the default value
     */
    virtual void addParameterDef(const paramkey& key, paramval* val, paramval def){
      *val = def;
      mapOfValues[key] = val;
    }

    /**
     This function is only provided for convenience. It does the same as addParameter but set the
     variable to the default value
     */
    virtual void addParameterDef(const paramkey& key, parambool* val, parambool def)
    {
      *val = def;
      mapOfBoolean[key] = val;
    }
    /**
     This function is only provided for convenience. It does the same as addParameter but set the
     variable to the default value
     */
    virtual void addParameterDef(const paramkey& key, paramint* val, paramint def)
    {
      *val = def;
      mapOfInteger[key] = val;
    }

    /** This is a utility function for inserting the filename and the revision number
     at the beginning of the given string buffer str and terminates it.
     @param str buffer (call by reference)
     that should have space for file+revision+2 characters
     @param file filename given by CVS i.e. $RCSfile$
     @param revision revision number given by CVS i.e. $Revision$
     */
    static void insertCVSInfo(paramkey& str, const char* file, const char* revision);

    /** stores the key values paires into the file : filenamestem.cfg
     including the comments given in the list
     */
    virtual bool storeCfg(const char* filenamestem, const std::list<std::string>& comments = std::list<std::string>());
    /** restores the key values paires from the file : filenamestem.cfg */
    virtual bool restoreCfg(const char* filenamestem);
    void print(FILE* f, const char* prefix) const;
    void parse(FILE* f);

  private:
    int id;
    paramkey name;
    paramkey revision;

    parammap mapOfValues;
    paramboolmap mapOfBoolean;
    paramintmap mapOfInteger;
};

#endif // __CONFIGURABLE_H
