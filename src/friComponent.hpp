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

// ros_rttcom
#include <sensor_msgs/JointState.h>
#include <motion_control_msgs/JointAccelerations.h>
#include <motion_control_msgs/JointPositions.h>
#include <motion_control_msgs/JointEfforts.h>
#include <motion_control_msgs/JointVelocities.h>

// KUKA FRI SDK
#include <friClient.hpp>
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

      /**
       * Configures the component, make sure the properties are
       * updated using the OCL::DeploymentComponent or the marshalling
       * interface of the component.
       *
       */
      virtual bool configureHook();
      
      /**
       * Starts the component
       */
      virtual bool startHook();
        /**
         */
        virtual void updateHook();

        virtual void stopHook();

        virtual void cleanupHook();

    private:
        
    protected:
        
    private:
	friClient client;
	// create new udp connection
   	UdpConnection connection;
	// pass connection and client to a new FRI client application
   	//ClientApplication app{connection,client};
	//ClientApplication app = ClientApplication(connection,client);
	ClientApplication app;
	//unsigned int port_prop;
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

	/*RTT::OutputPort<KDL::Frame> port_cart_pos_msr;
	RTT::OutputPort<KDL::Jacobian> port_jacobian;
	RTT::OutputPort<KDL::Frame> port_cart_frame_msr;*/

        
    protected:
        
    }; // class
}//namespace
#endif // __FRI_COMPONENT_H__
