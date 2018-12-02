//
// Created by zxy on 18-11-27.
//

#include "mytrack/AllTarget.h"
namespace mytrack {
    void AllTarget::insertTarget(Target::Ptr target) {
//        if (targets_.find(target->id_) == targets_.end()) {
//            targets_.insert(make_pair(target->id_, target));
//        } else {
//            targets_[target->id_] = target;
//        }
        targets_.push_back(target);
    }
    void  AllTarget::clearTarget() {
        targets_.clear();
    }
}
