# InfinityLearn

**InfinityLearn** is a C++ machine learning systems project focused on implementing core ML algorithms from first principles, then using them to build a concrete end-to-end ML demonstration project.

[![CI](https://github.com/SahilSingh-Code/InfinityLearn/actions/workflows/ci.yml/badge.svg?branch=develop)](https://github.com/SahilSingh-Code/InfinityLearn/actions/workflows/ci.yml)
[![Documentation](https://img.shields.io/badge/docs-GitHub%20Pages-blue)](https://sahilsingh-code.github.io/InfinityLearn/)

[Documentation](https://sahilsingh-code.github.io/InfinityLearn/) ·
[Design Principles](https://sahilsingh-code.github.io/InfinityLearn/design_principles.html) ·
[Roadmap](https://sahilsingh-code.github.io/InfinityLearn/roadmap.html)

The goal of this repository is to demonstrate deep understanding of:

* machine learning mechanics and optimization
* numerical correctness and stability
* reproducible experimentation
* performance-aware C++ systems engineering
* end-to-end ML project development

This is not a wrapper around existing ML libraries. It is an intentionally explicit implementation designed to reason about how learning systems actually work, from low-level math primitives through model training, evaluation, diagnostics, and final application.

---

## Why this project exists

Most ML codebases emphasize rapid experimentation using high-level frameworks. InfinityLearn takes the opposite approach:

* implement core algorithms explicitly
* validate correctness with tests and invariants
* make numerical assumptions visible
* measure performance deliberately
* build toward a final applied ML project

This project serves both as a learning vehicle and as a demonstration of senior-level ML systems engineering.

The intended final outcome is not just a set of algorithms. The repository is being built as a foundation for a small but complete ML demonstration project that shows the system working end-to-end.

---

## Project timeline

The project began on **June 15, 2026** and targets a three-month development window ending on **September 15, 2026**.

The roadmap is organized around capability milestones:

1. Infrastructure and documentation
2. Core math and data primitives
3. Optimization and learning loops
4. Classical ML models
5. Evaluation and reproducible experiments
6. Numerical stability and diagnostics
7. Performance-focused systems work
8. Final applied ML demonstration project

See the [Roadmap](https://sahilsingh-code.github.io/InfinityLearn/roadmap.html) for details.

---

## Current status

**Infrastructure**

* [x] CMake-based build system
* [x] Formatting and static checks
* [x] Unit test harness
* [x] Logging / timing / error utilities
* [x] Deterministic randomness utilities
* [x] Continuous integration
* [x] Hosted Doxygen documentation

**Core Math / Data Primitives**

* [x] Initial vector primitive
* [ ] Matrix primitive
* [ ] Shape and dimension validation
* [ ] Core reductions and elementwise operations
* [ ] Matrix/vector operations
* [ ] Deterministic dataset generators

**Core ML**

* [ ] Linear regression
* [ ] Logistic regression
* [ ] k-means clustering
* [ ] Gaussian Mixture Models using EM
* [ ] PCA
* [ ] Basic neural-network primitives

**Evaluation & Experiments**

* [ ] Training / validation splits
* [ ] Metrics: MSE, log loss, accuracy, reconstruction error
* [ ] Experiment runner
* [ ] Reproducible experiment logs
* [ ] Benchmark harness

**Final Project**

* [ ] Select final ML demonstration project
* [ ] Build end-to-end training/evaluation pipeline
* [ ] Document results and limitations
* [ ] Publish final project milestone release

---

## Planned final project

InfinityLearn is being built toward a final applied ML demonstration.

The final project should be small enough to complete within the project timeline, but substantial enough to demonstrate:

* data generation or ingestion
* model training
* evaluation
* reproducibility
* diagnostics
* performance awareness
* clear documentation

Candidate final projects include:

* a small neural-network demo,
* a TinyGPT-style character or token model,
* a physics-inspired ML demo using synthetic simulation data,
* or a polished classical ML showcase with reproducible experiments and performance notes.

The preferred direction is a final project that connects C++ ML systems engineering with numerical simulation or physics-inspired data, because that best reflects the author's background and strengths.

---

## Engineering principles

InfinityLearn emphasizes:

* explicit implementation over framework wrapping
* correctness through tests and invariants
* deterministic and reproducible experiments
* readable, maintainable C++ design
* numerical stability as a first-class concern
* measured performance instead of premature optimization
* professional workflow through CI, documentation, issues, and pull requests

---

## Quickstart

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```
