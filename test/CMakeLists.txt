add_executable(object_detection object_detection_yolo.cpp)
target_link_libraries(object_detection
        mytrack
        )

add_executable(yolotrack YoloTrack.cpp)
target_link_libraries(yolotrack
        mytrack
        )