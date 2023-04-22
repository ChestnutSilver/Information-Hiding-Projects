#pragma once

#include <string>
#include <Windows.h>
using namespace std;

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4);

class bitmap_image
{
protected:
    string bitmap_image_name;                  //ͼ������
    BITMAPINFOHEADER bitmap_image_message;     //ͼ��������Ϣ
    int bitmap_image_width;                    //ͼ����
    int bitmap_image_height;                   //ͼ��߶�
    int bitmap_image_len_width;                //λͼ����ÿ���ֽ���
    short bitmap_image_type;                   //ͼ������
    int bitmap_image_color_num;                //��ɫλ��
    RGBQUAD* pcolortable;                      //��ɫ��
    unsigned char* bitmap_image_data;          //λͼ����

    void initial_data();                        //��ʼ��ͼ�������Ϣ

public:
    int width() const;	            //����ͼƬ�Ŀ��
    int height() const;             //����ͼƬ�ĸ߶�,λͼ��Ϣ����
    int len_width() const;          //����λͼ��Ϣ����
    unsigned char* image_data();   //����λͼ����
    ~bitmap_image();
    bool read_image(string image_name);          //��ȡͼ��������Ϣ
    bool preserve_image(string image_name, unsigned char* image_data); //�洢ͼ����Ϣ
};
