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
	double x, y, z;		// 均匀分布[0,1]随机数
	int ber;			// Bernoulli分布[0,1]随机数
	double e_data;		// 指数分布β随机数
	double g_data;		// GGD分布随机数

	// 生成[0,1]均匀分布随机数设置
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> r_dist(0.0, 1.0);
	uniform_int_distribution<> i_dist(0, 1);

	if (fabs(in_c - 1.0) < 1e-6) {	// c=1.0时的GGD分布
		for (int i = 0; i < num_s; i++) {
			x = r_dist(generator);
			ber = i_dist(generator);

			// 指数分布β随机数，注意gamma_beta和in_beta的关系
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
	else if (fabs(in_c - 0.5) < 1e-6) {	// c=0.5时的GGD分布
		for (int i = 0; i < num_s; i++) {
			y = r_dist(generator);
			z = r_dist(generator);
			ber = i_dist(generator);

			// 指数分布 1/β^0.5 随机数，注意gamma_beta和in_beta的关系
			double gamma_beta = 1.0 / pow(in_beta, in_c);

			e_data = (-gamma_beta) * log(1 - y) + (-gamma_beta) * log(1 - z);
			// 随机变量Y和Z相互独立且服从相同的指数分布
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

// 方法三：通过取对数，从而直接使用gammln函数
void random_ggd::moment_estimation(double data[])
{
	double sig = 0;		// 计算sigma_square时的分子
	double test_c = 0;	// 使用查找匹配方法确定c
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
	sigma = sqrt(sigma_square);      // 求得sigma
	
	cout << endl << "实际STD(sigma)=" << sigma << endl;

	for (int i = 0; i < num_s; i++) {
		sum_abs += fabs(data[i]);
	}
	res = log(sum_abs / num_s) - log(sigma);   // 求得应与Gamma函数分式相等的值

	// 根据等式两端相等的原则，利用查找匹配的方法确定c
	for (test_c = 0; test_c < 2.0; test_c += 0.000001) {
		rho = gammln(2.0 / test_c) - 0.5 * gammln(1.0 / test_c) - 0.5 * gammln(3.0 / test_c);
		if (fabs(rho - res) < dist) {
			dist = fabs(rho - res);
			c = test_c;
		}
	}
	est_c = c;

}

// Gamma函数的计算参考《Numerical Recipe》
double random_ggd::gammln(double xx)
{
	if (xx <= 0)
		cout << "数据不合规" << endl;
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

