
#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main()
{
	Mat src = imread("lena.jpg");
	imshow("src", src);

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
	imshow("R", fin_img);


	// G
	vector<Mat> channels_g;
	channels_g.push_back(blank_ch);
	channels_g.push_back(rgbChannels[1]);
	channels_g.push_back(blank_ch);
	merge(channels_g, fin_img);
	imshow("G", fin_img);


	// B
	vector<Mat> channels_b;
	channels_b.push_back(rgbChannels[0]);
	channels_b.push_back(blank_ch);
	channels_b.push_back(blank_ch);
	merge(channels_b, fin_img);
	imshow("B", fin_img);

	// 合并
	vector<Mat> channels_merge;
	channels_merge.push_back(rgbChannels[0]);
	channels_merge.push_back(rgbChannels[1]);
	channels_merge.push_back(rgbChannels[2]);
	merge(channels_merge, fin_img);
	imshow("Merge", fin_img);

	waitKey(0);
	return 0;
}