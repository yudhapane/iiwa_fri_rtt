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
--- Loading ROS-based Libraries
gs = rtt.provides()
ros = gs:provides("ros")
--
ros:import("iiwa_fri_rtt")

depl:loadComponent("iiwa", "FRI::FRIDriver")
iiwa=depl:getPeer("iiwa")
--Configuration
iiwa:getProperty('fri_ip'):set("192.170.10.3")
iiwa:getProperty('fri_port'):set(30200)
iiwa:setPeriod(0.01)

depl:setActivity("iiwa", 0, 99, rtt.globals.ORO_SCHED_RT)
-- iiwa:configure()
-- iiwa:start()
