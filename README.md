Comparing space usage of regular vs hybrid CSR (various sizes).

This experiment was for comparing the space usage between:
1. **Regular CSR**.
2. **32bit Hybrid CSR** with **4bit block**, 28bit index (30 eff.).
3. **32bit Hybrid CSR** with **8bit block**, 24bit index (27 eff.).
4. **32bit Hybrid CSR** with **16bit block**, 16bit index (20 eff.).
5. **64bit Hybrid CSR** with **4bit block**, 60bit index (62 eff.).
6. **64bit Hybrid CSR** with **8bit block**, 56bit index (59 eff.).
7. **64bit Hybrid CSR** with **16bit block**, 48bit index (52 eff.).
8. **64bit Hybrid CSR** with **32bit block**, 32bit index (37 eff.).

Each datatype was attempted on different types of graphs, recording the no. of
*source-offsets*, *destination-indices*, and *total size in bytes*. The no. of
*source-offsets* is always one more that the order of graph, because it also
includes the final offset. As expected, for a given n-bit hybrid CSR using the
highest possible block size (taking into account effective index bits) results
is smallest space usage. For `coPapersCiteseer`, `coPapersDBLP`, and
`indochina-2004`, the *space usage* is **reduced by more than 50%** using a
**32bit hybrid CSR**. 64bit hybrid CSR supposed to be useful when the order
of a graph is more than 32bits.

All outputs are saved in [out](out/) and a small part of the output is listed
here. Some [charts] are also included below, generated from [sheets]. The input
data used for this experiment is available at ["graphs"] (for small ones), and
the [SuiteSparse Matrix Collection]. This experiment was done with guidance
from [Prof. Dip Sankar Banerjee] and [Prof. Kishore Kothapalli].

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

[![](https://i.imgur.com/wNVWLor.gif)][sheets]
[![](https://i.imgur.com/iykY5NL.gif)][sheets]
[![](https://i.imgur.com/9F7y0jX.gif)][sheets]

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
[charts]: https://photos.app.goo.gl/AXEesDgbxegtmus16
[sheets]: https://docs.google.com/spreadsheets/d/1MImPpZw_Hgrq0_BNvmcIZ3aj-cWHNcM0zBqsNvuyFXI/edit?usp=sharing
