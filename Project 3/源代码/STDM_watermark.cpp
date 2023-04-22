#include<iostream>
#include"STDM_watermark.h"
using namespace std;

double jpeg_attack[N][N] = { 16, 11, 10, 16, 24, 40, 51, 61,
						12, 12, 14, 19, 26, 58, 60, 55,
						14, 13, 16, 24, 40, 57, 69, 56,
						14, 17, 22, 29, 51, 87, 80, 62,
						18, 22, 37, 56, 68, 109, 103, 77,
						24, 35, 55, 64, 81, 104, 113, 92,
						49, 64, 78, 87, 103, 121, 120, 101,
						72, 92, 95, 98, 112, 100, 103, 99 };

STDM_watermark::STDM_watermark(string n_image, string n_mark)
{
	//��δǶ��ˮӡͼ��
	name_image = n_image;
	if (image.read_image(name_image) == FALSE)
		return;
	image_width = image.len_width();
	image_height = image.height();
	int image_size = image_width * image_height;
	unsigned char* data = image.image_data();

	//��ȡδǶ��ˮӡͼ��λͼ��Ϣ
	for (int i = 0; i < image_size; i++) {
		image_data.push_back(data[i]);
		image_embed.push_back(data[i]);
	}

	//��Ƕ��ˮӡͼ��
	name_mark = n_mark;
	if (mark.read_image(name_mark) == FALSE)
		return;
	mark_width = mark.len_width();
	mark_height = mark.height();
	int mark_size = mark_width * mark_height;
	unsigned char* data_mark = mark.image_data();

	//��ȡˮӡͼ��λͼ��Ϣ
	for (int j = 0; j < mark_size; j++)
		for (int k = N - 1; k >= 0; k--)
			mark_image.push_back((data_mark[j] >> k) & 0x01);
}

//ˮӡǶ��
void STDM_watermark::embed_watermasking()
{
	double data[8][8];
	int mark_size = mark_image.size();
	int b;

	for (int count_logo = 0; count_logo < mark_size; count_logo++) {
		b = mark_image[count_logo];

		//��ȡ8x8����λͼ��Ϣ
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				data[i][j] = image_data[count_logo * 64 + i * N + j];

		//8x8����鹲4096����Ƕ�����ݹ�4096λ��ѡ��ÿ1��8x8����Ƕ�벻ͬ����
		double data_enbed[8][8];
		embed_8x8_image(data, data_enbed, b);

		//����Ƕ��λͼ��Ϣ
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				image_embed[count_logo * 64 + i * N + j] = data_enbed[i][j];
	}
	return;
}

//ˮӡ����
void STDM_watermark::decode_watermasking()
{
	double data[8][8];
	int mark_size = mark_image.size();
	int code = 0;//����õ�����
	for (int count_logo = 0; count_logo < mark_size; count_logo++) {

		//��ȡ8x8����λͼ��Ϣ
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				data[i][j] = decode_image[count_logo * 64 + i * N + j];

		//ˮӡ����
		code = decode_8x8_image(data);
		mark_decode.push_back(code);
	}
	return;
}

//ˮӡǶ�뼰���벢ͳ�ƽ��������
void STDM_watermark::watermask_embed_and_decode()
{
	cout << "���������������� ";
	cin >> delta;

	//Ƕ��ˮӡ������ͼƬ
	embed_watermasking();

	string embed_name = "lena_embed.bmp";
	unsigned char* embed_data = new(nothrow)unsigned char[image_height * image_width];
	if (embed_data == NULL) {
		cout << "����ռ�ʧ��" << endl;
		return;
	}
	for (int i = 0; i < image_height * image_width; i++)
		embed_data[i] = image_embed[i];
	if (image.preserve_image(embed_name, embed_data))
		cout << "����ɹ�����Ƕ��ˮӡ��ͼ�񱣴���lena_embed.bmp�ļ���" << endl << endl;
	else {
		cout << "����ʧ�ܣ���Ƕ��ˮӡ��ͼ�񱣴�ʧ��" << endl << endl;
		return;
	}
	delete[]embed_data;

	//��ȡ��Ҫˮӡ������Ϣ
	cout << "��������Ҫ�����ͼƬ·��(�ܵ�������):";
	string path;
	cin >> path;
	if (de_image.read_image(path) == FALSE)
		return;
	image_width = de_image.len_width();
	image_height = de_image.height();
	int image_size = image_width * image_height;
	unsigned char* data = de_image.image_data();

	//��ȡ��Ҫˮӡ����ͼ��λͼ��Ϣ
	for (int i = 0; i < image_size; i++)
		decode_image.push_back(data[i]);

	//ˮӡ���벢�������ˮӡͼ��
	decode_watermasking();
	string decede_name = "logo_decode.bmp";
	unsigned char* decode_mark = transform_bit_to_byte();
	if (decode_mark == NULL)
		return;

	if (mark.preserve_image(decede_name, decode_mark))
		cout << "�����ˮӡͼ�񱣴���logo_decode.bmp�ļ���" << endl << endl;
	else {
		cout << "����ʧ�ܣ������ˮӡͼ�񱣴�ʧ��" << endl << endl;
		return;
	}
	delete[]decode_mark;
	//���������
	count_error();
}

//��8x8ͼ������ˮӡǶ��
void STDM_watermark::embed_8x8_image(double in_data[8][8], double out_data[8][8], int b)
{
	DCT transform;
	double dct[8][8];

	//dct���任
	transform.dc_transform(in_data, dct);

	//��ȡdctϵ��,K=34
	double embed_x[34];
	get_dct_data(dct, embed_x);

	//����Ƕ��ǿ��
	double avg_x = 0;
	for (int i = 0; i < 34; i++)
		avg_x += embed_x[i] * w[i];
	avg_x /= 34;
	double qb_x = 0;
	//�趨d[0]=-delta/4;d[1]=delta/4;
	if (b == 1)
		qb_x = delta * int((avg_x - (delta / 4)) / delta + 0.5) + delta / 4;
	else
		qb_x = delta * int((avg_x + (delta / 4)) / delta + 0.5) - delta / 4;

	//Ƕ��ˮӡ�����浽8x8������
	for (int i = 0; i < 34; i++)
		embed_x[i] = embed_x[i] + (qb_x - avg_x) * w[i];
	preserver_dct_data(embed_x, dct);
	

	//��ԭ����dct������һ�����ڼ���
	double new_dct[N][N];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			new_dct[i][j] = dct[i][j];
		}
	}

	//��dct���� = ԭ����dct���� / jpeg������ * ѹ��ϵ��
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			dct[i][j] = int(new_dct[i][j] / (jpeg_attack[i][j] * 0.9));
			dct[i][j] = dct[i][j] * jpeg_attack[i][j] * 0.9;
		}
	}

	//DCT���任
	transform.Inverse_dc_transform(dct, out_data);

	return;
}

//��8x8ͼ������ˮӡ����
int STDM_watermark::decode_8x8_image(double data[8][8])
{
	DCT transform;
	double de_data[8][8];
	transform.dc_transform(data, de_data);

	//��ȡDCT��Ƶϵ��
	double y[34];
	get_dct_data(de_data, y);

	//��DCT��Ƶϵ������
	double avg_y = 0;
	for (int i = 0; i < 34; i++)
		avg_y += y[i] * w[i];
	avg_y /= 34;
	double result = int((avg_y + delta / 4) / delta + 0.5) * delta - delta / 4;
	if (fabs(result - avg_y) < delta / 4)
		return 0;
	else
		return 1;
}

//��ȡ��ҪǶ��ˮӡ������
void STDM_watermark::get_dct_data(double in_data[8][8], double data[34])
{
	int count = 0;

	for (int i = 0; i < 6; i++) {
		data[count] = in_data[i][5 - i];
		count++;
	}

	for (int i = 6; i >= 0; i--) {
		data[count] = in_data[i][6 - i];
		count++;
	}

	for (int i = 0; i < 8; i++) {
		data[count] = in_data[i][7 - i];
		count++;
	}

	for (int i = 7; i >= 1; i--) {
		data[count] = in_data[i][8 - i];
		count++;
	}

	for (int i = 2; i < 8; i++) {
		data[count] = in_data[i][9 - i];
		count++;
	}

	return;
}

//����Ƕ��ˮӡ�����ݵ�ͼ��λͼ������
void STDM_watermark::preserver_dct_data(double in_data[34], double data[8][8])
{
	int count = 0;

	for (int i = 0; i < 6; i++) {
		data[i][5 - i] = in_data[count];
		count++;
	}

	for (int i = 6; i >= 0; i--) {
		data[i][6 - i] = in_data[count];
		count++;
	}

	for (int i = 0; i < 8; i++) {
		data[i][7 - i] = in_data[count];
		count++;
	}

	for (int i = 7; i >= 1; i--) {
		data[i][8 - i] = in_data[count];
		count++;
	}

	for (int i = 2; i < 8; i++) {
		data[i][9 - i] = in_data[count];
		count++;
	}

	return;
}

//ͳ��ˮӡ���������
double STDM_watermark::count_error()
{
	int count = 0;
	int size = mark_image.size();
	for (int i = 0; i < size; i++)
		if (mark_image[i] != mark_decode[i])
			count++;
	cout << "���������Ϊ��" << (count / (double)size) << endl;
	return 0.0;
}
unsigned char* STDM_watermark::transform_bit_to_byte()
{
	unsigned char* decode_mark = new(nothrow)unsigned char[mark_height * mark_width];
	if (decode_mark == NULL) {
		cout << "����ռ�ʧ��" << endl;
		return NULL;
	}
	int size = mark_height * mark_width;
	for (int i = 0; i < size; i++) {
		decode_mark[i] = 0;
		for (int j = 0; j < 8; j++) {
			unsigned char temp = 0;
			unsigned char temp_t;
			if (mark_decode[i * 8 + 7 - j] == 1) {
				temp_t = ((temp >> j) | 0x01) << j;
				decode_mark[i] |= temp_t;
			}
		}
	}

	return decode_mark;
}