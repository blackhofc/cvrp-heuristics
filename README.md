# CVRP-Heuristics

A small C++20 toolkit that

* reads VRPLIB/TSPLIB CVRP instances
* builds an initial solution with either Clarke–Wright (cw) or Nearest-Neighbor (nn)
* improves that solution with a configurable local-search pipeline (Relocation 1‑0, Swap 1‑1, 2‑Opt intra‑route)
* prints results in classic VRPLIB “SOLUTION\_SECTION” layout
* self‑tests with two lightweight, assert‑based unit tests

---

## 1 · Build

| Prerequisite   | Version used                   |
| -------------- | ------------------------------ |
| C++20 compiler | g++ 13 / clang 15 / MSVC 19.38 |
| GNU make       | 4.4+                           |
| External libs  | none                           |

```bash
# release binary: bin/cvrp-cli
make

# compile & run unit tests
make test
```

`make clean` removes objects and binaries.

---

## 2 · Command‑line usage

```
bin/cvrp-cli <instance>
             [--heuristic cw|nn]
             [--ls none|reloc|swap|2opt|all]
```

| option        | default | meaning                                                             |
| ------------- | ------- | ------------------------------------------------------------------- |
| `--heuristic` | `cw`    | `cw` = Clarke–Wright,  `nn` = Nearest‑Neighbor                      |
| `--ls`        | `all`   | choose local‑search bundle (`none`, `reloc`, `swap`, `2opt`, `all`) |

Examples

```bash
# Clarke‑Wright + full LS
./bin/cvrp-cli data/E045-04f.dat

# Nearest‑Neighbor, only Relocation
./bin/cvrp-cli data/E045-04f.dat --heuristic nn --ls reloc
```

Each run prints:

```
NAME    : <instance>
ROUTES  : <number of routes>
COST    : <total distance>
CONFIG  : heuristic=<...>, ls=<...>
TIME_MS : <wall‑clock runtime>
```

---

## 3 · Algorithms

| stage        | variants                                            |
| ------------ | --------------------------------------------------- |
| Constructive | Clarke‑Wright Savings (parallel) · Nearest‑Neighbor |
| Local search | Relocation 1‑0 · Swap 1‑1 · 2‑Opt (intra‑route)     |

### Complexity

| Component     | Big‑O | Notes |
| ------------- | ----- | ----- |
| Clarke‑Wright |       |       |

| **O(n² log n)**  | n² savings + sort |                         |
| ---------------- | ----------------- | ----------------------- |
| Nearest‑Neighbor | **O(n²)**         | greedy nearest pick     |
| Relocation 1‑0   | **O(r · n²)**     | inspect every insertion |
| Swap 1‑1         | **O(r · n²)**     | inspect unordered pairs |
| 2‑Opt            | **O(r · n²)**     | nested loops per route  |

All LS operators use *first‑improvement* with ε = 10⁻⁴, so each accepted move strictly lowers total cost ⇒ finite termination.

---

## 4 · Unit tests

| file                            | what it checks                                                            |
| ------------------------------- | ------------------------------------------------------------------------- |
| `operators_safe.cpp`            | after full LS: no capacity violation & each customer appears exactly once |
| `solution_cost_consistency.cpp` | `Solution::totalCost()` equals sum of route lengths                       |

Run:

```bash
make test
```

Successful output:

```
Running bin/test-operators_safe
  ✓ passed
Running bin/test-solution_cost_consistency
  ✓ passed
All tests passed.
```

Tests rely only on `<cassert>` (no external framework).

---

## 5 · Repository layout

```
include/            public headers
src/                library & algorithms
apps/main.cpp       CLI driver
tests/              two assert-based tests
data/               sample VRPLIB instances
Makefile            build + test targets
README.md           this file
```

---

## 6 · References

* Clarke, G. & Wright, J. W. (1964). *Scheduling of Vehicles from a Central Depot…* Transportation Science.
* Fisher, M. L. (1994). *Optimal Solution of Vehicle‑Routing Problems Using Minimum k‑trees.*

---

## 7 · License

MIT — see `LICENSE`.
