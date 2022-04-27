#include<iostream>
#include<fstream>
using namespace std;

const int r = 5;//��ֵ����
static float tempValues_ins[(32 + r * 31) * (24 + r * 23)];//��ֵ�������//ԭ����Ϊ24*32�ľ���
static float tempValues[768];//���ڴ洢ԭ����

void bilinear(int r) {
    //��ֵʵ�ֺ���//˫���Բ�ֵ
    int total_num = (32 + r * 31) * (24 + r * 23);
    int x_len = (32 + r * 31);//�����Ͻ�(0,0)������Ϊx����������Ϊy������x�ĳ��ȼ�Ϊһ�еĳ���
    int y_len = (24 + r * 23);//y�ĳ���Ϊһ�еĳ���
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