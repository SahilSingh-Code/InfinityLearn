# Design Principles

InfinityLearn is a machine learning systems project. The goal is not breadth of algorithms, but depth of understanding, correctness, and engineering quality. The idea is that any new algorithms should be easy to add once the core principles are established.

This document describes the principles that guide design decisions in this repository and define what “good” means here.

---

## 1. Native, low-level implementation by design

InfinityLearn is implemented in C++ intentionally.

Using a native, low-level language provides:
- direct control over memory layout and lifetime
- explicit visibility into performance costs
- fine-grained optimization opportunities
- ability to reason about behavior close to the underlying machine

This project treats machine learning as a systems problem. Implementing algorithms in C++ forces explicit handling of details that are often hidden in higher-level frameworks, including data movement, numerical precision, and control flow.

High-level libraries may be used later for comparison or interoperability, but the core implementation remains native and explicit.

---

## 2. ML mechanics before abstraction

Core machine learning algorithms are implemented from first principles.

The emphasis is on understanding:
- optimization dynamics
- numerical behavior
- convergence properties
- failure modes

Abstractions are introduced only after the underlying mechanics are well understood and validated. The code favors explicitness over convenience.

If a piece of logic cannot be clearly explained, tested, and reasoned about, it does not belong in the core.

---

## 3. Correctness is non-negotiable

Correctness is the primary design constraint.

Every component is expected to:
- define clear invariants
- validate assumptions explicitly
- fail loudly when violated

Implementations are considered incomplete unless correctness can be demonstrated through reasoning, tests, or both. Performance optimizations never come at the expense of correctness.

---

## 4. Numerical stability is explicit and intentional

Numerical stability is treated as a first-class concern, not an afterthought.

The codebase uses:
- explicit epsilons and tolerances
- stable formulations of loss functions and likelihoods
- guards against NaN and Inf propagation
- documented assumptions about numerical ranges.

Numerical behavior is part of the design, not an emergent property.

---

## 5. Modular, independently testable components

The project is structured so that components are small, composable, and independently testable.

In particular:
- math primitives are isolated from models
- models are isolated from training loops
- training is isolated from evaluation and IO

This separation enables:
- targeted unit testing
- safe refactoring
- controlled evolution of the codebase

No component should require global context to be tested or understood.

---

## 6. Testability over cleverness

Testability is prioritized over clever or compact implementations.

Code should be written so that:
- inputs and outputs are explicit
- behavior is deterministic where possible
- edge cases are easy to exercise

If code is difficult to test, it is considered a design flaw.

---

## 7. Measurement-driven development

Assumptions about performance or convergence are replaced with measurement.

The project includes:
- timing instrumentation
- benchmark harnesses
- regression awareness for performance-critical paths

Optimization is driven by evidence, not intuition.

---

## 8. Reproducibility is a feature

Reproducibility is required for both experiments and tests.

This includes:
- explicit random seeds
- deterministic dataset generation
- reproducible experiment configurations

If a result cannot be reproduced, it is treated as a bug.

---

## 9. Professional engineering hygiene enables velocity

Formatting, CI, testing, and review discipline exist to support sustained development over months.

These practices are not goals in themselves. They exist to:
- reduce friction
- prevent regressions
- enable confident iteration

---

## Non-goals (for now)

The following are intentionally out of scope until simpler baselines are correct and well-understood:

- GPU acceleration
- Automatic differentiation
- Deep learning architectures
- Full tensor broadcasting semantics
- Production-scale data ingestion

These may be added later if justified by concrete use cases.

---

## Definition of done

A change is considered complete when:
- correctness is demonstrated (tests or invariants)
- numerical behavior is understood and documented where relevant
- results are reproducible
- performance implications are known