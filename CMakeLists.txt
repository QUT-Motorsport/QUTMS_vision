cmake_minimum_required(VERSION 2.8)
project( OpenCVfile )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( OpenCVfile vision_system_v1.cpp )
target_link_libraries( OpenCVfile ${OpenCV_LIBS} )

# vision_system_v1
