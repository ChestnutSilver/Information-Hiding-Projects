#pragma once
#include"random_Gaussian.h"
#include"ggd_generate.h"
const int N = 1000;				// 数据个数 N=1000
const int Xn = 55;				// 横坐标个数
const double sigma = 10.0;		// 标准偏差 STD=10
const int cyclenums = 1000000;	// 达到 1e-6 的精度，需要 10^6 组数据

class embed {
protected:
	double a;				// 嵌入强度a
	double x_data[N];		// 数据Xi
	double w_data[N];		// 数据Wi
	double v_data[N];		// 数据Vi（有攻击情形）
	double s_h1_data[N];	// h1情形下的Si
	double s_h0_data[N];	// h0情形下的Si
	int pm_nums[Xn];		// Pm的频数，用于计算实验Pm

	/* 
	先自己定一组理论pfa作为自变量；然后用每一个理论pfa，都能算出对应的理论pm和阈值，
	用阈值进行检测，得到实际pfa和pm；最后画pfa（理论）为x轴，pm(理论、实际）为y轴的曲线；
	我们自己设的一个横坐标是对应一个阈值，然后检测10^6组数据对于一个阈值的实际pm，得到图像上的一个点。
	*/

	double the_pfa[Xn] = { 1e-6,2e-6,3e-6,4e-6,5e-6,6e-6,7e-6,8e-6,9e-6,
						1e-5,2e-5,3e-5,4e-5,5e-5,6e-5,7e-5,8e-5,9e-5,
						1e-4,2e-4,3e-4,4e-4,5e-4,6e-4,7e-4,8e-4,9e-4,
						1e-3,2e-3,3e-3,4e-3,5e-3,6e-3,7e-3,8e-3,9e-3,
						1e-2,2e-2,3e-2,4e-2,5e-2,6e-2,7e-2,8e-2,9e-2,
						1e-1,2e-1,3e-1,4e-1,5e-1,6e-1,7e-1,8e-1,9e-1,1 };// (规定的)理论Pfa，55个横坐标
	double exp_pfa[Xn];				// 实验Pfa
	double the_pm[Xn];				// 理论Pm
	double exp_pm[Xn];				// 实验Pm
	double threshold[Xn];			// 阈值

public:
	void init(double in_a, int status);// 参数初始化
	void input_data(random_ggd gen);	// 数据初始化（GGD分布，无攻击情形）
	void input_data(random_Gaussian gen);							// 数据初始化（高斯分布，无攻击情形）
	void input_data(random_Gaussian gen, random_Gaussian noise);	// 数据初始化（高斯分布，有攻击情形）
	void xdata(random_ggd gen);			// 计算Xi
	void xdata(random_Gaussian gen);	// 计算Xi
	void vdata(random_Gaussian noise);	// 计算Vi（有攻击情形）
	void wdata();						// 计算Wi
	void sdata();						// 计算Si（无攻击情形）
	void sdata_attacked();				// 计算Si（有攻击情形）
	void simulate(random_ggd gen);		// 嵌入仿真（GGD分布，无攻击情形）
	void simulate(random_Gaussian gen);	// 嵌入仿真（高斯分布，无攻击情形）
	void simulate(random_Gaussian gen, random_Gaussian noise);		// 嵌入仿真（高斯分布，有攻击情形）
	double cal_q(const double x);		// 计算Q函数
	double cal_qinv(const double y);	// 计算Q函数的反函数
	double ierfc(const double y);		// 计算ierfc函数

	void result(double in_a);			// 文件输出结果（无攻击情形）
	void result_attacked(double in_a, double noise);				// 文件输出结果（有攻击情形）
};