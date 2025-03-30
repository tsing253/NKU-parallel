#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <fstream>
#include <cstring> // 用于memset

using namespace std;

const int REPEAT_TIMES = 100;

// 算法函数声明
void normal(int n, int* a, int& sum);
void multiplelink(int n, int* a, int& sum);
void recursion(int n, int* a, int s);
void doubleloop(int n, int* a);

// 初始化函数
void init(int n, int* a) {
    for(int i = 0; i < n; i++) {
        a[i] = i;
    }
}

// 普通算法
void normal(int n, int* a, int& sum) {
    for(int i = 0; i < n; i++) {
        sum += a[i];
    }
}

// 多链路算法
void multiplelink(int n, int* a, int& sum) {
    int sum0 = 0, sum1 = 0;
    for(int i0 = 0, i1 = n/2; i0 < n/2 && i1 < n; i0++, i1++) {
        sum0 += a[i0];
        sum1 += a[i1];
    }
    sum = sum0 + sum1;
}

// 递归算法
void recursion(int n, int* a, int s) {
    if(s == 1) return;
    for(int i = 0; i < s/2; i++) {
        a[i] += a[s-i-1];
    }
    recursion(n, a, s/2);
}

// 双循环算法
void doubleloop(int n, int* a) {
    for(int i = n; i > 1; i /= 2) {
        for(int j = 0; j < i/2; j++) {
            a[j] = a[j*2]+a[j*2+1];
        }
    }
}

int main() {
    vector<int> sizes = {100000, 200000, 400000, 800000, 1600000}; // 测试不同数据规模
    ofstream outfile("algorithm_perf.csv");
    outfile << "Size,Algorithm,Time(ms)\n";

    for (int current_size : sizes) {
        int* a = new int[current_size];
        int sum = 0;
        
        // 测试普通算法
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEAT_TIMES; ++i) {
            init(current_size, a);
            sum = 0;
            normal(current_size, a, sum);
        }
        auto duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        outfile << current_size << ",Normal," << duration.count() << "\n";

        // 测试多链路算法
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEAT_TIMES; ++i) {
            init(current_size, a);
            sum = 0;
            multiplelink(current_size, a, sum);
        }
        duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        outfile << current_size << ",MultipleLink," << duration.count() << "\n";

        // 测试递归算法（使用备份数组）
        int* backup = new int[current_size]; // 防止递归破坏原始数据
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEAT_TIMES; ++i) {
            init(current_size, a);
            memcpy(backup, a, current_size*sizeof(int)); // 快速复制
            recursion(current_size, a, current_size);
            memcpy(a, backup, current_size*sizeof(int)); // 恢复数据
        }
        duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        outfile << current_size << ",Recursion," << duration.count() << "\n";
        delete[] backup;

        // 测试双循环算法
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEAT_TIMES; ++i) {
            init(current_size, a);
            doubleloop(current_size, a);
        }
        duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        outfile << current_size << ",DoubleLoop," << duration.count() << "\n";

        delete[] a; // 释放内存
    }

    outfile.close();
    return 0;
}
