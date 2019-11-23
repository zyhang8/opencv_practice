#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;
#define WindowNameSrc "canny_car"	//定义窗口名字的宏
#define WindowNameImg "canny_lena"	//定义窗口名字的宏
#define WindowNameHoughSrc "hough_car"	//定义窗口名字的宏
#define WindowNameHoughImg "hough_lena"	//定义窗口名字的宏

char Trackbarname1[50];		//用于存储滑动条1的名称
char Trackbarname2[50];		//用于存储滑动条2的名称
char Trackbarname3[50];		//用于存储滑动条3的名称
char Trackbarname4[50];		//用于存储滑动条4的名称

int pre_place1, pre_place2;	//滑动条对应的变量，两个阈值变量
int pre_place3, pre_place4;	//滑动条对应的变量，两个阈值变量
const int max_place = 255;	//定义Trackbar的最大值
const int hough_max_place = 200;


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

	/*
	namedWindow(WindowNameSrc, WINDOW_AUTOSIZE);    //定义自动适应图片大小的窗口
	namedWindow(WindowNameImg, WINDOW_AUTOSIZE);    //定义自动适应图片大小的窗口

	sprintf(Trackbarname1, "阈值1 %d", max_place);//sprintf函数，格式化赋值
	sprintf(Trackbarname2, "阈值2 %d", max_place);//sprintf函数，格式化赋值
	sprintf(Trackbarname3, "阈值3 %d", max_place);//sprintf函数，格式化赋值
	sprintf(Trackbarname4, "阈值4 %d", max_place);//sprintf函数，格式化赋值

	createTrackbar(Trackbarname1, WindowNameSrc, &pre_place1, max_place); //创建滑动条
	createTrackbar(Trackbarname2, WindowNameSrc, &pre_place2, max_place); //创建滑动条
	createTrackbar(Trackbarname3, WindowNameImg, &pre_place3, max_place); //创建滑动条
	createTrackbar(Trackbarname4, WindowNameImg, &pre_place4, max_place); //创建滑动条

	while (1) {
		pre_place1 = getTrackbarPos(Trackbarname1, WindowNameSrc);	//获取滑动条当前位置
		pre_place2 = getTrackbarPos(Trackbarname2, WindowNameSrc);	//获取滑动条当前位置
		pre_place3 = getTrackbarPos(Trackbarname3, WindowNameImg);	//获取滑动条当前位置
		pre_place4 = getTrackbarPos(Trackbarname4, WindowNameImg);	//获取滑动条当前位置

		Canny(gaosi_src, canny_src, pre_place1, pre_place2, 3);
		Canny(gaosi_img, canny_img, pre_place3, pre_place4, 3);

		imshow(WindowNameSrc, canny_src);
		imshow(WindowNameImg, canny_img);

		if (waitKey(10) == 27) break;     //按下Esc键退出程序
	}
	*/

	Canny(gaosi_src, canny_src, 246, 0, 3);
	Canny(gaosi_img, canny_img, 102, 12, 3);

	imshow(WindowNameSrc, canny_src);
	imshow(WindowNameImg, canny_img);

	//【3】进行霍夫线变换
	vector<Vec2f> lines;
	Mat cdst;
	HoughLines(canny_src, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}

	imshow("【效果图】", cdst);

	/*
	namedWindow(WindowNameHoughSrc, WINDOW_AUTOSIZE);    //定义自动适应图片大小的窗口
	namedWindow(WindowNameHoughImg, WINDOW_AUTOSIZE);    //定义自动适应图片大小的窗口

	sprintf(Trackbarname1, "阈值1 %d", hough_max_place);//sprintf函数，格式化赋值
	sprintf(Trackbarname2, "阈值2 %d", hough_max_place);//sprintf函数，格式化赋值

	createTrackbar(Trackbarname1, WindowNameHoughSrc, &pre_place1, hough_max_place); //创建滑动条
	createTrackbar(Trackbarname2, WindowNameHoughImg, &pre_place2, hough_max_place); //创建滑动条

	while (1) {
		pre_place1 = getTrackbarPos(Trackbarname1, WindowNameHoughSrc);	//获取滑动条当前位置
		pre_place2 = getTrackbarPos(Trackbarname2, WindowNameHoughImg);	//获取滑动条当前位置

		HoughLines(canny_src, lines_src, 1, CV_PI / 180, pre_place1, 0, 0);
		HoughLines(canny_src, lines_img, 1, CV_PI / 180, pre_place2, 0, 0);

		for (size_t i = 0; i < lines_src.size(); i++)
		{
			float rho = lines_src[i][0], theta = lines_src[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(hough_src, pt1, pt2, Scalar(55, 100, 195), 1, CV_AA);
		}
		for (size_t i = 0; i < lines_img.size(); i++)
		{
			float rho = lines_img[i][0], theta = lines_img[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(hough_img, pt1, pt2, Scalar(55, 100, 195), 1, CV_AA);
		}

		imshow(WindowNameHoughSrc, hough_src);
		imshow(WindowNameHoughImg, hough_img);

		if (waitKey(10) == 27) break;     //按下Esc键退出程序
	}
	//依次在图中绘制出每条线段
	*/

	waitKey(0);
	return 0;
}