#pragma once
#include <cmath>
#include <vector>
#include <utility>
#include <ostream>
#include <iostream>
#include "_main.hxx"
#include "edges.hxx"

using std::vector;
using std::ostream;
using std::cout;
using std::log;
using std::move;




// CSR
// ---

template <class T, class U=T>
struct Csr {
  vector<T> sourceOffsets;
  vector<U> destinationIndices;

  Csr() = default;
  Csr(vector<T>&& vto, vector<U>&& eto) :
  sourceOffsets(vto), destinationIndices(eto) {}
  Csr(vector<T>& vto, vector<U>& eto) :
  sourceOffsets(move(vto)), destinationIndices(move(eto)) {}
};




// CSR GRAPH-LIKE
// --------------

template <class T, class U>
int csrOrder(const Csr<T, U>& x) {
  return x.sourceOffsets.size()-1;
}


template <class T, class U>
int csrSize(const Csr<T, U>& x) {
  return x.destinationIndices.size();
}


template <class T, class U>
int csrDegree(const Csr<T, U>& x, T u) {
  const auto& vto = x.sourceOffsets;
  return vto[u+1] - vto[u];
}


template <class T, class U>
auto csrVertices(const Csr<T, U>& x) {
  return range(csrOrder(x));
}


template <class T, class U>
auto csrEdges(const Csr<T, U>& x, T u) {
  const auto& vto = x.sourceOffsets;
  return slice(x.destinationIndices, vto[u], vto[u+1]);
}




// CSR PRINT
// ---------

template <class T, class U>
void write(ostream& a, const Csr<T, U>& x, bool all=false) {
  const auto& vto = x.sourceOffsets;
  const auto& eto = x.destinationIndices;
  a << "order: " << csrOrder(x) << " size: " << csrSize(x);
  if (!all) { a << " {}"; return; }
  a << " {\n";
  for (int u=0, N=csrOrder(x); u<N; u++) {
    a << "  " << u << " ->";
    for (T j=vto[u], J=vto[u+1]; j<J; j++)
      a << " " << eto[j];
    a << "\n";
  }
  a << "}";
}

template <class T, class U>
void print(const Csr<T, U>& x, bool all=false) { write(cout, x, all); }

template <class T, class U>
void println(const Csr<T, U>& x, bool all=false) { print(x, all); cout << "\n"; }




// CSR (FROM GRAPH)
// ----------------

template <class G, class J, class K=int>
auto csr(const G& x, J&& ks, K typ=K()) {
  Csr<K> a;
  auto& vto = a.sourceOffsets;
  auto& eto = a.destinationIndices;
  auto ids  = indices(ks);
  for (int u : ks) {
    vto.push_back(eto.size());
    for (int v : x.edges(u))
      eto.push_back(ids[v]);
    // sort(eto.end()-x.degree(u), eto.end());
  }
  vto.push_back(eto.size());
  return a;
}

template <class G, class K=int>
auto csr(const G& x, K typ=K()) {
  return csr(x, x.vertices(), typ);
}




// HYBRID-CSR
// ----------

template <class T, class U=T>
struct HybridCsr : public Csr<T, U> {
  int blockSize;
  vector<T> sourceOffsets;
  vector<U> destinationIndices;

  HybridCsr(int blk=4) : blockSize(blk) {}
  HybridCsr(int blk, vector<T>&& vto, vector<T>&& eto) :
  blockSize(blk), sourceOffsets(vto), destinationIndices(eto) {}
  HybridCsr(int blk, vector<T>& vto, vector<T>& eto) :
  blockSize(blk), sourceOffsets(move(vto)), destinationIndices(move(eto)) {}
};




// HYBRID-CSR HELPERS
// ------------------

template <class T>
T hybridCsrEntryId(T e, int blk) {
  return e >> blk;
}

template <class T>
T hybridCsrEntryBlock(T e, int blk) {
  return e & oneBits(T(blk));
}

template <class T>
T hybridCsrValueId(T v, int blk2) {
  return v >> blk2;
}

template <class T>
T hybridCsrValueBlock(T v, int blk2) {
  return 1 << (v & oneBits(T(blk2)));
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
    if (hybridCsrEntryId(*it, blk) != vid) continue;
    if (hybridCsrEntryBlock(*it, blk) & vblk) return i;
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
  T eid = hybridCsrEntryId(a.back(), blk);
  T vid = hybridCsrValueId(v, blk2);
  if (eid!=vid) a.push_back(hybridCsrValueEntry(v, blk2));
  else a.back() |= hybridCsrValueBlock(v, blk2);
}




// HYBRID-CSR GRAPH-LIKE
// ---------------------

template <class T, class U>
int csrOrder(const HybridCsr<T, U>& x) {
  return x.sourceOffsets.size()-1;
}


template <class T, class U>
int csrDegree(const HybridCsr<T, U>& x, T u) {
  const auto& vto = x.sourceOffsets;
  const auto& eto = x.destinationIndices;
  int blk = x.blockSize, a = 0;
  for (T i=vto[u], I=vto[u+1]; i<I; i++)
    a += countBits(hybridCsrEntryBlock(eto[i], blk));
  return a;
}


template <class T, class U>
int csrSize(const HybridCsr<T, U>& x) {
  int a = 0;
  for (T u=0, N=csrOrder(x); u<N; u++)
    a += csrDegree(x, u);
  return a;
}


template <class T, class U>
auto csrVertices(const HybridCsr<T, U>& x) {
  return range(csrOrder(x));
}


// template <class T, class U>
// auto hybridCsrEdges(const Csr<T, U>& x, T u) {
// }




// HYBRID-CSR (FROM GRAPH)
// -----------------------

template <class G, class J, class K>
auto hybridCsr(const G& x, J&& ks, K blk) {
  HybridCsr<int, K> a(blk);
  auto& vto = a.sourceOffsets;
  auto& eto = a.destinationIndices;
  auto ids  = indices(ks);
  for (int u : ks) {
    vto.push_back(eto.size());
    auto vs = edges(x, u, [&](int v) { return ids[v]; });
    sort(vs.begin(), vs.end());
    if (!vs.empty()) hybridCsrPush(eto, K(vs[0]), blk);
    for (int v : sliceIter(vs, 1))
      hybridCsrSortedAdd(eto, K(v), blk);
  }
  vto.push_back(eto.size());
  return a;
}

template <class G, class K>
auto hybridCsr(const G& x, K blk) {
  return hybridCsr(x, x.vertices(), blk);
}




// HYBRID-CSR PRINT
// ----------------

template <class T, class U>
void write(ostream& a, const HybridCsr<T, U>& x, bool all=false) {
  const auto& vto = x.sourceOffsets;
  const auto& eto = x.destinationIndices;
  a << "order: " << csrOrder(x) << " size: " << csrSize(x);
  if (!all) { a << " {}"; return; }
  a << " {\n";
  int blk = x.blockSize, blk2 = int(log(blk)/log(2));
  for (int u=0, N=csrOrder(x); u<N; u++) {
    a << "  " << u << " ->";
    for (T j=vto[u], J=vto[u+1]; j<J; j++) {
      U pre = hybridCsrEntryId(eto[j], blk) << blk2;
      U dat = hybridCsrEntryBlock(eto[j], blk);
      for (int b=0; dat>0; dat>>=1, b++)
        if (dat & 1) a << " " << pre + b;
    }
    a << "\n";
  }
  a << "}";
}

template <class T, class U>
void print(const HybridCsr<T, U>& x, bool all=false) { write(cout, x, all); }

template <class T, class U>
void println(const HybridCsr<T, U>& x, bool all=false) { print(x, all); cout << "\n"; }




// CSR-EQUAL
// ---------

/*
template <class T, class U>
auto csrDehybridize(const HybridCsr<T, U>& x, int i) {
  vector<T> a;
  auto xb = x.sourceOffsets[i];
  auto xe = x.sourceOffsets[i+1];
  int blk  = x.blockSize;
  int blk2 = int(log(blk)/log(2));
  for (int xi=xb; xi<xe; xi++) {
    auto xe   = x.destinationIndices[xi];
    auto xid  = hybridCsrEntryId(xe, blk);
    auto xblk = hybridCsrEntryBlock(xe, blk);
    for (int j=0; xblk>0; xblk>>=1, j++) {
      if (xblk&1 == 0) continue;
      a.push_back((xid<<blk2)|j);
    }
  }
  return a;
}


template <class T, class U, class V, class W>
bool csrEqual(const HybridCsr<T, U>& x, const Csr<V, W>& y, int xi, int yi) {
  auto xes = slice(x.destinationIndices, x.sourceOffsets[xi], x.sourceOffsets[xi+1]);
  auto yes = slice(y.destinationIndices, y.sourceOffsets[yi], y.sourceOffsets[yi+1]);
  int blk = x.blockSize, blk2 = int(log(blk)/log(2));
  for (auto xe : xes) {
    auto xid  = hybridCsrEntryId(xe, blk);
    auto xblk = hybridCsrEntryBlock(xe, blk);
    printf("id: %d\n", int(xid<<blk2));
    println(yes);
    for (int j=0; xblk>0; xblk>>=1, j++) {
      if (xblk&1 == 0) continue;
      if (findIndex(yes, (xid<<blk2)|j) < 0) return false;
    }
  }
  return true;
}


template <class T, class U, class V, class W>
bool csrEqual(const HybridCsr<T, U>& x, const Csr<V, W>& y) {
  printf("x: "); println(x, true);
  printf("y: "); println(y, true);
  return true;
  int X = x.sourceOffsets.size();
  int Y = y.sourceOffsets.size();
  if (X != Y) return false;
  for (int i=0; i<X; i++) {
    auto xvs = csrDehybridize(x, i);
    auto yb = y.destinationIndices.begin() + y.sourceOffsets[i];
    auto ye = y.destinationIndices.begin() + y.sourceOffsets[i+1];
    auto yvs = vector<T>(yb, ye);
    printf("xvs: "); println(xvs);
    printf("yvs: "); println(yvs);
  }
  return true;
}
*/
