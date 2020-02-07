# Read Me - Vision System - Ishan Smith

### Concept
The idea is to use colour thresholding to identify the yellow and blue cones then find the mid-point between the cones. This midpoint can be used as a guide for the car slowly over the first lap as it collects data over LiDAR. After a map is created using LiDAR an optimal path will be calculated to increase time efficiency and the car will drive as fast as possible in the next lap. 

### Software
This uses Opencv version 3.2.0 which comes with ROS.  

You do not need to make a CMake file however if you wish to then see the link below. 
    - To create a Make file and CMake file for C++ use the following link: https://docs.opencv.org/master/db/df5/tutorial_linux_gcc_cmake.html

### To Edit
If you wish to edit the .cpp file then you will need to edit CMakeLists.txt with <yourfilename>.cpp.

Then type `make` in the terminal and may need to change paths in CMakeLists.txt or other files (terminal may prompt you).

### To Run
To run the vision code open terminal in OpenCV folder and type `./OpenCVfile`

Include correct libraries in you cpp.

Check education_files folder for educational material to help with understanding and learning about OpenCV. Note these files are not required for the vision system to run. If you wish to run files in this folder you will need to move them out from education_files folder to OpenCV folder.

### Code

Code has been heavily commented for your understanding 


