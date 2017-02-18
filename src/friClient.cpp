// Copyright (C) 2017 Enea Scioni <enea.scioni@kuleuven.be>
// Copyright (C) 2015 Wilm Decre <wilm.decre@mech.kuleuven.be>
// structure based on KUKA template

#include <stdio.h>
#include "friClient.hpp"

friClient::friClient()
{
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
         break;
      }       
      case MONITORING_READY:
      {
         break;
      }
      case COMMANDING_WAIT:
      {
         break;
      }   
      case COMMANDING_ACTIVE:
      {
         break;
      }   
      default:
      {
         break;
      }
   }
}

void friClient::monitor()
{
   LBRClient::monitor();   
   // place client code here
   
}

//******************************************************************************
void friClient::waitForCommand()
{
   // mirror joint values
   LBRClient::waitForCommand();
   // place client code here
   
   
}

//******************************************************************************
void friClient::command()
{
   // place client code here
   // e.g.
   //robotCommand().setJointPosition( newJointPositions );
}
