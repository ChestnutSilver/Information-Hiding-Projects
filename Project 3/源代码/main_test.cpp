#include<iostream>
#include"STDM_watermark.h"
using namespace std;


int main()
{
	//������Ƶˮӡ��Ƕ�������
	string image_name = "LENA.BMP";
	string logo_name = "LOGO.bmp";
	STDM_watermark test(image_name, logo_name);
	test.watermask_embed_and_decode();
	return 0;
}