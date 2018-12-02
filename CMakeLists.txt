cmake_minimum_required(VERSION 2.8)
project(UAVTrack)

set(CMAKE_CXX_STANDARD 11)
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

#Eigen
include_directories("/usr/include/eigen3")

#OpenCV
find_package(OpenCV 3.4.3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set( THIRD_PARTY_LIBS
        ${OpenCV_LIBS}
        )

include_directories(${PROJECT_SOURCE_DIR}/include/)

#target_link_libraries(mytrack
#        ${THIRD_PARTY_LIBS}
#        )

add_subdirectory(src)
add_subdirectory(test)

#add_executable(yolo3
#        test/object_detection_yolo.cpp)
#target_link_libraries(yolo3
#        ${OpenCV_LIBRARIES}
#        )
