#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/gpu/gpu.hpp"

int main (int argc, char* argv[])
{
    try
    {
        cv::Mat src_host = cv::imread("file.png", CV_LOAD_IMAGE_GRAYSCALE);
        cv::gpu::GpuMat dst, src;
        src.upload(src_host);
	cv::namedWindow("Result",CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
        cv::gpu::threshold(src, dst, 128.0, 255.0, CV_THRESH_BINARY);
	cv::Mat result_host;
     	dst.download(result_host);
	cv::imshow("Result", result_host);
	cv::waitKey(0);
	imwrite("a.png",result_host);
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
