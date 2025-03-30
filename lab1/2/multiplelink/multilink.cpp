#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
const int REPEAT_TIMES = 100;

void init(int n, int* a) {/* 同normal_main.cpp */}

void multiplelink(int n, int* a, int& sum) {
    int sum0 = 0, sum1 = 0;
    for(int i0=0, i1=n/2; i0<n/2; i0++,i1++) {
        sum0 += a[i0];
        sum1 += a[i1];
    }
    sum = sum0 + sum1;
}

int main() {
    vector<int> sizes = {100000, 200000, 400000, 800000, 1600000};
    
    for (int current_size : sizes) {
        int* a = new int[current_size];
        int sum = 0;
        
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEAT_TIMES; ++i) {
            init(current_size, a);
            sum = 0;
            multiplelink(current_size, a, sum);
        }
        auto duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        
        cout << "MultipleLink - Size: " << current_size 
             << "\tTime: " << duration.count() << "ms\n";
        delete[] a;
    }
    return 0;
}
