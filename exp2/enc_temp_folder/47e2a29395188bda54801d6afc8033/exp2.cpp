
#include<iostream>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat img, imgGray, result;
	img = imread("aaa.png");
	imshow("原图", img);
	cvtColor(img, imgGray, CV_BGR2GRAY);
	imshow("灰度图", imgGray);
	//blur(imgGray, imgGray, Size(3, 3));
	threshold(imgGray, result, 100, 255, CV_THRESH_BINARY);
	imshow("二值化后的图", result);
	imwrite("二值化的二维码.jpg", result);
	cout << "图片已保存" << endl;
	waitKey();

	return 0;
}