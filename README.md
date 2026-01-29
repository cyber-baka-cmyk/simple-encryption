# Simple Shuffle Cipher  
An encryption algorithm with custom shuffle rounds, XOR-based encryption, tests, benchmarks, and CI.

This project is a **learning‑oriented encryption experiment** written in C.  
It implements:

- a custom 4×4 state shuffle inspired by ChaCha‑style quarter rounds  
- a 64‑byte key generator using OS randomness  
- simple XOR‑based file encryption/decryption  
- GoogleTest‑based test suite (avalanche, structure, timing)  
- a benchmark tool  
- GitHub Actions CI pipeline  
- secure memory wiping  

> **Important:**  
> This is **not a secure cryptographic algorithm** and must **not** be used to protect real data.  
> It is a **pet project for learning**, experimentation, and exploring how block operations, randomness, and testing work in practice.

---

---

## Features

### Key Generation
- 64‑byte key derived from OS randomness (`/dev/urandom`)
- 4×4 matrix of 32‑bit words
- custom shuffle rounds for diffusion
- secure memory wiping after use

### Encryption / Decryption
- XOR‑based block cipher (64‑byte blocks)
- binary output + hex‑dump output

### Test Suite (GoogleTest)
- **Avalanche Test** — checks diffusion when flipping 1 bit  
- **Structure Test** — ensures shuffle changes non‑zero states  
- **Side‑Channel Timing Test** — checks for major timing variance  

### Benchmark
- Measures XOR throughput  
- Useful for comparing optimizations

### GitHub Actions CI
- Builds project  
- Runs tests  
- Ensures code stays working on every push  

---

## Build Instructions

### Requirements
- GCC
- CMake > 3.16  
- GoogleTest (installed automatically in CI)

### Build & Run (one click)

Use the included script:

```bash
./build_and_run.sh


