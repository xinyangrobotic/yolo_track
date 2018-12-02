//
// Created by zxy on 18-11-27.
//

#ifndef UAVTRACK_SYSTEM_H
#define UAVTRACK_SYSTEM_H

#include "mytrack/common_include.h"
#include "mytrack/AllTarget.h"
#include "mytrack/Detect.h"
#include <opencv2/dnn.hpp>
#include <thread>
#include <boost/timer.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/videoio.hpp>

namespace mytrack {
    class System {

    public:
        enum function{
            MANU = 0,
            AUTO = 1
        };
    public:
        System(const string &strNameFile, const string &strCfgFile, const string &strWeightFile,VideoCapture& cap);

        cv:: Mat MultiTracker(const Mat& im, const function way);

        vector<Rect> rois_;

        void SetROI(const Mat& input);

        bool GetROI();

        void StopYolo();

        void StartYolo();



    private:
        vector<string> classes_;

        AllTarget::Ptr TrackALLTarget_;
//        AllTarget::Ptr YoloAllTarget_;

        Target::Ptr target_;
        AllTarget::Ptr alltarget_;

        std::thread* mptDetect;

        Detect* mpDetect;

        cv::MultiTracker trackers_; //多目标追踪算法对象
        vector<Rect2d> obj_; //追踪目标的框坐标
        vector<Ptr<Tracker>> algorithms_;//追踪算法



    };
}

#endif //UAVTRACK_SYSTEM_H
