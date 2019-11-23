﻿#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int i;
int j;

// 归一化
//data 进行处理的像素集合
//grayscale 目标灰度级
//rows cols type 目标图像的行，列，以及类型

int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
double gray_prob[256] = { 0 };  //记录灰度分布密度
double gray_distribution[256] = { 0 }; //记录累计密度
int gray_equal[256] = { 0 }; //均衡化后的灰度值

int gray_sum = 0; //像素总数

Mat Normalize(vector<double> data, int grayscale, int rows, int cols, int type)
{
	double max = 0.0;
	double min = 0.0;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] > max)
			max = data[i];
		if (data[i] < min)
			min = data[i];
	}
	Mat dst;
	dst.create(rows, cols, type);
	int index = 0;
	for (int r = 0; r < dst.rows; r++)
	{
		uchar* dstRowData = dst.ptr<uchar>(r);
		for (int c = 0; c < dst.cols; c++)
		{
			dstRowData[c] = (uchar)(grayscale * ((data[index++] - min) * 1.0 / (max - min)));
		}
	}
	return dst;
}

// 幂律变化
Mat PowerTranseform(Mat src, double gamma, int parameter)
{
	Mat dst;
	dst.create(src.size(), src.type());
	vector<double> value;
	for (int r = 0; r < src.rows; r++)
	{
		uchar* srcRowData = src.ptr<uchar>(r);
		for (int c = 0; c < src.cols; c++)
		{
			//幂次变换的公式为 s = c * r ^ v r为输入图像像素
			value.push_back(parameter * pow((double)srcRowData[c], gamma));
		}
	}
	return Normalize(value, 255, src.rows, src.cols, src.type());
}


// 双线性插值
Mat bilinear(Mat src, int row, int col) {
	int rows = src.rows, cols = src.cols;
	cv::Mat dst(row, col, src.type());
	for (int i = 0; i < row; ++i) {
		//以ptr的方式访问dst的数据
		uchar* p = dst.ptr<uchar>(i);
		//使两个图像的几何中心重合，采样更合理
		float x = (i + 0.5) * rows / row - 0.5;
		int fx = (int)x;
		//x为坐标的小数部分
		x -= fx;
		//以整数计算速度更快
		short x1 = (1.f - x) * 2048;
		short x2 = 2048 - x1;
		for (int j = 0; j < col; ++j) {
			//trick
			float y = (j + 0.5) * cols / col - 0.5;
			int fy = (int)y;
			y -= fy;
			//trick
			short y1 = (1.f - y) * 2048;
			short y2 = 2048 - y1;
			//结果右移22位抵消2048的平方
			p[j] = (src.at<uchar>(fx, fy) * x1 * y1 + src.at<uchar>(fx + 1, fy) * x2 * y1
				+ src.at<uchar>(fx, fy + 1) * x1 * y2 + src.at<uchar>(fx + 1, fy + 1) * x2 * y2) >> 22;
		}
	}
	return dst;
}

Mat rollback(Mat dst,int row,int col){
	//图像反转(直接访问像素)
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			dst.at<uchar>(i, j) = 255 - dst.at<uchar>(i, j);
			// 每一个像素反转
		}
	}
	return dst;
}

Mat equalize_hist(Mat& input)
{
	Mat output = input.clone();
	gray_sum = input.cols * input.rows;

	//统计每个灰度下的像素个数
	for (int i = 0; i < input.rows; i++)
	{
		uchar* p = input.ptr<uchar>(i);
		for (int j = 0; j < input.cols; j++)
		{
			int vaule = p[j];
			//cout << input.cols*i+j << ':'<< vaule << endl;
			gray[vaule]++;
		}
	}
	//统计灰度频率
	for (int i = 0; i < 256; i++)
	{
		gray_prob[i] = ((double)gray[i] / gray_sum);
		cout << i + 1 << ':' << gray_prob[i] * gray_sum << endl;
	}
	return output;
}



int main()
{
	Mat img = cv::imread("lena_gray.jpg", 0);
	int row = img.rows;//获取行数
	int col = img.cols * img.channels();//注意是列数*通道数
	cout << row << "," << col << endl;
	equalize_hist(img);

	/*ofstream ofs("pixel.txt");
	for (int i = 0; i < row; ++i)
	{
		uchar* data = img.ptr<uchar>(i);//获取第i行首地址
		for (int j = 0; j < col; ++j)
		{
			//指针方式
			ofs << int(*data) << endl;
			data++;
		}
	}
	cout << row << "," << col << endl;
	cout << "遍历完毕" << endl;
	cout << endl;
	ofs.close();*/

	//显示原图
	imshow("img", img);
	Mat src;

	bitwise_not(src, img);
	//图像颜色反转的函数
	imshow("src", src);

	//幂律变换
	Mat dstImg;
	dstImg = PowerTranseform(img, 0.4, 1);
	imshow("变换后", dstImg);

	// 双线性插值
	Mat scale1, scale2;
	imshow("0.1", img);
	/*scale1 = bilinear(img1, row * 1.5, col * 1.5);
	imshow("1.5", scale1);
	cv::imwrite("1.5.jpg", scale1);
	scale2 = bilinear(img, row * 0.8, col * 0.8);
	imshow("0.8", scale2);
	cv::imwrite("0.8.jpg", scale2);*/

	waitKey(0); // 无限制的等待用户的按键事件
	return 0;
}