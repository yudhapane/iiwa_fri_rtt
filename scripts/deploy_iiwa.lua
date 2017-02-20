--[[
  Deploy rttlua script for testing iiwa-fri component
  Authors:
    * Enea Scioni, <enea.scioni@kuleuven.be>
   2017, KU Leuven, Belgium
--]]

require("rttlib")
require("rttros")
rtt.setLogLevel("Warning")
rttlib.color = true

tc=rtt.getTC()
depl=tc:getPeer("Deployer")


depl:import("rtt_rospack")
depl:import("rtt_ros")
depl:import("rtt_sensor_msgs")
depl:import("iiwa_fri_rtt")
--- Loading ROS-based Libraries
gs = rtt.provides()
ros = gs:provides("ros")
depl:import('rtt_rosnode')
--
depl:loadComponent("iiwa", "FRI::FRIDriver")
iiwa=depl:getPeer("iiwa")


--Configuration
iiwa:getProperty('simulation'):set(false)
iiwa:getProperty('fri_ip'):set("192.170.10.2")
iiwa:getProperty('fri_port'):set(30200)
depl:setActivity("iiwa", 0, 99, rtt.globals.ORO_SCHED_RT)
iiwa:setPeriod(0.005)

iiwa:configure()
-- iiwa:start()


-------------------------------------------
---- ROS Streams
-- depl:stream("iiwa.joint_states", ros:topic("/joint_states"))