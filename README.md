# IIWA FRI RTT

OROCOS-rtt FRI for KUKA IIWA (Sunrise.OS)

## Authors

  * Enea Scioni, <enea.scioni@kuleuven.be>
  * Wilm Decre,  <wilm.decre@kuleuven.be>
  * Keivan Zavari <keivan.zavari@frsrobotics.be>

## Installation

### Requirements

  1. Proper OROCOS installation
  2. motion control 
```
git clone -b CATKIN git@gitlab.mech.kuleuven.be:rob-hardware/motion_control.git
```
  3. This repository
```
```
  4. configure submodules (adds KUKA SDK)
  5. (Alternative to 4), clone the kuka sdk where you want, and
  6. 
  ```
  export KUKA_FRI_SDK=<path/to/sdk>
  ```

