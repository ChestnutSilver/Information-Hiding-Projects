#pragma once
#include"random_Gaussian.h"
#include"ggd_generate.h"
const int N = 1000;				// ���ݸ��� N=1000
const int Xn = 55;				// ���������
const double sigma = 10.0;		// ��׼ƫ�� STD=10
const int cyclenums = 1000000;	// �ﵽ 1e-6 �ľ��ȣ���Ҫ 10^6 ������

class embed {
protected:
	double a;				// Ƕ��ǿ��a
	double x_data[N];		// ����Xi
	double w_data[N];		// ����Wi
	double v_data[N];		// ����Vi���й������Σ�
	double s_h1_data[N];	// h1�����µ�Si
	double s_h0_data[N];	// h0�����µ�Si
	int pm_nums[Xn];		// Pm��Ƶ�������ڼ���ʵ��Pm

	/* 
	���Լ���һ������pfa��Ϊ�Ա�����Ȼ����ÿһ������pfa�����������Ӧ������pm����ֵ��
	����ֵ���м�⣬�õ�ʵ��pfa��pm�����pfa�����ۣ�Ϊx�ᣬpm(���ۡ�ʵ�ʣ�Ϊy������ߣ�
	�����Լ����һ���������Ƕ�Ӧһ����ֵ��Ȼ����10^6�����ݶ���һ����ֵ��ʵ��pm���õ�ͼ���ϵ�һ���㡣
	*/

	double the_pfa[Xn] = { 1e-6,2e-6,3e-6,4e-6,5e-6,6e-6,7e-6,8e-6,9e-6,
						1e-5,2e-5,3e-5,4e-5,5e-5,6e-5,7e-5,8e-5,9e-5,
						1e-4,2e-4,3e-4,4e-4,5e-4,6e-4,7e-4,8e-4,9e-4,
						1e-3,2e-3,3e-3,4e-3,5e-3,6e-3,7e-3,8e-3,9e-3,
						1e-2,2e-2,3e-2,4e-2,5e-2,6e-2,7e-2,8e-2,9e-2,
						1e-1,2e-1,3e-1,4e-1,5e-1,6e-1,7e-1,8e-1,9e-1,1 };// (�涨��)����Pfa��55��������
	double exp_pfa[Xn];				// ʵ��Pfa
	double the_pm[Xn];				// ����Pm
	double exp_pm[Xn];				// ʵ��Pm
	double threshold[Xn];			// ��ֵ

public:
	void init(double in_a, int status);// ������ʼ��
	void input_data(random_ggd gen);	// ���ݳ�ʼ����GGD�ֲ����޹������Σ�
	void input_data(random_Gaussian gen);							// ���ݳ�ʼ������˹�ֲ����޹������Σ�
	void input_data(random_Gaussian gen, random_Gaussian noise);	// ���ݳ�ʼ������˹�ֲ����й������Σ�
	void xdata(random_ggd gen);			// ����Xi
	void xdata(random_Gaussian gen);	// ����Xi
	void vdata(random_Gaussian noise);	// ����Vi���й������Σ�
	void wdata();						// ����Wi
	void sdata();						// ����Si���޹������Σ�
	void sdata_attacked();				// ����Si���й������Σ�
	void simulate(random_ggd gen);		// Ƕ����棨GGD�ֲ����޹������Σ�
	void simulate(random_Gaussian gen);	// Ƕ����棨��˹�ֲ����޹������Σ�
	void simulate(random_Gaussian gen, random_Gaussian noise);		// Ƕ����棨��˹�ֲ����й������Σ�
	double cal_q(const double x);		// ����Q����
	double cal_qinv(const double y);	// ����Q�����ķ�����
	double ierfc(const double y);		// ����ierfc����

	void result(double in_a);			// �ļ����������޹������Σ�
	void result_attacked(double in_a, double noise);				// �ļ����������й������Σ�
};