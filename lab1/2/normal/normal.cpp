#include <iostream>
#include <chrono>
#include <vector>
#include <cstring>

using namespace std;
const int REPEAT_TIMES = 100;

void init(int n, int* a) {
    for(int i = 0; i < n; i++) a[i] = i;
}

void normal(int n, int* a, int& sum) {
    for(int i = 0; i < n; i++) sum += a[i];
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
            normal(current_size, a, sum);
        }
        auto duration = chrono::duration_cast<chrono::milliseconds>(
            chrono::high_resolution_clock::now() - start);
        
        cout << "Normal - Size: " << current_size 
             << "\tTime: " << duration.count() << "ms\n";
        delete[] a;
    }
    return 0;
}
