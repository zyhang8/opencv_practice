
#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

using namespace std;
using namespace cv;


int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
double gray_prob[256] = { 0 };  //记录灰度分布密度
double gray_distribution[256] = { 0 };  //记录累计密度
int gray_equal[256] = { 0 };  //均衡化后的灰度值
int gray_sum = 0;  //像素总数
int g_nGaussianBlurValue = 6;	//高斯滤波内核值

//灰度值归一化
Mat bgr;
//输入图像
Mat img;
//HSV图像
Mat hsv;
//色相
int hmin = 0;
int hmin_Max = 360;
int hmax = 360;
int hmax_Max = 360;
//饱和度
int smin = 0;
int smin_Max = 255;
int smax = 255;
int smax_Max = 255;
//亮度
int vmin = 106;
int vmin_Max = 255;
int vmax = 250;
int vmax_Max = 255;
//输出图像
Mat out;

void Hist_Eql(Mat image)
{
	//计算像素分布
	int NumPixel[4][256] = { 0 };
	for (int y = 0; y < image.rows; ++y)
		for (int x = 0; x < image.cols; ++x)
			for (int c = 0; c < image.channels(); c++)
				NumPixel[c][image.at<Vec3b>(y, x)[c]] += 1;

	//计算像素分布密度
	double ProbPixel[4][256] = { 0 };
	for (int i = 0; i < image.channels(); i++)
		for (int j = 0; j < 256; j++)
		{
			ProbPixel[i][j] = NumPixel[i][j] / (image.rows * image.cols * 1.0);
		}

	//计算累计直方图分布
	double CumuPixel[4][256] = { 0 };
	for (int i = 0; i < image.channels(); i++)
		for (int j = 0; j < 256; j++)
		{
			if (j == 0)
				CumuPixel[i][j] = ProbPixel[i][j];
			else
				CumuPixel[i][j] = CumuPixel[i][j - 1] + ProbPixel[i][j];
		}

	//累计分布取整
	for (int i = 0; i < image.channels(); i++)
		for (int j = 0; j < 256; j++)
		{
			CumuPixel[i][j] = (int)(CumuPixel[i][j] * 255 + 0.5);
		}

	//映射
	for (int y = 0; y < image.rows; ++y)
		for (int x = 0; x < image.cols; ++x)
			image.at<Vec3b>(y, x) = Vec3b(CumuPixel[0][image.at<Vec3b>(y, x)[0]], CumuPixel[1][image.at<Vec3b>(y, x)[1]], CumuPixel[2][image.at<Vec3b>(y, x)[2]]);
}

//回调函数
void callBack(int, void*)
{
	//输出图像分配内存
	out = Mat::zeros(img.size(), CV_32FC3);
	//掩码
	Mat mask;
	inRange(hsv, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)), Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
	//只保留
	for (int r = 0; r < bgr.rows; r++)
	{
		for (int c = 0; c < bgr.cols; c++)
		{
			if (mask.at<uchar>(r, c) == 255)
			{
				out.at<Vec3f>(r, c) = bgr.at<Vec3f>(r, c);
			}
		}
	}
	//输出图像
	imshow("inrange", out);
	//保存图像
	out.convertTo(out, CV_8UC3, 255.0, 0);
	//imwrite("HSV_inRange.jpg", out);
}

int main()
{
	Mat src = imread("lena.jpg");
	//imshow("src", src);
	// 分离多通道
	vector<Mat> rgbChannels(3);
	split(src, rgbChannels);

	// 展示单独的色彩通道
	Mat blank_ch, fin_img;
	blank_ch = Mat::zeros(Size(src.cols, src.rows), CV_8UC1);

	// R
	// G和B通道保持为零矩阵
	vector<Mat> channels_r;
	channels_r.push_back(blank_ch);
	channels_r.push_back(blank_ch);
	channels_r.push_back(rgbChannels[2]);

	/// 合并
	merge(channels_r, fin_img);
	// imshow("R", fin_img);
	Mat img_r;
	img_r = fin_img;
	Hist_Eql(img_r);
	//imshow("hist_r", img_r);

	// G
	vector<Mat> channels_g;
	channels_g.push_back(blank_ch);
	channels_g.push_back(rgbChannels[1]);
	channels_g.push_back(blank_ch);
	merge(channels_g, fin_img);
	// imshow("G", fin_img);
	Mat img_g;
	img_g = fin_img;
	Hist_Eql(img_g);
	//imshow("hist_g", img_g);

	// B
	vector<Mat> channels_b;
	channels_b.push_back(rgbChannels[0]);
	channels_b.push_back(blank_ch);
	channels_b.push_back(blank_ch);
	merge(channels_b, fin_img);
	// imshow("B", fin_img);
	Mat img_b;
	img_b = fin_img;
	Hist_Eql(img_b);
	//imshow("hist_b", img_b);

	// 合并
	vector<Mat> channels_merge;
	channels_merge.push_back(rgbChannels[0]);
	channels_merge.push_back(rgbChannels[1]);
	channels_merge.push_back(rgbChannels[2]);
	merge(channels_merge, fin_img);
	// imshow("merge", fin_img);
	imwrite("merge.png", fin_img);

	//RGB->HSV,分割
	Mat HSVImage,Image[3],guass_h, guass_s, guass_v;
	cvtColor(src, HSVImage, COLOR_BGR2HSV);
	split(HSVImage, Image);
	//imshow("HSV", HSVImage);

	//高斯
	//imshow("H", Image[0]);
	GaussianBlur(Image[0], guass_h, cv::Size(5, 5), 3, 3);
	//imshow("guass_h", guass_h);
	//imshow("S", Image[1]);
	GaussianBlur(Image[1], guass_s, cv::Size(5, 5), 3, 3);
	//imshow("guass_s", guass_s);
	//imshow("V", Image[2]);
	GaussianBlur(Image[2], guass_v, cv::Size(5, 5), 3, 3);
	//imshow("guass_v", guass_v);

	//通道合并
	Mat newChannels[3] = { guass_h, guass_s, guass_v };
	Mat mergedHSV;
	merge(newChannels, 3, mergedHSV);
	//imshow("mergedHSV", mergedHSV);
	imwrite("mergedHSV.png", mergedHSV);

	// 第三问 车牌区域提取
	Mat xdst, ydst, dst_sobel, grayImage;
	img = imread("plate.png");
	imshow("plate", img);
	cvtColor(img, grayImage, CV_BGR2GRAY);
	//imshow("灰度图", grayImage);
	Sobel(grayImage, xdst, -1, 1, 0);
	//imshow("xdst", xdst);
	Sobel(grayImage, ydst, -1, 0, 1);
	//imshow("ydst", ydst);
	addWeighted(xdst, 0.5, ydst, 0.5, 1, dst_sobel);
	imshow("sobel", dst_sobel);


	Mat carImage[3];
	//彩色图像的灰度值归一化
	img.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);
	cvtColor(bgr, hsv, COLOR_BGR2HSV);
	split(hsv, carImage);
	imshow("HSV", hsv);

	//imshow("car_h", carImage[0]);
	//imshow("car_s", carImage[1]);
	//imshow("car_v", carImage[2]);
	
	//定义输出图像的显示窗口
	namedWindow("inrange", WINDOW_GUI_EXPANDED);
	//调节色相 H
	createTrackbar("hmin", "inrange", &hmin, hmin_Max, callBack);
	createTrackbar("hmax", "inrange", &hmax, hmax_Max, callBack);
	//调节饱和度 S
	createTrackbar("smin", "inrange", &smin, smin_Max, callBack);
	createTrackbar("smax", "inrange", &smax, smax_Max, callBack);
	//调节亮度 V
	createTrackbar("vmin", "inrange", &vmin, vmin_Max, callBack);
	createTrackbar("vmax", "inrange", &vmax, vmax_Max, callBack);
	callBack(0, 0);

	imshow("bw_blue", img);
	waitKey(0);
	return 0;
}