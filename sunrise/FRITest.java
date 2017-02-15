// FRITest: Sunrise Robot application program
// for testing FRI
// Author: Enea Scioni, <enea.scioni@kuleuven.be>
// 2017, KU Leuven, Belgium

// Basic concurrency
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

// Google protobuf
import com.google.flatbuffers.FlatBufferBuilder;
import com.google.flatbuffers.Table;


import com.kuka.common.ThreadUtil;
import com.kuka.connectivity.fastRobotInterface.FRIConfiguration;
import com.kuka.connectivity.fastRobotInterface.FRIJointOverlay;
import com.kuka.connectivity.fastRobotInterface.FRISession;
import com.kuka.connectivity.fastRobotInterface.FRIChannelInformation.FRISessionState;
//
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.controllerModel.recovery.IRecovery;
import com.kuka.roboticsAPI.controllerModel.sunrise.SunriseSafetyState.EmergencyStop;
import com.kuka.roboticsAPI.deviceModel.JointLimits;
import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;

public class FRITest extends RoboticsAPIApplication
{
  private FRIConfiguration _friConfiguration = null;
  private FRISession       _friSession       = null;
  private FRIJointOverlay  _motionOverlay    = null;
  private FRIMode _FRIModeRunnable = null;
  private Thread _FRIModeThread = null;
  boolean waitingForUserToEndFRIMode = false;
  
  private Controller _lbrController;
  private LBR        _lbr;
  private boolean    _stop;
  
  private int step_counter = 0;
  @Override
  public void initialize()
  {
    getLogger().info("FRI Test init");
    _friConfiguration   = FRIConfiguration.createRemoteConfiguration(_lbr, 
      "192.170.10.100"); //TODO IP Address of remote PC, now hardcoded (KONI)
    _friConfiguration.setSendPeriodMilliSec(4);
    _friSession = new FRISession(_friConfiguration);
    
//     _jointLimits = _lbr.getJointLimits();
    _FRIModeRunnable = new FRIMode(_lbr,_friSession);
    _FRIModeRunnable.setLogger(getLogger());
    _FRIModeThread = new Thread(_FRIModeRunnable);
    _FRIModeThread.start();
    _stop = false;
    _step_counter = 0;
  }
  
  @Override
  public void run()
  {
    getLogger().info("FRI Test running");
    _FRIModeRunnable.setControlMode(_smartServoMotionControlMode); //Check on this one
    _FRIModeRunnable.enable();
    
    while(!_stop && _lbr.getSafetyState().getEmergencyStopInt()==EmergencyStop.INACTIVE) {
      step_counter+=1;
      if( step_counter % 100 == 0) {
        getLogger().info("FRI Quality "+_friSession.getFRIChannelInformation().getQuality().toString());
      }
      //HACK - just to stop it at certain point
      if( step_counter = 1000) _stop = true;
    }
      
  }
  
  /**
  * main.
  *
  * @param args
  *            args
  */
  public static void main(final String[] args)
  {
    final FRITest app = new FRITest();
    app.runApplication();
  }
}