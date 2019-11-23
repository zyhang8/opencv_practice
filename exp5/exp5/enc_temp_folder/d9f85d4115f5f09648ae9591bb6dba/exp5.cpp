
#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>


using namespace std;
using namespace cv;


int main()
{
	IplImage* pSrcImage = cvLoadImage("lena.jpg");
	IplImage* pR_Plane = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	IplImage* pG_Plane = cvCloneImage(pR_Plane);
	IplImage* pB_Plane = cvCloneImage(pR_Plane);

	cvSplit(pSrcImage, pB_Plane, pG_Plane, pR_Plane, NULL);	//这个函数执行后的结果是单通道图片，显示出来是灰度图。NND，需要重新Merge下。

	IplImage* pR_Channel = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 3);
	cvZero(pR_Channel);	//如果没有这一句，你可以想象结果是什么样子，创建的图像中数据没有初始化，Merge的时候会乱的。
	IplImage* pG_Channel = cvCloneImage(pR_Channel);
	IplImage* pB_Channel = cvCloneImage(pR_Channel);

	cvMerge(NULL, NULL, pR_Plane, NULL, pR_Channel);
	cvMerge(NULL, pG_Plane, NULL, NULL, pG_Channel);
	cvMerge(pB_Plane, NULL, NULL, NULL, pB_Channel);

	cvShowImage("Source", pSrcImage);
	cvShowImage("R Channel", pR_Channel);
	cvShowImage("G Channel", pG_Channel);
	cvShowImage("B Channel", pB_Channel);
	cvWaitKey();
	return 0;
}