#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
using namespace std;

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

int main()
{
	//������
	//random_exp gen;

	//������
	int num;
	double beta;
	cout << "���������������������";
	cin >> num;
	cout << "����������£�";
	cin >> beta;

	random_exp gen(num, beta);

	gen.generate_exp();
	return 0;
}