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


template <class G>
void runCsr(const G& x, bool show) {
  int S = x.span();
  G empty;

  // Find space usage of regular CSR.
  auto csr1 = csr(x);
  runCsrPrint("csrRegular", csr1);

  // Find space usage of 32bit hybrid CSR with 4bit block, 28bit index (30 eff.).
  auto csr2 = hybridCsr(S < (1L<<30)? x:empty, uint32_t(4));
  runCsrPrint("csrHybrid32 [4bit block, 28bit index (30 eff.)]", csr2);

  // Find space usage of 32bit hybrid CSR with 8bit block, 24bit index (27 eff.).
  auto csr3 = hybridCsr(S < (1L<<27)? x:empty, uint32_t(8));
  runCsrPrint("csrHybrid32 [8bit block, 24bit index (27 eff.)]", csr3);

  // Find space usage of 32bit hybrid CSR with 16bit block, 16bit index (20 eff.).
  auto csr4 = hybridCsr(S < (1L<<20)? x:empty, uint32_t(16));
  runCsrPrint("csrHybrid32 [16bit block, 16bit index (20 eff.)]", csr4);

  // Find space usage of 64bit hybrid CSR with 4bit block, 60bit index (62 eff.).
  auto csr5 = hybridCsr(S < (1L<<62)? x:empty, uint64_t(4));
  runCsrPrint("csrHybrid64 [4bit block, 60bit index (62 eff.)]", csr5);

  // Find space usage of 64bit hybrid CSR with 8bit block, 56bit index (59 eff.).
  auto csr6 = hybridCsr(S < (1L<<59)? x:empty, uint64_t(8));
  runCsrPrint("csrHybrid64 [8bit block, 56bit index (59 eff.)]", csr6);

  // Find space usage of 64bit hybrid CSR with 16bit block, 48bit index (52 eff.).
  auto csr7 = hybridCsr(S < (1L<<52)? x:empty, uint64_t(16));
  runCsrPrint("csrHybrid64 [16bit block, 48bit index (52 eff.)]", csr7);

  // Find space usage of 64bit hybrid CSR with 32bit block, 32bit index (37 eff.).
  auto csr8 = hybridCsr(S < (1L<<37)? x:empty, uint64_t(32));
  runCsrPrint("csrHybrid64 [32bit block, 32bit index (37 eff.)]", csr8);
}


int main(int argc, char **argv) {
  char *file = argv[1];
  bool  show = argc > 2;
  printf("Loading graph %s ...\n", file);
  auto x = readMtx(file); println(x);
  runCsr(x, show);
  printf("\n");
  return 0;
}
