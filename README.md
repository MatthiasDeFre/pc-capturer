# Point Cloud Capturing / Preprocessing


## Project Structure


## Building
Currently the project can only be build on Windows by using the supplied [Visual Studio](https://visualstudio.microsoft.com/) solution. When building the project for testing you should make sure that you selected the release candidate, this is due to certain dependencies exhibiting suboptimal behaviour when using the debug build which can hinder the real-tine effeciency of the application. 

## Dependencies
This project requires several dependancies for the capturing and preprocessing of the point clouds. You can either build and install these dependencies yourself (and make sure the Visual Studio project is able to find them) or you can use the install script which uses vcpkg to build all required dependencies.  Following dependencies are required to build this project: 

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


## Roadmap

Add build support for Linux
