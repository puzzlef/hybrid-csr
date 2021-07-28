#include <cstdint>
#include <cstdio>
#include <iostream>
#include "src/main.hxx"

using namespace std;




template <class C>
void runCsrPrint(const char *name, const C& csr) {
  using T = decltype(csr.sourceOffsets[0]);
  using U = decltype(csr.destinationIndices[0]);
  const auto& vto = csr.sourceOffsets;
  const auto& eto = csr.destinationIndices;
  size_t so = vto.size(), di = eto.size(), sz = so*sizeof(T) + di*sizeof(U);
  printf("[%zu bytes %zu source-offsets %zu destination-indices] %s\n", sz, so, di, name);
}


template <class G, class J>
void runCsr(const G& x, J&& ks) {
  G empty;
  vector<int> none;
  int S = x.span();

  // Find space usage of 32bit regular CSR.
  auto csr1 = csr(x, ks, uint32_t());
  runCsrPrint("csrRegular32", csr1);

  // Find space usage of 64bit regular CSR.
  auto csr2 = csr(x, ks, uint64_t());
  runCsrPrint("csrRegular64", csr2);

  // Find space usage of 32bit hybrid CSR with 4bit block, 28bit index (30 eff.).
  bool has3 = S < (1L<<30);
  auto csr3 = hybridCsr(has3? x:empty, has3? ks:none, uint32_t(4));
  runCsrPrint("csrHybrid32 [4bit block, 28bit index (30 eff.)]", csr3);

  // Find space usage of 32bit hybrid CSR with 8bit block, 24bit index (27 eff.).
  bool has4 = S < (1L<<27);
  auto csr4 = hybridCsr(has4? x:empty, has4? ks:none, uint32_t(8));
  runCsrPrint("csrHybrid32 [8bit block, 24bit index (27 eff.)]", csr4);

  // Find space usage of 32bit hybrid CSR with 16bit block, 16bit index (20 eff.).
  bool has5 = S < (1L<<20);
  auto csr5 = hybridCsr(has5? x:empty, has5? ks:none, uint32_t(16));
  runCsrPrint("csrHybrid32 [16bit block, 16bit index (20 eff.)]", csr5);

  // Find space usage of 64bit hybrid CSR with 4bit block, 60bit index (62 eff.).
  bool has6 = S < (1L<<62);
  auto csr6 = hybridCsr(has6? x:empty, has6? ks:none, uint64_t(4));
  runCsrPrint("csrHybrid64 [4bit block, 60bit index (62 eff.)]", csr6);

  // Find space usage of 64bit hybrid CSR with 8bit block, 56bit index (59 eff.).
  bool has7 = S < (1L<<59);
  auto csr7 = hybridCsr(has7? x:empty, has7? ks:none, uint64_t(8));
  runCsrPrint("csrHybrid64 [8bit block, 56bit index (59 eff.)]", csr7);

  // Find space usage of 64bit hybrid CSR with 16bit block, 48bit index (52 eff.).
  bool has8 = S < (1L<<52);
  auto csr8 = hybridCsr(has8? x:empty, has8? ks:none, uint64_t(16));
  runCsrPrint("csrHybrid64 [16bit block, 48bit index (52 eff.)]", csr8);

  // Find space usage of 64bit hybrid CSR with 32bit block, 32bit index (37 eff.).
  bool has9 = S < (1L<<37);
  auto csr9 = hybridCsr(has9? x:empty, has9? ks:none, uint64_t(32));
  runCsrPrint("csrHybrid64 [32bit block, 32bit index (37 eff.)]", csr9);
}


int main(int argc, char **argv) {
  char *file = argv[1];
  printf("Loading graph %s ...\n", file);
  auto x  = readMtx(file); println(x);
  auto xt = transposeWithDegree(x); print(xt); printf(" (transposeWithDegree)\n");
  auto fl = [](const auto& a, const auto& b) { return a.size() < b.size(); };
  auto cs = components(x, xt);
  size_t cmin = (*minElement(cs, fl)).size();
  size_t cmax = (*maxElement(cs, fl)).size();
  printf("components: %zu min-size: %zu max-size: %zu\n", cs.size(), cmin, cmax);

  printf("\n# Components: unsorted\n");
  auto ks0 = join(cs);
  runCsr(x, ks0);

  printf("\n# Components: sorted\n");
  auto ks1 = join(sortedComponents(x, cs));
  runCsr(x, ks1);

  printf("\n");
  return 0;
}
