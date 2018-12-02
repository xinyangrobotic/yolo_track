//
// Created by zxy on 18-11-28.
//

#ifndef UAVTRACK_DETECT_H
#define UAVTRACK_DETECT_H

#include "mytrack/common_include.h"
#include <opencv2/dnn.hpp>
#include "mytrack/AllTarget.h"
#include <mutex>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;
namespace mytrack {
    class Detect {
    public:
        Detect( VideoCapture& cap, const cv::dnn::Net& net);

        void Run();

        void SetGetObject(bool flag);

        bool NeedProcess();

        bool isGetROI();

        vector<Target::Ptr> ALLObject_;

        void GetTheObjects();

        void StopYoloThread();

        void StartYoloThread();

    private:
        Mat frame_;
        Mat blob_;
        float confThreshold_; // Confidence threshold
        float nmsThreshold_;  // Non-maximum suppression threshold
        int inpWidth_;  // Width of network's input image
        int inpHeight_; // Height of network's input image
        cv::dnn::Net net_;
        AllTarget::Ptr YoloAllTarget_;
        vector<String> getOutputsNames(const cv::dnn::Net& net);
        void postprocess(Mat& frame, const vector<Mat>& outs, AllTarget::Ptr& yoloalltarget);
        VideoCapture cap_;

    protected:

        bool mbGetObject_;
        mutex mMutexGet_;

        bool bNeedProcess_;
        mutex mMutexProcess_;

        bool CheckFinish();
        mutex mMutexFinish_;
        bool mbFinishRequested_;

        bool mbFinished_;
        void SetFinish();

        //清空前一帧得到的目标
        void ClearPreTarget();





//        void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);
    };

}
#endif //UAVTRACK_DETECT_H
