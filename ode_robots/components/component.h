/**
 * component.h
 *
 * components are the building parts for robots
 * components can be composed of zero or more sub components
 * components can be bodies + geometry, sensors, ...
 * components may expose wires
 *
 * the interface to control the robot is a set of wires.
 */



#include <math.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>
#include <iostream>
#include <typeinfo>

#include <vector>
#include <list>

#include "vector.h"
#include "exceptions.h"
#include "cubic_spline.h"

#include "odehandle.h"
#include "configurable.h"
#include "abstractrobot.h"


#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#endif


#ifndef component_h
#define component_h



namespace university_of_leipzig {
namespace robots {

class IWire;
class IComponent;
class AbstractMotorComponent;
class UniversalMotorComponent;
class MotorWire;


typedef std::list<IComponent*> ComponentContainer;
typedef Vector3<dReal>              Vertex;
typedef std::list< Vertex > VertexList;


typedef Vector3<dReal>      Angle;
typedef std::list< Angle >  AngleList;

typedef std::list<IWire*> WireContainer;
//typedef std::vector<dJointID> joint_id_list;






/**
 * Wire
 *
 *
 * a wire is always attached to a component
 */
class IWire {
 public:
  virtual IComponent& get_component() const = 0;
};



class IInputWire : virtual public IWire {
 public:
  virtual void put(dReal value) = 0;
};


class IOutputWire : virtual public IWire {
 public:
  virtual dReal get() const = 0;
};

class IBidirectionalWire : virtual public IInputWire,
                           virtual public IOutputWire {
 public:
  // no new members required
};




/**
 * Component
 *
 * the building parts for robots
 *
 *
 */
class IComponent : public Configurable
{
 public:
  virtual unsigned get_sub_component_count() const = 0;

  virtual IComponent &get_sub_component(unsigned index) const = 0;

  virtual unsigned expose_wires(WireContainer &r_wire_set) = 0;
  
  virtual void draw() const = 0;

  virtual const IComponent* does_contain_geom(const dGeomID geom_id,
			        	      bool b_recursive) const = 0;

  virtual bool collision_callback(OdeHandle *p_ode_handle, 
			          dGeomID geom_id_0, 
				  dGeomID geom_id_1) const = 0;
};

 
/**
 * AbstractComponent
 *
 *
 */
class AbstractComponent : public IComponent {
 protected:
  OdeHandle ode_handle;
  
 public:
  AbstractComponent(OdeHandle &r_ode_handle);
  virtual ~AbstractComponent();

  virtual unsigned   get_sub_component_count() const;
  virtual IComponent &get_sub_component(unsigned index) const;
  
  virtual unsigned   expose_wires(WireContainer &r_wire_set);

  virtual const IComponent* does_contain_geom(const dGeomID geom_id,
			        	      bool b_recursive) const;


  paramkey getName() const;
  virtual paramlist getParamList() const;
  virtual paramval  getParam(const paramkey& key) const;
  virtual bool      setParam(const paramkey& key, paramval val);
};
 

/**
 * AbstractCompoundComponent
 *
 *
 */
class AbstractCompoundComponent : public AbstractComponent {
 protected:
  ComponentContainer component_container;
  
 public:
  AbstractCompoundComponent(OdeHandle &r_ode_handle);
  virtual ~AbstractCompoundComponent();

  virtual unsigned   get_sub_component_count() const;
  virtual IComponent &get_sub_component(unsigned index) const;
   
  virtual unsigned   expose_wires(WireContainer &r_wire_set);

  virtual const IComponent* does_contain_geom(const dGeomID geom_id,
			        	      bool b_recursive) const;

  virtual void draw() const;

  virtual paramlist getParamList() const;
  virtual paramval  getParam(const paramkey& key) const;
  virtual bool      setParam(const paramkey& key, paramval val);
};
 


/**
 * SimplePhysicalComponent
 *
 *
 */
class SimplePhysicalComponent : public AbstractComponent {
 protected:
  dBodyID body_id;
  dGeomID geom_id;

 public:
  SimplePhysicalComponent(OdeHandle &r_ode_handle,
			  dBodyID _body_id = NULL,
			  dGeomID _geom_id = NULL);


  void    set_body_id(dBodyID _body_id);
  dBodyID get_body_id() const;

  virtual void    set_geom_id(dGeomID _geom_id);
  virtual dGeomID get_geom_id() const;

  /*
  virtual unsigned expose_wires(WireContainer &out_r_wire_container);

  virtual unsigned get_sub_component_count() const;
  virtual IComponent &get_sub_component(unsigned index) const;
  */
  
  virtual const IComponent* does_contain_geom(const dGeomID _geom_id, 
					      bool b_recursive) const;
  
 
  virtual bool collision_callback(OdeHandle *p_ode_handle, 
				  dGeomID geom_id_0, dGeomID geom_id_1) const;

  virtual void draw() const;
};



/**
 * AbstractMotorComponent
 *
 *
 * a motor has output wires (new angular velocity)
 * and input wire (current angular velocity)
 */
class AbstractMotorComponent : public AbstractComponent {
 protected:
  dJointID joint_id; // the joint the motor is attached to

 public:
  AbstractMotorComponent(dJointID _joint_id);


  virtual void    set_angular_velocity(dReal angular_velocity)       = 0;
  virtual dReal get_angular_velocity()                         const = 0;

  // a motor usually exposes 2 wires: one for controlling the angular velocity
  // and one for retrieving the current velocity
  // some motors may also return wires for setting/getting the maximum force
  //virtual unsigned expose_wires(WireContainer &out_r_wire_container) const = 0;
};


class MotorWire : public IBidirectionalWire {
  AbstractMotorComponent *p_motor;

 public:
  MotorWire(AbstractMotorComponent *_p_motor = NULL);

  //UniversalMotorComponent *get_component() const;
  IComponent &get_component() const;

  dReal get()            const;
  void  put(dReal value);
};


/**
 * UniversalMotorComponent
 *
 *
 * a motor wrapper.
 *
 * must be attached to one of the 2 axis of a universal joint
 */
class UniversalMotorComponent : public AbstractMotorComponent {
 protected:
  char axis; // 0 for the one attached to body 0, 1 for the other one

  MotorWire wire;

  bool param_show_axis;

 public:
  UniversalMotorComponent(dJointID _joint_id, char _axis);

  void  set_angular_velocity(dReal angular_velocity);
  dReal get_angular_velocity() const;

  unsigned expose_wires(WireContainer &out_r_wire_container);

  void draw() const;

  bool collision_callback(OdeHandle *p_ode_handle, 
			  dGeomID geom_id_0, dGeomID geom_id_1) const;
  /*
  const IComponent* does_contain_geom(const dGeomID geom_id, 
				      bool b_recursive) const;
  */
  /*
  unsigned get_sub_component_count() const;
  IComponent &get_sub_component(unsigned index) const;
  */

  virtual paramlist getParamList() const;
  virtual paramval  getParam(const paramkey& key) const;
  virtual bool      setParam(const paramkey& key, paramval val);
};





class RobotArmDescription {
 public:
  OdeHandle *p_ode_handle;

  dReal segment_radius;
  dReal segment_mass;

  Vector3<dReal> v3_position;
  VertexList *p_vertex_list;
};



/**
 * RobotArmComponent
 *
 *
 * creates a robot arm, based on a RobotArmDescription
 *
 *
 * sub components are the motors
 *
 *
 * CCU = Capped Cyliner Universal (joint)
 */
class CCURobotArmComponent : public AbstractCompoundComponent
{
 protected:
  dJointGroupID joint_group_id;

 public:
  CCURobotArmComponent(const RobotArmDescription &r_desc);
  virtual ~CCURobotArmComponent();


  // functions specific for this class
  // (that means interface functions which go beyond the scope of IComponent)

  /* not implemented yet 
  unsigned get_segment_count();
  dBodyID  get_segment      (unsigned index);


  unsigned get_joint_count  ();
  dJointID get_joint_id     (unsigned index);
  */
  /*
  unsigned get_sub_component_count() const;
  IComponent &get_sub_component(unsigned index) const;
  */

  // returns "wires" to the motors
  // the number of exposes wires equals the number of motors
  // that is twice the (number of segments - 1)
  // note that there are two motors between each 2 segments
  // unsigned expose_wires(WireContainer &out_r_wire_container);
  const IComponent* does_contain_geom(const dGeomID geom_id, 
				      bool b_recursive) const;

  void draw() const;
  bool collision_callback(OdeHandle *p_ode_handle, 
			  dGeomID geom_id_0, 
			  dGeomID geom_id_1) const;

  // some function that might come in the future:
  // insert_segment, remove_segment
  // so that the segmnets can be dynamically reconfigured
};




class PlaneComponentDescription {
 public:
  PlaneComponentDescription();

  OdeHandle      *p_ode_handle;
  Vector3<dReal> v3_normal;
  dReal          d;
};

/*
class PlaneComponent : public SimplePhysicalComponent {
 public:
  PlaneComponent(const PlaneComponentDescription &r_desc);
  virtual ~PlaneComponent();
};
*/



class SpiderDescription {
 public:
  SpiderDescription();

  OdeHandle *p_ode_handle;

  Vertex position;
  double sphere_mass;
  double sphere_radius;

  dReal segment_mass;
  dReal segment_radius;
  dReal segment_length;
  dReal segment_count;

  AngleList *p_angle_list;
};


class SpiderComponent : public AbstractCompoundComponent
{
 protected:
  dJointGroupID joint_group_id;

 public:
  SpiderComponent(const SpiderDescription &r_desc);

  bool collision_callback(OdeHandle *p_ode_handle, 
			  dGeomID geom_id_0, 
			  dGeomID geom_id_1) const;

};



}
}


#endif
