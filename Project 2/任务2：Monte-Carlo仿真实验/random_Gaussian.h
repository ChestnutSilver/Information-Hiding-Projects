#pragma once
#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>

class random_Gaussian {
protected:
	int num_sample;		// ���ɸ�˹�ֲ����������
	double in_ave;		// �����˹�ֲ���
	double in_var;		// �����˹�ֲ���
	double c_ave;		// �������Ʀ�
	double c_var;		// �������Ʀ�
public:
	double data[1000];	// ���ɵĸ�˹�ֲ������
	random_Gaussian(int num_s, double ave, double in_var);	// ����������Ϣ��ʼ��
	void generate_Gaussian();								// ���ɸ�˹�ֲ�N(��,��)�����
};