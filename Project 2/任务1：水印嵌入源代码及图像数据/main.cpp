#include<iostream>
#include"ss_watermark.h"
using namespace std;


int main()
{
    //������Ƶˮӡ��Ƕ�������
	string image_name = "LENA.BMP";
	string logo_name = "tj-logo.bmp";
	ss_watermark test(image_name, logo_name);
	test.watermask_embed_and_decode();

    /* DCT�����任����
    DCT test_dct;
    // 8 x 8 ��ͼ������
    double input[8][8] =
    { {129, 101, 114, 125, 126, 115, 105, 96},
      {87, 115, 131, 147, 149, 155, 123, 113},
      {74, 134, 159, 178, 175, 164, 149, 137},
      {151, 143, 177, 86, 201, 189, 165, 150},
      {119, 141, 175, 201, 207, 186, 162, 144},
      {107, 130, 165, 189, 192, 171, 104, 125},
      {97, 119, 149, 171, 172, 145, 117, 96},
      {88, 107, 136, 156, 155, 129, 97, 75}
    };

    cout << "ԭʼͼ������" << endl;
    for (int u = 0; u < NUM; u++) {
        for (int v = 0; v < NUM; v++)
            cout << input[u][v] << "   ";
        cout << endl;
    }

    //DCT���任
    double output[8][8] = { 0 };
    test_dct.dc_transform(input, output);
    cout << "DCT���任:" << endl;
    for (int u = 0; u < NUM; u++) {
        for (int v = 0; v < NUM; v++)
            cout << output[u][v] << "   ";
        cout << endl;
    }

    //DCT���任
    test_dct.Inverse_dc_transform(output, input);
    cout << "DCT���任:" << endl;
    for (int i = 0; i < NUM; i++) {
        for (short j = 0; j < NUM; j++)
            cout << input[i][j] << "  ";
        cout << endl;
    }
    */
    return 0;
}
