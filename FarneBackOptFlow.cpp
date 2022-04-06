
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main () {
    
    VideoCapture cap;
    Mat curFrame, prevFrame;
    Mat optFlow;
    Mat curFrGray, prevFrGray;
    cap.open("/Users/chenxuan/Downloads/OpenCV_Proj/test_video.mov");
    
    if (!cap.isOpened())
        return -1;
    
    cap.read(curFrame);
    cvtColor(curFrame, prevFrGray, COLOR_BGR2GRAY);
    curFrame.copyTo(prevFrame);
    namedWindow("Output", WINDOW_AUTOSIZE);
    
    while (cap.read(curFrame)) {
        
        cvtColor(curFrame, curFrGray, COLOR_BGR2GRAY);
        calcOpticalFlowFarneback(prevFrGray, curFrGray, optFlow, 0.5, 3, 15, 3, 5, 1.5, 0);
        
        for (int i = 0; i < prevFrame.rows; i++) {
            
            for (int j = 0; j < prevFrame.cols; j++) {
                
                Point2f pt = optFlow.at<Point2f>(i, j);
                if (pt.x > 2 || pt.y > 2)
                    circle(prevFrame, Point(i, j), 2, Scalar(0, 255, 0), 2, 8, 0);
            
            }
            
        }
        
        imshow("Output", prevFrame);
        curFrGray.copyTo(prevFrGray);
        curFrame.copyTo(prevFrame);
        char c = waitKey(50);
        
        if (c == 27) break;
    
    }
    
    cap.release();
    waitKey(0);
    return 0;
    
}
