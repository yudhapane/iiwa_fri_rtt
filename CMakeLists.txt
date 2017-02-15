#
# The find_package macro for Orocos-RTT works best with
# cmake >= 2.6.3
#
cmake_minimum_required(VERSION 2.6.3)

#
# This creates a standard cmake project. You may extend this file with
# any cmake macro you see fit.
#
project(iiwa_fri_rtt)

# Use 'source orocos_toolchain/env.sh' if the command below fails:
find_package(OROCOS-RTT QUIET)
if (NOT OROCOS-RTT_FOUND)
  message (FATAL_ERROR "\nCould not find Orocos. Please use the shell command\n 'source orocos_toolchain/env.sh' and then run cmake again.")
endif()

find_package(orocos_kdl)
include_directories(${orocos_kdl_INCLUDE_DIRS})
link_directories(${orocos_kdl_LIBRARY_DIRS})

find_package(kdl_parser)

find_package(catkin REQUIRED
  rtt_motion_control_msgs
  rtt_sensor_msgs
  rtt_trajectory_msgs
)

## KUKA SDK specific
set(KUKA_FRI_SDK $ENV{KUKA_FRI_SDK})
set(FRISDK_INCLUDE "${KUKA_FRI_SDK}/include")
set(FRISDK_LIB "${KUKA_FRI_SDK}/lib")

# Defines the orocos_* cmake macros. See that file for additional
# documentation.
include( ${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake )
include_directories(${FRISDK_INCLUDE})
link_directories(${FRISDK_LIB})

# Rest of the configuration is in src/
add_subdirectory( src )
#
# Building a typekit using typegen (recommended):

#
# Creates a typekit library libnaxes_motion_control-types-<target>.so
# and installs in the directory lib/orocos/@target@/naxes_motion_control/types/
#
# The header will go in include/orocos/naxes_motion_control/types/naxes_motion_control/naxes_motion_control-types.hpp
# So you can #include <naxes_motion_control/naxes_motion_control-types.hpp>
#
#orocos_typegen_headers(include/naxes_motion_control/nAxesGenerator-types.hpp include/naxes_motion_control/nAxesController-types.hpp) # ...you may add multiple header files
#
# You may only have *ONE* orocos_typegen_headers statement in your toplevel CMakeFile.txt !

#
# Generates and installs our package. Must be the last statement such
# that it can pick up all above settings.
#
orocos_generate_package()
