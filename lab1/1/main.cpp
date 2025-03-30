#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

const int num_trials = 10;


void init(int n, int* a, int* sum, int** b) {
    for(int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = new int[n];
        for(int j = 0; j < n; j++)
            b[i][j] = i + j;
    }
    memset(sum, 0, n * sizeof(int)); 
}

int normal(int n, int* a, int* sum, int** b) {
    auto start = chrono::high_resolution_clock::now();
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            sum[i] += b[j][i] * a[j]; // 列优先访问，缓存不友好

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int optimize(int n, int* a, int* sum, int** b) {
    auto start = chrono::high_resolution_clock::now();
    
    for(int j = 0; j < n; j++)
        for(int i = 0; i < n; i++)
            sum[i] += b[j][i] * a[j]; // 行优先访问，提升缓存命中

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int main() {
    vector<int> sizes = {1000, 2000, 4000, 8000, 16000}; // 测试不同矩阵大小


    ofstream outfile("algorithm_times.csv");
    outfile << "MatrixSize,Normal(ms),Optimized(ms)\n";

    for (int current_size : sizes) {
        // 动态内存分配
        int* a = new int[current_size];
        int* sum = new int[current_size];
        int** b = new int*[current_size];
        
        init(current_size, a, sum, b);

        int total_normal = 0;
        int total_optimized = 0;

        for (int trial = 1; trial <= num_trials; ++trial) {
            memset(sum, 0, current_size * sizeof(int)); 
            total_normal += normal(current_size, a, sum, b);

            memset(sum, 0, current_size * sizeof(int));
            total_optimized += optimize(current_size, a, sum, b);

            
        }
        outfile << current_size << ","  
                    << total_normal << "," << total_optimized << "\n";

        cout << "\nMatrix Size: " << current_size 
             << "\nTotal Normal: " << total_normal << "ms"
             << "\nTotal Optimized: " << total_optimized << "ms"
             << "\nOptimization Benefit: " 
             << (total_normal - total_optimized) * 100.0 / total_normal << "%\n";

        delete[] a;
        delete[] sum;
        for(int i = 0; i < current_size; ++i) delete[] b[i];
        delete[] b;
    }

    outfile.close();
    return 0;
}
