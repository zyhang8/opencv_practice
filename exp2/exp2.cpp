﻿#include<iostream>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat img, imgGray, result;
	img = imread("people.png");
	int row = img.rows;//获取行数
	//cout << row << endl;
	int col = img.cols * img.channels();//注意是列数*通道数
	cout << "分辨率为:" << row << "*" << img.cols << endl;
	for (int i = 9; i < 12; ++i)
	{
		uchar* data = img.ptr<uchar>(i);//获取第i行首地址
		for (int j = 9; j < 13; ++j)
		{
			cout << int(*data) << endl;
			data++;
		}
	}
	cvtColor(img, imgGray, CV_BGR2GRAY);
	imshow("灰度图", imgGray);
	imwrite("灰度图.png", imgGray);
	imshow("原图", img);
	Rect m_select = Rect(0, 0, imgGray.cols/2 ,imgGray.rows / 2);
	Mat ROI = imgGray(m_select);
	imshow("一半分辨率", ROI);
	imwrite("一半分辨率.png", ROI);
	//blur(imgGray, imgGray, Size(3, 3));
	threshold(imgGray, result, 50, 150, CV_THRESH_BINARY);
	imshow("二值化", result);
	imwrite("people_gray.png", result);
	waitKey();

	return 0;
}