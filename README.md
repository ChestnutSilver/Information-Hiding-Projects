# Information-Hiding-Projects

**简介：**

同济大学信息隐藏课程作业.（2022）

我们的代码完成了所有任务，并进行了一些有创造性的研究，总评为“优”。

**引用：**

如您在作业中使用了我们的代码或报告的任何部分，请在参考文献中列出。

引文格式（yyyy.mm.dd是您访问本网站的日期）：

> [1] ChestnutSilver. 信息隐藏课程设计[EB/OL]. 2023[yyyy.mm.dd]. https://github.com/ChestnutSilver/Information-Hiding-Projects.

**编译环境：**

Visual Studio 2019

**帮助：**

如果您有建议或疑惑，欢迎在本仓库 Issues 提出。

欢迎您 star 本仓库！

----

**【重要】代码或报告中的错误订正：**

Project 1：

（1）z = sqrt((-2) * log(x)) * cos(temp); 可能LOG 0，应该LOG(1-x），因为UNIFORM_REAL_DISTRIBUTION产生[a, b)的均匀分布。

（2）矩估计法估计 GGD 分布的形状参数 c中方法1和2是一样，区别只不过是不是有中心。

（3）GGD 分布(c=0.5)的理论 CDF 与实践 CDF 对比中：X轴范围可以小一点，这样靠近0的地方显示的更加多一点。

注：我们的 Project 2 的代码中，订正了以上错误。

Project 3：

```c++
//设定d[0]=-delta/4;d[1]=delta/4;
if (b == 1)
qb_x = delta * int((avg_x - (delta / 4)) / delta + 0.5) + delta / 4;
else
qb_x = delta * int((avg_x + (delta / 4)) / delta + 0.5) - delta / 4;
```

应该用FLOOR，而不是直接类型转化，两者结果不一样的。
