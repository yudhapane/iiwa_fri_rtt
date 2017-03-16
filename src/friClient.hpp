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
   
    //
    void getJointPosition();
    void getJointEffort();
    void getJointExtEffort();
    //
    
    static const int CART_VECTOR_DIM = 6;     //!< number of elements in a Cartesian vector
    double meas_jnt_pos[LBRState::NUMBER_OF_JOINTS];
    double meas_torques[LBRState::NUMBER_OF_JOINTS];
    double meas_ext_torques[LBRState::NUMBER_OF_JOINTS];
    double cmd_jnt_pos[LBRState::NUMBER_OF_JOINTS];
    double cmd_torques[LBRState::NUMBER_OF_JOINTS];
    double cmd_wrench[CART_VECTOR_DIM];
    ESessionState current_state;
    //delete the following, just testing
    double phy;
    double stepphy;
};

#endif // _FRI_CLIENT_H

