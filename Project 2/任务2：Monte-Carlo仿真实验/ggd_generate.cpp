#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
#include"ggd_generate.h"
#include"embedding.h"
using namespace std;

random_ggd::random_ggd(int num, double c, double beta)
{
	num_s = num;
	in_c = c;
	in_beta = beta;
}

void random_ggd::generate_ggd()
{
	double x, y, z;		// ���ȷֲ�[0,1]�����
	int ber;			// Bernoulli�ֲ�[0,1]�����
	double e_data;		// ָ���ֲ��������
	double g_data;		// GGD�ֲ������

	// ����[0,1]���ȷֲ����������
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> r_dist(0.0, 1.0);
	uniform_int_distribution<> i_dist(0, 1);

	if (fabs(in_c - 1.0) < 1e-6) {	// c=1.0ʱ��GGD�ֲ�
		for (int i = 0; i < num_s; i++) {
			x = r_dist(generator);
			ber = i_dist(generator);

			// ָ���ֲ����������ע��gamma_beta��in_beta�Ĺ�ϵ
			double gamma_beta = 1.0 / pow(in_beta, in_c);

			e_data = (-gamma_beta) * log(1 - x);
			if (ber == 1) {
				g_data = e_data;
			}
			else if (ber == 0) {
				g_data = -e_data;
			}
			data[i] = g_data;
		}
	}
	else if (fabs(in_c - 0.5) < 1e-6) {	// c=0.5ʱ��GGD�ֲ�
		for (int i = 0; i < num_s; i++) {
			y = r_dist(generator);
			z = r_dist(generator);
			ber = i_dist(generator);

			// ָ���ֲ� 1/��^0.5 �������ע��gamma_beta��in_beta�Ĺ�ϵ
			double gamma_beta = 1.0 / pow(in_beta, in_c);

			e_data = (-gamma_beta) * log(1 - y) + (-gamma_beta) * log(1 - z);
			// �������Y��Z�໥�����ҷ�����ͬ��ָ���ֲ�
			if (ber == 1) {
				g_data = e_data * e_data;
			}
			else if (ber == 0) {
				g_data = -(e_data * e_data);
			}
			data[i] = g_data;
		}
	}
}

// ��������ͨ��ȡ�������Ӷ�ֱ��ʹ��gammln����
void random_ggd::moment_estimation(double data[])
{
	double sig = 0;		// ����sigma_squareʱ�ķ���
	double test_c = 0;	// ʹ�ò���ƥ�䷽��ȷ��c
	double rho;
	double dist = dist_max;
	double c = 0;

	double sigma;
	double sum_abs = 0;
	double res;

	for (int i = 0; i < num_s; i++) {
		sig += data[i] * data[i];
	}
	sigma_square = sig / num_s;
	sigma = sqrt(sigma_square);      // ���sigma
	
	cout << endl << "ʵ��STD(sigma)=" << sigma << endl;

	for (int i = 0; i < num_s; i++) {
		sum_abs += fabs(data[i]);
	}
	res = log(sum_abs / num_s) - log(sigma);   // ���Ӧ��Gamma������ʽ��ȵ�ֵ

	// ���ݵ�ʽ������ȵ�ԭ�����ò���ƥ��ķ���ȷ��c
	for (test_c = 0; test_c < 2.0; test_c += 0.000001) {
		rho = gammln(2.0 / test_c) - 0.5 * gammln(1.0 / test_c) - 0.5 * gammln(3.0 / test_c);
		if (fabs(rho - res) < dist) {
			dist = fabs(rho - res);
			c = test_c;
		}
	}
	est_c = c;

}

// Gamma�����ļ���ο���Numerical Recipe��
double random_ggd::gammln(double xx)
{
	if (xx <= 0)
		cout << "���ݲ��Ϲ�" << endl;
	double x, y, tmp, ser;
	static double cof[6] = { 76.18009172947146,-86.50532032941677,
						24.01409824083091,-1.231739572450155,
						0.1208650973866179e-2,0.5395239384953e-5 };
	int j;

	y = x = xx;
	tmp = x + 5.5;
	tmp -= (x + 0.5) * log(tmp);
	ser = 1.000000000190015;
	for (j = 0; j <= 5; j++)ser += cof[j] / ++y;
	return -tmp + log(2.5066282746310005 * ser / x);
}

