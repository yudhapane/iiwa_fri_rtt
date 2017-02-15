// Copyright (C) 2015 Wilm Decre <wilm.decre@mech.kuleuven.be>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "friComponent.hpp"
#include <rtt/Component.hpp>
#include <rtt/os/MutexLock.hpp>

namespace FRI
{
	using namespace RTT;
	using namespace KDL;
	using namespace std;
	typedef friComponent MyType;

	friComponent::friComponent(const string& name)
	: TaskContext(name,PreOperational), prop_simulation(true),numjoints(7),app(connection,client)
	{
	//Creating TaskContext

	//Adding properties
	//this->addProperty("port",port_prop).doc("FRI Connection Port Number");
	this->addProperty("simulation", prop_simulation);
	//Adding ports
	this->addPort("JointPositionCommand", port_joint_pos_command);
	this->addPort("JointVelocityCommand", port_joint_vel_command);
	this->addPort("JointEffortCommand", port_joint_effort_command);
	this->addPort("JointPositionMeasured", port_joint_pos_msr);

	//Adding Operations

	}

	friComponent::~friComponent()
	{}

	bool friComponent::configureHook()
	{

	m_joint_pos_command.positions.resize(numjoints);
	m_joint_pos_msr.positions.resize(numjoints);
	m_joint_vel_command.velocities.resize(numjoints);
	m_joint_effort_command.efforts.resize(numjoints);

	m_joint_pos_command.positions.assign(numjoints,0);
	m_joint_pos_msr.positions.assign(numjoints,0);
	m_joint_vel_command.velocities.assign(numjoints,0);
	m_joint_effort_command.efforts.assign(numjoints,0);

	if (prop_simulation)
	{
	}
	else
	{
		Logger::In in(this->getName());
		//ClientApplication app(connection, client);
		// hard coded port, to be removed	
		port = 30200;	
		// hard coded hostname, to be removed
		app.connect(port, NULL);
	}
	return true;
	}

	bool friComponent::startHook()
	{
	if (prop_simulation)
	{
		Logger::log() << Logger::Debug << "Entering startHook Simulation" << Logger::endl;
	}
	else
	{
		Logger::log() << Logger::Debug << "Entering startHook" << Logger::endl;
		success = true;	
		Logger::In in(this->getName());
	}
	return true;
	}

	void friComponent::updateHook()
	{
	if (prop_simulation)
	{
		Logger::log() << Logger::Debug << "Entering updateHook Simulation" << Logger::endl;
		// upon new data, get commanded joint position and set measured position to commanded position
		if (port_joint_pos_command.read(m_joint_pos_command) == NewData) {
			for (unsigned int ii=0; ii<numjoints; ii++)
			{
				m_joint_pos_msr.positions[ii] = m_joint_pos_command.positions[ii];
			}
			port_joint_pos_msr.write(m_joint_pos_msr);
		}
	}
	else
	{
		Logger::log() << Logger::Debug << "Entering updateHook" << Logger::endl;
		if (success)
		{
		      Logger::log() << Logger::Debug << "Doing app.step()" << Logger::endl;
		      success = app.step();
		}	
	}
	}

	void friComponent::stopHook()
	{
	}

	void friComponent::cleanupHook()
	{
	}
}//namespace

ORO_CREATE_COMPONENT(FRI::friComponent)
