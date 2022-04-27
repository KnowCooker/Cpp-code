#include<iostream>
#include<fstream>
using namespace std;

const int r = 5;//插值点数
static float tempValues_ins[(32 + r * 31) * (24 + r * 23)];//插值后的数据//原数据为24*32的矩阵
static float tempValues[768];//用于存储原数据

void bilinear(int r) {
    //插值实现函数//双线性插值
    int total_num = (32 + r * 31) * (24 + r * 23);
    int x_len = (32 + r * 31);//由左上角(0,0)，向下为x正方向，向右为y正方向，x的长度即为一行的长度
    int y_len = (24 + r * 23);//y的长度为一列的长度
    int dex = 0;

    for (int i = 0; i < total_num; i++) {
        int x = i / x_len;
        int y = i % x_len;
        if (x % (1 + r) == 0 && y % (1 + r) == 0) {
            tempValues_ins[i] = tempValues[dex];
            dex++;
        }
    }

    for (int i = 0; i < total_num; i++) {
        int x = i / x_len;
        int y = i % x_len;
        if (x % (1 + r) == 0 && y % (1 + r) != 0) {
            int y_plus = y % (1 + r);
            tempValues_ins[i] = (tempValues_ins[i - y_plus] * (r + 1 - y_plus) + tempValues_ins[i + r + 1 - y_plus] * y_plus) / float(r + 1);
        }
        if (x % (1 + r) != 0 && y % (1 + r) == 0) {
            int x_plus = x % (1 + r);
            tempValues_ins[i] = (tempValues_ins[(i - x_len * x_plus)] * (r + 1 - x_plus) + tempValues_ins[i + x_len * (r + 1 - x_plus)] * x_plus) / float(r + 1);
        }
    }

    for (int i = 0; i < total_num; i++) {
        int x = i / x_len;
        int y = i % x_len;
        int x_plus = x % (1 + r);
        int y_plus = y % (1 + r);

        if (x % (1 + r) != 0 && y % (1 + r) != 0) {
            tempValues_ins[i] = (tempValues_ins[i - x_len * x_plus] * (r + 1 - x_plus) + tempValues_ins[i + x_len * (r + 1 - x_plus)] * x_plus) / float(r + 1);
        }
    }
}

int main()
{
	ifstream in("C:\\Users\\78487\\Desktop\\code\\Arduino\\MLX90640\\irdata0.txt");
	for (int i = 0; i < 768; i++)
		in >> tempValues[i];

    bilinear(r);

	ofstream out("C:\\Users\\78487\\Desktop\\code\\Arduino\\MLX90640\\irdata5.txt");
    for (int i = 0; i < (32 + r * 31) * (24 + r * 23); i++) {
        out << tempValues_ins[i];
        out << "\n";
    }

	in.close();
	out.close();
}