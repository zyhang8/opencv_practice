
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
	return output;
}

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
	Mat img_r;
	img_r = fin_img;
	Hist_Eql(img_r);
	imshow("hist_r", img_r);

	// G
	vector<Mat> channels_g;
	channels_g.push_back(blank_ch);
	channels_g.push_back(rgbChannels[1]);
	channels_g.push_back(blank_ch);
	merge(channels_g, fin_img);
	imshow("G", fin_img);
	Mat img_g;
	img_g = fin_img;
	Hist_Eql(img_g);
	imshow("hist_g", img_g);

	// B
	vector<Mat> channels_b;
	channels_b.push_back(rgbChannels[0]);
	channels_b.push_back(blank_ch);
	channels_b.push_back(blank_ch);
	merge(channels_b, fin_img);
	imshow("B", fin_img);
	Mat img_b;
	img_b = fin_img;
	Hist_Eql(img_b);
	imshow("hist_b", img_b);

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