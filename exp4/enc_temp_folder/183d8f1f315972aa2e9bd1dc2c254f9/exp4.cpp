#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
double gray_prob[256] = { 0 };  //记录灰度分布密度
double gray_distribution[256] = { 0 };  //记录累计密度
int gray_equal[256] = { 0 };  //均衡化后的灰度值

int gray_sum = 0;  //像素总数


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
		//cout << i+1 << ':'<< gray_prob[i]*gray_sum << endl;
	}

	//计算累计密度
	gray_distribution[0] = gray_prob[0];
	for (int i = 1; i < 256; i++)
	{
		gray_distribution[i] = gray_distribution[i - 1] + gray_prob[i];
		//cout << i << ':'<< gray_distribution[i] << endl;
	}

	//重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
	for (int i = 0; i < 256; i++)
	{
		gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
	}

	//直方图均衡化,更新原图每个点的像素值
	for (int i = 0; i < output.rows; i++)
	{
		uchar* p = output.ptr<uchar>(i);
		for (int j = 0; j < output.cols; j++)
		{
			p[j] = gray_equal[p[j]];
			//cout << output.cols*i+j << ':'<< p[j] << endl;
		}
	}
	imshow("hist", output);
	
	return output;
}

int main()
{
	Mat img = imread("lena.png", 0);
	imshow("原图", img);
	equalize_hist(img);

	//进行均值滤波操作
	Mat out;
	blur(img, out, Size(3, 3));
	imshow("均值滤波3*3", out);
	blur(img, out, Size(5, 5));
	imshow("均值滤波5*5", out);
	blur(img, out, Size(7, 7));
	imshow("均值滤波7*7", out);

	//进行均值滤波操作
	Mat g_mDstImage;
	medianBlur(img, g_mDstImage, 3);
	imshow("中值滤波3*3", out);
	medianBlur(img, g_mDstImage, 5);
	imshow("中值滤波5*5", out);
	medianBlur(img, g_mDstImage, 7);
	imshow("中值滤波7*7", out);

	
	//sobel算子
	Mat gao_src, gray_src, xdst, ydst, dst;
	GaussianBlur(img, gao_src, Size(3, 3), 0, 0);
	///imshow("gaosioutput", gao_src);
	cvtColor(gao_src, gray_src, CV_RGB2GRAY);
	//imshow("grayoutput", gray_src);
	//Sobel(gray_src, xdst, -1, 1, 0);
	//imshow("xdst", xdst);
	//Sobel(gray_src, ydst, -1, 0, 1);
	//imshow("ydst", ydst);
	//addWeighted(xdst, 0.5, ydst, 0.5, 1, dst);
	//imshow("sobel", dst);
	/*
	//拉普拉斯算子
	Mat gaosrc, graysrc, ladst, dst;
	GaussianBlur(img, gaosrc, Size(3, 3), 0);
	//imshow("gaosrc", gaosrc);
	cvtColor(gaosrc, graysrc, CV_RGB2GRAY);
	//imshow("graysrc", graysrc);
	Laplacian(graysrc, ladst, -1);
	//imshow("ladst", ladst);
	convertScaleAbs(ladst, dst);
	imshow("la", dst);
	*/
	waitKey(0);
	return 0;
	

	

}
