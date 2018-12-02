//
// Created by zxy on 18-11-27.
//

#include "mytrack/Target.h"
namespace mytrack {
    Target::Target() : id_(-1), classId_(-1), score_(0) {}

    Target::Target(
            long unsigned int id,
            const int &classId,
            const Eigen::Vector4d &rect,
            const float &score
    ) : classId_(classId), score_(score) {

    }

    Target::Ptr Target::createTarget() {
        return Target::Ptr(
                new Target(factory_id_++, -1, Eigen::Vector4d(), 0)
        );
    }

    Target::Ptr Target::createTarget(
            const int &class_id,
            const Eigen::Vector4d &rect_,
            const float &score_
    ) {
        return Target::Ptr(
                new Target(factory_id_, class_id, rect_, score_)
        );
    }

    unsigned long Target::factory_id_ = 0;


}