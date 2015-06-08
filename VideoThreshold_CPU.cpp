#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/video/video.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	VideoCapture cap(0); // open the video camera no. 0
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
        	return -1;
        }
	//namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	//namedWindow("MyOutput",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
		
	cv::Mat src_host;
        cv::Mat dst, src, med;
        cv::Mat result_host;
	
	while (1)
        {
        	bool bSuccess = cap.read(src); // read a new frame from video
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		//imshow("MyVideo", src); //show the frame in "MyVideo" window
        	
		//The Main Code Comes Here
		//src.upload(src_host);
		cv::cvtColor(src, med, CV_BGR2GRAY);
        	cv::threshold(med, dst, 128.0, 255.0, CV_THRESH_BINARY);
     		//dst.download(result_host);
		
        	imshow("MyOutput",dst); //show the frame in "MyVideo" window

        	if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}
    return 0;
}

