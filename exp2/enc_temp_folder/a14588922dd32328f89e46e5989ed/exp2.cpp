#include<iostream>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat img, imgGray, result;
	img = imread("aaa.jpg");
	int row = img.rows;//获取行数
	cout << row << endl;
	int col = img.cols * img.channels();//注意是列数*通道数
	cout << "分辨率为:" << row << "*" << col << endl;
	imshow("原图", img);
	Rect m_select = Rect(0, 0, 450, 750);
	Mat ROI = img(m_select);
	//imshow("一半分辨率图", ROI);
	for (int i = 9; i < 12; ++i)
	{
		uchar* data = img.ptr<uchar>(i);//获取第i行首地址
		for (int j = 9; j < 13; ++j)
		{
			cout << "第" << i + 1 << "行" << "第" << j + 1 << "列的像素值为" << int(*data) << endl;
			data++;
		}
	}
	cvtColor(img, imgGray, CV_BGR2GRAY);
	imshow("灰度图", imgGray);
	//blur(imgGray, imgGray, Size(3, 3));
	threshold(imgGray, result, 50, 150, CV_THRESH_BINARY);
	imshow("二值化后的图", result);
	imwrite("aaa_gray.jpg", imgGray);
	cout << "图片已保存" << endl;
	waitKey();

	return 0;
}