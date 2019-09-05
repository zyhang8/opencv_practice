#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int i;
int j;

// 归一化
//data				进行处理的像素集合
//grayscale			目标灰度级
//rows cols type	目标图像的行，列，以及类型
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
			//幂次变换的公式为 s = c * r ^ v	r为输入图像像素
			value.push_back(parameter * pow((double)srcRowData[c], gamma));
		}
	}
	return Normalize(value, 255, src.rows, src.cols, src.type());
}


/* 双线性插值
Mat Bilinear(Mat img, double width, double height)
{

	return matDst1;
}
*/

int main()
{
	Mat img = cv::imread("lena_gray.jpg", 0);
	int row = img.rows;//获取行数
	int col = img.cols * img.channels();//注意是列数*通道数
	cout << row << "," << col << endl;
	ofstream ofs("pixel.txt");
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
	ofs.close();

	//显示原图
	imshow("img", img);

	//图像反转(直接访问像素)
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			img.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);   // 每一个像素反转
		}
	}
	//显示图片
	imshow("dst", img);

	//幂律变换
	Mat dstImg;
	dstImg = PowerTranseform(img, 0.4, 1);
	imshow("变换后", dstImg);

	// 双线性插值
	cv::Mat matSrc, matDst1, matDst2;
	matSrc = cv::imread("lena.jpg", 2 | 4);
	matDst1 = cv::Mat(cv::Size(800, 1000), matSrc.type(), cv::Scalar::all(0));
	matDst2 = cv::Mat(matDst1.size(), matSrc.type(), cv::Scalar::all(0));

	double scale_x = (double)matSrc.cols / matDst1.cols;
	double scale_y = (double)matSrc.rows / matDst1.rows;

	uchar* dataDst = matDst1.data;
	int stepDst = matDst1.step;
	uchar* dataSrc = matSrc.data;
	int stepSrc = matSrc.step;
	int iWidthSrc = matSrc.cols;
	int iHiehgtSrc = matSrc.rows;

	for (int j = 0; j < matDst1.rows; ++j)
	{
		float fy = (float)((j + 0.5) * scale_y - 0.5);
		int sy = cvFloor(fy);
		fy -= sy;
		sy = std::min(sy, iHiehgtSrc - 2);
		sy = std::max(0, sy);

		short cbufy[2];
		cbufy[0] = cv::saturate_cast<short>((1.f - fy) * 2048);
		cbufy[1] = 2048 - cbufy[0];

		for (int i = 0; i < matDst1.cols; ++i)
		{
			float fx = (float)((i + 0.5) * scale_x - 0.5);
			int sx = cvFloor(fx);
			fx -= sx;

			if (sx < 0) {
				fx = 0, sx = 0;
			}
			if (sx >= iWidthSrc - 1) {
				fx = 0, sx = iWidthSrc - 2;
			}

			short cbufx[2];
			cbufx[0] = cv::saturate_cast<short>((1.f - fx) * 2048);
			cbufx[1] = 2048 - cbufx[0];

			for (int k = 0; k < matSrc.channels(); ++k)
			{
				*(dataDst + j * stepDst + 3 * i + k) = (*(dataSrc + sy * stepSrc + 3 * sx + k) * cbufx[0] * cbufy[0] +
					*(dataSrc + (sy + 1) * stepSrc + 3 * sx + k) * cbufx[0] * cbufy[1] +
					*(dataSrc + sy * stepSrc + 3 * (sx + 1) + k) * cbufx[1] * cbufy[0] +
					*(dataSrc + (sy + 1) * stepSrc + 3 * (sx + 1) + k) * cbufx[1] * cbufy[1]) >> 22;
			}
		}
	}
	cv::imwrite("linear_1.jpg", matDst1);

	cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, 1);
	cv::imwrite("linear_2.jpg", matDst2);

	waitKey(0); // 无限制的等待用户的按键事件
	return 0;
}
