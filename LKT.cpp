
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat frame, gray;
Mat prev_frame, prev_gray;
vector<Point2f> features;   // Shi-Tomasi角点特征数据
vector<Point2f> iniPts;     // 初始化特征数据
vector<Point2f> fpts[2];    // 当前帧和前一帧的特征点位置
vector<uchar> status;       // 特征点跟踪成功标志位
vector<float> errs;     // 跟踪区域误差

void drawFeature(Mat& inFrame);
void detectFeat(Mat& inFrame, Mat& inGray);
void KLTracker();
void drawTrackLines();

int main() {
	VideoCapture cap;
	cap.open("C:/Users/teamf/Videos/VIVY/Vivy06.mkv");
	if (!cap.isOpened()) {
		cout << "Could not load file..." << endl;
		return -1;
	}

	namedWindow("Input", WINDOW_AUTOSIZE);

	while (cap.read(frame)) {
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		if (fpts[0].size() < 40) {
			detectFeat(frame, gray);
			fpts[0].insert(fpts[0].end(), features.begin(), features.end());
			iniPts.insert(iniPts.end(), features.begin(), features.end());
		}
		else {
			cout << "tracking..." << endl;
		}
		if (prev_gray.empty()) {
			gray.copyTo(prev_gray);
		}
		KLTracker();
		drawFeature(frame);
		//更新前一帧数据
		gray.copyTo(prev_gray);
		frame.copyTo(prev_frame);
		imshow("Input", frame);
		char c = waitKey(50);
		if (c == 27)
			break;
	}
	waitKey(0);
	return 0;
}

void detectFeat(Mat& inFrame, Mat& inGray) {
	double maxCorners = 5000;
	double qualityLevel = 0.01;
	double minDist = 10;
	double blockSize = 3;
	double k = 0.04;
	goodFeaturesToTrack(inGray, features, maxCorners, qualityLevel, minDist, Mat(), blockSize, false, k);
	cout << "Detected features: " << features.size() << endl;
}

void drawFeature(Mat& inFrame) {
	for (size_t t = 0; t < fpts[0].size(); t++) {
		circle(inFrame, fpts[0][t], 2, Scalar(0, 255, 0), 2, 8, 0);
	}
}

void KLTracker() {
	// KLT
	calcOpticalFlowPyrLK(prev_gray, gray, fpts[0], fpts[1], status, errs);
	int k = 0;
	// 特征点过滤
	for (int i = 0; i < fpts[1].size(); i++) {
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y = fpts[1][i].y);
		if (dist > 2 && status[i]) {
			iniPts[k] = iniPts[i];
			fpts[1][k++] = fpts[1][i];
		}
	}
	// 保存特征点并绘制跟踪轨迹
	iniPts.resize(k);
	fpts[1].resize(k);
	drawTrackLines();
	swap(fpts[1], fpts[0]);
}

void drawTrackLines() {
	for (size_t t = 0; t < fpts[1].size(); t++) {
		line(frame, iniPts[t], fpts[1][t], Scalar(0, 255, 0), 2, 8, 0);
		circle(frame, fpts[1][t], 2, Scalar(0, 255, 0), 2, 8, 0);
	}
}
