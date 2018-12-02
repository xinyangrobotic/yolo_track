//
// Created by zxy on 18-11-27.
//
#include "mytrack/System.h"
#include <sstream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

int main(int argc, char **argv){

    const char* keys =
            "{help h usage ? | | Usage examples: \n\t\t./object_detection_yolo.out --image=dog.jpg \n\t\t./object_detection_yolo.out --video=run_sm.mp4}"
            "{image i        |<none>| input image   }"
            "{video v       |<none>| input video   }"
    ;
    CommandLineParser parser(argc, argv, keys);
    parser.about("Use this script to run object detection using YOLO3 in OpenCV.");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    string classesFile = "./model/coco.names";
    String modelConfiguration = "./model/yolov3.cfg";
    String modelWeights = "./model/yolov3.weights";

    string str, outputFile;
    VideoCapture cap;
    VideoCapture cap1("/home/zxy/project/yolo3/video/road.flv");
    VideoWriter video;
    Mat frame, blob;

    try {

        outputFile = "yolo_out_cpp.avi";
        if (parser.has("image"))
        {
            // Open the image file
            str = parser.get<String>("image");
            ifstream ifile(str);
            if (!ifile) throw("error");
            cap.open(str);
            str.replace(str.end()-4, str.end(), "_yolo_out_cpp.jpg");
            outputFile = str;
        }
        else if (parser.has("video"))
        {
            // Open the video file
            str = parser.get<String>("video");
            ifstream ifile(str);
            if (!ifile) throw("error");
            cap.open(str);
            str.replace(str.end()-4, str.end(), "_yolo_out_cpp.avi");
            outputFile = "/home/zxy/project/yolo3/video/result.avi";
        }
            // Open the webcaom
        else cap.open(parser.get<int>("device"));

    }
    catch(...) {
        cout << "Could not open the input image/video stream" << endl;
        return 0;
    }

    // Get the video writer initialized to save the output video
    if (!parser.has("image")) {
        video.open(outputFile, VideoWriter::fourcc('M','J','P','G'), 28, Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));
    }
    // Create a window
    static const string kWinName = "Deep learning object detection in OpenCV";
    namedWindow(kWinName, WINDOW_NORMAL);

    cap >> frame;

    mytrack::System Detect(classesFile, modelConfiguration, modelWeights, cap1);
    while (Detect.GetROI() == false){
        usleep(500);
//        cout<<"here"<<endl;
    }
//    selectROIs("rois", frame, Detect.rois_, false);
    boost::timer timer0;
//    cout<<"setROI time cost: "<<timer0.elapsed()<<endl;
    Detect.SetROI(frame);
    int i = 0;

    while (cap.read(frame)) {
        // get frame from the video
        // Stop the program if reached end of video
        if (frame.empty()) {
            cout << "Done processing !!!" << endl;
            cout << "Output file is stored as " << outputFile << endl;
            waitKey(3000);
            break;
        }
        float time = timer0.elapsed();
//        cout<<"time: = "<<time<<endl;
        if( i % 60 == 0){
            Detect.StartYolo();
            while(Detect.GetROI() != true){
                usleep(50);
            }
                cout<<"here"<<endl;
                Detect.SetROI(frame);
        }
        Mat result;
        result = Detect.MultiTracker(frame, mytrack::System::function::MANU);
        imshow(kWinName, result);
        waitKey(1);
        i++;
    }
    cap.release();
    if (!parser.has("image")) video.release();

    return 0;
}

