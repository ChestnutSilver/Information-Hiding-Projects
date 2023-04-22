#include<iostream>
#include"ss_watermark.h"
using namespace std;

ss_watermark::ss_watermark(string n_image, string n_mark)
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
		for (int k = N-1; k >= 0; k--)
			mark_image.push_back((data_mark[j] >> k) & 0x01);
	
}
//ˮӡǶ��
void ss_watermark::embed_watermasking()
{
	double data[8][8];
	int mark_size = mark_image.size();
	int b;
	
	for (int count_logo = 0; count_logo < mark_size; count_logo++) {
		b = mark_image[count_logo];

		for (int k = 0; k < 4; k++) {
			
			//��ȡ8x8����λͼ��Ϣ
			for (int i = 0; i < N; i++) 
				for (int j = 0; j < N; j++) 
					data[i][j] = image_data[count_logo * 256 + k * 64 + i * N + j];
			
				//8x8����鹲4096����Ƕ������1024λ��ѡ��ÿ4��8x8����Ƕ����ͬ����
				double data_enbed[8][8];
				embed_8x8_image(data, data_enbed, b);

				//����Ƕ��λͼ��Ϣ
				for (int i = 0; i < N; i++)
					for (int j = 0; j < N; j++)
						image_embed[count_logo * 256 + k * 64 + i * N + j] = data_enbed[i][j];
			
		}
	}
	return;
}
//ˮӡ����
void ss_watermark::decode_watermasking()
{
	double data[8][8];
	int mark_size = mark_image.size();
	int code;//����õ�����
	for (int count_logo = 0; count_logo < mark_size; count_logo++) {
		double temp = 0;
		for (int k = 0; k < 4; k++) {

			//��ȡ8x8����λͼ��Ϣ
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					data[i][j] = decode_image[count_logo * 256 + k * 64 + i * N + j];

			//ˮӡ����
			temp += decode_8x8_image(data);
			if (k == 3) {
				temp /= NUM_UNIT;
				if (temp < 0)
					code = 0;
				else
					code = 1;
				mark_decode.push_back(code);
			}
		}
	}
	return;
}
//ˮӡǶ�뼰���벢ͳ�ƽ��������
void ss_watermark::watermask_embed_and_decode()
{
	cout << "������Ƕ��ǿ�ȣ� ";
	cin >> strength;

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

	//��ȡδǶ��ˮӡͼ��λͼ��Ϣ
	for (int i = 0; i < image_size; i++)
		decode_image.push_back(data[i]);

	//ˮӡ���벢�������ˮӡͼ��
	decode_watermasking();
	string decede_name = "logo_decode.bmp";
	unsigned char* decode_mark =transform_bit_to_byte();
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
void ss_watermark::embed_8x8_image(double in_data[8][8], double out_data[8][8], int b)
{
	DCT transform;
	double dct[8][8];
	//dct���任
	transform.dc_transform(in_data, dct);

	//��ȡdctϵ��,K=34��Ƕ��ˮӡ
	double temp_embed;
	int count = 0;
	/* ֻ��k=34ʱ��Ҫ��ȡ
	for (int i = 0; i < 6; i++){
		temp_embed = dct[i][5 - i];
		dct[i][5 - i] = temp_embed + strength * (b ? 1 : -1) * w[count];
		count++;
	}
	*/
	//ֻ��k=34��22ʱ��Ҫ��ȡ
	for (int i = 0; i < 7; i++){
		temp_embed = dct[i][6 - i];
		dct[i][6 - i] = temp_embed + strength * (b ? 1 : -1) * w[count];
		count++;
	}
	//k=8,22,34����Ҫ
	for (int i = 0; i < 8; i++){
		temp_embed = dct[i][7 - i];
		dct[i][7 - i] = temp_embed + strength * (b ? 1 : -1) * w[count];
		count++;
	}
	//ֻ��k=34��22ʱ��Ҫ��ȡ
	for (int i = 1; i < 8; i++){
		temp_embed = dct[i][8 - i];
		dct[i][8 - i] = temp_embed + strength * (b ? 1 : -1) * w[count];
		count++;
	}
	/* ֻ��k=34ʱ��Ҫ��ȡ
	for (int i = 2; i < 8; i++){
		temp_embed = dct[i][9 - i];
		dct[i][9 - i] = temp_embed + strength * (b ? 1 : -1) * w[count];
		count++;
	}
	*/
	//DCT���任
	transform.Inverse_dc_transform(dct, out_data);

	return;
}

//��8x8ͼ������ˮӡ����
double ss_watermark::decode_8x8_image(double data[8][8])
{
	DCT transform;
	double de_data[8][8];
	double test = 0;
	int count = 0;
	transform.dc_transform(data, de_data);

	//��DCT��Ƶϵ������

	/*  ֻ��k=34ʱ��Ҫ��ȡ
	for (int i = 0; i < 6; i++){
		test += de_data[i][5 - i] * w[count];
		count++;
	}
    */
	//ֻ��k=34��22ʱ��Ҫ��ȡ
	for (int i = 0; i < 7; i++){
		test += de_data[i][6 - i] * w[count];
		count++;
	}
	//k=8,22,34����Ҫ
	for (int i = 0; i < 8; i++){
		test += de_data[i][7 - i] * w[count];
		count++;
	}
	//ֻ��k=34��22ʱ��Ҫ��ȡ
	for (int i = 1; i < 8; i++){
		test += de_data[i][8 - i] * w[count];
		count++;
	}
	/*  ֻ��k=34ʱ��Ҫ��ȡ
	for (int i = 2; i < 8; i++){
		test += de_data[i][9 - i] * w[count];
		count++;
	}
	*/
	return test;
}
//ͳ��ˮӡ���������
double ss_watermark::count_error()
{
	int count = 0;
	int size = mark_image.size();
	for (int i = 0; i < size; i++)
		if (mark_image[i] != mark_decode[i])
			count++;
	cout << "���������Ϊ��" << (count / (double)size) << endl;
	return 0.0;
}
unsigned char* ss_watermark::transform_bit_to_byte()
{
	unsigned char*decode_mark = new(nothrow)unsigned char[mark_height * mark_width];
	if (decode_mark == NULL) {
		cout << "����ռ�ʧ��" << endl;
		return NULL;
	}
	int size = mark_height * mark_width;
	for (int i = 0; i < size; i++){
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
