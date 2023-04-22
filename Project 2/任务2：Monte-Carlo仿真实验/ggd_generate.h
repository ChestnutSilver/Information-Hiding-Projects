#pragma once
const double euler = 2.718281828459045;
const double dist_max = 1000000000.0;

class random_ggd {
protected:
	int num_s;			// 生成服从GGD分布的随机数个数
	double in_c;		// 输入形状参数c
	double in_beta;		// 输入尺度参数β
	double sum = 0;
	double miu = 0;
	double sigma_square = 0;
	double ex_abs;
	double R;
	double est_c;		// 矩估计形状参数
public:
	double data[1000];	// 生成的GGD分布随机数
	random_ggd(int num, double c, double beta);
	void generate_ggd();
	void moment_estimation(double data[]);	// 使用实验一作业中的方法三：通过取对数，从而直接使用gammln函数

	double gammln(double xx);
};