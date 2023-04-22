#include<math.h>
#include<iostream>
#include"DCT.h"
using namespace std;

//修正函数，在DCT反变换过程中使用
int DCT::modification(double a)
{
    if (a >= 255)
        return 255;
    else if (a <= 0)
        return 0;
    else return
        int(a + 0.5);
}

//DCT正变换
void DCT::dc_transform(double(*input)[NUM], double(*output)[NUM])
{
    double alpha, beta;
    for (int u = 0; u < NUM; u++) {
        for (int v = 0; v < NUM; v++) {
            if (u == 0)
                alpha = sqrt(1.0 / NUM);
            else
                alpha = sqrt(2.0 / NUM);

            if (v == 0)
                beta = sqrt(1.0 / NUM);
            else
                beta = sqrt(2.0 / NUM);

            double tmp = 0.0;
            for (int i = 0; i < NUM; i++)
                for (int j = 0; j < NUM; j++)
                    tmp += input[i][j] * cos((i * 2.0 + 1) * u * PI / (2.0 * NUM))
                    * cos((j * 2.0 + 1) * v * PI / (2.0 * NUM));
            output[u][v] = (alpha * beta * tmp);
        }
    }
}

//DCT反变换
void DCT::Inverse_dc_transform(double(*input)[NUM], double(*output)[NUM])
{
    double ALPHA, BETA;

    for (int i = 0; i < NUM; i++) {
        for (int j = 0; j < NUM; j++) {
            double tmp = 0.0;
            for (int u = 0; u < NUM; u++) {
                for (int v = 0; v < NUM; v++) {

                    if (u == 0)
                        ALPHA = sqrt(1.0 / NUM);
                    else
                        ALPHA = sqrt(2.0 / NUM);

                    if (v == 0)
                        BETA = sqrt(1.0 / NUM);
                    else
                        BETA = sqrt(2.0 / NUM);

                    tmp += ALPHA * BETA * input[u][v] * cos((i * 2.0 + 1) * u
                        * PI / (2.0 * NUM)) * cos((j * 2.0 + 1) * v * PI / (2.0 * NUM));
                }
            }
            output[i][j] = round(tmp);
        }
    }
}

