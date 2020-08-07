#include <cassert>
#include <iostream>
#include <stdio.h>
#include <hls_opencv.h>

#include "ImgPrejection.h"

#define INPUT_IMG "E:/test1.jpg"
#define OUTPUT_IMG "E:/testout.jpg"

#define W 960//512//640//1920
#define H 540//512//480//1080
#define CH 1//3



#define PACKETS (W*CH*H/4)
unsigned char input_buffer[W*H];
wide_stream input_data[PACKETS];
wide_stream output_data[PACKETS];
unsigned char output_buffer[W*H] = {0};

void reset_data(void) {
	for (int i = 0; i < PACKETS; ++i) {
		output_data[i].data = 0;
		output_data[i].last = 0;
		output_data[i].user = 0;
	}
}


using namespace std;
using namespace cv;

int main()
{

	ap_uint<12> x[4]={391,505,836,637};
	ap_uint<12> y[4]={302,526,273,34};

	cv::Mat imgSrc;
	imgSrc = cv::imread(INPUT_IMG,CV_8UC1);
	printf("r %d, c %d, d %d\n",imgSrc.rows,imgSrc.cols,imgSrc.dims);

	uint8_t *myData = imgSrc.data;
	int width = imgSrc.cols;
	int height = imgSrc.rows;
	int _stride = imgSrc.step;//in case cols != strides
	for(int i = 0; i < height; i++)
	{
	    for(int j = 0; j < width; j++)
	    {
	        input_buffer[i * _stride + j] = myData[ i * _stride + j];
	    }
	}

	for (int i = 0; i < PACKETS; ++i) {
		input_data[i].data.range(7,0) = input_buffer[4*i];
		input_data[i].data.range(15,8) = input_buffer[4*i + 1];
		input_data[i].data.range(23,16) = input_buffer[4*i + 2];
		input_data[i].data.range(31,24) = input_buffer[4*i + 3];
	}

	input_data[0].user = 1;
	input_data[PACKETS-1].last = 1;

	reset_data();
	int8_t k0[4] = {-1, 0, 1, 0};
	int8_t k1[4] = {-2, 0, 2, 0};
	int8_t k2[4] = {-1, 0, 1, 0};
	ap_uint<32> r1 = *((unsigned long *)k0);
	ap_uint<32> r2 = *((unsigned long *)k1);
	ap_uint<32> r3 = *((unsigned long *)k2);
	int th1 = 80;
	int th2 = 80;
	ImgPrejection(input_data, output_data,x[0],x[1],x[2],x[3],y[0],y[1],y[2],y[3]);
	printf("Evaluate results\n");

	for (int i = 0; i < PACKETS; ++i) {
		output_buffer[4*i] = output_data[i].data.range(7,0);
		output_buffer[4*i + 1] = output_data[i].data.range(15,8);
		output_buffer[4*i + 2] = output_data[i].data.range(23,16);
		output_buffer[4*i + 3] = output_data[i].data.range(31,24);
	}

	cv::Mat imgHWcalc = cv::Mat(imgSrc.rows,imgSrc.cols,CV_8UC1);
	myData = imgHWcalc.data;
	width = imgHWcalc.cols;
	height = imgHWcalc.rows;
	_stride = imgHWcalc.step;//in case cols != strides
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			myData[ i * _stride + j] = output_buffer[i * _stride + j];
		}
	}

	cv::imwrite(OUTPUT_IMG, imgHWcalc);

}
