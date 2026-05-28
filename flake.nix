{
  description = "Vostok Engine decompilation - Linux build environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    rust-overlay = {
      url = "github:oxalica/rust-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, rust-overlay }:
    let
      system = "x86_64-linux";

      pkgs = import nixpkgs {
        inherit system;
        overlays = [ rust-overlay.overlays.default ];
      };

      # Nightly Rust toolchain for vostok-delinker, vcproj2ninja, pdb-parser.
      rust = pkgs.rust-bin.nightly.latest.default.override {
        extensions = [ "rust-src" "rustfmt" "clippy" ];
      };

      #
      # Windows installers — large downloads exposed as named packages.
      # Fetch with:  nix build .#vs2008-iso    --out-link result-vs2008-iso
      #              nix build .#vs2008-sp1-iso --out-link result-vs2008-sp1-iso
      #              nix build .#dxsdk          --out-link result-dxsdk
      #              nix build .#ninja-win       --out-link result-ninja-win
      # Then run:    bash scripts/setup-toolchain.sh
      #

      vs2008-iso = pkgs.fetchurl {
        name = "en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
        url = "https://archive.org/download/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310/en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
        sha256 = "52ebf5731b75ccc460384ce3fd25bc984fb2d828ae51501ebaf0cadc27a33ee9";
      };

      vs2008-sp1-iso = pkgs.fetchurl {
        name = "VS2008SP1ENUX1512962.iso";
        urls = [
          "https://download.microsoft.com/download/a/3/7/a371b6d1-fc5e-44f7-914c-cb452b4043a9/VS2008SP1ENUX1512962.iso"
          "https://archive.org/download/vs90sp1-all-langs/SP1/VS2008SP1ENUX1512962.iso"
        ];
        sha256 = "580f717269faa10cf668140ef0a1a264cec194e20a0083cb0d0004a897cc675e";
      };

      dxsdk = pkgs.fetchurl {
        name = "DXSDK_Jun10.exe";
        urls = [
          "https://download.microsoft.com/download/a/e/7/ae743f1f-632b-4809-87a9-aa1bb3458e31/DXSDK_Jun10.exe"
          "https://archive.org/download/dxsdk-jun-10_202603/DXSDK_Jun10.exe"
        ];
        sha256 = "705271dc83bfee54d9b94e028426e288d5f070784b7446d164f48ecfbb2a02cb";
      };

      # ninja.exe (Windows, run under Wine) — drives the build.
      ninja-win = pkgs.fetchurl {
        name = "ninja-win.zip";
        url = "https://github.com/ninja-build/ninja/releases/download/v1.6.0/ninja-win.zip";
        sha256 = "18f55bc5de27c20092e86ace8ef3dd3311662dc6193157e3b65c6bc94ce006d5";
      };

    in {
      packages.${system} = {
        inherit vs2008-iso vs2008-sp1-iso dxsdk ninja-win;
      };

      devShells.${system}.default = pkgs.mkShell {
        name = "surv-decomp";

        packages = [
          # Nightly Rust for cargo builds of vostok-delinker, vcproj2ninja, pdb-parser
          rust

          # Wine — runs cl.exe / lib.exe / link.exe / ninja.exe (32-bit support)
          pkgs.wineWow64Packages.stable

          # Scripts
          pkgs.python3

          # Searching across repos
          pkgs.ripgrep

          # ISO + self-extracting exe extraction
          pkgs.p7zip

          # MSI extraction without Wine
          pkgs.msitools

          # Misc
          pkgs.file
        ];

        shellHook = ''
          VOSTOK_DIR="$(cd "$(dirname "''${BASH_SOURCE[0]:-$0}")" 2>/dev/null && pwd || echo "$PWD")"

          export WINEPREFIX="$VOSTOK_DIR/.wineprefix"
          export WINEARCH=win32
          export MSVC_DIR="$VOSTOK_DIR/toolchain/msvc"
          export DXSDK_DIR="$VOSTOK_DIR/toolchain/dxsdk"
          export NINJA_DIR="$VOSTOK_DIR/toolchain/ninja"

          # Resolve installer paths from result symlinks if present.
          _resolve() { [ -e "$1" ] && readlink -f "$1" || echo ""; }
          export VS2008_ISO="$(_resolve result-vs2008-iso)"
          export VS2008_SP1_ISO="$(_resolve result-vs2008-sp1-iso)"
          export DXSDK_EXE="$(_resolve result-dxsdk)"
          export NINJA_WIN_ZIP="$(_resolve result-ninja-win)"

          if [ ! -d "$MSVC_DIR/VC/bin" ]; then
            echo "[surv-decomp] Toolchain not set up. Steps:"
            echo "  nix build .#vs2008-iso    --out-link result-vs2008-iso    # 3.3 GB"
            echo "  nix build .#vs2008-sp1-iso --out-link result-vs2008-sp1-iso  # 831 MB"
            echo "  nix build .#dxsdk          --out-link result-dxsdk           # 572 MB"
            echo "  nix build .#ninja-win       --out-link result-ninja-win       #   1 MB"
            echo "  bash scripts/setup-toolchain.sh"
          fi
        '';
      };
    };
}
