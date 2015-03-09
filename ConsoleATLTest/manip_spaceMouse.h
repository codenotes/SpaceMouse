#pragma once
/* -*-c++-*- osgVisual - Copyright (C) 2009-2011 Torben Dannhauer
*
* This library is based on OpenSceneGraph, open source and may be redistributed and/or modified under
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
*
* osgVisual requires for some proprietary modules a license from the correspondig manufacturer.
* You have to aquire licenses for all used proprietary modules.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* OpenSceneGraph Public License for more details.
*/
namespace osgVisual
{
	/**
	* \brief This class interfaces osg to the device driver of 3Dconnexions Space Navigator
	*
	* @author Marius Heise
	* @date  Apr 2009
	*/
	class SpaceMouse
	{
	public:
		/**
		* \brief Constructor
		*
		*/
		SpaceMouse();
		/**
		* \brief Destructor
		*
		* @return
		*/
		virtual ~SpaceMouse();
		/**
		* \brief This function initializes the device driver.
		*
		* @return : 1=sucess, 0=failure.
		*/
		int initialize();
		/**
		* \brief This function shuts the device driver down.
		*
		*/
		void shutdown();
		/**
		* \brief This function writes the detected translation of all three axes to the specified variables.
		*
		* @param dX : delta translation along X axis.
		* @param dY : delta translation along Y axis.
		* @param dZ : delta translation along Y axis.
		* @return : 1=sucess, 0=failure.
		*/
		int getTranslations(double& dX, double& dY, double& dZ);
		/**
		* \brief This function writes the detected rotation of all three axes to the specified variables.
		*
		* @param dX : delta translation along X axis.
		* @param dY : delta translation along Y axis.
		* @param dZ : delta translation along Y axis.
		* @return : 1=sucess, 0=failure.
		*/
		int getRotations(double& dX, double& dY, double& dZ);
		int getKeyBoard();
		HRESULT OnDevKeyDown(long keycode);
	private:
		/**
		* \brief Device driver class
		*
		* @author Unknown/Marius Heise
		* @date  Apr 2009
		*/
		class SpaceMouseImpl;
		/**
		* Pointer to the device driver class.
		*/
		SpaceMouseImpl* _spaceMouseImpl;
		
	};
}       // END NAMESPACE