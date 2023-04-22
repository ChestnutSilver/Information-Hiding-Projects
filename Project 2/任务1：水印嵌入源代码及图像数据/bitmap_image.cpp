#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include "bitmap_image.h"


bitmap_image::~bitmap_image()
{
	if (bitmap_image_type < 24)
		delete[]pcolortable;
	delete[]bitmap_image_data;
}
//��ȡͼ��������Ϣ
bool bitmap_image::read_image(string image_name)
{
	ifstream infile;
	bitmap_image_name = image_name;
	infile.open(image_name, ios::in | ios::binary);
	if (infile.is_open() == 0) {
		cout << "bmp�ļ���ʧ��" << endl;
		return FALSE;
	}

	//��ȡͼ���ļ�������Ϣ
	infile.seekg(sizeof(BITMAPFILEHEADER), ios::beg);
	infile.read((char*)(&bitmap_image_message), sizeof(BITMAPINFOHEADER));
	if (infile.gcount() != sizeof(BITMAPINFOHEADER)) {
		infile.close();
		return FALSE;
	}
	initial_data();

	//��ȡͼ����ɫ��
	if (bitmap_image_type < 24) {
		infile.read((char*)(pcolortable), sizeof(RGBQUAD) * (long long)(bitmap_image_color_num));
		if (infile.gcount() != sizeof(RGBQUAD) * (long long)(bitmap_image_color_num)) {
			infile.close();
			return FALSE;
		}
	}

	//��ȡλͼ����
	infile.read((char*)(bitmap_image_data), (long long)(bitmap_image_height)*bitmap_image_len_width);
	if (infile.gcount() != (long long)(bitmap_image_height)*bitmap_image_len_width) {
		infile.close();
		return FALSE;
	}
	
	infile.close();
	return TRUE;
}
//�洢ͼ����Ϣ
bool bitmap_image::preserve_image(string image_name, unsigned char* image_data)
{
	ifstream infile;
	ofstream outfile;
	infile.open(bitmap_image_name, ios::in | ios::binary);
	if (infile.is_open() == 0) 
		return FALSE;
	outfile.open(image_name, ios::out | ios::binary);
	if (outfile.is_open() == 0)
		return FALSE;
	
	//д��ͼ���ļ�������Ϣ
	BITMAPFILEHEADER bitmap_image_header;
	infile.read((char*)(&bitmap_image_header), sizeof(BITMAPFILEHEADER));
	infile.close();
	outfile.write((const char*)(&bitmap_image_header), sizeof(BITMAPFILEHEADER));
	outfile.write((const char*)(&bitmap_image_message), sizeof(BITMAPINFOHEADER));

	//д��ͼ����ɫ��
	if (bitmap_image_type < 24) 
		outfile.write((char*)(pcolortable), sizeof(RGBQUAD) * (long long)(bitmap_image_color_num));

	//д��λͼ����
	outfile.write((char*)(image_data), (long long)(bitmap_image_height)*bitmap_image_len_width);
	outfile.close();
	return TRUE;
}
//��ʼ��ͼ�������Ϣ
void bitmap_image::initial_data()
{
	bitmap_image_width = bitmap_image_message.biWidth;
	bitmap_image_height = bitmap_image_message.biHeight;
	bitmap_image_type = bitmap_image_message.biBitCount;
	bitmap_image_len_width = WIDTHBYTES(bitmap_image_width * bitmap_image_type);
	if (bitmap_image_type < 24) {
		bitmap_image_color_num = (int)pow(2, bitmap_image_type);
		pcolortable = new(nothrow)RGBQUAD[bitmap_image_color_num];
		if (pcolortable == NULL) {
			cout << "����ռ�ʧ��" << endl;
			return;
		}
		memset(pcolortable, 0, sizeof(RGBQUAD) * bitmap_image_color_num);
	}
	else
		pcolortable = NULL;
	bitmap_image_data = new(nothrow)unsigned char[bitmap_image_height * bitmap_image_len_width];
	if (bitmap_image_data == NULL) {
		cout << "����ռ�ʧ��" << endl;
		return;
	}
	memset(bitmap_image_data, 0, bitmap_image_height * bitmap_image_len_width);
}
//����ͼƬ�Ŀ��
int bitmap_image::width() const	//����ͼƬ�Ŀ��
{
	return bitmap_image_width;
}
//����ͼƬ�ĸ߶ȣ�λͼ��Ϣ����
int bitmap_image::height() const //����ͼƬ�ĸ߶�
{
	return bitmap_image_height;
}
//����λͼ��Ϣ����
int bitmap_image::len_width() const
{
	return bitmap_image_len_width;
}
//����λͼ����
unsigned char* bitmap_image::image_data()
{
	return bitmap_image_data;
}
