# Point Cloud Capturing / Preprocessing


## Project Structure
The project is build in a modular way allowing you to provide your own implementations of each pipeline stage. An interface is provided for the capturing, sampling and encoding stages.  For capturing we currently use the Intel Realsense SDK 2.0. However, it is completely viable to implement different point cloud capturing solutions as long as you write your own code which converts the captured point cloud into a PCL compatible format for the other stages of the pipeline.

## Building
Currently the project can only be build on Windows by using the supplied [Visual Studio](https://visualstudio.microsoft.com/) solution. When building the project for testing you should make sure that you selected the release candidate, this is due to certain dependencies exhibiting suboptimal behaviour when using the debug build which can hinder the real-tine effeciency of the application. 

## Dependencies
This project requires several dependancies for the capturing and preprocessing of the point clouds. You can either build and install these dependencies yourself (and make sure the Visual Studio project is able to find them) or you can use the install script which uses vcpkg to build all required dependencies. The first parameter of the Powershell script determines where to install vcpkg. If you already have installed vcpkg you should just provide the current install location instead.  Following dependencies are required to build this project: 

<ul>
  <li> 
    <a href="https://www.intelrealsense.com/sdk-2/">Intel Realsense SDK 2.0</a>
    <a href="https://github.com/IntelRealSense/librealsense">(Code)</a>
  </li>
  <li> 
    <a href="https://pointclouds.org/">Point Cloud Library (PCL)</a>
    <a href="https://github.com/PointCloudLibrary/pcl">(Code)</a>
  </li>
  <li> 
    <a href="https://google.github.io/draco/">Draco</a>
    <a href="https://github.com/google/draco">(Code)</a>
  </li>
</ul>

## Usage
Following command line parameters can be used to change the behaviour of the application:

| **Parameter** 	| **Name**          	| **Description**                                                      	| **Example**     	|
|---------------	|-------------------	|----------------------------------------------------------------------	|-----------------	|
| -r            	| Result File       	| Location and file name where metrics will be saved                   	| results_100.csv 	|
| -a            	| Proxy Address     	| The IP address of the proxy where packets are forwarded to           	| 127.0.0.1       	|
| -p            	| Proxy Port        	| The port of the proxy where packets are forwarded to                 	| 8001            	|
| -i            	| Use Camera        	| Use a Realsense camera as input for the pipeline                     	| n.a.            	|
| -d            	| Content Directory 	| When not using a camera .ply files in this location are used instead 	| frames          	|

## Roadmap

Add build support for Linux
