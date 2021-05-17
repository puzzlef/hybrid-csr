#pragma once
#include <vector>
#include "_main.hxx"

using std::vector;




// CSR
// ---

template <class T>
void csrAdd(vector<T>& a, T v) {
  int i = findIndex(a, v);
  if (i<0) a.push_back(v);
}




// HYBRID-CSR HELPERS
// ------------------

template <class T>
T hybridCsrId(T e, int blk) {
  return e >> blk;
}

template <class T>
T hybridCsrBlock(T e, int blk) {
  return e & oneBits(blk);
}

template <class T>
T hybridCsrValueId(T v, int blk2) {
  return v >> blk2;
}

template <class T>
T hybridCsrValueBlock(T v, int blk2) {
  return 1 << (v & oneBits(blk2));
}

template <class T>
T hybridCsrValueEntry(T v, int blk2) {
  int blk = 1 << blk2;
  return (hybridCsrValueId(v, blk2) << blk) | hybridCsrValueBlock(v, blk2);
}




// HYBRID-CSR-FIND
// ---------------

template <class I, class T>
int hybridCsrFind(I ib, I ie, T v, int blk) {
  int blk2 = int(log(blk)/log(2)), i = 0;
  T vid  = hybridCsrValueId(v, blk2);
  T vblk = hybridCsrValueBlock(v, blk2);
  for (auto it=ib; it!=ie; ++it, i++) {
    if (hybridCsrId(*it) != vid) continue;
    if (hybridCsrBlock(*it) & vblk) return i;
  }
  return -1;
}

template <class J, class T>
int hybridCsrFind(J&& x, T v, int blk) {
  return hybridCsrFind(x.begin(), x.end(), v, blk);
}




// HYBRID-CSR-ADD
// --------------

template <class T>
void hybridCsrPush(vector<T>& a, T v, int blk) {
  int blk2 = int(log(blk)/log(2));
  a.push_back(hybridCsrValueEntry(v, blk2));
}


template <class T>
void hybridCsrAdd(vector<T>& a, T v, int blk) {
  int blk2 = int(log(blk)/log(2));
  int i = hybridCsrFind(a, v, blk);
  if (i<0) a.push_back(hybridCsrValueEntry(v, blk2));
  else a[i] |= hybridCsrValueBlock(v, blk2);
}


template <class T>
void hybridCsrSortedAdd(vector<T>& a, T v, int blk) {
  int blk2 = int(log(blk)/log(2));
  T eid = hybridCsrId(a.back());
  T vid = hybridCsrValueId(v, blk2);
  if (eid!=vid) a.push_back(hybridCsrValueEntry(v, blk2));
  else a.back() |= hybridCsrValueBlock(v, blk2);
}
