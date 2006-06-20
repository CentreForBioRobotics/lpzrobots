/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *    marcel@informatik.uni-leipzig.de                                     *
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
 ***************************************************************************/

#include "component.h"

namespace lpzrobots
{

/*****************************************************************************/
/* Component                                                                 */
/*****************************************************************************/
    
    Component::Component ( const OdeHandle &odeHandle, const OsgHandle &osgHandle , const ComponentConf& conf = getDefaultConf () ) : OdeRobot ( odeHandle, osgHandle , "Component" ) , conf ( conf )
    {
	originComponent = this;
    }

    Component::~Component ()
    {
    }

    bool 	Component::collisionCallback (void *data, dGeomID o1, dGeomID o2)
    {
	return false;
    }

    void 	Component::doInternalStuff (const GlobalData &globalData)
    {

    }

    osg::Vec3 Component::getPositionbetweenComponents ( Component* component )
    {
	osg::Vec3 posi1 = getMainPrimitive ()->getPosition ();
	osg::Vec3 posi2 = component->getMainPrimitive ()->getPosition ();
	osg::Vec3 anchor = osg::Vec3 ( posi1[0] + ( posi2[0] - posi1[0])/2 , posi1[1] + ( posi2[1] - posi1[1])/2 , posi1[2] + ( posi2[2] - posi1[2])/2 );

	return anchor;
    }

    int Component::getNumberSubcomponents ()
    {
	return connection.size ();
    }

    int Component::getNumberSubcomponentsAll ()
    {
	int size = 0;

	size += getNumberSubcomponents ();

	for ( unsigned int n = 0; n < connection.size (); n++ )
	{
	    if ( connection[n].softlink == false )
		size += connection[n].subcomponent->getNumberSubcomponentsAll ();
	}
	return size;
    }

    void Component::addSubcomponent ( Component* newsubcomponent , Joint* newconnectingjoint )
    {
	componentConnection newconnection;
	newconnection.subcomponent = newsubcomponent;
	newconnection.joint = newconnectingjoint;

	newconnection.softlink = false;
	newconnection.data = NULL;

	//sets the origin; it is always the origin of the adding component, only the true origin has the originComponent pointer of itself
//	if ( originComponent == this )
	    newconnection.subcomponent->originComponent = originComponent;
//	else
//	    newconnection.subcomponent->originComponent = originComponent;

	connection.push_back ( newconnection );

    }

    Component* Component::removeSubcomponent ( int n )
    {
	if ( getNumberSubcomponents ()  > 0 && getNumberSubcomponents()  > n )
	{
	    Component* tmpcomponent;

	    vector <componentConnection>::iterator eraseiterator;
	    eraseiterator = connection.begin () + n;
	    tmpcomponent = connection[n].subcomponent;

	    if (connection [n].data != NULL )
	        delete ( connection[n].data );
	    else
		cout<<"tries to delete void* data, but no pointer is set\n";

	    if ( connection[n].joint != NULL )
		delete ( connection[n].joint );
	    else
		cout<<"tries to delete Joint* joint, but no joint - pointer is set\n";

	    connection[n].subcomponent->originComponent = connection[n].subcomponent;

	    connection.erase ( eraseiterator );

	    return tmpcomponent;
	}
	return NULL;
    }


    Component* Component::removeSubcomponent ( Component* removedsubcomponent )
    {
	Component* tmpcomponent;

	vector<componentConnection>::iterator it = connection.begin ();
	for ( unsigned int n = 0; n < connection.size (); n++ )
	{
	    it++;
	    if ( connection[n].subcomponent == removedsubcomponent )
	    {
		//move origins
		tmpcomponent = connection[n].subcomponent;

		//deleting the joint
		if ( connection[n].joint != NULL )
		    delete ( connection[n].joint );
		else
		    cout<<"tries to delete Joint* joint, but no joint - pointer is set\n";

		//deleting the extra data pointer
		if (connection [n].data != NULL )
		    delete ( connection[n].data );
		else
		    cout<<"tries to delete void* data, but no pointer is set\n";

		connection.erase ( it );

		break;
	    }
	}

	return tmpcomponent;

    }

    bool Component::hasSubcomponent ( Component* subcomp )
    {
	for ( int n = 0; n < getNumberSubcomponents (); n++ )
	{
	    if ( subcomp == connection[n].subcomponent )
		if ( connection[n].softlink == false )
		    return true;
	}
	return false;
    }

    bool Component::hasSubcomponentAll ( Component* subcomp )
    {
	for ( int n = 0; n < getNumberSubcomponents (); n++ )
	{
	    if ( connection[n].softlink == false )
	    {
		if ( subcomp == connection[n].subcomponent )
		    return true;
		if ( connection[n].subcomponent->hasSubcomponentAll ( subcomp ) == true )
		    return true;
	    }
	}
	return false;
    }

    bool Component::isComponentConnected ( Component* connectedComp )
    {
//	if ( originComponent != NULL )
	    return originComponent->hasSubcomponentAll ( connectedComp );
//	else
//	    return false;
    }

    Component::componentConnection  Component::getConnection ( int connectionnumber )
    {
	return connection[connectionnumber];
    }

    bool Component::setSoftlink ( unsigned int position , bool state )
    {
	if ( connection.size () > position )
	{
	    connection[position].softlink = state;
	    return true;
	}
	else
	    return false;
    }

}

