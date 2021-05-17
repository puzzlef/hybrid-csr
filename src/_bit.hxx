#pragma once




// *-BITS
// ------

template <class T>
T oneBits(T n) {
  return (1<<n)-1;
}

template <class T>
T zeroBits(T n) {
  return T(-1)<<n;
}
