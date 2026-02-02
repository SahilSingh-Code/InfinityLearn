# Roadmap

InfinityLearn is a C++ ML systems project. The roadmap is capability-driven: each phase produces concrete, runnable artifacts.

The guiding priorities are:
- **Correctness**
- **Numerical stability**
- **Testability / modularity**
- **Measured performance**
- **Professional engineering workflow**

The start date for this project is February 2, 2026, and the target end date is May 1, 2026 (~3 months).

---

## Phase 0 — Infrastructure foundation (Day 0–2)

**Theme:** Make the repository a safe place to build for months.

### Goals
- Remove development friction early
- Enable disciplined iteration: formatting, tests, CI, review workflow
- Provide minimal utilities needed for ML experimentation (logging/timing/random)

### Capabilities
- CMake project hierarchy (`InfinityLearn` top-level → `Core` subproject)
- clang-format + enforced style
- Basic logging / timing / error utilities
- Deterministic randomness utilities (seed control)
- Unit test harness wired (and easy to extend)
- CI runs on every PR

### Exit criteria
This phase is complete when:
- CI is green and required on PRs
- Adding a new test is trivial
- You can write ML code without worrying about tooling / breakage

---

## Phase 1 — Core math & data primitives (Week 1)

**Theme:** Express ML computations explicitly and cleanly.

### Goals
- Own core math/data layout without premature abstraction
- Provide primitives sufficient for classical ML models

### Capabilities
- Minimal vector/matrix or tensor abstraction with explicit shape handling
- Core ops: dot, matmul, elementwise ops, reductions
- Assertions/invariants for shape and index correctness
- Deterministic synthetic dataset generators (small, test-friendly)

### Deliverables
- Forward-pass example executable (no learning yet)
- Unit tests validating math primitives and loss computations
- Clear separation between math primitives and ML models

---

## Phase 2 — Optimization & learning loop (Week 2)

**Theme:** Teach the system how to learn, with observable behavior.

### Goals
- Implement learning mechanics explicitly (no black boxes)
- Make training debuggable and testable

### Capabilities
- Gradient descent loop (batch)
- Loss functions: MSE and binary cross entropy (BCE)
- Convergence criteria and iteration limits
- Learning rate handling
- Numerical guards (NaN/Inf checks, sensible epsilons)

### Deliverables
- Trainable linear regression (GD), with loss decreasing on known datasets
- Tests validating expected learning behavior (sanity checks, invariants)
- Minimal training logs (loss vs iteration)

---

## Phase 3 — Core classical ML models (Weeks 3–5)

**Theme:** High-signal ML depth.

### Models (order matters)
1. Linear regression (closed form + GD)
2. Logistic regression (stable sigmoid, BCE)
3. k-means (init strategies, empty cluster handling)
4. Gaussian Mixture Models (EM; log-sum-exp; covariance regularization)
5. PCA (SVD-based; interpretation and reconstruction)

### For each model
- Clean implementation in `Core`
- Deterministic runnable example
- Unit tests for invariants and edge cases
- Notes on assumptions and failure modes

### Deliverables
- Example executable per model
- First meaningful benchmarks (at least 2 models)
- Documentation: "how to run" + "what can go wrong"

---

## Phase 4 — Evaluation & experimentation (Weeks 5–7)

**Theme:** Validate models like an ML engineer.

### Goals
- Evaluate properly, not just "it runs"
- Make experiments reproducible and comparable

### Capabilities
- Train/validation split utilities
- Metrics: MSE, log loss, accuracy
- Experiment runner executable (configurable)
- Logged configs: dataset params, seeds, hyperparameters

### Deliverables
- End-to-end experiment pipeline for at least 2 models
- Reproducible runs with fixed seeds
- Optional: baseline comparison against a reference library

---

## Phase 5 — Numerical stability & diagnostics (Weeks 7–9)

**Theme:** Make failure modes explicit and handled.

### Goals
- Prove maturity around numerical edge cases
- Catch and diagnose instability early

### Capabilities
- Stable formulations for likelihood-based models (e.g., log-sum-exp)
- Clipping/epsilons where appropriate (documented)
- Conditioning/regularization strategies (documented)
- Diagnostics: divergence detection, invalid covariance checks, etc.
- Tests targeting numerical edge cases

### Deliverables
- Stability-focused tests and docs
- Before/after benchmark or correctness notes demonstrating improvements

---

## Phase 6 — Performance & systems perspective (Weeks 9–12)

**Theme:** ML as a systems problem.

Pick a subset (depth over breadth).

### Possible directions
- Cache-friendly data layouts
- Multithreaded CPU execution (where it's justified)
- Benchmark regression tracking
- Memory profiling and allocation discipline
- Packaging for reuse (e.g., Conan)
- Thin Python bindings (optional; only if justified)

### Deliverables
- Benchmarks with evidence of improvement
- Release tags with concise changelogs
- "Engineering highlights" section in README

---

## End-state objective (after ~3 months)

The repository should demonstrate:
- Deep classical ML implementations (not toy code)
- Correctness and numerical stability discipline
- Modular design with strong test coverage where it matters
- Reproducible experiments and measurable performance
- Professional workflow: issues, PRs, CI, releases

The story should be:
> "This engineer can build ML systems end-to-end, correctly and efficiently."
