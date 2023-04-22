#pragma once

#include <string>
#include <Windows.h>
using namespace std;

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4);

class bitmap_image
{
protected:
    string bitmap_image_name;                  //图像名字
    BITMAPINFOHEADER bitmap_image_message;     //图像数据信息
    int bitmap_image_width;                    //图像宽度
    int bitmap_image_height;                   //图像高度
    int bitmap_image_len_width;                //位图数据每行字节数
    short bitmap_image_type;                   //图像类型
    int bitmap_image_color_num;                //颜色位数
    RGBQUAD* pcolortable;                      //颜色表
    unsigned char* bitmap_image_data;          //位图数据

    void initial_data();                        //初始化图像参数信息

public:
    int width() const;	            //返回图片的宽度
    int height() const;             //返回图片的高度,位图信息列数
    int len_width() const;          //返回位图信息列数
    unsigned char* image_data();   //返回位图数据
    ~bitmap_image();
    bool read_image(string image_name);          //读取图像数据信息
    bool preserve_image(string image_name, unsigned char* image_data); //存储图像信息
};
