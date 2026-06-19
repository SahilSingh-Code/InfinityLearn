# InfinityLearn Documentation {#mainpage}

Welcome to the documentation for **InfinityLearn**, a C++ machine learning project focused on implementing core ML algorithms from first principles with production-quality engineering discipline.

InfinityLearn is designed to demonstrate clear reasoning about machine learning mechanics, numerical correctness, reproducibility, performance, and maintainable C++ systems design.

---

## Project Documentation

* \ref design_principles
* \ref roadmap

---

## API Reference

* [Classes](annotated.html)
* [Functions](functions_func.html)

---

## Current Focus

The project is currently focused on building the foundational infrastructure required for a small machine learning library:

* CMake-based build system
* formatting and static checks
* unit test harness
* logging, timing, and error utilities
* continuous integration
* core math and tensor primitives

These foundations are intended to support later implementations of linear regression, logistic regression, clustering algorithms, PCA, and eventually small neural network experiments.

---

## Engineering Goals

InfinityLearn emphasizes:

* explicit implementation over framework wrapping
* correctness through tests and invariants
* deterministic experimentation
* readable and maintainable C++ design
* performance-aware numerical programming

The generated API documentation is intended to make the codebase easy to inspect, review, and extend.
