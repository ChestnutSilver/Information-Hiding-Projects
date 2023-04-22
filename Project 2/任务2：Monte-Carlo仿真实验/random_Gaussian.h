#pragma once
#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>

class random_Gaussian {
protected:
	int num_sample;		// 生成高斯分布随机数个数
	double in_ave;		// 输入高斯分布μ
	double in_var;		// 输入高斯分布σ
	double c_ave;		// 参数估计μ
	double c_var;		// 参数估计σ
public:
	double data[1000];	// 生成的高斯分布随机数
	random_Gaussian(int num_s, double ave, double in_var);	// 根据输入信息初始化
	void generate_Gaussian();								// 生成高斯分布N(μ,σ)随机数
};