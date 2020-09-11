# IIWA FRI RTT

OROCOS-rtt FRI for KUKA IIWA (Sunrise.OS)

## Authors

  * Enea Scioni, <enea.scioni@kuleuven.be>
  * Wilm Decre,  <wilm.decre@kuleuven.be>
  * Keivan Zavari <keivan.zavari@frsrobotics.be>

## Installation

### Requirements

  1. Proper OROCOS installation
  2. [motion control package](https://gitlab.kuleuven.be/rob-hardware/motion_control/-/tree/CATKIN)
```
git clone -b CATKIN git@gitlab.mech.kuleuven.be:rob-hardware/motion_control.git
```

  3. This repository
```
```
  4. configure submodules (adds KUKA SDK) by
```
git submodule init
git submodule update
```
  5. (Alternative to 4), clone the kuka sdk where you want, and
  6. compile the kuka fri sdk by
```
cd <path/to/sdk>/build/GNUMake
make all
```
     if compilation fails, do
 ``` 
 make clean
 ```
  7. Indicate where the kuka fri sdk has been installed
  ```
  export KUKA_FRI_SDK=<path/to/sdk>
  ```

