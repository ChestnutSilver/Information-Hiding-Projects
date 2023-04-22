#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
#include"random_Gaussian.h"
using namespace std;

// ����������Ϣ��ʼ��
random_Gaussian::random_Gaussian(int num_s, double ave, double var)
{
	num_sample = num_s;
	in_ave = ave;
	in_var = var;
	c_ave = 0;
	c_var = 0;
}

// ���ɸ�˹�ֲ�N(��,��)�����
void random_Gaussian::generate_Gaussian()
{
	double pi = 3.1415926535;
	double x, y;		// ���ȷֲ�[0,1]�����
	double g_data;		// ��˹�ֲ�N(��,��)�����
	double z;			// ��˹�ֲ�N(0,1)�����
	double temp;

	// ����[0,1]���ȷֲ����������
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	// ���ɸ�˹�ֲ�N(��,��)�����
	for (int i = 0; i < num_sample; i++) {
		x = dist(generator);
		y = dist(generator);

		// BOX-MULLER���ɸ�˹�ֲ�N(0,1)�����
		temp = 2 * pi * y;
		z = sqrt((-2) * log(1 - x)) * cos(temp);	// ��������ҵһlogx����

		// ��˹�ֲ�N(��,��)�����
		g_data = in_ave + z * in_var;
		data[i] = g_data;

		// �������Ʀ̺ͦ�
		c_ave += g_data;
		c_var += g_data * g_data;
	}

	c_ave /= num_sample;
	c_var = sqrt(c_var / num_sample - c_ave * c_ave);
}