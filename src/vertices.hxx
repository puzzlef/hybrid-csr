#pragma once
#include <vector>
#include "_main.hxx"

using std::vector;




// VERTICES
// --------

template <class G>
auto vertices(const G& x) {
  vector<int> a;
  a.reserve(x.order());
  for (int u : x.vertices())
    a.push_back(u);
  return a;
}


template <class G, class F>
auto verticesBy(const G& x, F fm) {
  auto a = vertices(x);
  sort(a.begin(), a.end(), [&](int u, int v) {
    return fm(u) < fm(v);
  });
  return a;
}




// VERTEX-DATA
// -----------

template <class G, class J>
auto vertexData(const G& x, J&& ks, int N) {
  using V = typename G::TVertex;
  vector<V> a;
  if (N>0) a.reserve(N);
  for (int u : ks)
    a.push_back(x.vertexData(u));
  return a;
}

template <class G, class J>
auto vertexData(const G& x, J&& ks) {
  return vertexData(x, ks, csize(ks));
}

template <class G>
auto vertexData(G& x) {
  return vertexData(x, x.vertices(), x.order());
}




// CONTAINER
// ---------

template <class G, class T>
auto createContainer(const G& x, const T& _) {
  return vector<T>(x.span());
}

template <class G, class T>
auto createCompressedContainer(const G& x, const T& _) {
  return vector<T>(x.order());
}


template <class G, class T, class J>
void decompressContainer(vector<T>& a, const G& x, const vector<T>& vs, J&& ks) {
  scatter(a, vs, ks);
}

template <class G, class T>
void decompressContainer(vector<T>& a, const G& x, const vector<T>& vs) {
  decompressContainer(a, x, vs, x.vertices());
}

template <class G, class T, class J>
auto decompressContainer(const G& x, const vector<T>& vs, J&& ks) {
  auto a = createContainer(x, T());
  decompressContainer(a, x, vs, ks);
  return a;
}

template <class G, class T>
auto decompressContainer(const G& x, const vector<T>& vs) {
  return decompressContainer(x, vs, x.vertices());
}


template <class G, class T, class J>
void compressContainer(vector<T>& a, const G& x, const vector<T>& vs, J&& ks) {
  gather(a, vs, ks);
}

template <class G, class T>
void compressContainer(vector<T>& a, const G& x, const vector<T>& vs) {
  return compressContainer(a, x, vs, x.vertices());
}

template <class G, class T, class J>
auto compressContainer(const G& x, const vector<T>& vs, J&& ks) {
  auto a = createCompressedContainer(x, T());
  compressContainer(a, x, vs, ks);
  return a;
}

template <class G, class T>
auto compressContainer(const G& x, const vector<T>& vs) {
  return compressContainer(x, vs, x.vertices());
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
