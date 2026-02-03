# InfinityLearn

**InfinityLearn** is a C++ machine learning project focused on implementing core ML algorithms from first principles with production-quality engineering discipline.

The goal of this repository is to demonstrate deep understanding of:
- machine learning mechanics and optimization
- numerical correctness and stability
- reproducible experimentation
- software engineering practices

This is not a wrapper around existing ML libraries. It is an intentionally minimal, explicit implementation designed to reason about how learning systems actually work.

---

## Why this project exists

Most ML codebases emphasize rapid experimentation using high-level frameworks. InfinityLearn takes the opposite approach:

- implement core algorithms explicitly
- validate correctness with tests and invariants
- measure performance deliberately

This project serves both as a learning vehicle and as a demonstration of senior-level ML systems engineering.

---

## Current status

**Infrastructure**
- [x] CMake-based build system
- [x] Formatting and static checks
- [ ] Unit test harness
- [ ] Logging / timing / error utilities
- [ ] Continuous integration

**Core ML (in progress)**
- [ ] Minimal tensor / math primitives
- [ ] Linear regression
- [ ] Logistic regression
- [ ] k-means clustering
- [ ] Gaussian Mixture Models (EM)
- [ ] PCA

**Evaluation & Experiments**
- [ ] Deterministic dataset generators
- [ ] Training / validation splits
- [ ] Metrics (MSE, log loss, accuracy)
- [ ] Benchmark harness

---

## Quickstart

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build```