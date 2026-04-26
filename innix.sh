#!/bin/bash
exec nix shell \
  nixpkgs#cmake \
  nixpkgs#ninja \
  nixpkgs#gnumake \
  nixpkgs#gcc-arm-embedded \
  nixpkgs#python312Packages.jinja2 \
  nixpkgs#gcc \
  nixpkgs#qemu \
  -c "$@"
