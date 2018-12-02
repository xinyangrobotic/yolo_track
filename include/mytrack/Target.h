//
// Created by zxy on 18-11-27.
//

#ifndef UAVTRACK_TARGET_H
#define UAVTRACK_TARGET_H

#include "mytrack/common_include.h"
namespace mytrack {
    class Target {
    public:
        typedef shared_ptr<Target> Ptr;
        unsigned long id_; //ID
        static unsigned long factory_id_;//factory id
        int classId_;                  //当前目标的类型
        cv::Rect rect_;                //当前目标的框,left, int top, int right, int bottom,
        float score_;

        Target();

        Target(
                unsigned long id,
                const int &classId,
                const Eigen::Vector4d &rect,
                const float &score
        );

        static Target::Ptr createTarget();

        static Target::Ptr createTarget(
                const int &class_id,
                const Eigen::Vector4d &rect_,
                const float &score_
        );



    };

}
#endif //UAVTRACK_TARGET_H
