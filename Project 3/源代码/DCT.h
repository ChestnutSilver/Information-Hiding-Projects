#pragma once

#define PI    3.1415926
#define NUM   8 //DCT�����任����ά��

//���ִ���ο���https://blog.csdn.net/BigDream123/article/details/104395587
//�ѽ��ж�Դ������нϴ��޸�
class DCT {
protected:
	int modification(double a); //������������DCT���任������ʹ��,����ͼ������ֵ��ָ����Χ�ڣ������������
public:
	void dc_transform(double(*input)[NUM], double(*output)[NUM]);//DCT���任
	void Inverse_dc_transform(double(*input)[NUM], double(*output)[NUM]);//DCT���任
};
