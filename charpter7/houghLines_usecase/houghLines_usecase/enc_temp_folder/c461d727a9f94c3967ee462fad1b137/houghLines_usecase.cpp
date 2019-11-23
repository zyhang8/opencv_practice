//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV3版书本配套示例程序61
//		程序描述：HoughLines函数用法示例
//		开发测试所用操作系统： Windows 7 64bit
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	3.0 beta
//		2014年11月 Created by @浅墨_毛星云
//		2014年12月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------



//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
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


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
int main()
{
	//【1】载入原始图和Mat变量定义   
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
	Canny(gaosi_src, canny_src, 246, 0, 3);
	Canny(gaosi_img, canny_img, 102, 12, 3);

	//【2】进行边缘检测和转化为灰度图
	//cvtColor(midImage, dstImage, COLOR_GRAY2BGR);//转化边缘检测后的图为灰度图

	//【3】进行霍夫线变换
	vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	HoughLines(canny_src, lines, 1, CV_PI / 180, 150, 0, 0);
	Mat hough_src;
	//【4】依次在图中绘制出每条线段
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
		//此句代码的OpenCV2版为:
		//line( dstImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);
		//此句代码的OpenCV3版为:
		line(hough_src, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA);
	}

	imshow(WindowNameSrc, canny_src);
	imshow(WindowNameImg, canny_img);


	//【7】显示效果图  
	imshow("【效果图】", hough_src);

	waitKey(0);

	return 0;
}


