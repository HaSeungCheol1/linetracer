#ifndef _LINETRACER_H_
#define _LINETARCER_H_
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "dxl.hpp"
#define K 0.12
#define RPM 50
using namespace std;
using namespace cv;
namespace hsc {
	void ROI_SET(Mat& f, Mat& d, Mat& r);
	void Center_Gravity(Mat& r, Mat& c, vector<Point2d>& pts, bool& Center, int cnt);
	int Get_Error(Mat& r, vector<Point2d> &pts);
}

#endif // !_LINETRACER_H_

