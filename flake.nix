{
  description = "Vostok Engine decompilation - Linux build environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    rust-overlay = {
      url = "github:oxalica/rust-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    naersk = {
      url = "github:nix-community/naersk";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    # Rust tools — fetched as plain source (no nested flake needed).
    pdb-parser-src = {
      url = "github:srp-survarium/pdb-parser";
      flake = false;
    };
    vostok-delinker-src = {
      url = "github:srp-survarium/vostok-delinker";
      flake = false;
    };
    vcproj2ninja-src = {
      url = "github:srp-survarium/vcproj2ninja";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, rust-overlay, naersk
            , pdb-parser-src, vostok-delinker-src, vcproj2ninja-src }:
    let
      system = "x86_64-linux";

      pkgs = import nixpkgs {
        inherit system;
        overlays = [ rust-overlay.overlays.default ];
      };

      # Nightly Rust — shared between the devShell and all tool builds.
      rust = pkgs.rust-bin.nightly.latest.default.override {
        extensions = [ "rust-src" "rustfmt" "clippy" ];
      };

      # naersk configured to use our nightly toolchain.
      naersk-lib = pkgs.callPackage naersk {
        cargo = rust;
        rustc = rust;
      };

      #
      # Rust tool derivations.
      #

      pdb-parser = naersk-lib.buildPackage {
        pname = "pdb-parser";
        src = pdb-parser-src;
      };

      vostok-delinker = naersk-lib.buildPackage {
        pname = "vostok-delinker";
        src = vostok-delinker-src;
      };

      vcproj2ninja = naersk-lib.buildPackage {
        pname = "vcproj2ninja";
        src = vcproj2ninja-src;
      };

      #
      # Windows installers — downloaded once into the Nix store.
      #

      vs2008Iso = pkgs.fetchurl {
        name = "en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
        url = "https://archive.org/download/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310/en_visual_studio_2008_professional_x86_dvd_x14-26326.iso";
        sha256 = "52ebf5731b75ccc460384ce3fd25bc984fb2d828ae51501ebaf0cadc27a33ee9";
      };

      vs2008Sp1Iso = pkgs.fetchurl {
        name = "VS2008SP1ENUX1512962.iso";
        urls = [
          "https://download.microsoft.com/download/a/3/7/a371b6d1-fc5e-44f7-914c-cb452b4043a9/VS2008SP1ENUX1512962.iso"
          "https://archive.org/download/vs90sp1-all-langs/SP1/VS2008SP1ENUX1512962.iso"
        ];
        sha256 = "580f717269faa10cf668140ef0a1a264cec194e20a0083cb0d0004a897cc675e";
      };

      # ninja.exe (Windows binary) — run under Wine to drive the build.
      # All build tools (cl, lib, link, ninja) execute inside the same Wine environment.
      ninjaWinZip = pkgs.fetchurl {
        name = "ninja-win.zip";
        url = "https://github.com/ninja-build/ninja/releases/download/v1.6.0/ninja-win.zip";
        sha256 = "18f55bc5de27c20092e86ace8ef3dd3311662dc6193157e3b65c6bc94ce006d5";
      };

      dxsdkExe = pkgs.fetchurl {
        name = "DXSDK_Jun10.exe";
        urls = [
          "https://download.microsoft.com/download/a/e/7/ae743f1f-632b-4809-87a9-aa1bb3458e31/DXSDK_Jun10.exe"
          "https://archive.org/download/dxsdk-jun-10_202603/DXSDK_Jun10.exe"
        ];
        sha256 = "705271dc83bfee54d9b94e028426e288d5f070784b7446d164f48ecfbb2a02cb";
      };

    in {
      # Expose the tools as packages so they can be used independently.
      packages.${system} = {
        inherit pdb-parser vostok-delinker vcproj2ninja;
        default = pkgs.symlinkJoin {
          name = "surv-decomp-tools";
          paths = [ pdb-parser vostok-delinker vcproj2ninja ];
        };
      };

      devShells.${system}.default = pkgs.mkShell {
        name = "surv-decomp";

        packages = [
          # Rust toolchain (nightly) for on-the-fly cargo runs / dev iteration
          rust

          # Prebuilt Rust tools
          pdb-parser
          vostok-delinker
          vcproj2ninja

          # Wine — runs VS2008 cl.exe/lib.exe/link.exe/ninja.exe (needs 32-bit support)
          pkgs.wineWowPackages.stable

          # Scripts
          pkgs.python3

          # Searching across repos
          pkgs.ripgrep

          # ISO and self-extracting archive extraction
          pkgs.p7zip

          # Extract MSI packages without Wine (provides msiextract)
          pkgs.msitools

          # Useful for inspecting extracted PE binaries
          pkgs.file
        ];

        shellHook = ''
          # Locate the vostok repo root (directory containing this flake).
          VOSTOK_DIR="$(cd "$(dirname "''${BASH_SOURCE[0]:-$0}")" 2>/dev/null && pwd || echo "$PWD")"

          export WINEPREFIX="$VOSTOK_DIR/.wineprefix"
          export WINEARCH=win32

          # Paths for the extracted portable toolchain (created by setup-toolchain.sh).
          export MSVC_DIR="$VOSTOK_DIR/toolchain/msvc"
          export DXSDK_DIR="$VOSTOK_DIR/toolchain/dxsdk"

          # Nix store paths for the downloaded installers.
          export VS2008_ISO="${vs2008Iso}"
          export VS2008_SP1_ISO="${vs2008Sp1Iso}"
          export DXSDK_EXE="${dxsdkExe}"
          export NINJA_WIN_ZIP="${ninjaWinZip}"

          if [ ! -d "$MSVC_DIR/VC/bin" ]; then
            echo "[surv-decomp] MSVC toolchain not yet extracted."
            echo "  Run: bash scripts/setup-toolchain.sh"
          fi
        '';
      };
    };
}
