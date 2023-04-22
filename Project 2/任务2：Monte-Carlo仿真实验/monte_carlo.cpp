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
const double ave = 0;				// 高斯分布中心
const double var = 10;				// 高斯分布标准差
const double noise_var_small = 2.5;	// 噪声期望2.5
const double noise_var_big = 5.0;	// 噪声期望5.0
const int progress_bar = 50;		// 进度条20个分块（用于绘制进度条）

// Gammln函数
double gammln(double xx)
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

// 绘制完整进度条（因为等待时间较长，所以用进度条会友好许多）
void print_total_progress_bar()
{
	// 用到了一些调整字体颜色的函数，也是为了交互友好
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << endl << endl << "完整进度条：";
	for (int i = 0; i < progress_bar; i++) {
		cout << "=";
	}
	cout << endl << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// main函数中的代码实现了对各种条件的选择和计算，并绘制实时进度条
int main()
{
	int num = 1000;
	double c, beta;
	double sigma = 10.0;
	char choice;
	cout << "随机数个数N=" << num << endl;
	cout << "标准偏差STD=" << sigma << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "请选择攻击情况（A: 无攻击, B: 有攻击）";
	cin >> choice;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	if (choice == 'A') {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "请输入GGD分布的形状参数c：（c=2.0, c=1.0 或 c=0.5）";
		cin >> c;	// c: GGD分布的形状参数
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		beta = 1.0 / sigma * sqrt(pow(euler, gammln(3.0 / c)) / pow(euler, gammln(1.0 / c))); // beta: GGD分布的尺度参数
		print_total_progress_bar();

		embed emb;
		double in_a;	// 嵌入强度
		if (fabs(c - 2.0) < 1e-6 || fabs(c - 1.0) < 1e-6 || fabs(c - 0.5) < 1e-6) {
			if (fabs(c - 0.5) < 1e-6 || fabs(c - 1.0) < 1e-6) {
				in_a = 1.8;
				emb.init(in_a, 1);
				cout << "正计算{攻击情况A: 无攻击、c=" << c << "、a=" << in_a << "}条件下的情形......" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "当前进度条：";
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
				cout << "正计算{攻击情况A: 无攻击、c=" << c << "、a=" << in_a << "}条件下的情形......" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "当前进度条：";
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
				cout << "正计算{攻击情况A: 无攻击、c=" << c << "、a=" << in_a << "}条件下的情形......" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "当前进度条：";
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
			cout << "非法输入" << endl;
		}
	}
	else if (choice == 'B') {
		c = 2.0;
		beta = 1.0 / sigma * sqrt(pow(euler, gammln(3.0 / c)) / pow(euler, gammln(1.0 / c)));

		print_total_progress_bar();

		embed emb;
		double in_a = 1.8;
		emb.init(in_a, 1);

		cout << "正计算{攻击情况B：有攻击、a=" << in_a << "、STD=0(原始状态)}条件下的情形......" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "当前进度条：";
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
		cout << "正计算{攻击情况B：有攻击、a=" << in_a << "、STD=2.5}条件下的情形......" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "当前进度条：";
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
		cout << "正计算{攻击情况B：有攻击、a=" << in_a << "、STD=5.0}条件下的情形......" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "当前进度条：";
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
		cout << "非法输入" << endl;
	}
	return 0;
}