FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Core C++ toolchain + common build utilities
RUN set -eux; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
      build-essential \
      clang \
      lldb \
      gdb \
      cmake \
      ninja-build \
      ccache \
      git \
      pkg-config \
      curl \
      zip \
      unzip \
      tar \
      python3 \
      python3-pip \
      valgrind \
    ; \
    rm -rf /var/lib/apt/lists/*

# Optional: helpful for some projects (comment out if you don't need them)
# RUN pip3 install --no-cache-dir conan

# Ensure ccache is on PATH and easy to enable in CMake toolchains
ENV CCACHE_DIR=/home/vscode/.ccache
