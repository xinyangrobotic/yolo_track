//
// Created by zxy on 18-11-27.
//

#include "mytrack/System.h"
using namespace cv;
using namespace dnn;
using namespace std;
namespace mytrack {
    System::System(const string &strNameFile, const string &strCfgFile, const string &strWeightFile, VideoCapture& cap) {

        cout << "Begin the Detect and Track program" << endl;


        ifstream ifs(strNameFile.c_str());
        string line;
        while (getline(ifs, line)) classes_.push_back(line);

        cv::dnn::Net net = readNetFromDarknet(strCfgFile, strWeightFile);
        net.setPreferableBackend(DNN_BACKEND_OPENCV);
        net.setPreferableTarget(DNN_TARGET_CPU);

        mpDetect = new Detect(cap, net);
        mptDetect = new thread(&mytrack::Detect::Run, mpDetect);
    }

    cv:: Mat System::MultiTracker(const Mat& im, const function way) {
            if (way == MANU){
                    boost::timer timer;
                    trackers_.update(im);
                    Mat im_clone = im.clone();
                    for (auto j = 0; j < trackers_.getObjects().size(); j++) {
                            rectangle(im_clone, trackers_.getObjects()[j], Scalar(255, 0, 0), 2, 1);
                    }
//                    cout<<"cost times : "<<1 / timer.elapsed()<<" FPS"<<endl;

                    return im_clone;
            } else{
                    return Mat();
            }

    }

    void System::SetROI(const Mat& input) {
            obj_.clear();
            algorithms_.clear();

//            trackers_ = cv::MultiTracker::create();

//                trackers_.getObjects();
            for (auto i = 0; i < rois_.size(); i++) {
                    obj_.push_back(rois_[i]);
                    algorithms_.push_back(TrackerKCF::create());
            }
            trackers_.add(algorithms_, input, obj_);
            StopYolo();

    }

    void System::StopYolo() {
            mpDetect->StopYoloThread();
    }

    void System::StartYolo() {
            mpDetect->StartYoloThread();
    }

    bool System::GetROI() {
        if(mpDetect->isGetROI()){
                rois_.clear();
                for(auto &pertarget:mpDetect->ALLObject_){
                        rois_.push_back(pertarget->rect_);
                }
                return true;
        } else
                return false;
    }
}
