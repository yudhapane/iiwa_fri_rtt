// Copyright (C) 2017 Enea Scioni <enea.scioni@kuleuven.be>
// Copyright (C) 2015 Wilm Decre <wilm.decre@mech.kuleuven.be>
// 2017, KU Leuven, Belgium

#ifndef __FRI_COMPONENT_H__
#define __FRI_COMPONENT_H__


// OROCOS-rtt specific
#include <rtt/RTT.hpp>
#include <rtt/os/TimeService.hpp>
#include <rtt/os/Mutex.hpp>
#include <rtt/TaskContext.hpp>
#include <rtt/Port.hpp>
#include <rtt/Component.hpp>
#include <rtt/os/MutexLock.hpp>

// ros_rttcom
#include <sensor_msgs/JointState.h>
#include <motion_control_msgs/JointAccelerations.h>
#include <motion_control_msgs/JointPositions.h>
#include <motion_control_msgs/JointEfforts.h>
#include <motion_control_msgs/JointVelocities.h>

// KUKA FRI SDK
#include "friClient.hpp"
#include <friUdpConnection.h>
#include <friClientApplication.h>

#include <vector>

using namespace KUKA::FRI;

namespace FRI
{
  using namespace RTT;
  using namespace std;
  
  class FRIDriver : public TaskContext
  {
    public:
      FRIDriver(const std::string& name);
      virtual ~FRIDriver();

      virtual bool configureHook();
      virtual bool startHook();
      virtual void updateHook();
      virtual void stopHook();
      virtual void cleanupHook();
        
    private:
      ///FRI-related
      friClient         client;
      UdpConnection     connection;
      ClientApplication app;
      /// Properties
      bool               p_simulation;
      const unsigned int p_numjoints;
      string             p_fri_ip;
      unsigned int       p_fri_port;
      string             p_baseframe;
      string             p_effort_origin;
      
      bool success;

      // Internal, mem alloc
      motion_control_msgs::JointPositions  m_joint_pos_command;
      motion_control_msgs::JointVelocities m_joint_vel_command;
      motion_control_msgs::JointEfforts    m_joint_effort_command;
      motion_control_msgs::JointEfforts    m_t_ext;
      sensor_msgs::JointState              m_joint_states;
      vector<double>                       m_qdes;
      vector<double>                       m_q_actual;
      vector<double>                       m_t_actual;
      vector<double>                       m_qdot_actual;
      
      // Port Interface
      InputPort<string>                               port_ein;
      InputPort<vector<double> >                      port_qdes;
      InputPort<motion_control_msgs::JointPositions>  port_joint_pos_command;
      InputPort<motion_control_msgs::JointVelocities> port_joint_vel_command;
      InputPort<motion_control_msgs::JointEfforts>    port_joint_effort_command;
      //
      OutputPort<string>                              port_eout;
      OutputPort<vector<double> >                     port_q_actual;
      OutputPort<vector<double> >                     port_t_actual;
	    OutputPort<vector<double> >                     port_qdot_actual;
      OutputPort<motion_control_msgs::JointPositions> port_joint_pos_msr;
      OutputPort<motion_control_msgs::JointEfforts>   port_joint_ext_jnt;
      OutputPort<sensor_msgs::JointState>             port_joint_state;
    };
}//namespace

#endif // __FRI_COMPONENT_H__
