#!/bin/sh
PKGS="github:NixOS/nixpkgs/nixos-25.11"
set -x
exec nix shell \
  $PKGS#cmake \
  $PKGS#ninja \
  $PKGS#gnumake \
  $PKGS#gcc-arm-embedded \
  $PKGS#python312Packages.jinja2 \
  $PKGS#gcc \
  $PKGS#qemu \
  $PKGS#sdcc \
  -c "$@"
