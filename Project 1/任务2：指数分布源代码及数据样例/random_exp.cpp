#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
using namespace std;

class random_exp {
protected:
	int num_s;//生成服从指数β分布的随机数个数
	double in_beta;//输入参数β
	double c_beta;//最大似然估计β
public:
	random_exp(); //无输入，随机初始化β，样本数1000
	random_exp(int num, double beta);//根据输入信息初始化
	void generate_exp();//生成指数分布β随机数
};

random_exp::random_exp()
{
	num_s = 1000;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> ran_real(0.0, 100.0);
	in_beta = ran_real(generator);
	c_beta = 0;
}

random_exp::random_exp(int num, double beta)
{
	num_s = num;
	in_beta = beta;
	c_beta = 0;
}

void random_exp::generate_exp()
{
	ofstream file("random_exp.txt", ios::out);
	double x;//均匀分布[0,1]随机数
	double e_data;//指数分布β随机数


	//生成[0,1]均匀分布随机数设置
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	//生成指数分布β随机数
	for (int i = 0; i < num_s; i++) {
		x = dist(generator);

		//指数分布β随机数
		e_data = (-in_beta) * log(1 - x);

		file << e_data << endl;
		c_beta += e_data;
	}
	file.close();

	//最大似然估计参数β
	c_beta /= num_s;

	cout << "参数β：" << in_beta << endl;
	cout << "最大似然估计β：" << c_beta << endl;
}

int main()
{
	//无输入
	//random_exp gen;

	//有输入
	int num;
	double beta;
	cout << "请输入生成随机数个数：";
	cin >> num;
	cout << "请输入参数β：";
	cin >> beta;

	random_exp gen(num, beta);

	gen.generate_exp();
	return 0;
}