#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
const int REPEAT_TIMES = 100;

void init(int n, int* a) {/* 同normal_main.cpp */}

void doubleloop(int n, int* a) {
    for(int i=n; i>1; i/=2)
        for(int j=0; j<i/2; j++) 
            a[j] = a[j*2] + a[j*2+1];
}

int main() {
    vector<int> sizes = {100000, 200000, 400000, 800000, 1600000};
    
    for (int current_size : sizes) {
        int* a = new int[current_size];
        
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEAT_TIMES; ++i) {
            init(current_size, a);
            doubleloop(current_size, a);
        }
        auto duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        
        cout << "DoubleLoop - Size: " << current_size 
             << "\tTime: " << duration.count() << "ms\n";
        delete[] a;
    }
    return 0;
}
