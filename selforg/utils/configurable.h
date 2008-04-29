#ifndef __CONFIGURABLE_H
#define __CONFIGURABLE_H

#include <iostream>
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

class Configurable {
public:
  
  typedef std::string paramkey;
  typedef double paramval;
  typedef std::list< std::pair<paramkey, paramval> > paramlist;
  typedef std::map< paramkey, paramval* > parammap;

  /// nice predicate function for finding by ID
  struct matchId : public std::unary_function<Configurable*, bool> {
    matchId(int id) : id(id) {}
    int id;
    bool operator()(Configurable* c) { return c->id == id; }
  };

  Configurable(){ id = rand();}
  /// intialise with name and revision (use "$ID$")
  Configurable(const std::string& name, const std::string& revision)
    : name(name), revision(revision) {
      id = rand();
    }
  virtual ~Configurable(){}

  /// return the id of the configurable objects, which is created by random on initialisation
  int getId() const { return id;}


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
  virtual void getRevision(const paramkey& revision) {
    this->revision = revision; 
  }

  /** returns the value of the requested parameter 
      or 0 (+ error message to stderr) if unknown.
  */
  virtual paramval getParam(const paramkey& key) const{
    parammap::const_iterator it = mapOfValues.find(key);
    if(it!=mapOfValues.end()){
      return *((*it).second);
    }else{
      std::cerr << name << ": "<< __FUNCTION__ << ": parameter " << key << " unknown\n";
      return 0;
    }    
  }

  /** sets the value of the given parameter 
      or does nothing if unknown.
  */
  virtual bool setParam(const paramkey& key, paramval val){
    parammap::const_iterator it = mapOfValues.find(key);
    if(it!=mapOfValues.end()){
      *(mapOfValues[key]) = val;
      return true;
    }else{
      return false; // fprintf(stderr, "%s:parameter %s unknown\n", __FUNCTION__, key);
    }
  }
  /** The list of all parameters with there value as allocated lists.
      @return list of key-value pairs 
  */
  virtual paramlist getParamList() const {
    return paramlist(); // return an empty list
  }
  
  /**
     This is the new style for adding configurable parameters. Just call this function 
     for each parameter and you are done.
     If you need to do some special treatment for setting (or getting) of the parameter
     you can handle this by overloading getParam and setParam
  */
  virtual void addParameter(const paramkey& key, paramval* val){
    mapOfValues[key]=val;
  }

  /**
     This function is only provided for convenience. It does the same as addParameter but set the
     variable to the default value 
  */
  virtual void addParameterDef(const paramkey& key, paramval* val, paramval def){
    *val=def;
    mapOfValues[key]=val;
  }

  /** This is a utility function for inserting the filename and the revision number
      at the beginning of the given string buffer str and terminates it.
      @param str buffer (call by reference) 
        that should have space for file+revision+2 characters
      @param file filename given by CVS i.e. $RCSfile$
      @param revision revision number given by CVS i.e. $Revision$
  */
  static void insertCVSInfo(paramkey& str, const char* file, const char* revision);

#ifndef AVR
  /** stores the key values paires into the file : filenamestem.cfg 
      including the comments given in the list
  */
  bool storeCfg(const char* filenamestem, 
		const std::list< std::string>& comments = std::list< std::string>());
  /** restores the key values paires from the file : filenamestem.cfg */
  bool restoreCfg(const char* filenamestem);
  void print(FILE* f, const char* prefix) const;
  void parse(FILE* f);
#endif  
private: 
  int id;
  paramkey name;
  paramkey revision;

  parammap mapOfValues;

};


#endif
