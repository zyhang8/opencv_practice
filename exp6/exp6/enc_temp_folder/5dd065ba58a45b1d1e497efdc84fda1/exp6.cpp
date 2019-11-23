#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>


using namespace std;
using namespace cv;


int main()
{
	Mat src = imread("car_gray.png");
	Mat img = imread("lena_gray.png");
	Mat gaosi_src, gaosi_img;
	cv::GaussianBlur(src, gaosi_src, cv::Size(5, 5), 3, 3);
	cv::GaussianBlur(img, gaosi_img, cv::Size(5, 5), 3, 3);

	imshow("GRAY", gaosi_src);
	imshow("GRAY", gaosi_img);
	
	return 0;
}