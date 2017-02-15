#
# Components, types and plugins.
#
# The CMake 'target' names are identical to the first argument of the
# macros below, except for orocos_typegen_headers, where the target is fully
# controlled by generated code of 'typegen'.
#

# Creates a component library libnaxes_motion_control-<target>.so
# and installs in the directory lib/orocos/naxes_motion_control/
#
orocos_component(FRIDriver FRIDriver.hpp FRIDriver.cpp friClient.hpp friClient.cpp) # ...you may add multiple source files
target_link_libraries(FRIDriver ${kdl_parser_LIBRARIES}  ${orocos_kdl_LIBRARIES} FRIClient)

#
# You may add multiple orocos_component statements.

#
# Additional headers:
#
# Installs in the include/orocos/naxes_motion_control/ directory
#
orocos_install_headers(FRIDriver.hpp) # ...you may add multiple header files
#
# You may add multiple orocos_install_headers statements.

#
# Building a Plugin
#
# Creates a plugin library libnaxes_motion_control-plugin-<target>.so
# and installs in the directory lib/orocos/naxes_motion_control/plugins/
#
# Be aware that a plugin may only have the loadRTTPlugin() function once defined in a .cpp file.
# This function is defined by the plugin and service CPP macros.
#
#orocos_plugin(naxes_motion_control-plugin naxes_motion_control-plugin.cpp) # ...only one plugin function per library !
#
# You may add multiple orocos_plugin statements.


#
# Building a Service:
#
# Creates a plugin library libnaxes_motion_control-service-<target>.so
# and installs in the directory lib/orocos/naxes_motion_control/plugins/
#
#orocos_service(naxes_motion_control-service naxes_motion_control-service.cpp) # ...only one service per library !
#
# You may add multiple orocos_service statements.


#
# Building a normal library (optional):
#
# Creates a library libnaxes_motion_control-support-<target>.so and installs it in
# lib/
#
#orocos_library(naxes_motion_control-support support.cpp) # ...you may add multiple source files
#
# You may add multiple orocos_library statements.


