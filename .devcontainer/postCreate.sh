#!/usr/bin/env bash
set -euo pipefail

echo "=== Tool versions ==="
gcc --version | head -n 1 || true
g++ --version | head -n 1 || true
clang --version | head -n 1 || true
cmake --version | head -n 1
ninja --version
gdb --version | head -n 1 || true
lldb --version | head -n 1 || true

# If this repo uses CMake, do a first configure to validate environment.
# Safe: only runs if CMakeLists.txt exists at repo root.
if [ -f "CMakeLists.txt" ]; then
  echo "=== Initial CMake configure (Ninja) ==="
  cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
  echo "=== Initial build ==="
  cmake --build build -j
  echo "=== Initial test (if configured) ==="
  ctest --test-dir build --output-on-failure || true
else
  echo "No CMakeLists.txt found at repo root; skipping configure/build."
fi

echo "=== Done ==="
