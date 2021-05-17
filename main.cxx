#include <cstdint>
#include <cstdio>
#include <iostream>
#include "src/main.hxx"

using namespace std;




template <class C>
void runCsrPrint(const char *name, const vector<int>& vto, const C& eto) {
  using T = typename C::value_type;
  size_t so = vto.size(), di = eto.size(), sz = so*sizeof(int) + di*sizeof(T);
  printf("[%08zu bytes %06zu source-offsets %06zu destination-indices] %s\n", sz, so, di, name);
}

template <class G>
void runCsr(const G& x, bool show) {
  auto vto = sourceOffsets(x);

  // Find space usage of regular CSR.
  auto eto = destinationIndices(x);
  runCsrPrint("csrRegular", vto, eto);

  // Find space usage of 32bit hybrid CSR with 4bit block, 28bit index (30 eff.).
  if (x.span() < (1L << 30)) {
    auto eto = destinationIndicesHybrid(x, uint32_t(4));
    runCsrPrint("csrHybrid32 [4bit block, 28bit index (30 eff.)]", vto, eto);
  }

  // Find space usage of 32bit hybrid CSR with 8bit block, 24bit index (27 eff.).
  if (x.span() < (1L << 27)) {
    auto eto = destinationIndicesHybrid(x, uint32_t(8));
    runCsrPrint("csrHybrid32 [8bit block, 24bit index (27 eff.)]", vto, eto);
  }

  // Find space usage of 32bit hybrid CSR with 16bit block, 16bit index (20 eff.).
  if (x.span() < (1L << 20)) {
    auto eto = destinationIndicesHybrid(x, uint32_t(16));
    runCsrPrint("csrHybrid32 [16bit block, 16bit index (20 eff.)]", vto, eto);
  }

  // Find space usage of 64bit hybrid CSR with 4bit block, 60bit index (62 eff.).
  if (x.span() < (1L << 62)) {
    auto eto = destinationIndicesHybrid(x, uint64_t(4));
    runCsrPrint("csrHybrid64 [4bit block, 60bit index (62 eff.)]", vto, eto);
  }

  // Find space usage of 64bit hybrid CSR with 8bit block, 56bit index (59 eff.).
  if (x.span() < (1L << 59)) {
    auto eto = destinationIndicesHybrid(x, uint64_t(8));
    runCsrPrint("csrHybrid64 [8bit block, 56bit index (59 eff.)]", vto, eto);
  }

  // Find space usage of 64bit hybrid CSR with 16bit block, 48bit index (52 eff.).
  if (x.span() < (1L << 52)) {
    auto eto = destinationIndicesHybrid(x, uint64_t(16));
    runCsrPrint("csrHybrid64 [16bit block, 48bit index (52 eff.)]", vto, eto);
  }

  // Find space usage of 64bit hybrid CSR with 32bit block, 32bit index (37 eff.).
  if (x.span() < (1L << 37)) {
    auto eto = destinationIndicesHybrid(x, uint64_t(32));
    runCsrPrint("csrHybrid64 [32bit block, 32bit index (37 eff.)]", vto, eto);
  }
}


int main(int argc, char **argv) {
  char *file = argv[1];
  bool  show = argc > 2;
  printf("Loading graph %s ...\n", file);
  auto x  = readMtx(file); println(x);
  runCsr(x, show);
  printf("\n");
  return 0;
}
