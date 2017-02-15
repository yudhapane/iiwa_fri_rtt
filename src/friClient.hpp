// Copyright (C) 2015 Wilm Decre <wilm.decre@mech.kuleuven.be>
// structure based on KUKA template

#ifndef _FRI_CLIENT_H
#define _FRI_CLIENT_H

#include <friLBRClient.h>

using namespace KUKA::FRI;

class friClient : public LBRClient
{
   
public:
   friClient(); 
   ~friClient();
   
   virtual void onStateChange(ESessionState oldState, ESessionState newState);
   virtual void monitor();
   virtual void waitForCommand();
   virtual void command();
};

#endif // _FRI_CLIENT_H
