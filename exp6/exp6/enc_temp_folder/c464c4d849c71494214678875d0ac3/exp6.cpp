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

	namedWindow("gaosi_src", WINDOW_AUTOSIZE);
	namedWindow("gaosi_img", WINDOW_AUTOSIZE);
	imshow("gaosi_src", gaosi_src);
	imshow("gaosi_img", gaosi_img);

	Mat canny_src, canny_img;
	Canny(gaosi_src, canny_src, 100, 200);
	Canny(gaosi_img, canny_img, 100, 200);
	imshow("canny_src", canny_src);
	imshow("canny_img", canny_img);
	waitKey(0);
	return 0;
}