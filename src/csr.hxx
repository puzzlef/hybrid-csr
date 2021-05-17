#pragma once
#include <cmath>
#include <vector>
#include "_main.hxx"
#include "edges.hxx"

using std::vector;
using std::log;




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
    if (hybridCsrId(*it, blk) != vid) continue;
    if (hybridCsrBlock(*it, blk) & vblk) return i;
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
  T eid = hybridCsrId(a.back(), blk);
  T vid = hybridCsrValueId(v, blk2);
  if (eid!=vid) a.push_back(hybridCsrValueEntry(v, blk2));
  else a.back() |= hybridCsrValueBlock(v, blk2);
}




// SOURCE-OFFSETS
// --------------

template <class G, class J>
auto sourceOffsets(const G& x, J&& ks, int N) {
  int i = 0;
  vector<int> a;
  if (N>0) a.reserve(N+1);
  for (auto u : ks) {
    a.push_back(i);
    i += x.degree(u);
  }
  a.push_back(i);
  return a;
}

template <class G, class J>
auto sourceOffsets(const G& x, J&& ks) {
  return sourceOffsets(x, ks, csize(ks));
}

template <class G>
auto sourceOffsets(const G& x) {
  return sourceOffsets(x, x.vertices(), x.order());
}




// SOURCE-OFFSETS-HYBRID
// ---------------------

template <class G, class J, class K=int>
auto sourceOffsetsHybrid(const G& x, J&& ks, K blk) {
  vector<int> a;
  vector<K>   b;
  auto ids = indices(ks);
  int  i   = 0;
  for (int u : ks) {
    auto vs = edges(x, u, [&](int v) { return ids[v]; });
    sort(vs.begin(), vs.end()); b.clear();
    if (!vs.empty()) hybridCsrPush(b, K(vs[0]), blk);
    for (int v : slice(vs, 1))
      hybridCsrSortedAdd(b, K(v), blk);
    a.push_back(i);
    i += b.size();
  }
  a.push_back(i);
  return a;
}

template <class G, class K=int>
auto sourceOffsetsHybrid(const G& x, K blk) {
  return sourceOffsetsHybrid(x, x.vertices(), blk);
}




// DESTINATION-INDICES
// -------------------

template <class G, class J>
auto destinationIndices(const G& x, J&& ks) {
  auto ids = indices(ks);
  vector<int> a;
  for (int u : ks) {
    for (int v : x.edges(u))
      a.push_back(ids[v]);
    // sort(a.end()-x.degree(u), a.end());
  }
  return a;
}

template <class G>
auto destinationIndices(const G& x) {
  return destinationIndices(x, x.vertices());
}




// DESTINATION-INDICES-HYBRID
// --------------------------

template <class G, class J, class K=int>
auto destinationIndicesHybrid(const G& x, J&& ks, K blk) {
  vector<K> a;
  auto ids = indices(ks);
  for (int u : ks) {
    auto vs = edges(x, u, [&](int v) { return ids[v]; });
    sort(vs.begin(), vs.end());
    if (!vs.empty()) hybridCsrPush(a, K(vs[0]), blk);
    for (int v : slice(vs, 1))
      hybridCsrSortedAdd(a, K(v), blk);
  }
  return a;
}

template <class G, class K=int>
auto destinationIndicesHybrid(const G& x, K blk) {
  return destinationIndicesHybrid(x, x.vertices(), blk);
}
