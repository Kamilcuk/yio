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

# --- ICC Target (Intel oneAPI) ---
FROM intel/oneapi-compiler-dpcpp-cpp:latest AS icc
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    ninja-build \
    python3 \
    python3-jinja2 \
    libunistring-dev \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /yio
COPY . .
RUN --mount=type=cache,target=_build \
    . /opt/intel/oneapi/setvars.sh && make test PRESET=icc

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
