#pragma once

#define PI    3.1415926
#define NUM   8 //DCT正反变换矩阵维数

//部分代码参考于https://blog.csdn.net/BigDream123/article/details/104395587
//已进行对源代码进行较大修改
class DCT {
protected:
	int modification(double a); //修正函数，在DCT反变换过程中使用,调整图像像素值在指定范围内，求其最近整数
public:
	void dc_transform(double(*input)[NUM], double(*output)[NUM]);//DCT正变换
	void Inverse_dc_transform(double(*input)[NUM], double(*output)[NUM]);//DCT反变换
};
