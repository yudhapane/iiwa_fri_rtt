package lbrrttdriverapp;

/*
 * Simple LBR-RTT Driver
 * Author: Enea Scioni
 * KU Leuven, 2017
 */
//import static com.kuka.roboticsAPI.motionModel.BasicMotions.ptp;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.positionHold;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.kuka.connectivity.fastRobotInterface.ClientCommandMode;
import com.kuka.connectivity.fastRobotInterface.FRIChannelInformation;
import com.kuka.connectivity.fastRobotInterface.FRIConfiguration;
import com.kuka.connectivity.fastRobotInterface.FRIJointOverlay;
import com.kuka.connectivity.fastRobotInterface.FRISession;
import com.kuka.connectivity.fastRobotInterface.IFRISessionListener;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import com.kuka.roboticsAPI.controllerModel.Controller;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.CartDOF;
import com.kuka.roboticsAPI.geometricModel.Tool;
// Control modes
import com.kuka.roboticsAPI.motionModel.controlModeModel.CartesianImpedanceControlMode;
//import com.kuka.roboticsAPI.motionModel.controlModeModel.PositionControlMode;
import com.kuka.roboticsAPI.motionModel.controlModeModel.JointImpedanceControlMode;

import com.kuka.roboticsAPI.uiModel.ApplicationDialogType;


/**
 * Creates a FRI Session.
 */
public class LBRRTTDriver extends RoboticsAPIApplication
{
    private Controller _lbrController;
    private LBR _lbr;
    private String _clientName;
    private FRIConfiguration _friConfiguration;
    private FRISession       _friSession;
    private FRIJointOverlay _jointOverlay;
    private FRIJointOverlay _torqueOverlay;
    private FRIJointOverlay _wrenchOverlay;
    private Tool            _gripper;
    private boolean         _connok;
    private IFRISessionListener _listener = new IFRISessionListener(){
    	@Override
    	public void onFRIConnectionQualityChanged(
    	FRIChannelInformation friChannelInformation){
    	getLogger().info("QualityChangedEvent - quality:" +
    	friChannelInformation.getQuality());
    	}
    	@Override
    	public void onFRISessionStateChanged(
    	FRIChannelInformation friChannelInformation){
    	getLogger().info("SessionStateChangedEvent - session state:" +
    	friChannelInformation.getFRISessionState());
    	}
    	};
    	

	private final static String informationText=
			"This application is intended for FRI communication!"+ "\n" +
			"\n" +
			"The robot moves to the start position and you have to choose which " +
			"control mode you want to use." ;

    @Override
    public void initialize()
    {
        _lbrController = (Controller) getContext().getControllers().toArray()[0];
        _lbr = (LBR) _lbrController.getDevices().toArray()[0];
        _gripper = getApplicationData().createFromTemplate("sealTwistingTool");
        _gripper.attachTo(_lbr.getFlange());
        // configure and start FRI session
        //_clientName = "192.170.10.3";  // TODO this should not be hardcoded
        _clientName = "192.168.1.50";  //  The Drummie PC settings
        _friConfiguration = FRIConfiguration.createRemoteConfiguration(_lbr, _clientName);
        _friConfiguration.setSendPeriodMilliSec(10);
        _friSession = new FRISession(_friConfiguration);
        _friSession.addFRISessionListener(_listener);
        _connok = true;
    }

    @Override
    public void run()
    {
        // move to start pose
        //_lbr.move(ptp(Math.toRadians(140), .0, .0, Math.toRadians(90), .0, Math.toRadians(-90), .0).setJointVelocityRel(0.25));
        
        
    	int mode = getApplicationUI().displayModalDialog(ApplicationDialogType.QUESTION, informationText, "Position", "Torque", "Wrench");
        getLogger().info("Creating FRI connection to " + _friConfiguration.getHostName());
        getLogger().info("SendPeriod: " + _friConfiguration.getSendPeriodMilliSec() + "ms |"
                + " ReceiveMultiplier: " + _friConfiguration.getReceiveMultiplier());

    	while(true) {
        	switch (mode) {
    		case 0:
    			getLogger().info("Joint position control mode");	
    	    	_jointOverlay  = new FRIJointOverlay(_friSession);
    			break;
    		case 1:
    			getLogger().info("Joint torque control mode");
    			_torqueOverlay = new FRIJointOverlay(_friSession, ClientCommandMode.TORQUE);
    			break;
    		case 2:
    			getLogger().info("Wrench control mode (cartesian)");
    	        _wrenchOverlay = new FRIJointOverlay(_friSession, ClientCommandMode.WRENCH);
    			break;
    		}
        // wait until FRI session is ready to switch to command mode
    		try
    		{
    			_friSession.await(10, TimeUnit.SECONDS);
    		}
    		catch (final TimeoutException e)
    		{
    			getLogger().error(e.getLocalizedMessage());
    			_friSession.close();
    			_connok=false;
    		}
    		
    		if (_connok) {

    			getLogger().info("FRI connection established.");
    			
    			try{
    			  switch (mode) {
    			  	case 0:
    			  	// TODO: In position control mode, the robot doesn't hold the position....
    				//PositionControlMode controlModeJoint = new PositionControlMode();    
    				  JointImpedanceControlMode controlModeJoint = new JointImpedanceControlMode(_lbr.getJointCount()).setDampingForAllJoints(0.7).setStiffnessForAllJoints(600);
    				  _lbr.move(positionHold(controlModeJoint, -1, TimeUnit.SECONDS).addMotionOverlay(_jointOverlay));
    			      break;
    		    	case 1:
    		    		JointImpedanceControlMode controlModeTorque = new JointImpedanceControlMode(_lbr.getJointCount()).setDampingForAllJoints(0.7).setStiffnessForAllJoints(500);
    		    		_lbr.move(positionHold(controlModeTorque, -1, TimeUnit.SECONDS).addMotionOverlay(_torqueOverlay));
    		    		break;
    		    	case 2:

    				CartesianImpedanceControlMode controlModeWrench = new CartesianImpedanceControlMode();
    				controlModeWrench.parametrize(CartDOF.TRANSL).setStiffness(100);
    				_lbr.move(positionHold(controlModeWrench, -1, TimeUnit.SECONDS).addMotionOverlay(_wrenchOverlay));
    				break;
    			  }
    			}
    			catch(Exception e) {
    				FRIChannelInformation channelInfo =
    						_friSession.getFRIChannelInformation();
    						getLogger().error("Timeout occured - quality: " +
    						channelInfo.getQuality() +
    						" - session state: " + channelInfo.getFRISessionState());
    			}
    			finally{
        		  // done
        		  _friSession.close();
    			}
    		}

    		getLogger().info("FRI connection closed - retry");
    		_connok=true;
    		//getLogger().info("is fault here?");
    		_friSession = new FRISession(_friConfiguration);
    		//getLogger().info("or here?");
    		_friSession.addFRISessionListener(_listener);
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
        final LBRRTTDriver app = new LBRRTTDriver();
        app.runApplication();
    }

}
