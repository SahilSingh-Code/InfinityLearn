# Roadmap {#roadmap}

InfinityLearn is a C++ ML systems project focused on building a small machine learning library from first principles, then using it to ship a concrete end-to-end ML demonstration project.

The roadmap is capability-driven: each phase produces concrete, runnable artifacts that build toward a final project demonstrating ML understanding, numerical discipline, and production-quality C++ engineering.

The guiding priorities are:

* **Correctness**
* **Numerical stability**
* **Testability / modularity**
* **Reproducible experimentation**
* **Measured performance**
* **Professional engineering workflow**
* **A final applied ML project that proves the system works end-to-end**

The start date for this project is **June 15, 2026**, and the target end date is **September 15, 2026**.

---

## End-state objective

By the end of the project, InfinityLearn should demonstrate that the author can build an ML system end-to-end:

1. Implement the required math and model primitives from first principles.
2. Validate correctness with tests, invariants, and deterministic experiments.
3. Train and evaluate classical ML models.
4. Measure and improve performance where it matters.
5. Use the resulting infrastructure to build a final applied ML demo.

The final story should be:

> "This engineer can build ML systems end-to-end, from numerical foundations to a working applied model, with strong software engineering discipline."

---

## Phase 0 — Infrastructure foundation

**Theme:** Make the repository a safe place to build for months.

### Goals

* Remove development friction early.
* Enable disciplined iteration through formatting, tests, CI, documentation, and review workflow.
* Provide minimal utilities needed for ML experimentation: logging, timing, error handling, deterministic randomness, and test helpers.
* Establish professional repository hygiene before implementing ML code.

### Capabilities

* CMake project hierarchy.
* Formatting and static checks.
* Unit test harness.
* Logging, timing, error, and assertion utilities.
* Deterministic randomness utilities.
* Continuous integration on pull requests.
* Hosted Doxygen documentation.
* Branch and PR workflow suitable for disciplined development.

### Exit criteria

This phase is complete when:

* CI is green and required on PRs.
* Adding a new test is trivial.
* Documentation builds automatically from `develop`.
* The repository is ready for sustained ML development without tooling friction.

---

## Phase 1 — Core math and data primitives

**Theme:** Express ML computations explicitly and cleanly.

### Goals

* Own the core numerical building blocks instead of wrapping an existing ML framework.
* Provide enough math infrastructure to implement classical ML algorithms cleanly.
* Design primitives that are understandable, testable, and performance-aware.

### Capabilities

* Vector and matrix abstractions with explicit size and shape handling.
* Static and dynamic storage where appropriate.
* Core operations: dot product, norms, normalization, elementwise operations, reductions, and matrix/vector operations.
* Shape and bounds invariants.
* Deterministic synthetic dataset generators for small controlled experiments.
* Clear separation between math primitives and ML models.

### Deliverables

* Unit tests for vector and matrix correctness.
* Forward-pass examples using math primitives.
* Basic loss computation utilities.
* Documentation for design decisions and intended usage.

---

## Phase 2 — Optimization and learning loop

**Theme:** Teach the system how to learn, with observable behavior.

### Goals

* Implement learning mechanics explicitly.
* Make training behavior debuggable and reproducible.
* Establish the core training loop that later models can reuse.

### Capabilities

* Batch gradient descent.
* Loss functions: mean squared error and binary cross entropy.
* Learning rate handling.
* Convergence criteria and iteration limits.
* Training logs for loss and iteration count.
* Numerical guards for NaN, Inf, divide-by-zero, and unstable updates.

### Deliverables

* Trainable linear regression using gradient descent.
* Loss decreasing on deterministic datasets.
* Tests validating expected learning behavior.
* Minimal training executable with reproducible output.

---

## Phase 3 — Core classical ML models

**Theme:** Build high-signal ML depth before moving to a final demo.

### Models

Recommended implementation order:

1. Linear regression, closed form and gradient descent.
2. Logistic regression with stable sigmoid and binary cross entropy.
3. k-means clustering with deterministic initialization and empty-cluster handling.
4. Gaussian Mixture Models using EM, log-sum-exp, and covariance regularization.
5. PCA with projection, reconstruction, and explained variance.

### Goals

* Demonstrate understanding of model mechanics, not just API usage.
* Handle numerical and edge-case behavior deliberately.
* Produce runnable examples that clearly show each model working.

### For each model

* Clean implementation.
* Deterministic example executable.
* Unit tests for invariants and edge cases.
* Notes on assumptions, limitations, and failure modes.
* Evaluation metrics where applicable.

### Deliverables

* Example executable per model.
* Tests covering model behavior.
* Documentation explaining how to run each model.
* First meaningful benchmarks for selected algorithms.

---

## Phase 4 — Evaluation and experimentation

**Theme:** Validate models like an ML engineer.

### Goals

* Move beyond "it runs" into reproducible, inspectable experiments.
* Make model behavior comparable across datasets, seeds, and hyperparameters.
* Establish the infrastructure needed for the final project.

### Capabilities

* Train/validation split utilities.
* Metrics: MSE, log loss, accuracy, reconstruction error, and clustering diagnostics.
* Experiment runner executable.
* Logged configuration: dataset parameters, seeds, model settings, and hyperparameters.
* Repeatable runs with fixed seeds.
* Optional baseline comparison against a reference implementation.

### Deliverables

* End-to-end experiment pipeline for multiple models.
* Reproducible experiment outputs.
* Documented example commands.
* Clear record of model behavior and limitations.

---

## Phase 5 — Numerical stability and diagnostics

**Theme:** Make failure modes explicit and handled.

### Goals

* Demonstrate maturity around numerical edge cases.
* Catch instability early.
* Document tradeoffs and assumptions.

### Capabilities

* Stable formulations for likelihood-based models.
* Log-sum-exp where appropriate.
* Epsilon handling and clipping where justified.
* Conditioning and regularization strategies.
* Divergence detection.
* Invalid covariance and degenerate-data checks.
* Tests targeting numerical edge cases.

### Deliverables

* Stability-focused tests.
* Documentation for numerical safeguards.
* Before/after notes showing why stability improvements matter.
* Cleaner failure messages for invalid model states.

---

## Phase 6 — Performance and systems perspective

**Theme:** Treat ML as a systems problem.

### Goals

* Measure before optimizing.
* Improve performance where the evidence justifies it.
* Show systems-level thinking in memory layout, parallelization, and benchmarking.

### Possible directions

Pick a focused subset rather than trying to optimize everything:

* Cache-friendly vector and matrix operations.
* Multithreaded CPU execution for large operations.
* Benchmark harness for core math and selected ML algorithms.
* Allocation discipline and memory profiling.
* Regression tracking for performance-sensitive operations.
* Optional packaging improvements.
* Optional thin Python bindings if they support the final demo.

### Deliverables

* Benchmarks with documented methodology.
* Evidence-backed performance improvements.
* Clear distinction between correctness tests and performance tests.
* Engineering highlights suitable for the README.

---

## Phase 7 — Final ML demonstration project

**Theme:** Use InfinityLearn to build something concrete.

### Goal

Build a final applied ML project that demonstrates the library working as a coherent system, not just a collection of isolated algorithms.

The final project should be small enough to complete, but substantial enough to show:

* data generation or ingestion,
* model training,
* evaluation,
* reproducibility,
* diagnostics,
* documentation,
* and a clear result.

### Candidate final projects

Potential directions include:

1. **Tiny neural network demo**

   * A small neural network trained on a simple dataset.
   * Demonstrates forward pass, loss, backpropagation, training, and evaluation.

2. **TinyGPT-style language model**

   * A deliberately small character-level or token-level model.
   * Demonstrates sequence modeling, embeddings, training loops, and sampling.
   * Stretch goal only if the math and tensor infrastructure are ready.

3. **Physics-inspired ML demo**

   * A small model trained on synthetic physics/simulation data.
   * Example: learning a simple dynamical system, regression on generated physical data, or clustering simulation states.
   * Strong alignment with the author's physics and simulation background.

4. **Classical ML showcase**

   * A polished experiment comparing implemented models on deterministic datasets.
   * Includes metrics, plots/logs, failure cases, and performance notes.

### Recommended target

The preferred final target is a **small end-to-end ML demo that connects the author's background in numerical simulation with machine learning systems**.

A physics-inspired ML project or a small neural-network project would make the strongest portfolio story because it connects:

* C++ systems engineering,
* numerical methods,
* ML fundamentals,
* reproducible experimentation,
* and the author's existing simulation expertise.

### Deliverables

* Final demo executable.
* Clear README section explaining the demo.
* Reproducible run instructions.
* Example output.
* Evaluation results.
* Engineering notes describing architecture, limitations, and future work.
* Release tag marking the final project milestone.

---

## Final repository story

At the end of the three-month project, the repository should demonstrate:

* Deep implementation of ML fundamentals.
* Correctness and numerical stability discipline.
* Modular C++ design.
* Strong test coverage where it matters.
* Reproducible experiments.
* Measured performance improvements.
* Hosted documentation.
* A concrete final ML project proving the system works end-to-end.

The intended reviewer takeaway is:

> "This is not a toy repo. This is a serious ML systems portfolio project built with the engineering standards of someone who can own production-quality numerical software."
