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
  class FRIDriver : public RTT::TaskContext
  {
    public:
      /**
       * Constructor of the class.
       * The interface of the component is build.
       * @param name name of the TaskContext
      */
      FRIDriver(const std::string& name);
      virtual ~FRIDriver();

      virtual bool configureHook();
      virtual bool startHook();
      virtual void updateHook();
      virtual void stopHook();
      virtual void cleanupHook();
        
    private:
      friClient client;
      UdpConnection connection;
      ClientApplication app;
      bool prop_simulation;
      const unsigned int numjoints;
      unsigned int port;
      bool success;

      motion_control_msgs::JointPositions m_joint_pos_command;
      motion_control_msgs::JointPositions m_joint_pos_msr;
      motion_control_msgs::JointVelocities m_joint_vel_command;
      motion_control_msgs::JointEfforts m_joint_effort_command;
      RTT::InputPort<motion_control_msgs::JointPositions> port_joint_pos_command;
      RTT::InputPort<motion_control_msgs::JointVelocities> port_joint_vel_command;
      RTT::InputPort<motion_control_msgs::JointEfforts> port_joint_effort_command;
      RTT::OutputPort<motion_control_msgs::JointPositions> port_joint_pos_msr;  
    };
}//namespace

#endif // __FRI_COMPONENT_H__
