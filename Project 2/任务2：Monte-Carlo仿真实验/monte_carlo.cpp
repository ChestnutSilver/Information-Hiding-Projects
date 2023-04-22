#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
#include<windows.h>
#include"random_Gaussian.h"
#include"ggd_generate.h"
#include"embedding.h"
using namespace std;
const double ave = 0;				// ��˹�ֲ�����
const double var = 10;				// ��˹�ֲ���׼��
const double noise_var_small = 2.5;	// ��������2.5
const double noise_var_big = 5.0;	// ��������5.0
const int progress_bar = 50;		// ������20���ֿ飨���ڻ��ƽ�������

// Gammln����
double gammln(double xx)
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

// ������������������Ϊ�ȴ�ʱ��ϳ��������ý��������Ѻ���ࣩ
void print_total_progress_bar()
{
	// �õ���һЩ����������ɫ�ĺ�����Ҳ��Ϊ�˽����Ѻ�
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << endl << endl << "������������";
	for (int i = 0; i < progress_bar; i++) {
		cout << "=";
	}
	cout << endl << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// main�����еĴ���ʵ���˶Ը���������ѡ��ͼ��㣬������ʵʱ������
int main()
{
	int num = 1000;
	double c, beta;
	double sigma = 10.0;
	char choice;
	cout << "���������N=" << num << endl;
	cout << "��׼ƫ��STD=" << sigma << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "��ѡ�񹥻������A: �޹���, B: �й�����";
	cin >> choice;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	if (choice == 'A') {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "������GGD�ֲ�����״����c����c=2.0, c=1.0 �� c=0.5��";
		cin >> c;	// c: GGD�ֲ�����״����
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		beta = 1.0 / sigma * sqrt(pow(euler, gammln(3.0 / c)) / pow(euler, gammln(1.0 / c))); // beta: GGD�ֲ��ĳ߶Ȳ���
		print_total_progress_bar();

		embed emb;
		double in_a;	// Ƕ��ǿ��
		if (fabs(c - 2.0) < 1e-6 || fabs(c - 1.0) < 1e-6 || fabs(c - 0.5) < 1e-6) {
			if (fabs(c - 0.5) < 1e-6 || fabs(c - 1.0) < 1e-6) {
				in_a = 1.8;
				emb.init(in_a, 1);
				cout << "������{�������A: �޹�����c=" << c << "��a=" << in_a << "}�����µ�����......" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "��ǰ��������";
				for (int i = 0; i < cyclenums; i++) {
					if (i % (cyclenums / progress_bar) == 0)
						cout << "=";
					random_ggd gen(num, c, beta);
					gen.generate_ggd();
					emb.simulate(gen);
				}
				cout << endl << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				emb.result(in_a);
			}
			else if (fabs(c - 2.0) < 1e-6) {
				in_a = 1.8;
				emb.init(in_a, 1);
				cout << "������{�������A: �޹�����c=" << c << "��a=" << in_a << "}�����µ�����......" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "��ǰ��������";
				for (int i = 0; i < cyclenums; i++) {
					if (i % (cyclenums / progress_bar) == 0)
						cout << "=";
					random_Gaussian gen(num, ave, var);
					gen.generate_Gaussian();
					emb.simulate(gen);
				}
				cout << endl << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				emb.result(in_a);

				in_a = 1.5;
				emb.init(in_a, 1);
				cout << "������{�������A: �޹�����c=" << c << "��a=" << in_a << "}�����µ�����......" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "��ǰ��������";
				for (int i = 0; i < cyclenums; i++) {
					if (i % (cyclenums / progress_bar) == 0)
						cout << "=";
					random_Gaussian gen(num, ave, var);
					gen.generate_Gaussian();
					emb.simulate(gen);
				}
				cout << endl << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				emb.result(in_a);
			}
		}
		else {
			cout << "�Ƿ�����" << endl;
		}
	}
	else if (choice == 'B') {
		c = 2.0;
		beta = 1.0 / sigma * sqrt(pow(euler, gammln(3.0 / c)) / pow(euler, gammln(1.0 / c)));

		print_total_progress_bar();

		embed emb;
		double in_a = 1.8;
		emb.init(in_a, 1);

		cout << "������{�������B���й�����a=" << in_a << "��STD=0(ԭʼ״̬)}�����µ�����......" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "��ǰ��������";
		for (int i = 0; i < cyclenums; i++) {
			if (i % (cyclenums / progress_bar) == 0)
				cout << "=";
			random_Gaussian gen(num, ave, var);
			gen.generate_Gaussian();
			emb.simulate(gen);
		}
		cout << endl << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		emb.result(in_a);

		emb.init(in_a, 2);
		cout << "������{�������B���й�����a=" << in_a << "��STD=2.5}�����µ�����......" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "��ǰ��������";
		for (int i = 0; i < cyclenums; i++) {
			if (i % (cyclenums / progress_bar) == 0)
				cout << "=";
			random_Gaussian gen(num, ave, var);
			gen.generate_Gaussian();
			random_Gaussian noise_small(num, ave, noise_var_small);
			noise_small.generate_Gaussian();
			emb.simulate(gen, noise_small);
		}
		cout << endl << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		emb.result_attacked(in_a, noise_var_small);

		emb.init(in_a, 3);
		cout << "������{�������B���й�����a=" << in_a << "��STD=5.0}�����µ�����......" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "��ǰ��������";
		for (int i = 0; i < cyclenums; i++) {
			if (i % (cyclenums / progress_bar) == 0)
				cout << "=";
			random_Gaussian gen(num, ave, var);
			gen.generate_Gaussian();
			random_Gaussian noise_big(num, ave, noise_var_big);
			noise_big.generate_Gaussian();
			emb.simulate(gen, noise_big);
		}
		cout << endl << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		emb.result_attacked(in_a, noise_var_big);
	}
	else {
		cout << "�Ƿ�����" << endl;
	}
	return 0;
}