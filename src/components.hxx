#pragma once
#include <vector>
#include "_main.hxx"
#include "vertices.hxx"
#include "dfs.hxx"
#include "topologicalSort.hxx"

using std::vector;



// COMPONENTS
// ----------
// Finds Strongly Connected Components (SCC) using Kosaraju's algorithm.

template <class G, class H>
auto components(const G& x, const H& xt) {
  vector2d<int> a;
  vector<int> vs;
  // original dfs
  auto vis = createContainer(x, bool());
  for (int u : x.vertices())
    if (!vis[u]) dfsEndLoop(vs, vis, x, u);
  // transpose dfs
  fill(vis, false);
  while (!vs.empty()) {
    int u = vs.back(); vs.pop_back();
    if (vis[u]) continue;
    a.push_back(vector<int>());
    dfsLoop(a.back(), vis, xt, u);
  }
  return a;
}




// COMPONENTS-IDS
// --------------
// Get component id of each vertex.

template <class G>
auto componentIds(const G& x, const vector2d<int>& comps) {
  auto a = createContainer(x, int()); int i = 0;
  for (const auto& comp : comps) {
    for (int u : comp)
      a[u] = i;
    i++;
  }
  return a;
}




// BLOCKGRAPH
// ----------

template <class H, class G>
void blockgraph(H& a, const G& x, const vector2d<int>& comps) {
  auto c = componentIds(x, comps);
  for (int u : x.vertices()) {
    a.addVertex(c[u]);
    for (int v : x.edges(u))
      if (c[u] != c[v]) a.addEdge(c[u], c[v]);
  }
}

template <class G>
auto blockgraph(const G& x, const vector2d<int>& comps) {
  G a; blockgraph(a, x, comps);
  return a;
}




// SORTED-COMPONENTS
// -----------------

template <class G>
auto sortedComponents(const G& x, vector2d<int> cs) {
  auto b = blockgraph(x, cs);
  auto bks = topologicalSort(b);
  reorder(cs, bks);
  return cs;
}

template <class G, class H>
auto sortedComponents(const G& x, const H& xt) {
  auto cs = components(x, xt);
  return sortedComponents(x, cs);
}




// COMPONENTS-EQUAL
// ----------------

template <class G>
bool componentsEqual(const G& x, const vector<int>& xc, const G& y, const vector<int>& yc) {
  if (xc != yc) return false;
  for (int i=0, I=xc.size(); i<I; i++)
    if (!verticesEqual(x, xc[i], y, yc[i])) return false;
  return true;
}

template <class G, class H>
bool componentsEqual(const G& x, const H& xt, const vector<int>& xc, const G& y, const H& yt, const vector<int>& yc) {
  return componentsEqual(x, xc, y, yc) && componentsEqual(xt, xc, yt, yc);
}




// COMPONENTS-HASH
// ---------------

auto componentsHash(const vector2d<int>& comps) {
  vector<size_t> a;
  for (const auto& comp : comps)
    a.push_back(hashValue(comp));
  return a;
}
