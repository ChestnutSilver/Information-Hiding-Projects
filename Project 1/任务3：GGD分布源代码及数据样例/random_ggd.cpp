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
	int num_s;//���ɷ���ָ���·ֲ������������
	double in_beta;//���������
	double c_beta;//�����Ȼ���Ʀ�
public:
	random_exp(); //�����룬�����ʼ���£�������1000
	random_exp(int num, double beta);//����������Ϣ��ʼ��
	void generate_exp();//����ָ���ֲ��������
};

class random_ggd {
protected:	
	int num_s;		//���ɷ���GGD�ֲ������������
	double in_c;	//������״����c
	double in_beta;	//����߶Ȳ�����
	double sum = 0;
	double miu = 0;
	double sigma_square = 0;
	double ex_abs;
	double R;
	double est_c;	//�ع�����״����
	double est_c_quick;  //���׿����㷨������״����
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
	double x;//���ȷֲ�[0,1]�����
	double e_data;//ָ���ֲ��������


	//����[0,1]���ȷֲ����������
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	//����ָ���ֲ��������
	for (int i = 0; i < num_s; i++) {
		x = dist(generator);

		//ָ���ֲ��������
		e_data = (-in_beta) * log(1 - x);

		file << e_data << endl;
		c_beta += e_data;
	}
	file.close();

	//�����Ȼ���Ʋ�����
	c_beta /= num_s;

	cout << "�����£�" << in_beta << endl;
	cout << "�����Ȼ���Ʀ£�" << c_beta << endl;
}

random_ggd::random_ggd(int num, double c, double beta)
{
	num_s = num;
	in_c = c;
	in_beta = beta;
}

void random_ggd::generate_ggd()
{
	ofstream file_detail("random_ggd_detail.txt", ios::out);  //��ϸ��������
	ofstream file_data("random_ggd_data.txt", ios::out);      //GGD�ֲ�����
	double x, y, z;		//���ȷֲ�[0,1]�����
	int ber;			//Bernoulli�ֲ�[0,1]�����
	double e_data;		//ָ���ֲ��������
	double g_data;		//GGD�ֲ������
	double* data = new double[num_s];

	//����[0,1]���ȷֲ����������
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> r_dist(0.0, 1.0);
	uniform_int_distribution<> i_dist(0, 1);

	if (fabs(in_c - 1.0) < 1e-6) {	//c=1.0ʱ��GGD�ֲ�
		for (int i = 0; i < num_s; i++) {
			x = r_dist(generator);
			ber = i_dist(generator);

			//ָ���ֲ��������
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
	else if(fabs(in_c - 0.5) < 1e-6) {	//c=0.5ʱ��GGD�ֲ�
		for (int i = 0; i < num_s; i++) {
			y = r_dist(generator);
			z = r_dist(generator);
			ber = i_dist(generator);

			//ָ���ֲ� 1/��^0.5 �����
			e_data = (-sqrt(in_beta)) * log(1 - y) + (-sqrt(in_beta)) * log(1 - z);
			//�������Y��Z�໥�����ҷ�����ͬ��ָ���ֲ�
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

	moment_estimation(data);         //���оع���
	moment_estimation_quick(data);   //���������㷨���ٹ���

	//�����Ȼ���Ʋ�����
	//c_beta /= num_s;

	cout << "�����£�" << in_beta << endl;
	cout << "��״����c��" << in_c << endl;
	//cout << "�����Ȼ���Ʀ£�" << c_beta << endl;
	cout << "�ع�����״����c��" << est_c << endl;
	cout << "���ף����ڷ�����������ϵ�GGD��״������������״����c��" << est_c_quick << endl;
}
/*
//����һ���� https://blog.csdn.net/sinat_36438332/article/details/88363492 GGD��״����c�ľع���
void random_ggd::moment_estimation(int*& data)
{
	double sig = 0;   //����sigma_squareʱ�ķ���
	double ex = 0;	  //����ex_absʱ�ķ���
	double test_c = 0;//ʹ�ò���ƥ�䷽��ȷ��c
	double rho;
	double dist = dist_max;
	double c = 0;

	//�����ֵ�ͷ���Ĺ���ֵ���������ֵ������ƽ��ֵ�Ĺ���ֵ��R
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

	//���ݵ�ʽ������ȵ�ԭ�����ò���ƥ��ķ���ȷ��c
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
//�����������μ��Ƶ�
void random_ggd::moment_estimation(int*& data)
{
	double sig = 0;   //����sigma_squareʱ�ķ���
	double test_c = 0;//ʹ�ò���ƥ�䷽��ȷ��c
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
	sigma = sqrt(sigma_square);      //���sigma

	for (int i = 0; i < num_s; i++) {
		sum_abs += fabs(data[i]);
	}
	res = sum_abs / num_s / sigma;   //���Ӧ��Gamma������ʽ��ȵ�ֵ

	//���ݵ�ʽ������ȵ�ԭ�����ò���ƥ��ķ���ȷ��c
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

//��������ͨ��ȡ�������Ӷ�ֱ��ʹ��gammln����
void random_ggd::moment_estimation(double*& data)
{
	double sig = 0;   //����sigma_squareʱ�ķ���
	double test_c = 0;//ʹ�ò���ƥ�䷽��ȷ��c
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
	sigma = sqrt(sigma_square);      //���sigma

	for (int i = 0; i < num_s; i++) {
		sum_abs += fabs(data[i]);
	}
	res = log(sum_abs / num_s) - log(sigma);   //���Ӧ��Gamma������ʽ��ȵ�ֵ

	//���ݵ�ʽ������ȵ�ԭ�����ò���ƥ��ķ���ȷ��c
	for (test_c = 0; test_c < 2.0; test_c += 0.000001) {
		rho = gammln(2.0 / test_c) - 0.5 * gammln(1.0 / test_c) - 0.5 * gammln(3.0 / test_c);
		if (fabs(rho - res) < dist) {
			dist = fabs(rho - res);
			c = test_c;
		}
	}
	est_c = c;

}

//̽��������һ�ֻ��ھع��Ƶ�GGD��״�����Ŀ��ٹ����㷨
//������������������״�����Ĳ���:��һ������������һ�׺Ͷ��׾��Ծ�, �ڶ�������Ϻ���ʽ (6) ����״�������ơ�
//�ο����ף�[1]������.һ�ֹ����˹�ֲ���״�����Ŀ��ٹ����㷨[J].��ɽ����,2012(05):45-48.
void random_ggd::moment_estimation_quick(double*& data)
{
	double abs_moment_1;//������һ�׾��Ծ�
	double abs_moment_2;//�����Ķ��׾��Ծ�
	double sum_abs = 0;
	double sig = 0;		//����sigma_squareʱ�ķ��ӣ����������ڼ�����׾��Ծ�
	double x;			//�������е��Ա���
	double c = 0;

	for (int i = 0; i < num_s; i++) {	//����һ�׾��Ծ�
		sum_abs += fabs(data[i]);
	}
	abs_moment_1 = sum_abs / num_s;

	for (int i = 0; i < num_s; i++) {	//������׾��Ծ�
		sig += data[i] * data[i];
	}
	abs_moment_2 = sig / num_s;

	x = abs_moment_1 / sqrt(abs_moment_2);  //������������ߵ��Ա���

	c = moment_estimation_quick_func(x);

	est_c_quick = c;

}

//����������ķ������������
double random_ggd::moment_estimation_quick_func(double x)
{
	return -0.169111 * x * x + 0.220023 * x - (0.072175 / (x * x - 0.740842)) - (0.188638 / (x - 0.926277)) - 0.231659;
}

//Gamma�����ļ���ο���Numerical Recipe��
double random_ggd::gammln(double xx)
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

int main()
{
	int num;
	double c, beta;
	cout << "���������������������";
	cin >> num;
	cout << "������GGD�ֲ�����״����c����c=1.0 �� c=0.5��";
	cin >> c;
	cout << "������GGD�ֲ��ĳ߶Ȳ����£�";
	cin >> beta;
	if (fabs(c - 1.0) < 1e-6 || fabs(c - 0.5) < 1e-6){
		random_ggd gen(num, c, beta);
		gen.generate_ggd();
	}
	else{
		cout << "�Ƿ�����" << endl;
	}

	return 0;
}