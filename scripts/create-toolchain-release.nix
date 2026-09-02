# SPDX-License-Identifier: GPL-3.0-or-later
# Entry point for building the vostok-toolchain release tarball.
#
# Fetches all required source ISOs into the Nix store, sets up Wine + tools,
# and runs vostok.tool.toolchain_release automatically.
#
# Usage (from the vostok/ repo root):
#   nix-shell scripts/create-toolchain-release.nix

{ pkgs ? import <nixpkgs> {} }:

let
  vs2008-iso = pkgs.fetchurl {
    name = "en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
    url = "https://archive.org/download/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310/en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
    sha256 = "52ebf5731b75ccc460384ce3fd25bc984fb2d828ae51501ebaf0cadc27a33ee9";
  };

  sp1-iso = pkgs.fetchurl {
    name = "VS2008SP1ENUX1512962.iso";
    urls = [
      "https://download.microsoft.com/download/a/3/7/a371b6d1-fc5e-44f7-914c-cb452b4043a9/VS2008SP1ENUX1512962.iso"
      "https://archive.org/download/vs90sp1-all-langs/SP1/VS2008SP1ENUX1512962.iso"
    ];
    sha256 = "580f717269faa10cf668140ef0a1a264cec194e20a0083cb0d0004a897cc675e";
  };

  dxsdk-exe = pkgs.fetchurl {
    name = "DXSDK_Jun10.exe";
    urls = [
      "https://download.microsoft.com/download/a/e/7/ae743f1f-632b-4809-87a9-aa1bb3458e31/DXSDK_Jun10.exe"
      "https://archive.org/download/dxsdk-jun-10_202603/DXSDK_Jun10.exe"
    ];
    sha256 = "705271dc83bfee54d9b94e028426e288d5f070784b7446d164f48ecfbb2a02cb";
  };

  ninja-zip = pkgs.fetchurl {
    name = "ninja-win.zip";
    url = "https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-win.zip";
    sha256 = "0yj6128i5fyw793blsldcy8pd8vp4963fg6vy9cgzmmn0p3zwl7m";
  };

in pkgs.mkShell {
  packages = [
    pkgs.wineWow64Packages.stable
    pkgs.p7zip
    pkgs.msitools
    pkgs.xvfb-run
    pkgs.python3
  ];

  shellHook = ''
    export VS2008_ISO="${vs2008-iso}"
    export VS2008_SP1_ISO="${sp1-iso}"
    export DXSDK_EXE="${dxsdk-exe}"
    export NINJA_WIN_ZIP="${ninja-zip}"
    export VOSTOK_DIR="$PWD"
    # Run from the repo root (see Usage): scripts/ is the package root.
    export PYTHONPATH="$VOSTOK_DIR/scripts''${PYTHONPATH:+:$PYTHONPATH}"
    exec python3 -m vostok.tool.toolchain_release
  '';
}
