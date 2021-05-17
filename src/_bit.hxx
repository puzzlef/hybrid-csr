#pragma once
#include <cstdint>

using std::uint32_t;
using std::uint64_t;




// ONE, ZERO
// ---------

template <class T>
T oneBits(T n) {
  return (T(1)<<n)-T(1);
}

template <class T>
T zeroBits(T n) {
  return T(-1)<<n;
}




// COUNT
// -----

int countBits(uint32_t x) {
  x = x - ((x>>1) & 0x55555555U);
  x = (x & 0x33333333U) + ((x>>2) & 0x33333333U);
  return ((x + (x>>4) & 0x0F0F0F0FU) * 0x01010101U)>>24;
}

int countBits(uint64_t x) {
  x = x - ((x>>1) & 0x5555555555555555UL);
  x = (x & 0x3333333333333333UL) + ((x>>2) & 0x3333333333333333UL);
  x = ((x + (x>>4) & 0x0F0F0F0F0F0F0F0FUL) * 0x0101010101010101UL)>>56;
  return (int) x;
}
