// Copyright (C) 2017 Enea Scioni <enea.scioni@kuleuven.be>
// Copyright (C) 2015 Wilm Decre <wilm.decre@mech.kuleuven.be>
// structure based on KUKA template

#include <iostream>
#include "friClient.hpp"
#include <cstring>

// deleteme, just testing
#include <math.h> //sin

using namespace std;

friClient::friClient()
{
  for(unsigned i=0; i<LBRState::NUMBER_OF_JOINTS; i++)
    m_cmd_jnt_pos[i] = 0.0;
  
  for(unsigned i=0; i<LBRState::NUMBER_OF_JOINTS; i++)
    m_cmd_torques[i] = 0.0;
  
  for(unsigned i=0; i<CART_VECTOR_DIM; i++)
    m_cmd_wrench[i] = 0.0;
  
  stepphy = 0.01;
  phy = 0.0;
}

friClient::~friClient()
{
}
      
void friClient::onStateChange(ESessionState oldState, ESessionState newState)
{
   LBRClient::onStateChange(oldState, newState);
   switch (newState)
   {
      case MONITORING_WAIT:
      {
        std::cout << "MONITORING WAIT ("<< newState << ")" << std::endl;
         break;
      }       
      case MONITORING_READY:
      {
        std::cout << "MONITORING READY ("<< newState << ")" << std::endl;
         break;
      }
      case COMMANDING_WAIT:
      {
        std::cout << "COMANDING WAIT ("<< newState << ")" << std::endl;
         break;
      }   
      case COMMANDING_ACTIVE:
      {
        std::cout << "COMMAND ACTIVE (" << newState << ")" << std::endl;
         memcpy(m_meas_jnt_pos,this->robotState().getMeasuredJointPosition(),LBRState::NUMBER_OF_JOINTS * sizeof(double));
         for(unsigned int i=0;i<LBRState::NUMBER_OF_JOINTS;i++) 
           std::cout << m_meas_jnt_pos[i] << " ";
         std::cout << std::endl;
         std::cout << "---------------" << std::endl;
         break;
      }   
      default:
      {
        std::cout << "UNKNOW ("<< newState << ")" << std::endl;
         break;
      }
   }
}

void friClient::monitor()
{
   LBRClient::monitor();   
   // place client code here
   memcpy(m_meas_jnt_pos, this->robotState().getMeasuredJointPosition(),LBRState::NUMBER_OF_JOINTS * sizeof(double));
   for(unsigned int i=0;i<LBRState::NUMBER_OF_JOINTS;i++) 
   std::cout << m_meas_jnt_pos[i] << " ";
   std::cout << std::endl;   
}

//******************************************************************************
void friClient::waitForCommand()
{
   // mirror joint values
   LBRClient::waitForCommand();
   // place client code here
   
   if (robotState().getClientCommandMode() == POSITION) {
      robotCommand().setJointPosition(m_cmd_jnt_pos);
   } else if (robotState().getClientCommandMode() == TORQUE) {
      robotCommand().setTorque(m_cmd_torques);
   } else if (robotState().getClientCommandMode() == WRENCH) {
      phy = 0.0;
      robotCommand().setWrench(m_cmd_wrench);
   }
   
}

//******************************************************************************
void friClient::command()
{
//    std::cout << "command mode" << std::endl;
   // place client code here
   // e.g.
   // send nothing
   if (robotState().getClientCommandMode() == POSITION) {
     memcpy(m_cmd_jnt_pos, this->robotState().getMeasuredJointPosition(), LBRState::NUMBER_OF_JOINTS * sizeof(double));
     m_cmd_jnt_pos[6] += 0.001;
     robotCommand().setJointPosition( m_cmd_jnt_pos );
   } else if (robotState().getClientCommandMode() == TORQUE) {
     LBRClient::command();
     robotCommand().setTorque(m_cmd_torques);
   } else if (robotState().getClientCommandMode() == WRENCH) {
     LBRClient::command();
     m_cmd_wrench[2] = 5.0 * sin(phy);
     phy += stepphy;
     robotCommand().setWrench(m_cmd_wrench);
   }
     
}
