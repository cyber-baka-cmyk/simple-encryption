# Simple Shuffle Cipher

An encryption algorithm with custom shuffle rounds, XOR-based encryption, Qt GUI, CLI tool, tests, and CI.

This project is a **learning‑oriented encryption experiment** written in C with a C++/Qt GUI.  
It implements:

- a custom 4×4 state shuffle inspired by ChaCha‑style quarter rounds  
- a 64‑byte key generator using OS randomness  
- simple XOR‑based file encryption/decryption  
- **Qt6/Qt5 GUI application** with Encrypt/Decrypt buttons  
- **CLI tool** for command-line usage
- GoogleTest‑based test suite (avalanche, structure, timing)  
- a benchmark tool  
- GitHub Actions CI pipeline  
- secure memory wiping  

> **Important:**  
> This is **not a secure cryptographic algorithm** and must **not** be used to protect real data.  
> It is a **pet project for learning**, experimentation, and exploring how block operations, randomness, and testing work in practice.

---

## Project Structure

```
.
├── CMakeLists.txt              # Build configuration
├── boot.sh                     # Build script
├── README.md                   # This file
├── .github/
│   └── workflows/
│       └── ci.yml              # GitHub Actions CI
├── include/
│   ├── cipher.h                # Cipher API header
│   └── MainWindow.h            # Qt GUI header
├── src/
│   ├── cipher.c                # Cipher implementation
│   ├── main.c                  # CLI entry point
│   ├── main.cpp                # Qt GUI entry point
│   └── MainWindow.cpp          # Qt GUI implementation
└── tests/
    ├── test_avalanche.cpp      # Avalanche effect test
    ├── test_rfc_7539.cpp       # Structure tests
    └── test_side_channels.cpp  # Timing side-channel test
```

---

## Features

### Key Generation
- 64‑byte key derived from OS randomness (`/dev/urandom`)
- 4×4 matrix of 32‑bit words
- Custom shuffle rounds for diffusion
- Secure memory wiping after use

### Encryption / Decryption
- XOR‑based block cipher (64‑byte blocks)
- Binary output (`.bin`) + hex‑dump output (`.txt`)
- Separate key files for encryption/decryption

### Qt GUI Application
- User-friendly graphical interface
- Key generation with file browser
- Separate sections for encryption and decryption
- Auto-generated output filenames
- Activity log with timestamps
- Progress indicators

### CLI Tool
- Generate keys from command line
- Encrypt/decrypt files without GUI
- Useful for scripting and automation

### Test Suite (GoogleTest)

| Test | Purpose | Why It Matters |
|------|---------|----------------|
| **Avalanche Test** | Flipping 1 bit → many bits change | Ensures **diffusion** - patterns in plaintext don't show in ciphertext |
| **Structure Test** | Shuffle changes state, different inputs → different outputs | Ensures **correctness** - verifies shuffle works and avoids collisions |
| **Side-Channel Test** | Timing is consistent across inputs | Prevents **timing attacks** - variable time could leak secrets |

### Benchmark
- Measures XOR throughput  
- Useful for comparing optimizations

### GitHub Actions CI
- Builds both GUI and CLI applications  
- Runs all tests automatically  
- Tests CLI functionality end-to-end
- Ensures code stays working on every push  

---

## Build Instructions

### Requirements
- GCC / G++
- CMake >= 3.16  
- Qt6-base-dev (or Qt5)
- GoogleTest (for tests)

### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libgtest-dev
```

### Build & Run

**Using the build script:**
```bash
chmod +x boot.sh
./boot.sh
```

**Manual build:**
```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

---

## Usage

### GUI Application

**Encrypt a file:**
1. Click "Generate New Key" or "Browse..." to select an existing key
2. Click "Browse..." to select the source file
3. Output files are auto-generated (can be changed)
4. Click **ENCRYPT**

**Decrypt a file:**
1. Click "Browse..." to select the encrypted `.bin` file
2. Click "Browse..." to select the key file
3. Output file is auto-generated
4. Click **DECRYPT**

### Test details:

**Avalanche Test:**
```
Tests that flipping a single bit in the input causes at least 256 bits 
(out of 512) to change in the output. This verifies the diffusion 
property essential for secure encryption.
```

**Structure Test:**
```
1. Verifies shuffle() actually modifies the state (not a no-op)
2. Verifies different inputs produce different outputs (prevents collisions)
```

**Side-Channel Test:**
```
Measures timing variance across 200 different inputs. If max/min ratio
exceeds 10x, the test fails - indicating potential timing attack vulnerability.
```

---

## Algorithm Details

### Key Generation
1. Generate 16 random 32-bit words using `/dev/urandom`
2. Arrange into 4×4 matrix
3. Apply 20 rounds of shuffle operations
4. Write 64 bytes to key file

### Shuffle Round
Each round performs:
- Column mixing (quarter rounds on columns)
- Row mixing (quarter rounds on rows)
- Diagonal mixing (quarter rounds on diagonals)

### Encryption
1. Read 64-byte key from file
2. Read input file in 64-byte blocks
3. XOR each block with the key
4. Write encrypted binary + hex representation

### Decryption
1. Read 64-byte key from file
2. Read encrypted file in 64-byte blocks
3. XOR each block with the key (XOR is its own inverse)
4. Write decrypted output


## Security Note

This cipher is **educational only**. For real-world applications, use established algorithms like:
- **AES-256-GCM** for symmetric encryption
- **ChaCha20-Poly1305** for authenticated encryption
- **libsodium** or **OpenSSL** libraries

---


