#include "linetracer.hpp"
using namespace hsc;
bool mode = false;
bool ctrl_c_pressed = false;
void ctrlc_handler(int) { ctrl_c_pressed = true; }
int main(void)
{
	// VideoCapture source("6_lt_ccw_100rpm_out.mp4");
	// if (!source.isOpened()) { cerr << "Video open failed!" << endl; return-1; }

	Dxl mx;
	struct timeval start, end1;
	double diff1;
	int rightvel = 0, leftvel = 0;

	signal(SIGINT, ctrlc_handler); 				
	if (!mx.open()) { cout << "dxl open error" << endl; return -1; } 

	string src = "nvarguscamerasrc sensor-id=0 ! \
   	video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
   	format=(string)NV12 ! nvvidconv flip-method=0 ! video/x-raw, \
  	width=(int)640, height=(int)360, format=(string)BGRx ! \
   	videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	VideoCapture source(src, CAP_GSTREAMER);
	if (!source.isOpened()) { cout << "Camera error" << endl; return -1; }

	string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
   	nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
   	h264parse ! rtph264pay pt=96 ! \
   	udpsink host=192.168.0.51 port=6100 sync=false";
	// 167, 151, 203.234.58.167
	VideoWriter writer1(dst1, 0, (double)30, Size(640, 360), true);
	if (!writer1.isOpened()) { cerr << "Writer open failed!" << endl; return -1; }

	string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
   nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
   h264parse ! rtph264pay pt=96 ! \
   udpsink host=192.168.0.51 port=6200 sync=false";
	// 167, 151
	VideoWriter writer2(dst2, 0, (double)30, Size(640, 90), true);
	if (!writer2.isOpened()) { cerr << "Writer open failed!" << endl; return -1; }

	vector<Point2d> pts;
	int cnt, error = 0;
	bool Center = true;
	Mat frame, ROI, dst;
	Mat labels, stats, centroids;
	while (true)
	{
		gettimeofday(&start, NULL);
		source >> frame;
		if (frame.empty())break;
		writer1 << frame;

		ROI_SET(frame,dst,ROI);
		cnt = connectedComponentsWithStats(ROI, labels, stats, centroids); 
		cvtColor(ROI, ROI, COLOR_GRAY2BGR);
		Center_Gravity(ROI, centroids, pts, Center, cnt);
		error = Get_Error(ROI, pts);

		writer2 <<ROI;

		leftvel = RPM - K * error;
		rightvel = -(RPM + K * error);

		cout << "Error:" << error;

		if (mx.kbhit())
		{
			char ch = mx.getch();
			if (ch == 's') mode = true;
		}
		if (ctrl_c_pressed) break;
		if (mode) mx.setVelocity(leftvel, rightvel);

		usleep(30 * 1000);
		gettimeofday(&end1, NULL);
		diff1 = end1.tv_sec + end1.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
		cout << ", leftvel:" << leftvel << ',' << "rightvel:" << rightvel << ", time:" << diff1 << endl;

		// imshow("frame", frame);
		// imshow("ROI", ROI);
		// if (waitKey(30) == 27)break;
	}
	mx.close();
	return 0;
}