#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "_main.hxx"
#include "csr.hxx"

using std::vector;
using std::unordered_map;
using std::sort;




// EDGES
// -----

template <class G, class F>
auto edges(const G& x, int u, F fn) {
  using K = decltype(fn(0));
  vector<K> a;
  for (int v : x.edges(u))
    a.push_back(fn(v));
  return a;
}

template <class G>
auto edges(const G& x, int u) {
  return edges(x, u, [](int v) { return v; });
}


template <class G, class F>
auto inEdges(const G& x, int v, F fn) {
  using K = decltype(fn(0));
  vector<K> a;
  for (int u : x.inEdges(v))
    a.push_back(fn(u));
  return a;
}

template <class G>
auto inEdges(const G& x, int v) {
  return inEdges(x, v, [](int u) { return u; });
}




// EDGES-DATA
// ----------

template <class G, class J>
auto edgeData(const G& x, J&& ks) {
  using E = typename G::TEdge;
  vector<E> a;
  for (int u : ks) {
    for (int v : x.edges(u))
      a.push_back(x.edgeData(u, v));
  }
  return a;
}

template <class G>
auto edgeData(const G& x) {
  return edgeData(x, x.vertices());
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
  auto ids = indices(ks);
  vector<K> a;
  for (int u : ks) {
    auto vs = edges(x, u, [&](int v) { return ids[v]; });
    sort(vs.begin(), vs.end());
    hybridCsrPush(a, vs[0], blk);
    for (int v : slice(vs, 1))
      hybridCsrSortedAdd(a, v, blk);
  }
  return a;
}

template <class G, class K=int>
auto destinationIndicesHybrid(const G& x, K blk) {
  return destinationIndicesHybrid(x, x.vertices(), blk);
}
