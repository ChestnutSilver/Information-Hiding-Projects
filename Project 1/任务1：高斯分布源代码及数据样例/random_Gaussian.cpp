#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
using namespace std;

class random_Gaussian {
protected:
	int num_sample;//生成高斯分布随机数个数
	double in_ave;//输入高斯分布μ
	double in_var;//输入高斯分布σ
	double c_ave;//参数估计μ
	double c_var;//参数估计σ
public:
	random_Gaussian();//无输入，随机初始化μ和σ，样本数1000
	random_Gaussian(int num_s, double ave, double in_var);//根据输入信息初始化
	void generate_Gaussian();//生成高斯分布N(μ,σ)随机数
};
random_Gaussian::random_Gaussian()
{
	num_sample = 1000;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> ran_real(0.0, 1.0);
	in_ave = ran_real(generator);
	in_var = ran_real(generator);
	c_ave = 0;
	c_var = 0;
}

random_Gaussian::random_Gaussian(int num_s, double ave, double var)
{
	num_sample = num_s;
	in_ave = ave;
	in_var = var;
	c_ave = 0;
	c_var = 0;
}

void random_Gaussian::generate_Gaussian()
{
	double pi = 3.1415926;
	ofstream file("random_Gaussian.txt", ios::out);
	double x, y;//均匀分布[0,1]随机数
	double g_data;//高斯分布N(μ,σ)随机数
	double z;//高斯分布N(0,1)随机数
	double temp;

	//file << "num_sample = " << num_sample << endl;

	//生成[0,1]均匀分布随机数设置
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	//生成高斯分布N(μ,σ)随机数
	for (int i = 0; i < num_sample; i++) {
		x = dist(generator);
		y = dist(generator);

		//BOX-MULLER生成高斯分布N(0,1)随机数
		temp = 2 * pi * y;
		z = sqrt((-2) * log(x)) * cos(temp);

		//高斯分布N(μ,σ)随机数
		g_data = in_ave + z * in_var;
		file << g_data << endl;

		//参数估计μ和σ
		c_ave += g_data;
		c_var += g_data * g_data;
	}
	file.close();

	c_ave /= num_sample;
	c_var = sqrt(c_var / num_sample - c_ave * c_ave);

	cout << "期望:" << in_ave << endl;
	cout << "估计期望:" << c_ave << endl;
	cout << "方差:" << in_var << endl;
	cout << "估计方差:" << c_var << endl;
}
int main()
{
	//无输入
	//random_Gaussian gen;

	//输入参数μ和σ以及生成样本数
	int num;
	double ave, var;
	cout << "请输入参数μ：";
	cin >> ave;
	cout << "请输入参数σ：";
	cin >> var;
	cout << "请输入生成随机数个数：";
	cin >> num;
	random_Gaussian gen(num, ave, var);

	gen.generate_Gaussian();

	return 0;
}