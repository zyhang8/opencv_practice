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

//均值化
void histBar(Mat src)
{
	int histSize = 30;
	MatND hist;
	float range[] = { 0, 255 };
	const float* ranges = { range };
	//计算直方图
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	//将直方图bin的数值归一化到0-255
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	//显示直方图
	int binsW = cvRound((double)500 / histSize);
	Mat histImg = Mat::zeros(500, 500, CV_8UC3);
	RNG rng(123);
	for (int i = 0; i < histSize; i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(histImg, Point(i * binsW, 500), Point((i + 1) * binsW, 500 - cvRound(hist.at<float>(i) * 500 / 255.0)), color, -1);
	}
	imshow("histogram", histImg);
}

void sharpen(const Mat image, Mat& result)
{
	result.create(image.size(), image.type());
	for (int j = 1; j < image.rows - 1; j++)
	{
		const uchar* previous = image.ptr<const uchar>(j - 1);
		const uchar* current = image.ptr<const uchar>(j);
		const uchar* next = image.ptr<const uchar>(j + 1);

		uchar* output = result.ptr<uchar>(j);
		for (int i = 1; i < image.cols - 1; i++)
		{
			//sharpened_pixel=5*current-left-right-up-down;
			//cv::saturate_cast用以对计算结果进行截断(0-255)
			*output++ = saturate_cast<uchar>(
				5 * current[i] - current[i - 1]
				- current[i + 1] - previous[i] - next[i]);
		}
	}
	result.row(0).setTo(Scalar(0));
	result.row(result.rows - 1).setTo(Scalar(0));
	result.col(0).setTo(Scalar(0));
	result.col(result.cols - 1).setTo(Scalar(0));
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
	
	Mat dst;
	bitwise_not(img, dst, noArray());

	//show image 
	imshow("dst", dst);

	//幂律变换
	Mat dstImg;
	dstImg = PowerTranseform(img, 0.4, 1);
	imshow("变换后", dstImg);

	// 双线性插值
	Mat scale1, scale2;
	/*scale1 = bilinear(img, row * 1.5, col * 1.5);
	imshow("1.5", scale1);
	cv::imwrite("1.5.jpg", scale1);*/
	Mat dst_img1,dst_img2;
	// 双线性插值(resize和源算法)
	resize(img, dst_img1, Size(), 1.5, 1.5);
	imshow("resize_1.5", dst_img1);
	imwrite("resize_1.5.jpg", dst_img1);
	resize(img, dst_img2, Size(), 0.8, 0.8);
	imshow("resize_0.8", dst_img2);
	imwrite("resize_0.8.jpg", dst_img2);
	scale2 = bilinear(img, row * 0.8, col * 0.8);
	imshow("0.8", scale2);
	imwrite("0.8.jpg", scale2);

	histBar(img);

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
	

	//边缘检测
	//sobel算子
	Mat gao_src, gray_src, xdst, ydst, dst_sobel;
	/*GaussianBlur(img, gao_src, Size(3, 3), 0, 0);//高斯滤波
	/imshow("gaosioutput", gao_src);
	cvtColor(gao_src, gray_src, CV_RGB2GRAY);
	//imshow("grayoutput", gray_src);//转换为灰度图*/
	Sobel(img, xdst, -1, 1, 0);
	imshow("xdst", xdst);
	Sobel(img, ydst, -1, 0, 1);
	imshow("ydst", ydst);
	addWeighted(xdst, 0.5, ydst, 0.5, 1, dst_sobel);
	imshow("sobel", dst_sobel);
	//拉普拉斯算子
	Mat gaosrc, graysrc, ladst, dst_la;
	/*GaussianBlur(img, gaosrc, Size(3, 3), 0);
	//imshow("gaosrc", gaosrc);
	cvtColor(gaosrc, graysrc, CV_RGB2GRAY);
	//imshow("graysrc", graysrc);*/
	Laplacian(img, ladst, -1);
	//imshow("ladst", ladst);
	convertScaleAbs(ladst, dst_la);
	imshow("la", dst_la);

	//锐化
	//拉普拉斯算子
	Mat la_dst, sobel_dst;
	la_dst.create(img.size(), img.type());
	sharpen(img, la_dst);
	imshow("sharpen_la", la_dst);
	//sobel算子
	//Generate Gradient X
	
	//imshow("sharpen_la", sobel_dst);

	waitKey(0); // 无限制的等待用户的按键事件
	return 0;
}