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
    , p_simulation(true)
    , p_numjoints(7)
    , p_fri_ip("192.170.10.3")
    , p_fri_port(30200)
    , p_baseframe("base_link")
    , app(connection,client)
    , m_qdes(7)
    , m_q_actual(7)
    , m_t_actual(7)
    , m_qdot_actual(7)
  {
    //Adding properties
    this->addProperty("fri_ip",     p_fri_ip).doc("FRI IP address");
    this->addProperty("fri_port",   p_fri_port).doc("FRI Connection Port Number");
    this->addProperty("simulation", p_simulation);
    this->addProperty("baseframe",  p_baseframe).doc("Frame name of the robot base");
    //Adding ports
    /// Input
    this->addPort("event_in",              port_ein).doc("Events IN - eg supervisor");
    this->addPort("q_desired",             port_qdes).doc("desired joint position [rad]");
    this->addPort("JointPositionCommand",  port_joint_pos_command).doc("desired joint positions [rad]");
    this->addPort("JointVelocityCommand",  port_joint_vel_command).doc("desired joint velocities [rad/s]");
    this->addPort("JointEffortCommand",    port_joint_effort_command).doc("desired torque command");
    /// Output
    this->addPort("event_out",             port_eout).doc("Events OUT - eg faults to supervisor");
    this->addPort("q_actual",              port_q_actual).doc("current joint positions [rad]");
    this->addPort("qdot_actual",           port_qdot_actual).doc("current joint velocities [rad/s]");
    this->addPort("JointPositionMeasured", port_joint_pos_msr).doc("current joint positions [rad]");
    this->addPort("JointExternalEffort",   port_joint_ext_jnt).doc("external torque on joints [Nm]");
    this->addPort("joint_states",          port_joint_state).doc("joint_states (ROS)");
    
    //Fixed size
    m_joint_pos_command.positions.resize(p_numjoints);
    m_joint_vel_command.velocities.resize(p_numjoints);
    m_joint_effort_command.efforts.resize(p_numjoints);

    m_joint_pos_command.positions.assign(p_numjoints,0);
    m_joint_vel_command.velocities.assign(p_numjoints,0);
    m_joint_effort_command.efforts.assign(p_numjoints,0);
    
    m_t_ext.names.resize(p_numjoints);
    m_t_ext.efforts.assign(p_numjoints,0);
    
    m_joint_states.name.resize(p_numjoints);
    m_joint_states.position.resize(p_numjoints);
    m_joint_states.effort.resize(p_numjoints);
    m_joint_states.header.frame_id = p_baseframe;
    
    //For now, names are created by following convention
    //TODO make them properties
    for (unsigned int i=0; i<p_numjoints; i++) {
      std::ostringstream ss;
      ss << "arm_" << i+1 << "_joint";
      m_joint_states.name[i] = ss.str();
      m_t_ext.names[i]        = ss.str();
    }
    port_joint_state.setDataSample(m_joint_states);
    
  }

  FRIDriver::~FRIDriver()
  {}

  bool FRIDriver::configureHook()
  {
    if (p_simulation)
    {
    }
    else
    {
      Logger::In in(this->getName());
      //ClientApplication app(connection, client);
      // hard coded hostname, to be removed
//       app.connect(port, NULL);
    }
    return true;
  }

  bool FRIDriver::startHook()
  {
    if (p_simulation)
    {
      Logger::log() << Logger::Debug << "Entering startHook Simulation" << Logger::endl;
    }
    else
    {
      Logger::log() << Logger::Debug << "Entering startHook" << Logger::endl;
      app.connect(p_fri_port, p_fri_ip.c_str());
      success = true;  
      Logger::In in(this->getName());
    }
    return true;
  }

  void FRIDriver::updateHook()
  {
    if (p_simulation)
    {
      Logger::log() << Logger::Debug << "Entering updateHook Simulation" << Logger::endl;
      // upon new data, get commanded joint position and set measured position to commanded position
//       if (port_joint_pos_command.read(m_joint_pos_command) == NewData) {
//         for (unsigned int ii=0; ii<p_numjoints; ii++)
//           m_joint_pos_msr.positions[ii] = m_joint_pos_command.positions[ii];
//         port_joint_pos_msr.write(m_joint_pos_msr);
//       }
    }
    else
    {
      Logger::log() << Logger::Debug << "Entering updateHook" << Logger::endl;
      if (success)
      {
        Logger::log() << Logger::Debug << "Doing app.step()" << Logger::endl;
        success = app.step();
        
        //TODO:
        // * Implementing lost sampling behaviour
        // * check on message dimension
        if ( client.current_state == COMMANDING_ACTIVE) {
          switch(client.robotState().getClientCommandMode()) {
            case POSITION: { // Position MODE
              if (port_joint_pos_command.read(m_joint_pos_command) != NoData ) {
                for(unsigned int i=0;i<m_joint_pos_command.positions.size();i++)
                  client.cmd_jnt_pos[i] = m_joint_pos_command.positions[i]; //HP: order!
              } else if ( port_qdes.read(m_qdes) != NoData ) {
                  for(unsigned int i=0;i<m_qdes.size();i++)
                    client.cmd_jnt_pos[i] = m_qdes[i]; //HP: order!
              } else if ( port_joint_vel_command.read(m_joint_vel_command) != NoData) {
                for(unsigned int i=0;i<m_joint_vel_command.velocities.size();i++)
                  client.cmd_jnt_pos[i] += m_joint_vel_command.velocities[i]
                    * client.robotState().getSampleTime();
              } else if ( port_qdes.read(m_qdes) != NoData ) {
                for(unsigned int i=0;i<p_numjoints; i++)
                  client.cmd_jnt_pos[i] += m_qdes[i] 
                    * client.robotState().getSampleTime();
              }
              break;
            }
            case TORQUE: {   // Torque MODE
              if ( port_joint_effort_command.read(m_joint_effort_command) != NoData ) {
                for(unsigned int i=0;i<m_joint_effort_command.efforts.size();i++)
                  client.cmd_torques[i] = m_joint_effort_command.efforts[i];  //HP order!!
              }
              break;
            }
            case WRENCH: {   // Wrench MODE
              Logger::log() << Logger::Warning << "Wrench MODE not implemented yet" << Logger::endl;
              break;
            }
            default: {
              Logger::log() << Logger::Warning << "Command Active, but no mode is valid" << Logger::endl;
            }
          }
        } 
      }  
    }
    
    client.getJointPosition();
    client.getJointEffort();
    for(unsigned int i=0;i<p_numjoints;i++) {
      m_joint_states.position[i] = client.meas_jnt_pos[i];
      // Velocities not implemented yet
      m_joint_states.effort[i]    = client.meas_torques[i];
    }
    client.getJointExtEffort();
    
    //Writing on ports
    m_q_actual.assign(client.meas_jnt_pos,client.meas_jnt_pos+p_numjoints);
    m_t_actual.assign(client.meas_torques,client.meas_torques+p_numjoints);
    m_t_ext.efforts.assign(client.meas_ext_torques,client.meas_ext_torques+p_numjoints);
    port_q_actual.write(m_q_actual);
    port_t_actual.write(m_t_actual);
    port_joint_state.write(m_joint_states);
    port_joint_ext_jnt.write(m_t_ext);
  }

  void FRIDriver::stopHook() {
    app.disconnect();
  }
  
  void FRIDriver::cleanupHook() {}
}//namespace

ORO_CREATE_COMPONENT(FRI::FRIDriver)
