#include <iostream>
#include <chrono>
#include <vector>
#include <cstring>

using namespace std;
const int num_trials = 10;

void init(int n, int* a, int* sum, int** b) {
    // 保持与原始代码一致
    for(int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = new int[n];
        for(int j = 0; j < n; j++)
            b[i][j] = i + j;
    }
    memset(sum, 0, n * sizeof(int)); 
}

int normal(int n, int* a, int* sum, int** b) {
    // 原始列优先算法
    auto start = chrono::high_resolution_clock::now();
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            sum[i] += b[j][i] * a[j]; 
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int main() {
    vector<int> sizes = {1000, 2000, 4000, 8000, 16000};

    for (int current_size : sizes) {
        // 动态内存分配与原始一致
        int* a = new int[current_size];
        int* sum = new int[current_size];
        int** b = new int*[current_size];
        init(current_size, a, sum, b);

        int total_normal = 0;
        for (int trial = 1; trial <= num_trials; ++trial) {
            memset(sum, 0, current_size * sizeof(int));
            total_normal += normal(current_size, a, sum, b);
        }
        
        // 输出结果
        cout << "Matrix Size: " << current_size 
             << "\nTotal Normal: " << total_normal << "ms\n";

        // 内存释放
        delete[] a;
        delete[] sum;
        for(int i = 0; i < current_size; ++i) delete[] b[i];
        delete[] b;
    }
    return 0;
}
