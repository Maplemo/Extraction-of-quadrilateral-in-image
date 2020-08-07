/*
图像投影变换HLS实现
https://github.com/Maplemo/Extraction-of-quadrilateral-in-image
KK 2020.7.29
*/
#include "ImgPrejection.h"

//计算行列式的值
double det(double a[8][8], int n)
{
	double result = 1;
	int i, j;
	double m;                          //m为中间变量
	int max_a_line = 0;               //max_a_line为最大元素所在的行
	double max_a = 0;            //max_a为最大元素的值
	int out_i = 0;                      //out_i为i的初始值
	det_loop:
	for (i = 0; i < n; i++)
	{
		#pragma HLS loop_tripcount min=7 max=8
		max_a_line = i;               //max_a_line为最大元素所在的行
		max_a = a[i][i];            //max_a为最大元素的值
		out_i = i;                      //out_i为i的初始值
		find_line_loop:				//找出列为i的最大主元素所在的行
		for (int t = i + 1; t < n; t++)
		{
			#pragma HLS loop_tripcount min=1 max=8
			if (a[t][out_i] < 0)
				if (max_a < -a[t][out_i])
				{
					max_a = -a[t][out_i];
					max_a_line = t;
				};
			if (max_a < a[t][out_i])
			{
				max_a = a[t][out_i];
				max_a_line = t;
			};
		}
		j=max_a_line;

		if (i != j)
		{//交换两行的从line列到n列的元素
			chang_line_loop:
			for (int k = i; k < n; k++)
			{
				#pragma HLS loop_tripcount min=1 max=8
				m = a[i][k];
				a[i][k] = a[j][k];
				a[j][k] = m;
			}
			result = -result;
		};
		if (a[i][i] == 0)
			return 0;
		result = result * a[i][i];
		if (i < n - 1)
		{//消去第i列、i行以下的元素
			det_label9:for (int v = i + 1; v < n; v++)
			{
				#pragma HLS loop_tripcount min=1 max=8
				m = -a[v][i] / a[i][i];
				clear_num_inLoop:
				for (int b = i; b < n; b++)
				{
					#pragma HLS loop_tripcount min=1 max=8
					a[v][b] = a[v][b] + m * a[i][b];
				};
			}
		}
	}
	return result;
}

//利用行列式求转换矩阵的方程组
void GetMatrix(ap_uint<32> x[4], ap_uint<32> y[4], ap_uint<32> h, ap_uint<32> w,double Mat_Trans[8])
{
	double Mat_Val[8] = { 0 }, Mat_Coef[8][8] = { 0 };
	double a[8][8], Mat_TEMP[8][8], A[8][8], Det_val;
	double TempResult = 0;

	/*初始化转换矩阵方程组的系数矩阵*/
	Mat_Coef[0][0] = x[0]; Mat_Coef[0][3] = y[0]; Mat_Coef[0][6] = 1;
	Mat_Coef[1][1] = x[0]; Mat_Coef[1][4] = y[0]; Mat_Coef[1][7] = 1;
	Mat_Coef[2][0] = x[1]; Mat_Coef[2][3] = y[1]; Mat_Coef[2][6] = 1;
	Mat_Coef[3][1] = x[1]; Mat_Coef[3][2] = x[1] * (double)(-h); Mat_Coef[3][4] = y[1]; Mat_Coef[3][5] = y[1] * (double)(-h); Mat_Coef[3][7] = 1;
	Mat_Coef[4][0] = x[2]; Mat_Coef[4][2] = x[2] * (double)(-w); Mat_Coef[4][3] = y[2]; Mat_Coef[4][5] = y[2] * (double)(-w); Mat_Coef[4][6] = 1;
	Mat_Coef[5][1] = x[2]; Mat_Coef[5][2] = x[2] * (double)(-h); Mat_Coef[5][4] = y[2]; Mat_Coef[5][5] = y[2] * (double)(-h); Mat_Coef[5][7] = 1;
	Mat_Coef[6][0] = x[3]; Mat_Coef[6][2] = x[3] * (double)(-w); Mat_Coef[6][3] = y[3]; Mat_Coef[6][5] = y[3] * (double)(-w); Mat_Coef[6][6] = 1;
	Mat_Coef[7][1] = x[3]; Mat_Coef[7][4] = y[3]; Mat_Coef[7][7] = 1;

	Mat_Val[3] = h; Mat_Val[4] = w; Mat_Val[5] = h; Mat_Val[6] = w;//等式右边

	GetMatrix_label0:for (int i = 0; i < 8; i++)
	{
		GetMatrix_label8:for (int j = 0; j < 8; j++)
		{
			a[i][j] = Mat_Coef[i][j];
		}
	}

	Det_val = det(a, 8);//求行列式
	GetMatrix_label1:for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			GetMatrix_label10:for (int b = 0; b < 8; b++)
			{
				GetMatrix_loop2:
				for (int c = 0; c < 8; c++)
				{
					a[b][c] = Mat_Coef[b][c];
				}
			}
			GetMatrix_label11:for (int b = 0; b < 8 - 1; b++)
			{
				GetMatrix_loop3:
				for (int c = 0; c < 8 - 1; c++)
				{
					if (b < i && c < j) Mat_TEMP[b][c] = a[b][c];
					else if (b < i && c >= j) Mat_TEMP[b][c] = a[b][c + 1];
					else if (b >= i && c < j) Mat_TEMP[b][c] = a[b + 1][c];
					else Mat_TEMP[b][c] = a[b + 1][c + 1];
				}
			}

			if ((i + j) % 2 == 0)A[i][j] = det(Mat_TEMP, 8 - 1);
			else A[i][j] = (-1) * det(Mat_TEMP, 8 - 1);
		}
	}
	GetMatrix_label2:for (int i = 0; i < 8; i++)
	{
		TempResult = 0;
		GetMatrix_loop5:
		for (int j = 0; j < 8; j++)
		{
			TempResult += (A[j][i] / Det_val) * Mat_Val[j];
		}
		Mat_Trans[i] = TempResult;
	}
}


/*
输入原始图像,需要拉伸提取的四个顶点坐标以及目标图像的尺寸
其中横坐标和纵坐标分别传递，x为横坐标值，y为纵坐标值
*/
void ImgPrejection(wide_stream* in_stream, wide_stream* out_stream,
		ap_uint<32> x0,ap_uint<32> x1,ap_uint<32> x2,ap_uint<32> x3,
		ap_uint<32> y0,ap_uint<32> y1,ap_uint<32> y2,ap_uint<32> y3)
{
	#pragma HLS INTERFACE axis port=in_stream bundle=INPUT
	#pragma HLS INTERFACE axis port=out_stream bundle=OUTPUT

	#pragma HLS INTERFACE s_axilite port=x0 bundle=CONTROL_BUS offset=0x14 clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=x1 bundle=CONTROL_BUS offset=0x1C clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=x2 bundle=CONTROL_BUS offset=0x24 clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=x3 bundle=CONTROL_BUS offset=0x2C clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=y0 bundle=CONTROL_BUS offset=0x34 clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=y1 bundle=CONTROL_BUS offset=0x3C clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=y2 bundle=CONTROL_BUS offset=0x44 clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=y3 bundle=CONTROL_BUS offset=0x4C clock=AXI_LITE_clk
	#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS clock=AXI_LITE_clk

	#pragma HLS INTERFACE ap_stable port=x0
	#pragma HLS INTERFACE ap_stable port=x1
	#pragma HLS INTERFACE ap_stable port=x2
	#pragma HLS INTERFACE ap_stable port=x3
	#pragma HLS INTERFACE ap_stable port=y0
	#pragma HLS INTERFACE ap_stable port=y1
	#pragma HLS INTERFACE ap_stable port=y2
	#pragma HLS INTERFACE ap_stable port=y3

	double M_Trans[8]={0};
	ap_uint<32> hight=540,width=960;
	ap_uint<32> x[4],y[4];
	x[0]=x0;x[1]=x1;x[2]=x2;x[3]=x3;
	y[0]=y0;y[1]=y1;y[2]=y2;y[3]=y3;
	GetMatrix(x,y,hight,width,M_Trans);
	int u = 0, v = 0;//变换后的坐标
	RGB_IMAGE img(540, 960);//输入图像
	RGB_IMAGE imgOut(540, 960);//输出图像
	RGB_PIXEL in_pixel;
	RGB_PIXEL out_pixel;
	ap_uint<8> B[960][540];

	const int col_packets = 960/4;
	const int packets = col_packets*540;
	ImgProjection_label3:for(int r = 0; r < packets; r++){
#pragma HLS pipeline II=4
		ap_uint<32> dat = in_stream->data;//输入流数据赋值**
		img.write(GRAY_PIXEL(dat.range(7,0)));
		img.write(GRAY_PIXEL(dat.range(15,8)));
		img.write(GRAY_PIXEL(dat.range(23,16)));
		img.write(GRAY_PIXEL(dat.range(31,24)));
		++in_stream;
	}

	ImgProjection_label4:for(int a=0;a<540;a++)
	{
		ImgProjection_loop1:
		for(int b=0;b<960;b++)
			{
				img>>in_pixel;
				B[b][a]=in_pixel.val[0];
			}
	}
	ImgProjection_label5:for (int i = 0; i < 540; i++)
	{
		ImgProjection_loop2:
		for (int j = 0; j < 960; j++)
		{
			v = (j * (M_Trans[1] - M_Trans[2] * M_Trans[7]) + i * (M_Trans[2] * M_Trans[6] - M_Trans[0]) + M_Trans[0] * M_Trans[7] - M_Trans[1] * M_Trans[6])
				/ (j * (M_Trans[2] * M_Trans[4] - M_Trans[1] * M_Trans[5]) + i * (M_Trans[0] * M_Trans[5] - M_Trans[2] * M_Trans[3]) + M_Trans[1] * M_Trans[3] - M_Trans[0] * M_Trans[4]);

			u = ((j - M_Trans[6]) - (M_Trans[3] - M_Trans[5] * j) * v) / (M_Trans[0] - M_Trans[2] * j);

			if (u > 0 && u < 960 && v > 0 && v < 540)//点在区域内
			{
				out_pixel.val[0]=B[u][v];
				imgOut << out_pixel;
			}
		}
	}

    ImgProjection_label6:for(int r = 0; r < 540; r++){
		ImgPrejection_label7:for(int c = 0; c < col_packets; c++){
			ap_uint<32> dat;
			dat.range(7,0) = imgOut.read().val[0];
			dat.range(15,8) = imgOut.read().val[0];
			dat.range(23,16) = imgOut.read().val[0];
			dat.range(31,24) = imgOut.read().val[0];
			out_stream->data = dat;
			out_stream->user = (r == 0 && c == 0)? 1: 0;
			out_stream->last = (r == 540-1 && c == col_packets-1)? 1: 0;
			++out_stream;
		}
	}
}
