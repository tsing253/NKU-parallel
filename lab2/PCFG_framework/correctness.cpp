#include "PCFG.h"
#include <chrono>
#include <fstream>
#include "md5.h"
#include <iomanip>
#include <string>
#include <vector>
using namespace std;
using namespace chrono;

// 编译指令如下：
// g++ correctness.cpp train.cpp guessing.cpp md5.cpp -o test.exe
// g++ correctness.cpp train.cpp guessing.cpp md5_SIMD.cpp -o test.exe

// 通过这个函数，你可以验证你实现的SIMD哈希函数的正确性
int main()
{
    bit32 state[4];
    uint32x4_t state_SIMD[4];
    string s1 = "bvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdvabvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdvabvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdvabvaisdbjasdkafkasdfnavkjnakdjfejfanjsdnfkajdfkajdfjkwanfdjaknsvjkanbjbjadfajwefajksdfakdnsvjadfasjdva";
    string[4] s2; 
    for(int i = 0; i < 4; i++)  s2[i] = s;
    MD5Hash(s1, state);
    // MD5Hash_SIMD(s2, state_SIMD);
    cout << "Original result:";
    for (int i1 = 0; i1 < 4; i1 += 1)
    {
        cout << std::setw(8) << std::setfill('0') << hex << state[i1] << endl;

    }
    cout << endl;
    // cout << "SIMD result:";
    // for (int i1 = 0; i1 < 4; i1 += 1)
    // {
    //     cout << std::setw(8) << std::setfill('0') << hex << state_SIMD[0][i1];
    // }
    cout << endl;
}