//
// Created by zxy on 18-11-28.
//

#include "mytrack/Detect.h"


namespace mytrack{

    Detect::Detect(VideoCapture& cap, const cv::dnn::Net& net): net_(net), confThreshold_(0.5),
    nmsThreshold_(0.4), inpWidth_(416), inpHeight_(416), YoloAllTarget_(new AllTarget()), bNeedProcess_(true),cap_(cap),
    mbFinishRequested_(false) {
        cap_>>frame_;
    }

    void Detect::Run() {
        mbFinished_ = false;
        while(cap_.read(frame_)){

            SetGetObject(false);
            //如果需要处理目标检测程序
            if(NeedProcess()) {
                ClearPreTarget();
//                YoloAllTarget_->clearTarget();
                // Create a 4D blob from a frame.
                cv::dnn::blobFromImage(frame_, blob_, 1 / 255.0, cvSize(inpWidth_, inpHeight_), Scalar(0, 0, 0), true,
                                       false);

                //Sets the input to the network
                net_.setInput(blob_);

                // Runs the forward pass to get output of the output layers
                vector<Mat> outs;
                net_.forward(outs, getOutputsNames(net_));

                postprocess(frame_, outs, YoloAllTarget_);

                SetGetObject(true);
            }

            if(CheckFinish())
                break;

            usleep(300);//3毫秒
        }
        SetFinish();
    }

    void Detect::StopYoloThread() {
        unique_lock<mutex> lock(mMutexProcess_);
        bNeedProcess_ = false;
    }

    void Detect::StartYoloThread() {
        unique_lock<mutex> lock(mMutexProcess_);
        bNeedProcess_ = true;
    }

    void Detect::ClearPreTarget() {
        YoloAllTarget_->clearTarget();
    }

    void Detect::SetFinish() {
        unique_lock<mutex> lock(mMutexFinish_);
        mbFinished_ = true;
    }

    bool Detect::CheckFinish() {
        unique_lock<mutex> lock(mMutexFinish_);
        return mbFinishRequested_;
    }

    bool Detect::NeedProcess() {
        unique_lock<mutex> lock(mMutexProcess_);
        return bNeedProcess_;
    }


    void Detect::SetGetObject(bool flag) {
        unique_lock<mutex> lock(mMutexGet_);
        mbGetObject_ = flag;
    }

    vector<String> Detect::getOutputsNames(const cv::dnn::Net& net)
    {
        static vector<String> names;
        if (names.empty())
        {
            //Get the indices of the output layers, i.e. the layers with unconnected outputs
            vector<int> outLayers = net.getUnconnectedOutLayers();

            //get the names of all the layers in the network
            vector<String> layersNames = net.getLayerNames();

            // Get the names of the output layers in names
            names.resize(outLayers.size());
            for (size_t i = 0; i < outLayers.size(); ++i)
                names[i] = layersNames[outLayers[i] - 1];
        }
        return names;
    }

    void Detect::postprocess(Mat& frame, const vector<Mat>& outs, AllTarget::Ptr& yoloalltarget)
    {
        vector<int> classIds;
        vector<float> confidences;
        vector<Rect> boxes;

        for (size_t i = 0; i < outs.size(); ++i)//out 代表输出的类别, cols = 85, 0~3分别代表centry.x,centry.y,width,heigh,
            //4~84代表了每一种类别的得分
        {
            // Scan through all the bounding boxes output from the network and keep only the
            // ones with high confidence scores. Assign the box's class label as the class
            // with the highest score for the box.
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
            {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols); //1 * 80;
                Point classIdPoint;
                double confidence;
                // Get the value and location of the maximum score
                cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold_)
                {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(Rect(left, top, width, height));

                }
            }
        }

        // Perform non maximum suppression to eliminate redundant overlapping boxes with
        // lower confidences
        vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, confThreshold_, nmsThreshold_, indices);
        for (size_t i = 0; i < indices.size(); ++i)
        {
            int idx = indices[i];
            Target::Ptr  target = Target::Ptr( new Target());
            target->classId_ = classIds[idx];
            target->rect_ = boxes[idx];
            target->score_ = confidences[idx];
            YoloAllTarget_->insertTarget(target);
//            drawPred(classIds[idx], confidences[idx], box.x, box.y,
//                     box.x + box.width, box.y + box.height, frame);
        }
    }

    bool Detect::isGetROI() {
        unique_lock<mutex> lock(mMutexGet_);
        if(mbGetObject_){
            ALLObject_.clear();
            GetTheObjects();
            return true;
        }
        else
            return false;

    }

    void Detect::GetTheObjects() {
        ALLObject_  = YoloAllTarget_->targets_;
    }

//    void Detect::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame)
//    {
//        //Draw a rectangle displaying the bounding box
//        rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);
//
//        //Get the label for the class name and its confidence
//        string label = format("%.2f", conf);
//        if (!classes.empty())
//        {
//            CV_Assert(classId < (int)classes.size());
//            label = classes[classId] + ":" + label;
//        }
//
//        //Display the label at the top of the bounding box
//        int baseLine;
//        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
//        top = max(top, labelSize.height);
//        rectangle(frame, Point(left, top - round(1.5*labelSize.height)), Point(left + round(1.5*labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
//        putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,0),1);
//    }
}