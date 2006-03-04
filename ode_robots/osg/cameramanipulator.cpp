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
 ***************************************************************************
 *                                                                         *
 *                                                                         *
 *   $Log$
 *   Revision 1.1.2.8  2006-03-04 15:04:33  robot3
 *   cameramanipulator is now updated with every draw intervall
 *
 *   Revision 1.1.2.7  2006/03/03 12:08:50  robot3
 *   preparations made for new cameramanipulators
 *
 *   Revision 1.1.2.6  2006/02/01 10:24:34  robot3
 *   new camera manipulator added
 *
 *   Revision 1.1.2.5  2006/01/30 13:12:45  martius
 *   bug in setByMatrix
 *
 *   Revision 1.1.2.4  2005/12/29 12:55:59  martius
 *   setHome
 *
 *   Revision 1.1.2.3  2005/12/15 17:03:42  martius
 *   cameramanupulator setPose is working
 *   joints have setter and getter parameters
 *   Primitives are not longer inherited from OSGPrimitive, moreover
 *   they aggregate them
 *
 *   Revision 1.1.2.2  2005/12/11 23:35:08  martius
 *   *** empty log message ***
 *
 *   Revision 1.1.2.1  2005/12/09 16:56:21  martius
 *   camera is working now
 *
 *   Revision 1.1.2.1  2005/12/06 17:38:21  martius
 *   *** empty log message ***
 *
 *                                                                 *
 ***************************************************************************/

#include <osg/Notify>
#include "cameramanipulator.h"
#include "mathutils.h"
#include "pos.h"

#include <vector>
#include <iterator>

namespace lpzrobots {

  using namespace osg;
  using namespace osgGA;

  int isnf=0;

  // globalData braucht er f�r alles
  CameraManipulator::CameraManipulator(osg::Node* node,const GlobalData& global)
    : node(node), eye(0,0,0), view(0,0,0), home_externally_set(false), globalData(global) {
    if (this->node.get()) {    
      const BoundingSphere& boundingSphere=this->node->getBound();
      modelScale = boundingSphere._radius;
    }else 
      modelScale = 0.01f;
  }

  CameraManipulator::~CameraManipulator(){
  }

  void CameraManipulator::setNode(Node* node){
    // we do not support this since we give it manually to the constructor
  }
  const Node* CameraManipulator::getNode() const{
    return node.get();
  }

  Node* CameraManipulator::getNode(){
    return node.get();
  }

  /// set the home position of the camera. (and place it there)
  void CameraManipulator::setHome(const osg::Vec3& _eye, const osg::Vec3& _view){
    this->home_eye = _eye;
    this->home_view = _view;
    this->home_externally_set=true;
    eye  = home_eye;
    view = home_view;
    desiredEye=_eye;
    desiredView=_view;
    computeMatrix();
  }


  void CameraManipulator::home(const GUIEventAdapter& ea,GUIActionAdapter& us){
    if(node.get() && !home_externally_set) {
      const BoundingSphere& boundingSphere=node->getBound();

      home_eye = boundingSphere._center+
	Vec3(-boundingSphere._radius*1.2f,0, boundingSphere._radius*0.2f);

      home_view = Vec3(-90,-10,0);
    }
    //    eye  = home_eye;
    //    view = home_view;
    desiredEye=home_eye;
    desiredView=home_view;
    computeMatrix();
    
    us.requestRedraw();
    
    us.requestWarpPointer((ea.getXmin()+ea.getXmax())/2.0f,(ea.getYmin()+ea.getYmax())/2.0f);
    
    flushMouseEventStack();
    
  }

  void CameraManipulator::init(const GUIEventAdapter& ea,GUIActionAdapter& us){
    flushMouseEventStack();

    us.requestContinuousUpdate(false);

    if (ea.getEventType()!=GUIEventAdapter::RESIZE)
      {
        us.requestWarpPointer((ea.getXmin()+ea.getXmax())/2.0f,(ea.getYmin()+ea.getYmax())/2.0f);
      }
  }
  bool CameraManipulator::handle(const GUIEventAdapter& ea,GUIActionAdapter& us){
    int key=0;
    switch(ea.getEventType())
      {
      case(GUIEventAdapter::PUSH): 
	{
	  flushMouseEventStack();
	  return true;
	}
         
      case(GUIEventAdapter::RELEASE):
	{
	  flushMouseEventStack();
	  return true;
	}
         
      case(GUIEventAdapter::DRAG):
        {
	  addMouseEvent(ea);
	  us.requestContinuousUpdate(true);
	  if (calcMovement()) us.requestRedraw();
	  return true;
        }

      case(GUIEventAdapter::KEYDOWN):
	key=ea.getKey();
	// F-keys (F1 to F12)
	if ((65470<=key)&&(key<=65481)) {
	  manageAgents(key-65469);
	  return true; // was handled
	}
	switch(key) {
	case ' ':	
	  {
	    flushMouseEventStack();
	    home(ea,us);
	    us.requestRedraw();
	    us.requestContinuousUpdate(false);
	    break;
	  }
	case 'p':
	  {
	    printf("Camera Position/View: (Pos(%g, %g, %g), ", eye.x(), eye.y(), eye.z());
	    printf(" Pos(%g, %g, %g));\n", view.x(), view.y(), view.z());
	    break;
	  }	  
     	default:
	  return false;
	}
      case(GUIEventAdapter::RESIZE):
	init(ea,us);
	us.requestRedraw();
	return true;
	
      default:
	return false;
      }
    return true;
  }
  
  void CameraManipulator::getUsage(ApplicationUsage& usage) const{
    usage.addKeyboardMouseBinding("Camera: Space","Reset the viewing position to home");
    usage.addKeyboardMouseBinding("Camera: p","Print position of the camera");
  }

  void CameraManipulator::flushMouseEventStack(){
    event_old = NULL;
    event = NULL;
  }
  void CameraManipulator::addMouseEvent(const GUIEventAdapter& ea){
    event_old = event;
    event = &ea;
  }

  void CameraManipulator::setByMatrix(const Matrixd& matrix){

    eye = matrix.getTrans();
    Vec3 xaxis(1,0,0);    
    Pos head = Matrix::transform3x3(xaxis, matrix);
    view.x() = RadiansToDegrees(getAngle(xaxis, head)) *       
      sign(head.y()); // this resolves the ambiguity of getAngle

    Pos tilt = Matrix::transform3x3(Vec3(0,1,0), matrix);
    tilt.print();
    view.y() = RadiansToDegrees(getAngle(Vec3(0,0,1), tilt)) * 
      sign(tilt.y()); // this resolves the ambiguity of getAngle    
    desiredEye=eye;
    desiredView=view;
    computeMatrix();    
  }



  // hier reinh�ngen nicht, wird nur beim switch des manipulators aufgerufen
  Matrixd CameraManipulator::getMatrix() const {
    return pose;
  }

  void CameraManipulator::update() {
    // the call from simulation.cpp works, but is made for ALL cameramanipulators!
    //    std::cout << "i am updating the camera!";
  }


  /**
   * is called every time the draw is updated. computes the
   * movement of the camera, which is a difference between
   * the desired pos and view and the actual pos and view.
   */
  /*  void CameraManipulator::computeMovement() {
    desiredEye=eye; // a test
    //    desiredView;
    
    }*/
  
  // hier reinhaengen?? is called every drawstep!!!! really? ;)
  // should we call a CameraManipulator-routine from simulation.cpp?
  Matrixd CameraManipulator::getInverseMatrix() const {
    return Matrixd::inverse(pose);
  }

  void CameraManipulator::computeMatrix(){
    for (int i=0; i<3; i++) {
      while (view[i] > 180) view[i] -= 360;
      while (view[i] < -180) view[i] += 360;
    }
    osg::Matrix rot;
    rot.makeRotate( M_PI/2,                          osg::Vec3(1, 0, 0),
		    osg::DegreesToRadians(view.x()), osg::Vec3(0, 0, 1), // heading
		    osg::DegreesToRadians(view.y()), osg::Vec3(cos(osg::DegreesToRadians(view.x())), 
							       sin(osg::DegreesToRadians(view.x())), 
							       0) // pitch
		    );

    pose = rot * Matrix::translate(eye);
  }

  bool CameraManipulator::calcMovement(){
    // _camera->setFusionDistanceMode(Camera::PROPORTIONAL_TO_SCREEN_DISTANCE);

    // return if less then two events have been added.
    if (event.get()==NULL || event_old.get()==NULL) return false;

    //    double dt = event->time()-event_old->time();
    double dx = 10.0*(event->getXnormalized() - event_old->getXnormalized());
    double dy = 10.0*(event->getYnormalized() - event_old->getYnormalized());
    double s =  sin(osg::DegreesToRadians(view.x()));
    double c =  cos(osg::DegreesToRadians(view.x()));

    unsigned int buttonMask = event_old->getButtonMask();
    if (buttonMask==GUIEventAdapter::LEFT_MOUSE_BUTTON) {
      view.x() += dx*3.0f;
      view.y() -= dy*3.0f;
      // make changes to desired too
      desiredView.x() += dx*3.0f;
      desiredView.y() -= dy*3.0f;
    } else if (buttonMask==GUIEventAdapter::MIDDLE_MOUSE_BUTTON ||
	       buttonMask==(GUIEventAdapter::LEFT_MOUSE_BUTTON | GUIEventAdapter::RIGHT_MOUSE_BUTTON)) { 
      eye.z() += -dy;
      eye.x() += - c*dx;
      eye.y() += - s*dx;
      // make changes to desired too
      desiredEye.z() += -dy;
      desiredEye.x() += - c*dx;
      desiredEye.y() += - s*dx;
    } else if (buttonMask==GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
      eye.x() +=  s*dy - c*dx;
      eye.y() +=  -c*dy - s*dx;      
      // make changes to desired too
      desiredEye.x() +=  s*dy - c*dx;
      desiredEye.y() +=  -c*dy - s*dx;      
    } else return false;
    computeMatrix();
    return true;
  }


  void CameraManipulator::calcMovementByAgent() {
    if (watchingAgent) {
      // then manipulate desired view and desired eye
      // the default camera manipulator does not need to change the eye and view
    }
  }

  void CameraManipulator::manageAgents(const int& fkey) {
    std::cout << "new robot choosed: " << fkey << "\n";
    int i=1;
    // go through the agent list
    for(OdeAgentList::iterator it=globalData.agents.begin(); it != globalData.agents.end(); it++){
      watchingAgent=(*it);
      std::cout << "new robot choosed: " << i++ << "\n";
    }
    std::cout << "number of agents: " << i << "\n";
    if (watchingAgent)
      std::cout << "no agent was choosed!\n";
    else {
      std::cout << "an agent was choosed!\n";
      setHomeViewByAgent();
      setHomeEyeByAgent();
    }
  }

  void CameraManipulator::setHomeViewByAgent() {
    // the default camera manipulator does not need to change the view
    // normally the desired view should be changed
  }

  void CameraManipulator::setHomeEyeByAgent() {
    // the default camera manipulator does not need to change the eye
    // normally the desired eye should be changed
  }

}
