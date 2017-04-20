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
depl:import("rtt_motion_control_msgs")
--- Loading ROS-based Libraries
gs = rtt.provides()
ros = gs:provides("ros")
depl:import('rtt_rosnode')
--
depl:loadComponent("iiwa", "FRI::FRIDriver")
iiwa=depl:getPeer("iiwa")


--Configuration
iiwa:getProperty('simulation'):set(false)
iiwa:getProperty('fri_ip'):set("192.168.1.16")
iiwa:getProperty('fri_port'):set(30200)
depl:setActivity("iiwa", 0, 99, rtt.globals.ORO_SCHED_RT)
iiwa:setPeriod(0.005)

iiwa:configure()
-- iiwa:start()

----- THIS IS ONLY FOR TESTING PURPOSES
cmd_qdes  = rttlib.port_clone_conn(iiwa:getPort("q_desired"))
cmd_qdot   = rttlib.port_clone_conn(iiwa:getPort("JointVelocityCommand"))
p_pos     = rttlib.port_clone_conn(iiwa:getPort("q_actual"))

function setpos(tab)
  qdes = rtt.Variable("array")
  qdes:fromtab(tab)
  cmd_qdes:write(qdes)
end

function setvel(tab)
  qdot = rtt.Variable("motion_control_msgs/JointVelocities")
  qdot:fromtab({names={}, velocities=tab})
  cmd_qdot:write(qdot)
end

-- Print position
function print_pos(tab)
  print(p_pos:read())
end

-- Send a position
function sendpos()
  isdata, current_pos = p_pos:read()
  if (isdata == 'NoData') then print('no data incoming'); return end
  newpos = {}
  newpos[1] = current_pos[0]
  newpos[2] = current_pos[1]
  newpos[3] = current_pos[2]
  newpos[4] = current_pos[3]
  newpos[5] = current_pos[4]
  newpos[6] = current_pos[5]
  newpos[7] = current_pos[6]
  newpos[7] = newpos[7] + 0.001
  setpos(newpos)
end

-- send velocities
function sendvel()
  setvel({0.0,0.0,0.0,0.0,0.0,0.0,0.05})
end

function stop()
  setvel({0.0,0.0,0.0,0.0,0.0,0.0,0.0})
end
-------------------------------------------
---- ROS Streams
-- depl:stream("iiwa.joint_states", ros:topic("/joint_states"))
