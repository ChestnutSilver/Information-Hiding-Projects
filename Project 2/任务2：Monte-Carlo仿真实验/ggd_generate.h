#pragma once
const double euler = 2.718281828459045;
const double dist_max = 1000000000.0;

class random_ggd {
protected:
	int num_s;			// ���ɷ���GGD�ֲ������������
	double in_c;		// ������״����c
	double in_beta;		// ����߶Ȳ�����
	double sum = 0;
	double miu = 0;
	double sigma_square = 0;
	double ex_abs;
	double R;
	double est_c;		// �ع�����״����
public:
	double data[1000];	// ���ɵ�GGD�ֲ������
	random_ggd(int num, double c, double beta);
	void generate_ggd();
	void moment_estimation(double data[]);	// ʹ��ʵ��һ��ҵ�еķ�������ͨ��ȡ�������Ӷ�ֱ��ʹ��gammln����

	double gammln(double xx);
};