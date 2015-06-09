#include <iostream>
#include <vector>
#include <sstream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/gpu/gpu.hpp"

using namespace std;
using namespace cv;
using namespace cv::gpu;

template <typename T>
inline T mapVal(T x, T a, T b, T c, T d)
{
x = ::max(::min(x, b), a);
return c + (d-c) * (x-a) / (b-a);
}

static void colorizeFlow(const Mat &u, const Mat &v, Mat &dst)
{
	double uMin, uMax;
	minMaxLoc(u, &uMin, &uMax, 0, 0);
	double vMin, vMax;
	minMaxLoc(v, &vMin, &vMax, 0, 0);
	uMin = ::abs(uMin); uMax = ::abs(uMax);
	vMin = ::abs(vMin); vMax = ::abs(vMax);
	float dMax = static_cast<float>(::max(::max(uMin, uMax), ::max(vMin, vMax)));
	dst.create(u.size(), CV_8UC3);
	for (int y = 0; y < u.rows; ++y)
	{
		for (int x = 0; x < u.cols; ++x)
		{
			dst.at<uchar>(y,3*x) = 0;
			dst.at<uchar>(y,3*x+1) = (uchar)mapVal(-v.at<float>(y,x), -dMax, dMax, 0.f, 255.f);
			dst.at<uchar>(y,3*x+2) = (uchar)mapVal(u.at<float>(y,x), -dMax, dMax, 0.f, 255.f);
		}
	}
}

int main(int argc, char **argv)
{
	namedWindow("frameL",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	namedWindow("frameR",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"


	VideoCapture cap(0); // open the video camera no. 0
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
        	return -1;
        }

	Mat frameL, frameR;
	Mat flowxy, flowx, flowy, image;
	
	bool bSuccess = cap.read(frameL); // read a new frame from video
	if (!bSuccess) //if not success, break loop
	{
		cout << "Cannot read a frame from video stream" << endl;
		return -1;
	}
	if (frameL.empty()) cout << "Can't open Camera." << "'\n";

	GpuMat d_frameL, d_frameR;
	GpuMat d_flowx, d_flowy, med;
	
	med.upload(frameL);
	cv::gpu::cvtColor(med, d_frameL, CV_BGR2GRAY);
	
	FarnebackOpticalFlow d_calc;

	bool running = true, gpuMode = true;
	int64 t, t0=0, t1=1, tc0, tc1;
	cout << "Use 'm' for CPU/GPU toggling\n";
	while (running)
	{
		t = getTickCount();

		bool bSuccess = cap.read(frameR); // read a new frame from video
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			return -1;
		}
		if (frameR.empty()) cout << "Can't open Camera." << "'\n";

		med.upload(frameR);
		cv::gpu::cvtColor(med, d_frameR, CV_BGR2GRAY);

		d_frameL.download(frameL);
		d_frameR.download(frameR);
		imshow("frameL", frameL); //show the frame in "MyVideo" window
		imshow("frameR", frameR); //show the frame in "MyVideo" window
				

		
		

		if (gpuMode)
		{
			tc0 = getTickCount();
			d_calc(d_frameL, d_frameR, d_flowx, d_flowy);
			tc1 = getTickCount();
			d_flowx.download(flowx);
			d_flowy.download(flowy);
		}
		else
		{
			tc0 = getTickCount();
			calcOpticalFlowFarneback(
			frameL, frameR, flowxy, d_calc.pyrScale, d_calc.numLevels, d_calc.winSize,
			d_calc.numIters, d_calc.polyN, d_calc.polySigma, d_calc.flags);
			tc1 = getTickCount();
			Mat planes[] = {flowx, flowy};
			split(flowxy, planes);
			flowx = planes[0]; flowy = planes[1];
		}
		colorizeFlow(flowx, flowy, image);
		stringstream s;
		s << "mode: " << (gpuMode?"GPU":"CPU");
		putText(image, s.str(), Point(5, 25), FONT_HERSHEY_SIMPLEX, 1., Scalar(255,0,255), 2);
		s.str("");
		s << "opt. flow FPS: " << cvRound((getTickFrequency()/(tc1-tc0)));
		putText(image, s.str(), Point(5, 65), FONT_HERSHEY_SIMPLEX, 1., Scalar(255,0,255), 2);
		s.str("");
		s << "total FPS: " << cvRound((getTickFrequency()/(t1-t0)));
		putText(image, s.str(), Point(5, 105), FONT_HERSHEY_SIMPLEX, 1., Scalar(255,0,255), 2);
		imshow("flow", image);
		char ch = (char)waitKey(3);
		if (ch == 27)
			running = false;
		else if (ch == 'm' || ch == 'M')
			gpuMode = !gpuMode;
		t0 = t;
		t1 = getTickCount();
		
		//frameL = frameR ;
		//d_frameL = d_frameR;
		
	}
	return 0;
}
