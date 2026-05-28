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
      # Includes the Windows GNU target for cross-compiling vcproj2ninja.exe.
      rust = pkgs.rust-bin.nightly.latest.default.override {
        extensions = [ "rust-src" "rustfmt" "clippy" ];
        targets = [ "x86_64-pc-windows-gnu" ];
      };

      #
      # Large downloads exposed as named Nix packages.
      # Fetch with:  nix build .#<name> --out-link result-<name>
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
      # v1.12.1: minimum version for implicit outputs (| output syntax), needed by vcproj2ninja.
      ninja-win = pkgs.fetchurl {
        name = "ninja-win.zip";
        url = "https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-win.zip";
        sha256 = "0yj6128i5fyw793blsldcy8pd8vp4963fg6vy9cgzmmn0p3zwl7m";
      };

      # Survarium v0.100b game installer — contains survarium.exe + survarium.pdb.
      # setup-toolchain.sh extracts them to binaries/game/ and SURVARIUM_BIN points there.
      survarium-game = pkgs.fetchurl {
        name = "survarium_setup_v0100b.exe";
        url = "https://archive.org/download/vostok_engine_v0.1_build_802_internal_id_489_may_9_2013/survarium_setup_v0100b.exe";
        sha256 = "16aassxvbbhqx9czfvsl3zynl41n2xa7xf9n0l1aip07qgfz2l24";
      };

    in {
      packages.${system} = {
        inherit vs2008-iso vs2008-sp1-iso dxsdk ninja-win survarium-game;
      };

      devShells.${system}.default = pkgs.mkShell {
        name = "surv-decomp";

        packages = [
          # Nightly Rust for cargo builds of vostok-delinker, vcproj2ninja, pdb-parser
          rust

          # Wine — runs cl.exe / lib.exe / link.exe / ninja.exe / vcproj2ninja.exe
          pkgs.wineWow64Packages.stable

          # MinGW cross-compiler for building vcproj2ninja.exe (Windows target)
          pkgs.pkgsCross.mingwW64.buildPackages.gcc

          # Scripts
          pkgs.python3

          # Google Drive downloads (for vostok-libs)
          pkgs.gdown

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
          VOSTOK_DIR="$PWD"

          export WINEPREFIX="$VOSTOK_DIR/binaries/.wineprefix"
          export MSVC_DIR="$VOSTOK_DIR/binaries/toolchain/msvc"
          export WINSDK_DIR="$VOSTOK_DIR/binaries/toolchain/winsdk"
          export DXSDK_DIR="$VOSTOK_DIR/binaries/toolchain/dxsdk"
          export NINJA_DIR="$VOSTOK_DIR/binaries/toolchain/ninja"

          # Suppress Wine Mono / Gecko installation pop-up dialogs.
          export WINEDLLOVERRIDES="mscoree,mshtml="

          # Game binaries: extracted by setup-toolchain.sh, or set manually.
          # vostok-delinker reads SURVARIUM_BIN for survarium.exe + .pdb.
          export SURVARIUM_BIN="$VOSTOK_DIR/binaries/game"

          # Resolve installer paths from result symlinks if present.
          _resolve() { [ -e "$1" ] && readlink -f "$1" || echo ""; }
          export VS2008_ISO="$(_resolve result-vs2008-iso)"
          export VS2008_SP1_ISO="$(_resolve result-vs2008-sp1-iso)"
          export DXSDK_EXE="$(_resolve result-dxsdk)"
          export NINJA_WIN_ZIP="$(_resolve result-ninja-win)"
          export SURVARIUM_GAME_EXE="$(_resolve result-survarium-game)"

          if [ ! -d "$MSVC_DIR/VC/bin" ] || [ ! -d "$WINSDK_DIR/Include" ]; then
            echo "[surv-decomp] Toolchain not set up. Steps:"
            echo "  nix build .#vs2008-iso        --out-link result-vs2008-iso        # 3.3 GB"
            echo "  nix build .#vs2008-sp1-iso    --out-link result-vs2008-sp1-iso    # 831 MB"
            echo "  nix build .#dxsdk             --out-link result-dxsdk             # 572 MB"
            echo "  nix build .#ninja-win         --out-link result-ninja-win         #   1 MB"
            echo "  nix build .#survarium-game    --out-link result-survarium-game    # 920 MB"
            echo "  bash scripts/setup-toolchain.sh"
          fi
        '';
      };
    };
}
