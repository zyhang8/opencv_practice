#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;
#define WindowNameSrc "canny_lena"	//定义窗口名字的宏
#define WindowNameImg "canny_car"	//定义窗口名字的宏

char Trackbarname1[50];		//用于存储滑动条1的名称
char Trackbarname2[50];		//用于存储滑动条2的名称
char Trackbarname3[50];		//用于存储滑动条3的名称
char Trackbarname4[50];		//用于存储滑动条4的名称

int pre_place1, pre_place2;	//滑动条对应的变量，两个阈值变量
int pre_place3, pre_place4;	//滑动条对应的变量，两个阈值变量
const int max_place = 255;	//定义Trackbar的最大值


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

	namedWindow(WindowNameSrc, WINDOW_AUTOSIZE);    //定义自动适应图片大小的窗口
	namedWindow(WindowNameImg, WINDOW_AUTOSIZE);    //定义自动适应图片大小的窗口
	Mat canny_src, canny_img;

	sprintf(Trackbarname1, "阈值1 %d", max_place);//sprintf函数，格式化赋值
	sprintf(Trackbarname2, "阈值2 %d", max_place);//sprintf函数，格式化赋值
	sprintf(Trackbarname3, "阈值1 %d", max_place);//sprintf函数，格式化赋值
	sprintf(Trackbarname4, "阈值2 %d", max_place);//sprintf函数，格式化赋值

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

	waitKey(0);
	return 0;
}