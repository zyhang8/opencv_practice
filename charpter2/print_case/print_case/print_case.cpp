// print_case.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <opencv2/opencv.hpp>
using namespace cv;

void main()
{
	int a = 66, b = 68;
	printf("\n\t%d %d\n", a, b);//输出十进制整型
	printf("\n\t%06d,%06d\n", a, b);//输出6位十进制整型
	printf("\n\t%c,%c\n", a, b);//按字符型输出
	printf("\n\t结果为：a=%d,b=%d", a, b);//可以配合其他内容一同输出
	getchar();//等待读入任意字符而结束，在此用于保持窗口显示，直到任意按键按下
}
