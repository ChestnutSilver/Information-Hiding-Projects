#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
using namespace std;

class random_Gaussian {
protected:
	int num_sample;//���ɸ�˹�ֲ����������
	double in_ave;//�����˹�ֲ���
	double in_var;//�����˹�ֲ���
	double c_ave;//�������Ʀ�
	double c_var;//�������Ʀ�
public:
	random_Gaussian();//�����룬�����ʼ���̺ͦң�������1000
	random_Gaussian(int num_s, double ave, double in_var);//����������Ϣ��ʼ��
	void generate_Gaussian();//���ɸ�˹�ֲ�N(��,��)�����
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
	double x, y;//���ȷֲ�[0,1]�����
	double g_data;//��˹�ֲ�N(��,��)�����
	double z;//��˹�ֲ�N(0,1)�����
	double temp;

	//file << "num_sample = " << num_sample << endl;

	//����[0,1]���ȷֲ����������
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 generator(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	//���ɸ�˹�ֲ�N(��,��)�����
	for (int i = 0; i < num_sample; i++) {
		x = dist(generator);
		y = dist(generator);

		//BOX-MULLER���ɸ�˹�ֲ�N(0,1)�����
		temp = 2 * pi * y;
		z = sqrt((-2) * log(x)) * cos(temp);

		//��˹�ֲ�N(��,��)�����
		g_data = in_ave + z * in_var;
		file << g_data << endl;

		//�������Ʀ̺ͦ�
		c_ave += g_data;
		c_var += g_data * g_data;
	}
	file.close();

	c_ave /= num_sample;
	c_var = sqrt(c_var / num_sample - c_ave * c_ave);

	cout << "����:" << in_ave << endl;
	cout << "��������:" << c_ave << endl;
	cout << "����:" << in_var << endl;
	cout << "���Ʒ���:" << c_var << endl;
}
int main()
{
	//������
	//random_Gaussian gen;

	//��������̺ͦ��Լ�����������
	int num;
	double ave, var;
	cout << "����������̣�";
	cin >> ave;
	cout << "����������ң�";
	cin >> var;
	cout << "���������������������";
	cin >> num;
	random_Gaussian gen(num, ave, var);

	gen.generate_Gaussian();

	return 0;
}