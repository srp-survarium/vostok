# Entry point for building the vostok-toolchain via a real Windows VM (QEMU/KVM)
# instead of Wine. The point: a real Windows `msiexec` applies VS2008 SP1 fully -
# including the *static* CRT (libcmt.lib etc.) - so the Wine-only `overlay_sp1_crt`
# hack and the wine-staging/C1902/xvfb fragilities all go away.
#
# Usage (from the vostok/ repo root):
#   WINDOWS_ISO=/path/to/winxp.iso nix-shell scripts/vm-toolchain/create-toolchain-vm.nix
#
# The VS2008 / SP1 / DXSDK ISOs are the same pinned fetchers as the Wine build;
# only the Windows guest ISO is new and (for now) comes from $WINDOWS_ISO. Once a
# stable archive.org URL is known it becomes a pinned fetchurl like the rest.

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
    pkgs.qemu                 # qemu-system-i386, qemu-img
    pkgs.mtools               # mformat/mcopy at image@@offset: FAT floppy + disk, rootless
    pkgs.util-linux           # sfdisk: write the MBR partition table to a raw image
    pkgs.p7zip                # pull the one SP1 MSP out of the SP1 ISO (host side)
    pkgs.python3
    pkgs.coreutils            # truncate
  ];

  shellHook = ''
    export VS2008_ISO="${vs2008-iso}"
    export VS2008_SP1_ISO="${sp1-iso}"
    export DXSDK_EXE="${dxsdk-exe}"
    export NINJA_WIN_ZIP="${ninja-zip}"
    export WINDOWS_ISO="''${WINDOWS_ISO:-}"
    export VOSTOK_DIR="$PWD"
    if [ -z "$WINDOWS_ISO" ]; then
      echo "NOTE: set WINDOWS_ISO=/path/to/winxp.iso before running the build." >&2
    fi
    exec python3 ${./create-toolchain-vm.py}
  '';
}
