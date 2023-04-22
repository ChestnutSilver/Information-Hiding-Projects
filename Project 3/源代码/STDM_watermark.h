#pragma once
#include<vector>
#include<string>
#include"bitmap_image.h"
#include"DCT.h"
using namespace std;

#define N   8

class STDM_watermark {
protected:
	double delta;                     //��������
	string name_image;                //Ƕ��ͼ���ļ���
	string name_mark;                 //Ƕ��ˮӡ�ļ���
	bitmap_image image;               //Ƕ��ͼ���ļ�
	bitmap_image mark;                //Ƕ��ˮӡ�ļ�
	bitmap_image de_image;            //��Ҫ����ͼ���ļ�
	int image_width;                  //ͼ�����ݿ��
	int image_height;                 //ͼ�����ݸ߶�
	int mark_width;                   //ˮӡ���ݿ��
	int mark_height;                  //ˮӡ���ݸ߶�
	vector<double>image_data;         //ԭʼͼ������
	vector<double>image_embed;        //Ƕ����Ƶˮӡ��ͼ������
	vector<double>decode_image;       //��Ҫ����ͼ����Ϣ
	vector<int>mark_image;            //ԭʼǶ��ˮӡ������Ϣ
	vector<int>mark_decode;           //��Ƕ��ͼ��������ˮӡ������Ϣ

	void get_dct_data(double in_data[8][8], double data[34]);                  //��ȡDCT��Ƶϵ������ҪǶ��ˮӡ������
	void preserver_dct_data(double in_data[34], double data[8][8]);            //����Ƕ��ˮӡ�����ݵ�ͼ��λͼ������
	void embed_8x8_image(double in_data[8][8], double out_data[8][8], int b);  //��8x8ͼ������ˮӡǶ��
	int decode_8x8_image(double data[8][8]);                                //��8x8ͼ������ˮӡ����
	double count_error();                                                      //ͳ��ˮӡ���������
	unsigned char* transform_bit_to_byte();                                    //��ˮӡ�洢����תΪbmp�洢��ʽ
public:
	STDM_watermark(string n_image, string n_mark);
	void embed_watermasking();                  //ˮӡǶ��
	void decode_watermasking();                 //ˮӡ����
	void watermask_embed_and_decode();          //ˮӡǶ�뼰���벢ͳ�ƽ��������
};

//w����
const int w[34] = { -1,1,-1,1,1,-1,1,-1,1,-1,
					1,-1,-1,1,-1,1,-1,1,-1,1,
					-1,1,-1,1,1,-1,1,-1,1,-1,
					1,-1,1,-1 };