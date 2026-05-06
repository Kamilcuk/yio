#!/bin/sh
PKGS="github:NixOS/nixpkgs/nixos-25.11"
PKGS="github:NixOS/nixpkgs/26ef669cffa904b6f6832ab57b77892a37c1a671"
set -x
exec nix shell \
  $PKGS#cmake \
  $PKGS#clang \
  $PKGS#ninja \
  $PKGS#gnumake \
  $PKGS#gcc-arm-embedded \
  $PKGS#python312Packages.jinja2 \
  $PKGS#gcc15 \
  $PKGS#qemu \
  $PKGS#sdcc \
  -c "$@"
