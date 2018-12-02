add_library(mytrack SHARED
        Target.cpp AllTarget.cpp System.cpp Detect.cpp ../include/mytrack/Detect.h)


target_link_libraries(mytrack
        ${THIRD_PARTY_LIBS}
        )