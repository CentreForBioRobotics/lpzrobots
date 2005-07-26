#include <drawstuff/drawstuff.h>
#include <ode/ode.h>

#include "simulation.h"

#include "nimm2.h"


Nimm2::Nimm2(dWorldID w, dSpaceID s, dJointGroupID c, double size/*=1.0*/, double force /*=0.1*/, double speed/*=10*/):
  AbstractRobot::AbstractRobot(w, s, c){ 

  created=false;

  initial_pos.x=0.0;
  initial_pos.y=0.0;
  initial_pos.z=0.0;
  
  //Nimm2 color ;-)
  color.r=4;
  color.g=0.9;
  color.b=0.5;
  

  max_force   = force*size*size;
  this->speed = speed;

  height=size;  

  length=size/3; 
  width=size/2; 
  radius=size/4+size/200;
  wheelthickness=size/20;
  cmass=8*size;  
  wmass=size;  
  sensorno=2; 
  motorno=2;  
  segmentsno=3;
};

/** sets actual motorcommands
    @param motors motors scaled to [-1,1] 
    @param motornumber length of the motor array
*/
void Nimm2::setMotors(const motor* motors, int motornumber){
  //  double tmp;
  int len = (motornumber < motorno)? motornumber : motorno;
  for (int i=0; i<len; i++){ 
    //    tmp=dJointGetHinge2Param(joint[i],dParamVel2);
    //    dJointSetHinge2Param(joint[i],dParamVel2,tmp + 0.5*(motors[i]*speed-tmp) );       
    dJointSetHinge2Param(joint[i],dParamVel2, motors[i]*speed);       
    dJointSetHinge2Param (joint[i],dParamFMax2,max_force);
  }
};

/** returns actual sensorvalues
    @param sensors sensors scaled to [-1,1] (more or less)
    @param sensornumber length of the sensor array
    @return number of actually written sensors
*/
int Nimm2::getSensors(sensor* sensors, int sensornumber){
  int len = (sensornumber < sensorno)? sensornumber : sensorno;
  for (int i=0; i<len; i++){
    sensors[i]=dJointGetHinge2Angle2Rate(joint[i]);
    sensors[i]/=speed;  //scaling
  }
  return len;
};

/** sets the vehicle to position pos, sets color to c, and creates robot if necessary
    @params pos desired position of the robot in struct Position
    @param c desired color for the robot in struct Color
*/
void Nimm2::place(Position pos, Color *c /*= 0*/){
  if (!c==0) {
    color=*c;
  }
  pos.z+=radius; // to put wheels on ground, not in ground
  if (!created){ 
    create(pos);
  }
  else{
    dBodySetPosition (object[1].body,pos.x ,pos.y +width*0.5,pos.z);
    dBodySetPosition (object[2].body,pos.x ,pos.y -width*0.5,pos.z);
    dBodySetPosition (object[0].body,pos.x ,pos.y           ,pos.z);    
  }
};

/** returns position of robot 
    @return position robot position in struct Position  
*/
Position Nimm2::getPosition(){
  Position pos;
  const dReal* act_pos=dBodyGetPosition(object[0].body);
  pos.x=act_pos[0];
  pos.y=act_pos[1];
  pos.z=act_pos[2]-radius; // substract wheel radius, because vehicle stands on the ground
  return pos;
};

/** returns a vector with the positions of all segments of the robot
    @param poslist vector of positions (of all robot segments) 
    @return length of the list
*/
int Nimm2::getSegmentsPosition(vector<Position> &poslist){
  Position pos;
  for (int i=0; i<segmentsno; i++){
    const dReal* act_pos = dBodyGetPosition(object[i].body);
    pos.x=act_pos[0];
    pos.y=act_pos[1];
    pos.z=act_pos[2];
    poslist.push_back(pos);
  }   
  return segmentsno;
};  



/**
 * draws the vehicle
 */
void Nimm2::draw(){
  dsSetColor (color.r,color.g,color.b); // set color for cylinder
  dsSetTexture (DS_WOOD);
  dsDrawCappedCylinder(dBodyGetPosition(object[0].body),dBodyGetRotation(object[0].body),length, width/2 );
  dsSetColor (1,1,1); // set color for wheels
  // draw wheels
  for (int i=1; i<3; i++) { 
    dsDrawCylinder (dBodyGetPosition(object[i].body), dBodyGetRotation(object[i].body),wheelthickness,radius);
  }
};

void Nimm2::mycallback(void *data, dGeomID o1, dGeomID o2){
  Nimm2* me = (Nimm2*)data;  
  if(isGeomInObjectList(me->object, me->segmentsno, o1) && isGeomInObjectList(me->object, me->segmentsno, o2)){
    return;
  }
}

bool Nimm2::collisionCallback(void *data, dGeomID o1, dGeomID o2){
  //checks if one of the collision objects is part of the robot
  if( o1 == (dGeomID)car_space || o2 == (dGeomID)car_space){
    dSpaceCollide(car_space, this, mycallback);
    bool colwithme;  
    bool colwithbody;  
    int i,n;  
    const int N = 10;
    dContact contact[N];
    n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
    for (i=0; i<n; i++){
      colwithbody = false;
      colwithme = false;  
      if( contact[i].geom.g1 == object[0].geom || contact[i].geom.g2 == object[0].geom){
	colwithbody = true;
	colwithme = true;
	// fprintf(stderr,"col with body\n");
      }
      if( contact[i].geom.g1 == object[1].geom || contact[i].geom.g2 == object[1].geom || 
	  contact[i].geom.g1 == object[2].geom || contact[i].geom.g2 == object[2].geom ){
	colwithme = true;
	// fprintf(stderr,"col with wheels\n");
      }
      if( colwithme){
	contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
	  dContactSoftERP | dContactSoftCFM | dContactApprox1;
	contact[i].surface.slip1 = 0.005;
	contact[i].surface.slip2 = 0.005;
	if(colwithbody){
	  contact[i].surface.mu = 0.1; // small friction of smooth body
	  contact[i].surface.soft_erp = 0.5;
	  contact[i].surface.soft_cfm = 0.001;
	}else{
	  contact[i].surface.mu = 1.1; //large friction
	  contact[i].surface.soft_erp = 0.9;
	  contact[i].surface.soft_cfm = 0.001;
	}
	dJointID c = dJointCreateContact( world, contactgroup, &contact[i]);
	dJointAttach ( c , dGeomGetBody(contact[i].geom.g1) , dGeomGetBody(contact[i].geom.g2)) ;	
      }
    }
    return true;
  }
  return false;
}


/** creates vehicle at desired position 
    @param pos struct Position with desired position
*/
void Nimm2::create(Position pos){
  if (created) {
    destroy();
  }
  // create car space and add it to the top level space
  car_space = dSimpleSpaceCreate (space);
  dSpaceSetCleanup (car_space,0);

  dMass m;
  // cylinder
  object[0].body = dBodyCreate (world);
  dBodySetPosition (object[0].body,pos.x,pos.y,pos.z);
  dQuaternion q;
  dQFromAxisAndAngle (q,0,1,0,M_PI*0.5);
  dBodySetQuaternion (object[0].body,q);
    
  dMassSetCappedCylinder(&m,1,1,width/2,length);
  dMassAdjust (&m,cmass);
  dBodySetMass (object[0].body,&m);
  object[0].geom = dCreateCCylinder (car_space, width/2,length);
  dGeomSetBody (object[0].geom, object[0].body);

  // wheel bodies
  for (int i=1; i<3; i++) {
    object[i].body = dBodyCreate (world);
    dQuaternion q;
    dQFromAxisAndAngle (q,1,0,0,M_PI*0.5);
    dBodySetQuaternion (object[i].body,q);
    dMassSetSphere (&m,1,radius);
    dMassAdjust (&m,wmass);
    dBodySetMass (object[i].body,&m);
    object[i].geom = dCreateSphere (car_space, radius);
    dGeomSetBody (object[i].geom,object[i].body);
  }
  dBodySetPosition (object[1].body, pos.x, pos.y+ width*0.5+wheelthickness, pos.z );
  dBodySetPosition (object[2].body, pos.x, pos.y -width*0.5-wheelthickness, pos.z );


  for (int i=0; i<2; i++) {
    joint[i] = dJointCreateHinge2 (world,0);
    dJointAttach (joint[i],object[0].body,object[i+1].body);
    const dReal *a = dBodyGetPosition (object[i+1].body);
    dJointSetHinge2Anchor(joint[i],a[0],a[1],a[2]);
    dJointSetHinge2Axis1 (joint[i],0,0,1);
    dJointSetHinge2Axis2 (joint[i],0,1,0);
  }
  for (int i=0; i<2; i++) {
    // set stops to make sure wheels always stay in alignment
    dJointSetHinge2Param (joint[i],dParamLoStop,0);
    dJointSetHinge2Param (joint[i],dParamHiStop,0);
  }
//   // create car space and add it to the top level space
//   car_space = dSimpleSpaceCreate (space);
//   dSpaceSetCleanup (car_space,0);
//   for (int i=0; i<3; i++){
//     dSpaceAdd (car_space,object[i].geom);
//   }

  created=true;
}; 


/** destroys vehicle and space
 */
void Nimm2::destroy(){
  if (created){
    dSpaceDestroy(car_space);
    for (int i=0; i<segmentsno; i++){
      dBodyDestroy(object[i].body);
      dGeomDestroy(object[i].geom);
    }
  }
  created=false;
}






