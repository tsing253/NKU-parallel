#include "md5.h"
#include <arm_neon.h>


//下面为SIMD版的函数
inline uint32x4_t F_SIMD(uint32x4_t x, uint32x4_t y, uint32x4_t z) {  // 这里的函数名后面加了_SIMD，表示是SIMD版本的函数
  return vorrq_u32(vandq_u32(x, y), vandq_u32(vmvnq_u32(x), z));
}

inline uint32x4_t G_SIMD(uint32x4_t x, uint32x4_t y, uint32x4_t z) {
  return vorrq_u32(vandq_u32(x, z), vandq_u32(y, vmvnq_u32(z)));
}

inline uint32x4_t H_SIMD(uint32x4_t x, uint32x4_t y, uint32x4_t z) {
  return veorq_u32(veorq_u32(x, y), z);
}

inline uint32x4_t I_SIMD(uint32x4_t x, uint32x4_t y, uint32x4_t z) {
  return veorq_u32(y, vorrq_u32(x, vmvnq_u32(z)));
}


inline uint32x4_t ROTATELEFT_SIMD(uint32x4_t num, int n) {
  return vorrq_u32(vshlq_n_u32(num, n),vshrq_n_u32(num, 32 - n));
}


inline void FF_SIMD(uint32x4_t &a, uint32x4_t b, uint32x4_t c, uint32x4_t d, uint32x4_t x, int s, uint32x4_t ac) {
  a = vaddq_u32(a,vaddq_u32(F_SIMD(b, c, d), vaddq_u32(x, ac)));
  a = ROTATELEFT_SIMD(a, s);
  a = vaddq_u32(a, b);
}

inline void GG_SIMD(uint32x4_t &a, uint32x4_t b, uint32x4_t c, uint32x4_t d, uint32x4_t x, int s, uint32x4_t ac) {
  a = vaddq_u32(a,vaddq_u32(G_SIMD(b, c, d), vaddq_u32(x, ac)));
  a = ROTATELEFT_SIMD(a, s);
  a = vaddq_u32(a, b);
}

inline void HH_SIMD(uint32x4_t &a, uint32x4_t b, uint32x4_t c, uint32x4_t d, uint32x4_t x, int s, uint32x4_t ac) {
  a = vaddq_u32(a,vaddq_u32(H_SIMD(b, c, d), vaddq_u32(x, ac)));
  a = ROTATELEFT_SIMD(a, s);
  a = vaddq_u32(a, b);
}

inline void II_SIMD(uint32x4_t &a, uint32x4_t b, uint32x4_t c, uint32x4_t d, uint32x4_t x, int s, uint32x4_t ac) {
  a = vaddq_u32(a,vaddq_u32(I_SIMD(b, c, d), vaddq_u32(x, ac)));
  a = ROTATELEFT_SIMD(a, s);
  a = vaddq_u32(a, b);
}

void MD5Hash_SIMD(vector<string>& input, bit32 state[4][4]);