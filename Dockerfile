# syntax=docker/dockerfile:1

# --- Base stage with common dependencies ---
FROM ubuntu:26.04 AS base
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    python3 \
    python3-jinja2 \
    libunistring-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /yio

# --- GCC Target ---
FROM base AS gcc-sh
RUN apt-get update && apt-get install -y --no-install-recommends gcc g++ && rm -rf /var/lib/apt/lists/*

FROM gcc-sh AS gcc
COPY . .
RUN --mount=type=cache,target=_build \
    make test CC=gcc

# --- Clang Target ---
FROM base AS clang
RUN apt-get update && apt-get install -y --no-install-recommends clang && rm -rf /var/lib/apt/lists/*
COPY . .
RUN --mount=type=cache,target=_build \
    make test CC=clang

# --- ARM none-eabi Target ---
FROM base AS arm-sh
RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    libnewlib-arm-none-eabi \
    qemu-user \
    && rm -rf /var/lib/apt/lists/*
FROM arm-sh AS arm
COPY . .
RUN --mount=type=cache,target=_build \
    make test PRESET=arm

# --- Alpine Target ---
FROM alpine:3.23 AS alpine-sh
RUN apk add --no-cache \
    build-base \
    cmake \
    ninja \
    python3 \
    py3-jinja2 \
    libunistring-dev \
    bash
WORKDIR /yio
FROM alpine-sh AS alpine
COPY . .
RUN --mount=type=cache,target=_build \
    make test

# --- ICX Target (Intel oneAPI) ---
FROM docker.io/intel/oneapi:2026.0.0-devel-ubuntu24.04 AS icx
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    ninja-build \
    python3 \
    python3-jinja2 \
    libunistring-dev \
    libdfp-dev \
    locales \
    && echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen \
    && echo "pl_PL.UTF-8 UTF-8" >> /etc/locale.gen \
    && echo "unm_US.UTF-8 UTF-8" >> /etc/locale.gen \
    && locale-gen \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /yio
COPY . .
RUN --mount=type=cache,target=_build \
    make test PRESET=icx

# --- SDCC Target ---
FROM base AS sdcc-sh
ENV PRESET=sdcc
RUN apt-get update && apt-get install -y --no-install-recommends \
    sdcc \
    && rm -rf /var/lib/apt/lists/*
FROM sdcc-sh AS sdcc
COPY . .
RUN --mount=type=cache,target=_build \
    make test PRESET=sdcc NPROC=1

# --- STM8 Target ---
FROM sdcc-sh AS stm8-sh
RUN apt-get update && apt-get install -y --no-install-recommends \
    sdcc-ucsim \
    && rm -rf /var/lib/apt/lists/*
FROM stm8-sh AS stm8
COPY . .
RUN --mount=type=cache,target=_build \
    make test PRESET=stm8 NPROC=1

# --- IAR Target ---
FROM ghcr.io/iarsystems/arm:latest AS iar-sh
ENV PRESET=iar
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    python3 \
    python3-jinja2 \
    libunistring-dev \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /yio
FROM iar-sh AS iar
COPY . .
RUN --mount=type=cache,target=_build \
    make test PRESET=iar

# --- Keil Target ---
FROM base AS keil
# Note: Arm Compiler 6 (Keil) Community Edition requires manual installation.
# This assumes the environment or a custom image provides 'armclang'.
COPY . .
RUN --mount=type=cache,target=_build \
    make test PRESET=keil

# --- Pages Target ---
FROM gcc-sh AS pages
COPY . .
RUN make gitlab_pages

# --- LTO Target ---
FROM gcc-sh AS lto
COPY . .
RUN --mount=type=cache,target=_build \
    make test CFLAGS="-flto" LDFLAGS="-flto"

# --- Valgrind Target ---
FROM gcc-sh AS valgrind
RUN apt-get update && apt-get install -y --no-install-recommends valgrind && rm -rf /var/lib/apt/lists/*
COPY . .
RUN --mount=type=cache,target=_build \
    make valgrind && make test_project

# --- No-Malloc + No-GNU Target ---
FROM gcc-sh AS nomalloc_nognu
COPY . .
RUN --mount=type=cache,target=_build \
    make test PRESET=nomalloc_nognu

###############################################################################
# --- Godbolt Runtime ---
FROM base AS godbolt
RUN apt-get update && apt-get install -y --no-install-recommends \
    nodejs \
    npm \
    curl \
    ca-certificates \
    git \
    gcc \
    g++ \
    clang \
    gcc-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    libnewlib-arm-none-eabi \
    sdcc \
    sdcc-ucsim \
    gcc-15-plugin-dev \
    libdfp-dev \
    libclang-dev \
    llvm-dev \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /compiler-explorer
RUN curl -L https://github.com/compiler-explorer/compiler-explorer/archive/refs/tags/gh-9992.tar.gz | tar xz --strip-components=1
RUN npm install
RUN npm run webpack
# Fix 404s: Ensure assets are everywhere Godbolt might look
RUN mkdir -p out/dist/static static && \
    cp -rv out/webpack/static/* out/dist/static/ && \
    cp -rv out/webpack/static/* static/
RUN ln -s out/dist/manifest.json manifest.json
RUN mkdir -p lib/storage/data
COPY godbolt/c.yio.properties etc/config/c.yio.properties
# Aggressive config cleanup: only keep what's needed
RUN mv etc/config/c.defaults.properties /tmp/ && \
    mv etc/config/compiler-explorer.defaults.properties /tmp/ && \
    mv etc/config/c.yio.properties /tmp/ && \
    rm etc/config/*.properties && \
    mv /tmp/c.defaults.properties etc/config/ && \
    mv /tmp/compiler-explorer.defaults.properties etc/config/ && \
    mv /tmp/c.yio.properties etc/config/
# Configure Godbolt: only C, no cloud warnings, etc.
RUN sed -i 's/^compilers=.*/compilers=yio_gcc:yio_clang:yio_arm:yio_stm8/' etc/config/c.defaults.properties && \
    sed -i 's/^defaultCompiler=.*/defaultCompiler=yio_gcc/' etc/config/c.defaults.properties && \
    sed -i '/^group\./d' etc/config/c.defaults.properties && \
    sed -i '/^compiler\.cg/d' etc/config/c.defaults.properties && \
    sed -i '/^compiler\.cclang/d' etc/config/c.defaults.properties && \
    cat etc/config/c.yio.properties >> etc/config/c.defaults.properties && \
    echo "languages=c" >> etc/config/compiler-explorer.defaults.properties && \
    echo "suppressCloudWarnings=true" >> etc/config/compiler-explorer.defaults.properties && \
    echo "loadAllProperties=false" >> etc/config/compiler-explorer.defaults.properties
WORKDIR /yio
COPY . .
# Build and install extensions
RUN --mount=type=cache,target=extensions/gcc/build \
    make -C extensions/gcc install
RUN --mount=type=cache,target=extensions/clang/build \
    make -C extensions/clang install
WORKDIR /compiler-explorer
# Setup templates
RUN rm -rf examples/* && \
    mkdir -vp examples/c && \
    ln -vs /yio/godbold/example.c examples/c/ && \
    cp -v /yio/examples/*.c examples/c/
# Environment for read-only filesystem
ENV NPM_CONFIG_CACHE=/tmp/.npm
ENV HOME=/tmp
ENV NODE_ENV=production
EXPOSE 10240
CMD ["node", "--no-warnings=ExperimentalWarning", "--loader", "ts-node/esm", "app.ts", "--port", "10240", "--host", "0.0.0.0"]
