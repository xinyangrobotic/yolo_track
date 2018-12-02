//
// Created by zxy on 18-11-27.
//

#ifndef UAVTRACK_ALLTARGET_H
#define UAVTRACK_ALLTARGET_H

#include "mytrack/common_include.h"
#include "mytrack/Target.h"
namespace mytrack {
    class AllTarget {

    public:
        typedef shared_ptr<AllTarget> Ptr;
        vector<Target::Ptr> targets_;

        AllTarget() {}

        void insertTarget(Target::Ptr target);
        void clearTarget();
    };
}

#endif //UAVTRACK_ALLTARGET_H
