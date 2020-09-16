# IIWA FRI RTT

OROCOS-rtt FRI for KUKA IIWA (Sunrise.OS)

## Authors

  * Enea Scioni, <enea.scioni@kuleuven.be>
  * Wilm Decre,  <wilm.decre@kuleuven.be>
  * Keivan Zavari <keivan.zavari@frsrobotics.be>

## Installation

### Requirements

  1. Install OROCOS installation
  2. [motion control package](https://gitlab.kuleuven.be/rob-hardware/motion_control/-/tree/CATKIN)
```
git clone -b CATKIN git@gitlab.kuleuven.be:rob-hardware/motion_control.git
```
  3. This repository

### Procedure
  1. configure submodules (adds KUKA FRI SDK) by
```
git submodule init
git submodule update
```
  2. (Alternative to 1), clone the KUKA FRI SDK where you want, and
  3. compile the KUKA FRI SDK by
```
cd <path/to/sdk>/build/GNUMake
make all
```
   if compilation fails, do

``` 
 make clean
 ```
   and re-compile
  
  4. Indicate where the kuka fri sdk has been installed
  ```
  export KUKA_FRI_SDK=<path/to/sdk>
  ```
  
  5. Compile the package e.g. with catkin
  ```
  cd ~/catkin_ws/src
  catkin_make
  ```
