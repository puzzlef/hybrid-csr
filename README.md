Comparing space usage of regular vs hybrid CSR (various sizes).

This experiment was for comparing the space usage between:
1. **32bit Regular CSR**.
2. **32bit Hybrid CSR** with **4bit block**, 28bit index (30 eff.).
3. **32bit Hybrid CSR** with **8bit block**, 24bit index (27 eff.).
4. **32bit Hybrid CSR** with **16bit block**, 16bit index (20 eff.).
5. **64bit Hybrid CSR** with **4bit block**, 60bit index (62 eff.).
6. **64bit Hybrid CSR** with **8bit block**, 56bit index (59 eff.).
7. **64bit Hybrid CSR** with **16bit block**, 48bit index (52 eff.).
8. **64bit Hybrid CSR** with **32bit block**, 32bit index (37 eff.).

The purpose of this experiment to assess the **size needed for graph representation**
for various possible hybrid CSR formats, by adjusting the *size of dense bitset*
*(block)*, and hence the *index-bits*. Both *32-bit* and *64-bit hybrid CSR* are
studied, and compared with *32-bit regular CSR*. A **32-bit regular CSR** is
represented using a *uint32_t* data type, and uses *all 32 bits* for *vertex-index*
(*index-bits*). It can support graphs with a maximum of *2^32* vertices (or simply
a 32-bit vertex-id). A **32-bit hybrid CSR** is also represented using a *uint32_t*
data type, where *lower b bits* are used to store the *dense bitset* (*block*), and
*upper i = 32-b bits* to store the *index-bits*. It supports an *effective vertex-id*
of *i+log2(b) = 32-b+log2(b) bits*. For this experiment, the size of block *b* is
adjusted from *4 to 16 bits*. Similarly, a **64-bit hybrid CSR** is represented
using *uint64_t* data type, where *lower b bits* are used to store the *dense bitset*
(*block*) and *upper i = 64-b bits* to store the *index-bits*. Hence, the
*effective vertex-id* supported is of *i+log2(b) = 64-b+log2(b) bits*. For this
experiment, the size of block *b* is adjusted from *4 to 32 bits*. For a given
*vertex-id* *v* , [*index-bits*] are defined as *v >> b* , [*block-bits*]
are defined as *1 << (v & ones(b))*, and thus the [*hybrid CSR entry*] is
*(index-bits << b) | block-bits*. Finding an *edge-id* in an *edge-list*
involves scanning all entries with *matching index-bits*, and once matched,
checking if the *appropriate block-bit is set* (for both hybrid CSRs). Since
lowering the number of *index-bits* reduces the maximum possible order of graph
representable by the format, the *effective bits* usable for *vertex-id* for each
hybrid CSR variation is listed for reference. For this experiment, *edge-ids* for
each graph are first loaded into a *32-bit array of arrays structure*, and then
converted to the desired CSR formats.

All graphs used are stored in the *MatrixMarket (.mtx)* file format, and obtained
from the *SuiteSparse Matrix Collection*. The experiment is implemented in *C++*,
and compiled using *GCC 9* with *optimization level 3 (-O3)*. The system used is a
*Dell PowerEdge R740 Rack server* with two *Intel Xeon Silver 4116 CPUs @ 2.10GHz*,
*128GB DIMM DDR4 Synchronous Registered (Buffered) 2666 MHz (8x16GB) DRAM*, and
running *CentOS Linux release 7.9.2009 (Core)*. Statistics of each test case is
printed to *standard output (stdout)*, and redirected to a *log file*, which is
then processed with a *script* to generate a *CSV file*, with each *row* representing
the details of a *single test case*. This *CSV file* is imported into *Google Sheets*,
and necessary tables are set up with the help of the *FILTER* function to create
the *charts*.

It is observed that for a given **n-bit hybrid CSR using the highest possible**
**block size** (taking into account *effective index-bits*) results in the
**smallest space usage**. The **32-bit hybrid CSR** with a *16-bit block* is able
to achieve a maximum *space usage (bytes)* reduction of *~5x*, but is *unable* to
represent all the graphs under test (it has a *20-bit effective vertex-id*). With
an *8-bit block* the space usage is reduced by *~3x - 3.5x* for *coPapersCiteseer*,
*coPapersDBLP*, and *indochina-2004*. The **64-bit hybrid CSR** with a *32-bit*
*block* is able to achieve a maximum *space usage* reduction of *~3.5x*, but generally
does not perform well. However, for *massive graphs* which *can not* be represented
with a *32-bit vertex-id*, it is likely to provide significant reduction in space
usage. This can be gauged by comparing the number of destination-indices needed
for each CSR variant, where it achieves a maximum *destination-indices* reduction
of *~7x*. This reduction is likely to be higher with graphs partitioned by
*hosts / heuristics / clustering algorithms* which is usually necessary for
massive graphs deployed in a distributed setting. This could be assessed in
a future study.

All outputs are saved in [out](out/) and a small part of the output is listed
here. Some [charts] are also included below, generated from [sheets]. The input
data used for this experiment is available at ["graphs"] (for small ones), and
the [SuiteSparse Matrix Collection]. This experiment was done with guidance
from [Prof. Dip Sankar Banerjee] and [Prof. Kishore Kothapalli].

<br>

```bash
$ g++ -O3 main.cxx
$ ./a.out ~/data/min-1DeadEnd.mtx
$ ./a.out ~/data/min-2SCC.mtx
$ ...

# ...
#
# Loading graph /home/subhajit/data/web-Stanford.mtx ...
# order: 281903 size: 2312497 {}
# [10377604 bytes 281904 source-offsets 2312497 destination-indices] csrRegular
# [10377080 bytes 281904 source-offsets 2312366 destination-indices] csrHybrid32 [4bit block, 28bit index (30 eff.)]
# [10376488 bytes 281904 source-offsets 2312218 destination-indices] csrHybrid32 [8bit block, 24bit index (27 eff.)]
# [10374184 bytes 281904 source-offsets 2311642 destination-indices] csrHybrid32 [16bit block, 16bit index (20 eff.)]
# [19626544 bytes 281904 source-offsets 2312366 destination-indices] csrHybrid64 [4bit block, 60bit index (62 eff.)]
# [19625360 bytes 281904 source-offsets 2312218 destination-indices] csrHybrid64 [8bit block, 56bit index (59 eff.)]
# [19620752 bytes 281904 source-offsets 2311642 destination-indices] csrHybrid64 [16bit block, 48bit index (52 eff.)]
# [19604088 bytes 281904 source-offsets 2309559 destination-indices] csrHybrid64 [32bit block, 32bit index (37 eff.)]
#
# ...
#
# Loading graph /home/subhajit/data/soc-LiveJournal1.mtx ...
# order: 4847571 size: 68993773 {}
# [295365380 bytes 4847572 source-offsets 68993773 destination-indices] csrRegular
# [259873684 bytes 4847572 source-offsets 60120849 destination-indices] csrHybrid32 [4bit block, 28bit index (30 eff.)]
# [246424504 bytes 4847572 source-offsets 56758554 destination-indices] csrHybrid32 [8bit block, 24bit index (27 eff.)]
# [4 bytes 1 source-offsets 0 destination-indices] csrHybrid32 [16bit block, 16bit index (20 eff.)]
# [500357080 bytes 4847572 source-offsets 60120849 destination-indices] csrHybrid64 [4bit block, 60bit index (62 eff.)]
# [473458720 bytes 4847572 source-offsets 56758554 destination-indices] csrHybrid64 [8bit block, 56bit index (59 eff.)]
# [450554736 bytes 4847572 source-offsets 53895556 destination-indices] csrHybrid64 [16bit block, 48bit index (52 eff.)]
# [431781256 bytes 4847572 source-offsets 51548871 destination-indices] csrHybrid64 [32bit block, 32bit index (37 eff.)]
#
# ...
```

[![](https://i.imgur.com/xg39RO0.png)][sheets]
[![](https://i.imgur.com/NkfDg7g.png)][sheets]
[![](https://i.imgur.com/wCQkoh0.png)][sheets]
[![](https://i.imgur.com/UGRum3D.png)][sheets]
[![](https://i.imgur.com/HDdZ1Fv.png)][sheets]
[![](https://i.imgur.com/xnd2Sgj.png)][sheets]

<br>
<br>


## References

- [Parallel algorithms for multi-source graph traversal and its applications](https://www.slideshare.net/SubhajitSahu/parallel-algorithms-for-multisource-graph-traversal-and-its-applications)
- [SuiteSparse Matrix Collection]

<br>
<br>

[![](https://i.imgur.com/eR6BeVh.jpg)](https://www.youtube.com/watch?v=1yoDFJ-JSag)

[Prof. Dip Sankar Banerjee]: https://sites.google.com/site/dipsankarban/
[Prof. Kishore Kothapalli]: https://cstar.iiit.ac.in/~kkishore/
[SuiteSparse Matrix Collection]: https://suitesparse-collection-website.herokuapp.com
["graphs"]: https://github.com/puzzlef/graphs
[*index-bits*]: https://github.com/puzzlef/csr-regular-vs-hybrid/blob/main/src/csr.hxx#L161
[*block-bits*]: https://github.com/puzzlef/csr-regular-vs-hybrid/blob/main/src/csr.hxx#L166
[*hybrid CSR entry*]: https://github.com/puzzlef/csr-regular-vs-hybrid/blob/main/src/csr.hxx#L171
[charts]: https://photos.app.goo.gl/AXEesDgbxegtmus16
[sheets]: https://docs.google.com/spreadsheets/d/1MImPpZw*Hgrq0*BNvmcIZ3aj-cWHNcM0zBqsNvuyFXI/edit?usp=sharing
