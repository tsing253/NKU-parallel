#include <iostream>
#include <chrono>
#include <vector>
#include <cstring>

using namespace std;
const int REPEAT_TIMES = 100;

void init(int n, int* a) {/* 同normal_main.cpp */}

void recursion(int n, int* a, int s) {
    if(s == 1) return;
    for(int i=0; i<s/2; i++) a[i] += a[s-i-1];
    recursion(n, a, s/2);
}

int main() {
    vector<int> sizes = {100000, 200000, 400000, 800000, 1600000};
    
    for (int current_size : sizes) {
        int* a = new int[current_size];
        int* backup = new int[current_size];
        
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEAT_TIMES; ++i) {
            init(current_size, a);
            memcpy(backup, a, current_size*sizeof(int));
            recursion(current_size, a, current_size);
            memcpy(a, backup, current_size*sizeof(int));
        }
        auto duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        
        cout << "Recursion - Size: " << current_size 
             << "\tTime: " << duration.count() << "ms\n";
        delete[] a;
        delete[] backup;
    }
    return 0;
}
