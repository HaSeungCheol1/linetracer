#include "linetracer.hpp"
namespace hsc {
	void ROI_SET(Mat& f, Mat& d, Mat &r) {
		d = f(Rect(0, 270, 640, 90));
		cvtColor(d, d, COLOR_BGR2GRAY);
		//bilateralFilter(d, d, -1, 10, 5); 
		r = d + (100 - mean(d)[0]);  
		threshold(r, r, 135, 255, THRESH_BINARY); 
	}

	
	void Center_Gravity(Mat& r, Mat& c, vector<Point2d>& pts, bool& Center, int cnt) {
		if (cnt >= 2) {
			if (Center) { pts.push_back(Point2d(r.cols / 2.0, r.rows / 2.0)); Center = false; }

			double min = 0;
			Point2d other(c.at<double>(1, 0), c.at<double>(1, 1));
			min = norm(pts.at(0) - other);
			for (int i = 2; i < cnt; i++) {
				double current_norm = norm(pts.at(0) - Point2d(c.at<double>(i, 0), c.at<double>(i, 1)));
				if (min > current_norm) {
					min = current_norm;
					other = Point2d(c.at<double>(i, 0), c.at<double>(i, 1));
				}
			}
			
			if (!(abs(pts.at(0).x - other.x) > 100 || abs(pts.at(0).y - other.y) > 60)) {
				pts.insert(pts.begin(), 1, other);
			}
		}
		circle(r, pts.at(0), 1, Scalar(0, 0, 255), 2);
	}

	int Get_Error(Mat& r, vector<Point2d>& pts) {
		int error = 0;
		error = (r.cols / 2) - pts.at(0).x;
		return error;
	}
}