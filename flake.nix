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
      # Toolchain component packages.
      # Individual components can be built separately; vostok-toolchain bundles them.
      # Build with:  nix build .#<name> --out-link binaries/result-<name>
      #

      # Internal: raw VS2008 ISO download — not exposed as a user package.
      _vs2008-iso-src = pkgs.fetchurl {
        name = "en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
        url = "https://archive.org/download/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310/en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
        sha256 = "52ebf5731b75ccc460384ce3fd25bc984fb2d828ae51501ebaf0cadc27a33ee9";
      };

      # VS2008 base compiler (cl.exe, headers, x86 libs) + Windows SDK 6.0A.
      # Extracted from the 3.3 GB ISO using msitools (no Wine needed).
      # Output: $out/VC/      — compiler, headers, libs
      #         $out/WinSDK/  — Windows SDK headers + x86 libs
      # Note: VS2008 SP1 cannot be applied here (requires wine msiexec on a registered
      # product, which our msitools-extracted layout doesn't support). Base compiler is
      # sufficient for the build; SP1 delta is a TODO.
      vs2008-toolchain = pkgs.runCommandNoCC "vs2008-toolchain" {
        src = _vs2008-iso-src;
        nativeBuildInputs = [ pkgs.p7zip pkgs.msitools ];
      } ''
        BUILD_DIR="$PWD"
        mkdir work

        # Exit code 2 = non-fatal UDF sector warnings; files extract fine.
        7z x "$src" -tUDF -o"work" -y > /dev/null || true

        # --- VC Compiler ---
        _vs_msi=""
        for candidate in work/Setup/vs_setup.msi work/vs_setup.msi; do
          [ -f "$candidate" ] && _vs_msi="$candidate" && break
        done
        [ -z "$_vs_msi" ] && echo "ERROR: vs_setup.msi not found in ISO" && exit 1

        mkdir vc-extracted
        (cd "$(dirname "$BUILD_DIR/$_vs_msi")" && \
          msiextract -C "$BUILD_DIR/vc-extracted" "$(basename "$_vs_msi")")

        _vc_bin=$(find vc-extracted -name "cl.exe" -path "*/bin/cl.exe" \
          ! -path "*/amd64/*" ! -path "*/x86_amd64/*" ! -path "*/ce/*" \
          -printf "%h\n" | head -1)
        [ -z "$_vc_bin" ] && echo "ERROR: cl.exe not found after msiextract" && exit 1

        _vc_dir=$(dirname "$_vc_bin")
        _vs9_root=$(dirname "$(dirname "$_vc_dir")")
        _ide_dir="$_vs9_root/Common7/IDE"

        mkdir -p "$out/VC"
        cp -r "$_vc_dir"/. "$out/VC/"
        # PDB DLLs must live next to cl.exe for PDB writing to work.
        for f in mspdb80.dll mspdbcore.dll mspdbsrv.exe msobj80.dll; do
          [ -f "$_ide_dir/$f" ] && cp "$_ide_dir/$f" "$out/VC/bin/"
        done

        # --- Windows SDK 6.0A ---
        _sdk_exe="work/WCU/WinSDK/WinSDK_Build.exe"
        [ ! -f "$_sdk_exe" ] && echo "ERROR: WinSDK_Build.exe not found in ISO" && exit 1

        mkdir sdk-sfx
        7z x "$_sdk_exe" -o"sdk-sfx" -y > /dev/null
        [ ! -f "sdk-sfx/VistaClientHeadersLibs-x86.msi" ] && \
          echo "ERROR: VistaClientHeadersLibs-x86.msi not found after sfxcab" && exit 1

        mkdir sdk-extracted
        (cd sdk-sfx && msiextract -C "$BUILD_DIR/sdk-extracted" VistaClientHeadersLibs-x86.msi)

        _include=$(find sdk-extracted -maxdepth 6 -type d -name "Include" | head -1)
        _lib=$(find sdk-extracted -maxdepth 6 -type d -name "Lib" ! -path "*/x64" | head -1)
        [ -z "$_include" ] && echo "ERROR: WinSDK Include/ not found" && exit 1
        [ -z "$_lib" ]     && echo "ERROR: WinSDK Lib/ not found"     && exit 1

        mkdir -p "$out/WinSDK"
        cp -r "$_include" "$out/WinSDK/Include"
        cp -r "$_lib"     "$out/WinSDK/Lib"
      '';

      # DirectX SDK June 2010 — extracted headers + libs.
      # Output: $out/Include/  $out/Lib/
      dxsdk = pkgs.runCommandNoCC "dxsdk-jun10" {
        src = pkgs.fetchurl {
          name = "DXSDK_Jun10.exe";
          urls = [
            "https://download.microsoft.com/download/a/e/7/ae743f1f-632b-4809-87a9-aa1bb3458e31/DXSDK_Jun10.exe"
            "https://archive.org/download/dxsdk-jun-10_202603/DXSDK_Jun10.exe"
          ];
          sha256 = "705271dc83bfee54d9b94e028426e288d5f070784b7446d164f48ecfbb2a02cb";
        };
        nativeBuildInputs = [ pkgs.p7zip ];
      } ''
        mkdir extract
        7z x "$src" -o"extract" -y > /dev/null
        for candidate in extract/DXSDK extract; do
          if [ -d "$candidate/Include" ]; then
            mkdir -p "$out"
            cp -r "$candidate/Include" "$out/"
            cp -r "$candidate/Lib"     "$out/"
            break
          fi
        done
        [ ! -d "$out/Include" ] && echo "ERROR: DXSDK Include/ not found" && exit 1
      '';

      # ninja.exe v1.12.1 (Windows, run under Wine) — extracted from zip.
      # v1.12.1: minimum version for implicit outputs (| output syntax), needed by vcproj2ninja.
      ninja-win = pkgs.runCommandNoCC "ninja-win" {
        src = pkgs.fetchurl {
          name = "ninja-win.zip";
          url = "https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-win.zip";
          sha256 = "0yj6128i5fyw793blsldcy8pd8vp4963fg6vy9cgzmmn0p3zwl7m";
        };
        nativeBuildInputs = [ pkgs.p7zip ];
      } ''
        mkdir -p "$out"
        7z e "$src" ninja.exe -o"$out" -y > /dev/null
        [ ! -f "$out/ninja.exe" ] && echo "ERROR: ninja.exe not found in zip" && exit 1
      '';

      # Combined Windows toolchain: MSVC + WinSDK + DXSDK + Ninja.
      # All paths are in the Nix store (read-only) — no local copies needed.
      # Output layout (symlinks into component derivations):
      #   $out/msvc/VC/   — cl.exe, headers, libs  (MSVC_DIR = $out/msvc)
      #   $out/winsdk/    — Windows SDK Include + Lib
      #   $out/dxsdk/     — DXSDK Include + Lib
      #   $out/ninja/     — ninja.exe
      vostok-toolchain = pkgs.runCommandNoCC "vostok-toolchain" {
        vs2008 = vs2008-toolchain;
        dxsdkPkg = dxsdk;
        ninjaPkg = ninja-win;
      } ''
        mkdir -p "$out/msvc"
        ln -s "$vs2008/VC"     "$out/msvc/VC"
        ln -s "$vs2008/WinSDK" "$out/winsdk"
        ln -s "$dxsdkPkg"      "$out/dxsdk"
        ln -s "$ninjaPkg"      "$out/ninja"
      '';

      # Survarium v0.100b — extracted game directory (survarium.exe, survarium.pdb, DLLs).
      # SURVARIUM_BIN in the devShell points to this output (or binaries/game as fallback).
      survarium-game = pkgs.runCommandNoCC "survarium-game" {
        src = pkgs.fetchurl {
          name = "survarium_setup_v0100b.exe";
          url = "https://archive.org/download/vostok_engine_v0.1_build_802_internal_id_489_may_9_2013/survarium_setup_v0100b.exe";
          sha256 = "16aassxvbbhqx9czfvsl3zynl41n2xa7xf9n0l1aip07qgfz2l24";
        };
        nativeBuildInputs = [ pkgs.p7zip ];
      } ''
        mkdir extract
        7z x "$src" -o"extract" -y > /dev/null
        surv_exe=$(find extract -iname "survarium.exe" ! -path "*uninstall*" \
          -printf "%s %p\n" | sort -rn | head -1 | awk '{print $2}')
        if [ -z "$surv_exe" ]; then
          echo "ERROR: survarium.exe not found in extracted installer"
          find extract -maxdepth 3 -name "*.exe" | head -10
          exit 1
        fi
        game_dir=$(dirname "$surv_exe")
        mkdir -p "$out"
        cp -r "$game_dir"/. "$out/"
      '';

    in {
      packages.${system} = {
        inherit vs2008-toolchain dxsdk ninja-win vostok-toolchain survarium-game;
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

          # Suppress Wine Mono / Gecko installation pop-up dialogs.
          export WINEDLLOVERRIDES="mscoree,mshtml="

          # Resolve a result symlink to its Nix store path, or empty if not built yet.
          _resolve() { [ -e "$1" ] && readlink -f "$1" || echo ""; }

          # Combined toolchain: all paths derived from a single result symlink.
          _toolchain="$(_resolve binaries/result-vostok-toolchain)"
          export VOSTOK_TOOLCHAIN="$_toolchain"
          if [ -n "$_toolchain" ]; then
            export MSVC_DIR="$_toolchain/msvc"
            export WINSDK_DIR="$_toolchain/winsdk"
            export DXSDK_DIR="$_toolchain/dxsdk"
            export NINJA_DIR="$_toolchain/ninja"
          else
            export MSVC_DIR="$VOSTOK_DIR/binaries/toolchain/msvc"
            export WINSDK_DIR="$VOSTOK_DIR/binaries/toolchain/winsdk"
            export DXSDK_DIR="$VOSTOK_DIR/binaries/toolchain/dxsdk"
            export NINJA_DIR="$VOSTOK_DIR/binaries/toolchain/ninja"
          fi

          # SURVARIUM_BIN: Nix-extracted game dir if built, else binaries/game.
          _surv="$(_resolve binaries/result-survarium-game)"
          if [ -n "$_surv" ]; then
            export SURVARIUM_BIN="$_surv"
          else
            export SURVARIUM_BIN="$VOSTOK_DIR/binaries/game"
          fi

          if [ ! -d "$MSVC_DIR/VC/bin" ] || [ ! -d "$WINSDK_DIR/Include" ]; then
            echo "[surv-decomp] Toolchain not set up. Steps:"
            echo "  nix build .#vostok-toolchain  --out-link binaries/result-vostok-toolchain  # 3.3 GB"
            echo "  bash scripts/setup-toolchain.sh  # configures Wine env + vostok-libs"
          fi
          if [ ! -f "$SURVARIUM_BIN/survarium.exe" ]; then
            echo "[surv-decomp] Game binaries not found. Build with:"
            echo "  nix build .#survarium-game    --out-link binaries/result-survarium-game    # 920 MB"
          fi
        '';
      };
    };
}
