#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
using namespace std;

const double euler = 2.718281828459045;
const double dist_max = 1000000000.0;

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

class random_ggd {
protected:	
	int num_s;		//生成服从GGD分布的随机数个数
	double in_c;	//输入形状参数c
	double in_beta;	//输入尺度参数β
	double sum = 0;
	double miu = 0;
	double sigma_square = 0;
	double ex_abs;
	double R;
	double est_c;	//矩估计形状参数
	double est_c_quick;  //文献快速算法估计形状参数
public:
	random_ggd(int num, double c, double beta);
	void generate_ggd();
	void moment_estimation(double*& data);
	void moment_estimation_quick(double*& data);
	double moment_estimation_quick_func(double x);

	void likelihood_estimation(double*& data);


	double gammln(double xx);
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

random_ggd::random_ggd(int num, double c, double beta)
{
	num_s = num;
	in_c = c;
	in_beta = beta;
}

void random_ggd::generate_ggd()
{
	ofstream file_detail("random_ggd_detail.txt", ios::out);  //详细参数数据
	ofstream file_data("random_ggd_data.txt", ios::out);      //GGD分布数据
	double x, y, z;		//均匀分布[0,1]随机数
	int ber;			//Bernoulli分布[0,1]随机数
	double e_data;		//指数分布β随机数
	double g_data;		//GGD分布随机数
	double* data = new double[num_s];

	//生成[0,1]均匀分布随机数设置
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> r_dist(0.0, 1.0);
	uniform_int_distribution<> i_dist(0, 1);

	if (fabs(in_c - 1.0) < 1e-6) {	//c=1.0时的GGD分布
		for (int i = 0; i < num_s; i++) {
			x = r_dist(generator);
			ber = i_dist(generator);

			//指数分布β随机数
			e_data = (-in_beta) * log(1 - x);
			if (ber == 1) {
				g_data = e_data;
			}
			else if (ber == 0) {
				g_data = -e_data;
			}
			data[i] = g_data;
			file_detail << "x=" << x << "  e_data=" << e_data << "  ber=" << ber << "  g_data=" << data[i] << endl;
			file_data << data[i] << endl;
			//c_beta += e_data;
		}
	}
	else if(fabs(in_c - 0.5) < 1e-6) {	//c=0.5时的GGD分布
		for (int i = 0; i < num_s; i++) {
			y = r_dist(generator);
			z = r_dist(generator);
			ber = i_dist(generator);

			//指数分布 1/β^0.5 随机数
			e_data = (-sqrt(in_beta)) * log(1 - y) + (-sqrt(in_beta)) * log(1 - z);
			//随机变量Y和Z相互独立且服从相同的指数分布
			if (ber == 1) {
				g_data = e_data * e_data;
			}
			else if (ber == 0) {
				g_data = -(e_data * e_data);
			}
			data[i] = g_data;
			file_detail << "y=" << y << "  z=" << z << "  e_data=" << e_data << "  ber=" << ber << "  g_data=" << data[i] << endl;
			file_data << data[i] << endl;
			//c_beta += e_data;
		}
	}

	file_detail.close();
	file_data.close();

	moment_estimation(data);         //进行矩估计
	moment_estimation_quick(data);   //进行文献算法快速估计

	//最大似然估计参数β
	//c_beta /= num_s;

	cout << "参数β：" << in_beta << endl;
	cout << "形状参数c：" << in_c << endl;
	//cout << "最大似然估计β：" << c_beta << endl;
	cout << "矩估计形状参数c：" << est_c << endl;
	cout << "文献（基于反函数曲线拟合的GGD形状参数）估计形状参数c：" << est_c_quick << endl;
}
/*
//方法一：按 https://blog.csdn.net/sinat_36438332/article/details/88363492 GGD形状参数c的矩估计
void random_ggd::moment_estimation(int*& data)
{
	double sig = 0;   //计算sigma_square时的分子
	double ex = 0;	  //计算ex_abs时的分子
	double test_c = 0;//使用查找匹配方法确定c
	double rho;
	double dist = dist_max;
	double c = 0;

	//计算均值和方差的估计值、计算绝对值的修正平均值的估计值、R
	for (int i = 0; i < num_s; i++) {
		sum += data[i];
	}
	cout << "sum=" << sum << endl;
	miu = sum / num_s;

	for (int i = 0; i < num_s; i++) {
		sig += data[i] * data[i];
		//sig += (data[i] - miu) * (data[i] - miu);
		ex += fabs(data[i] - miu);
	}
	sigma_square = sig / num_s;
	ex_abs = ex / num_s;
	R = sigma_square / (ex_abs * ex_abs);

	//根据等式两端相等的原则，利用查找匹配的方法确定c
	for (test_c = 0; test_c < 2.0; test_c += 0.001){
		rho = pow(euler, gammln(1 / test_c)) * pow(euler, gammln(3 / test_c)) / pow(pow(euler, gammln(2 / test_c)), 2);
		if (fabs(rho - R) < dist) {
			dist = fabs(rho - R);
			c = test_c;
		}
	}
	est_c = c;

}
*/
/*
//方法二：按课件推导
void random_ggd::moment_estimation(int*& data)
{
	double sig = 0;   //计算sigma_square时的分子
	double test_c = 0;//使用查找匹配方法确定c
	double rho;
	double dist = dist_max;
	double c = 0;

	double sigma;
	double sum_abs = 0;
	double res;

	for (int i = 0; i < num_s; i++) {
		sig += data[i] * data[i];
	}
	sigma_square = sig / num_s;
	sigma = sqrt(sigma_square);      //求得sigma

	for (int i = 0; i < num_s; i++) {
		sum_abs += fabs(data[i]);
	}
	res = sum_abs / num_s / sigma;   //求得应与Gamma函数分式相等的值

	//根据等式两端相等的原则，利用查找匹配的方法确定c
	for (test_c = 0; test_c < 2.0; test_c += 0.001) {
		rho = pow(euler, gammln(2 / test_c)) / sqrt(pow(euler, gammln(1 / test_c)) * pow(euler, gammln(3 / test_c)));
		if (fabs(rho - res) < dist) {
			dist = fabs(rho - res);
			c = test_c;
		}
	}
	est_c = c;

}
*/

//方法三：通过取对数，从而直接使用gammln函数
void random_ggd::moment_estimation(double*& data)
{
	double sig = 0;   //计算sigma_square时的分子
	double test_c = 0;//使用查找匹配方法确定c
	double rho;
	double dist = dist_max;
	double c = 0;

	double sigma;
	double sum_abs = 0;
	double res;

	for (int i = 0; i < num_s; i++) {
		sig += data[i] * data[i];
	}
	sigma_square = sig / num_s;
	sigma = sqrt(sigma_square);      //求得sigma

	for (int i = 0; i < num_s; i++) {
		sum_abs += fabs(data[i]);
	}
	res = log(sum_abs / num_s) - log(sigma);   //求得应与Gamma函数分式相等的值

	//根据等式两端相等的原则，利用查找匹配的方法确定c
	for (test_c = 0; test_c < 2.0; test_c += 0.000001) {
		rho = gammln(2.0 / test_c) - 0.5 * gammln(1.0 / test_c) - 0.5 * gammln(3.0 / test_c);
		if (fabs(rho - res) < dist) {
			dist = fabs(rho - res);
			c = test_c;
		}
	}
	est_c = c;

}

//探究：复现一种基于矩估计的GGD形状参数的快速估计算法
//文中所述方法估计形状参数的步骤:第一步计算样本的一阶和二阶绝对矩, 第二步用拟合函数式 (6) 对形状参数估计。
//参考文献：[1]董阳武.一种广义高斯分布形状参数的快速估计算法[J].矿山测量,2012(05):45-48.
void random_ggd::moment_estimation_quick(double*& data)
{
	double abs_moment_1;//样本的一阶绝对矩
	double abs_moment_2;//样本的二阶绝对矩
	double sum_abs = 0;
	double sig = 0;		//计算sigma_square时的分子；在这里用于计算二阶绝对矩
	double x;			//反函数中的自变量
	double c = 0;

	for (int i = 0; i < num_s; i++) {	//计算一阶绝对矩
		sum_abs += fabs(data[i]);
	}
	abs_moment_1 = sum_abs / num_s;

	for (int i = 0; i < num_s; i++) {	//计算二阶绝对矩
		sig += data[i] * data[i];
	}
	abs_moment_2 = sig / num_s;

	x = abs_moment_1 / sqrt(abs_moment_2);  //反函数拟合曲线的自变量

	c = moment_estimation_quick_func(x);

	est_c_quick = c;

}

//文献中提出的反函数拟合曲线
double random_ggd::moment_estimation_quick_func(double x)
{
	return -0.169111 * x * x + 0.220023 * x - (0.072175 / (x * x - 0.740842)) - (0.188638 / (x - 0.926277)) - 0.231659;
}

//Gamma函数的计算参考《Numerical Recipe》
double random_ggd::gammln(double xx)
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

int main()
{
	int num;
	double c, beta;
	cout << "请输入生成随机数个数：";
	cin >> num;
	cout << "请输入GGD分布的形状参数c：（c=1.0 或 c=0.5）";
	cin >> c;
	cout << "请输入GGD分布的尺度参数β：";
	cin >> beta;
	if (fabs(c - 1.0) < 1e-6 || fabs(c - 0.5) < 1e-6){
		random_ggd gen(num, c, beta);
		gen.generate_ggd();
	}
	else{
		cout << "非法输入" << endl;
	}

	return 0;
}