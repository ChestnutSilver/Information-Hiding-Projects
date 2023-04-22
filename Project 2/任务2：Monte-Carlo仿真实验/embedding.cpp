#include<iostream>
#include<random>
#include<cmath>
#include<chrono>
#include<fstream>
#include"random_Gaussian.h"
#include"ggd_generate.h"
#include"embedding.h"
using namespace std; 

// ����Xi
void embed::xdata(random_ggd gen)
{
    for (int i = 0; i < N; i++) {
        x_data[i] = gen.data[i];
    }
}

// ����Xi
void embed::xdata(random_Gaussian gen)
{
    for (int i = 0; i < N; i++) {
        x_data[i] = gen.data[i];
    }
}

// ����Vi���й������Σ�
void embed::vdata(random_Gaussian noise)
{
    for (int i = 0; i < N; i++) {
        v_data[i] = noise.data[i];
    }
}

// ����Wi
void embed::wdata()
{
	//��һ�����ȡ+1, -1, +1, -1��
	for (int i = 0; i < N; i++) {
		w_data[i] = i % 2 == 0 ? 1 : -1;
	}
}

// ����Si���޹������Σ�
void embed::sdata()
{
	for (int i = 0; i < N; i++) {
		s_h1_data[i] = x_data[i] + a * w_data[i];
        s_h0_data[i] = x_data[i];
	}
}

// ����Si���й������Σ�
void embed::sdata_attacked()
{
    for (int i = 0; i < N; i++) {
        s_h1_data[i] = x_data[i] + a * w_data[i] + v_data[i];
        s_h0_data[i] = x_data[i] + v_data[i];
    }
}

// ���ݳ�ʼ����GGD�ֲ����޹������Σ�
void embed::input_data(random_ggd gen)
{
	xdata(gen);
	wdata();
	sdata();
}

// ���ݳ�ʼ������˹�ֲ����޹������Σ�
void embed::input_data(random_Gaussian gen)
{
    xdata(gen);
    wdata();
    sdata();
}

// ���ݳ�ʼ������˹�ֲ����й������Σ�
void embed::input_data(random_Gaussian gen, random_Gaussian noise)
{
    vdata(noise);
    xdata(gen);
    wdata();
    sdata_attacked();
}

// ������ʼ��
void embed::init(double in_a, int status)
{
    a = in_a;
    for (int i = 0; i < Xn; i++) {
        pm_nums[i] = 0;
    }
    for (int i = 0; i < Xn; i++) {
        if (status == 1) {
            the_pm[i] = 1 - cal_q(cal_qinv(the_pfa[i]) - sqrt(N) * a / sigma);  // ��������Pm
            threshold[i] = sigma * cal_qinv(the_pfa[i]) / (sqrt(N));            // ������ֵ
        }
        else if (status == 2) {
            the_pm[i] = 1 - cal_q(cal_qinv(the_pfa[i]) - sqrt(N) * a / sqrt(sigma * sigma + 2.5 * 2.5));  // ��������Pm
            threshold[i] = sqrt(sigma * sigma + 2.5 * 2.5) * cal_qinv(the_pfa[i]) / (sqrt(N));            // ������ֵ
        }
        else if (status == 3) {
            the_pm[i] = 1 - cal_q(cal_qinv(the_pfa[i]) - sqrt(N) * a / sqrt(sigma * sigma + 5.0 * 5.0));  // ��������Pm
            threshold[i] = sqrt(sigma * sigma + 5.0 * 5.0) * cal_qinv(the_pfa[i]) / (sqrt(N));            // ������ֵ
        }
    }
}

// Ƕ����棨GGD�ֲ����޹������Σ�
void embed::simulate(random_ggd gen)
{
	input_data(gen);

    double Sigma_SiWi = 0;          // ����L(S)=1/N*Sigma(i=1,N){Si*wi}
    for (int i = 0; i < N; i++) {
        Sigma_SiWi += s_h1_data[i] * w_data[i];
    }
    double LS = Sigma_SiWi / N;
    
    for (int i = 0; i < Xn; i++) {  // ����ʵ��Pm
        if (LS < threshold[i]) {
            pm_nums[i]++;           // ����Pm��Ƶ��
        }
    }
}

// Ƕ����棨��˹�ֲ����޹������Σ�
void embed::simulate(random_Gaussian gen)
{
    input_data(gen);

    double Sigma_SiWi = 0;          // ����L(S)=1/N*Sigma(i=1,N){Si*wi}
    for (int i = 0; i < N; i++) {
        Sigma_SiWi += s_h1_data[i] * w_data[i];
    }
    double LS = Sigma_SiWi / N;

    for (int i = 0; i < Xn; i++) {  // ����ʵ��Pm
        if (LS < threshold[i]) {
            pm_nums[i]++;           // ����Pm��Ƶ��
        }
    }
}

// Ƕ����棨��˹�ֲ����й������Σ�
void embed::simulate(random_Gaussian gen, random_Gaussian noise)
{
    input_data(gen, noise);

    double Sigma_SiWi = 0;          // ����L(S)=1/N*Sigma(i=1,N){Si*wi}
    for (int i = 0; i < N; i++) {
        Sigma_SiWi += s_h1_data[i] * w_data[i];
    }
    double LY = Sigma_SiWi / N;

    for (int i = 0; i < Xn; i++) {  // ����ʵ��Pm
        if (LY < threshold[i]) {
            pm_nums[i]++;           // ����Pm��Ƶ��
        }
    }
}


// ����Q����
double embed::cal_q(const double x)
{
    return 0.5 * erfc(x / (sqrt(2.0)));
}

// ����Q�����ķ�����
double embed::cal_qinv(const double y)
{
    return sqrt(2.0) * ierfc(2 * y);
}

// ����ierfc����
double embed::ierfc(const double y)
// inverse of the error function erfc
// Copyright(C) 1996 Takuya OOURA (email: ooura@mmm.t.u-tokyo.ac.jp)
{
    const double IERFC_LIM = 27;
    double z = (y > 1) ? 2 - y : y;
    if (z < 1e-300) return (y > 1) ? -IERFC_LIM : IERFC_LIM;
    double w = 0.916461398268964 - log(z);
    double u = sqrt(w);
    double s = (log(u) + 0.488826640273108) / w;
    double t = 1 / (u + 0.231729200323405);
    double x = u * (1 - s * (s * 0.124610454613712 + 0.5)) -
        ((((-0.0728846765585675 * t + 0.269999308670029) * t +
            0.150689047360223) * t + 0.116065025341614) * t +
            0.499999303439796) * t;
    t = 3.97886080735226 / (x + 3.97886080735226);
    u = t - 0.5;
    s = (((((((((0.00112648096188977922 * u +
        1.05739299623423047e-4) * u - 0.00351287146129100025) * u -
        7.71708358954120939e-4) * u + 0.00685649426074558612) * u +
        0.00339721910367775861) * u - 0.011274916933250487) * u -
        0.0118598117047771104) * u + 0.0142961988697898018) * u +
        0.0346494207789099922) * u + 0.00220995927012179067;
    s = ((((((((((((s * u - 0.0743424357241784861) * u -
        0.105872177941595488) * u + 0.0147297938331485121) * u +
        0.316847638520135944) * u + 0.713657635868730364) * u +
        1.05375024970847138) * u + 1.21448730779995237) * u +
        1.16374581931560831) * u + 0.956464974744799006) * u +
        0.686265948274097816) * u + 0.434397492331430115) * u +
        0.244044510593190935) * t -
        z * exp(x * x - 0.120782237635245222);
    x += s * (x * s + 1);
    return (y > 1) ? -x : x;
}

// �ļ����������޹������Σ�
void embed::result(double in_a)
{
    // �������Pfa
    ofstream file_the_pfa("nonattack_the_pfa.txt", ios::out);
    for (int i = 0; i < Xn; i++) {
        file_the_pfa << the_pfa[i] << endl;
    }

    // ���Ƕ��ǿ�� a=1.8 �����£�ʵ��Pm������Pm
    if (fabs(in_a - 1.8) < 1e-6) {
        ofstream file_exp_pm_bigA("nonattack_exp_pm_1.8.txt", ios::out);
        ofstream file_the_pm_bigA("nonattack_the_pm_1.8.txt", ios::out);
        for (int i = 0; i < Xn; i++) {
            exp_pm[i] = double(pm_nums[i]) / cyclenums;
            file_exp_pm_bigA << exp_pm[i] << endl;
            file_the_pm_bigA << the_pm[i] << endl;
        }
    }

    // ���Ƕ��ǿ�� a=1.5 �����£�ʵ��Pm������Pm
    else if (fabs(in_a - 1.5) < 1e-6) {
        ofstream file_exp_pm_smallA("nonattack_exp_pm_1.5.txt", ios::out);
        ofstream file_the_pm_smallA("nonattack_the_pm_1.5.txt", ios::out);
        for (int i = 0; i < Xn; i++) {
            exp_pm[i] = double(pm_nums[i]) / cyclenums;
            file_exp_pm_smallA << exp_pm[i] << endl;
            file_the_pm_smallA << the_pm[i] << endl;
        }
    }
}

// �ļ����������й������Σ�
void embed::result_attacked(double in_a, double noise)
{
    // �������Pfa
    ofstream file_the_pfa("attack_the_pfa.txt", ios::out);
    for (int i = 0; i < Xn; i++) {
        file_the_pfa << the_pfa[i] << endl;
    }

    // ������� STD=2.5 �����£�ʵ��Pm������Pm
    if (fabs(noise - 2.5) < 1e-6) {
        ofstream file_exp_pm_smallSTD("attack_exp_pm_2.5.txt", ios::out);
        ofstream file_the_pm_smallSTD("attack_the_pm_2.5.txt", ios::out);
        for (int i = 0; i < Xn; i++) {
            exp_pm[i] = double(pm_nums[i]) / cyclenums;
            file_exp_pm_smallSTD << exp_pm[i] << endl;
            file_the_pm_smallSTD << the_pm[i] << endl;
        }
    }

    // ������� STD=5.0 �����£�ʵ��Pm������Pm
    else if (fabs(noise - 5.0) < 1e-6) {
        ofstream file_exp_pm_bigSTD("attack_exp_pm_5.0.txt", ios::out);
        ofstream file_the_pm_bigSTD("attack_the_pm_5.0.txt", ios::out);
        for (int i = 0; i < Xn; i++) {
            exp_pm[i] = double(pm_nums[i]) / cyclenums;
            file_exp_pm_bigSTD << exp_pm[i] << endl;
            file_the_pm_bigSTD << the_pm[i] << endl;
        }
    }
}