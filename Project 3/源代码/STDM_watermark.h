#pragma once
#include<vector>
#include<string>
#include"bitmap_image.h"
#include"DCT.h"
using namespace std;

#define N   8

class STDM_watermark {
protected:
	double delta;                     //量化步长
	string name_image;                //嵌入图像文件名
	string name_mark;                 //嵌入水印文件名
	bitmap_image image;               //嵌入图像文件
	bitmap_image mark;                //嵌入水印文件
	bitmap_image de_image;            //需要解码图像文件
	int image_width;                  //图像数据宽度
	int image_height;                 //图像数据高度
	int mark_width;                   //水印数据宽度
	int mark_height;                  //水印数据高度
	vector<double>image_data;         //原始图像数据
	vector<double>image_embed;        //嵌入扩频水印后图像数据
	vector<double>decode_image;       //需要解码图像信息
	vector<int>mark_image;            //原始嵌入水印数据信息
	vector<int>mark_decode;           //对嵌入图像解码后获得水印数据信息

	void get_dct_data(double in_data[8][8], double data[34]);                  //获取DCT中频系数—需要嵌入水印的数据
	void preserver_dct_data(double in_data[34], double data[8][8]);            //保存嵌入水印后数据到图像位图数据中
	void embed_8x8_image(double in_data[8][8], double out_data[8][8], int b);  //对8x8图像块进行水印嵌入
	int decode_8x8_image(double data[8][8]);                                //对8x8图像块进行水印解码
	double count_error();                                                      //统计水印解码错误率
	unsigned char* transform_bit_to_byte();                                    //将水印存储数据转为bmp存储格式
public:
	STDM_watermark(string n_image, string n_mark);
	void embed_watermasking();                  //水印嵌入
	void decode_watermasking();                 //水印解码
	void watermask_embed_and_decode();          //水印嵌入及解码并统计解码错误率
};

//w序列
const int w[34] = { -1,1,-1,1,1,-1,1,-1,1,-1,
					1,-1,-1,1,-1,1,-1,1,-1,1,
					-1,1,-1,1,1,-1,1,-1,1,-1,
					1,-1,1,-1 };