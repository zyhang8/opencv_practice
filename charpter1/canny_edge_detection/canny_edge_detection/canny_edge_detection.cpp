﻿// canny_edge_detection.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
using namespace cv;

//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
int main()
{
	//【1】载入原始图
	Mat srcImage = imread("1.jpg");

	//【2】显示原始图
	imshow("均值滤波【原图】", srcImage);

	//【3】进行均值滤波操作
	Mat dstImage;
	blur(srcImage, dstImage, Size(7, 7));

	//【4】显示效果图
	imshow("均值滤波【效果图】", dstImage);

	waitKey(0);
}