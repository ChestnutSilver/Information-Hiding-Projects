#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
#include"random_Gaussian.h"
using namespace std;

// 根据输入信息初始化
random_Gaussian::random_Gaussian(int num_s, double ave, double var)
{
	num_sample = num_s;
	in_ave = ave;
	in_var = var;
	c_ave = 0;
	c_var = 0;
}

// 生成高斯分布N(μ,σ)随机数
void random_Gaussian::generate_Gaussian()
{
	double pi = 3.1415926535;
	double x, y;		// 均匀分布[0,1]随机数
	double g_data;		// 高斯分布N(μ,σ)随机数
	double z;			// 高斯分布N(0,1)随机数
	double temp;

	// 生成[0,1]均匀分布随机数设置
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	// 生成高斯分布N(μ,σ)随机数
	for (int i = 0; i < num_sample; i++) {
		x = dist(generator);
		y = dist(generator);

		// BOX-MULLER生成高斯分布N(0,1)随机数
		temp = 2 * pi * y;
		z = sqrt((-2) * log(1 - x)) * cos(temp);	// 订正了作业一logx问题

		// 高斯分布N(μ,σ)随机数
		g_data = in_ave + z * in_var;
		data[i] = g_data;

		// 参数估计μ和σ
		c_ave += g_data;
		c_var += g_data * g_data;
	}

	c_ave /= num_sample;
	c_var = sqrt(c_var / num_sample - c_ave * c_ave);
}