// Copyright (C) 2017 Enea Scioni <enea,scioni@kuleuven.be>
// Copyright (C) 2015 Wilm Decre  <wilm.decre@mech.kuleuven.be>
// 2017, KU Leuven, Belgium
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

#include "FRIDriver.hpp"

namespace FRI
{
  using namespace RTT;
  using namespace KDL;
  using namespace std;
  typedef FRIDriver MyType;

  FRIDriver::FRIDriver(const string& name) : 
    TaskContext(name,PreOperational)
    , prop_simulation(true)
    , numjoints(7)
    , port(30200)
    , app(connection,client)
  {
    //Adding properties
    this->addProperty("port",port).doc("FRI Connection Port Number");
    this->addProperty("simulation", prop_simulation);
    //Adding ports
    this->addPort("JointPositionCommand", port_joint_pos_command);
    this->addPort("JointVelocityCommand", port_joint_vel_command);
    this->addPort("JointEffortCommand", port_joint_effort_command);
    this->addPort("JointPositionMeasured", port_joint_pos_msr);
  }

  FRIDriver::~FRIDriver()
  {}

  bool FRIDriver::configureHook()
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
      // hard coded hostname, to be removed
      app.connect(port, NULL);
    }
    return true;
  }

  bool FRIDriver::startHook()
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

  void FRIDriver::updateHook()
  {
    if (prop_simulation)
    {
      Logger::log() << Logger::Debug << "Entering updateHook Simulation" << Logger::endl;
      // upon new data, get commanded joint position and set measured position to commanded position
      if (port_joint_pos_command.read(m_joint_pos_command) == NewData) {
        for (unsigned int ii=0; ii<numjoints; ii++)
          m_joint_pos_msr.positions[ii] = m_joint_pos_command.positions[ii];
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

  void FRIDriver::stopHook() {}
  void FRIDriver::cleanupHook() {}
}//namespace

ORO_CREATE_COMPONENT(FRI::FRIDriver)
