#!/bin/bash

set -e

echo "=== Cleaning old build ==="
rm -rf build

echo "=== Creating build directory ==="
mkdir build
cd build

echo "=== Running CMake ==="
cmake ..

echo "=== Building project ==="
make -j$(nproc)

echo "=== Running tests ==="
ctest --output-on-failure

echo "=== Running main program ==="
./cipher_app

echo "=== Done ==="
