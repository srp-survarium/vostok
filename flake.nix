# SPDX-License-Identifier: GPL-3.0-or-later

{
  description = "Vostok Engine decompilation - Linux build environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    rust-overlay = {
      url = "github:oxalica/rust-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    # Sibling repos fetched from GitHub (path inputs don't get narHash in Nix 2.x,
    # so they can't be used as derivation sources in sandboxed builds).
    vostok-pdb-parser-src = {
      # Pinned to the branch tip, newest first. Re-track master once these land.
      # Output is gitignored/reference-only, so a bump can never move the bytes.
      #
      #   1eee4a0  decodes residual DBI string/hash tables, TPI/IPI hashes and
      #            index maps, GSI/PSI maps, C13 inner records, and optional
      #            FPO/frame-data streams instead of hiding them as raw bytes.
      #   5cd58bd  compares the complete observable MSF/PDB stream topology:
      #            allocation pages, stream roles, DBI/TPI/IPI/symbol/module
      #            order, source/line records, and raw fallbacks.
      #   50a8f49  source-definition order requires both attributed line order
      #            and compiland procedure-symbol order, rejecting #line-only
      #            inversions.
      #   c8124fd  normalized order-distance/locality metrics, causal grouping
      #            by library/record kind, and explicit base/target definition
      #            orders for actionable source rows.
      #   ee1cfd5  raw class variant/multiplicity comparison and physical
      #            DBI/TPI/global/module-symbol order diagnostics.
      #   e4ed03e  reverts 93165d2: the undname-style record-name split was a
      #            Wine artifact (its builtin msvcr90 __unDName renders enums
      #            bare where Microsoft's elaborates them). The build now loads
      #            the native VC90 CRT (see vostok.build.native_crt), so both
      #            PDBs spell records the same natively - the normalization was
      #            redundant AND masked real class-vs-struct source drifts.
      #   93165d2  (reverted) pdb_divergence headers side normalized
      #            undname-style record names.
      #   d0eb201  gitignores that checkout's own nix gcroots.
      #   c9ad86c  every printed address says whether it is a VA or an RVA
      #            (headers carry both; columns are `va`/`t.va`/`b.va`). The
      #            rich indexes and the ledger speak RVA and this tool spoke
      #            VA, differing by the image base with nothing to say which -
      #            paste one for the other and you land 64 KB off, silently.
      #   01020c6  pdb_divergence joins source functions by mangled symbol.
      #   7460355  `pub mod divergence;` + the `gen_sources::for_each_function`
      #            compiland walker, so `pdb_divergence` (the base-vs-target
      #            structure-divergence verifier) builds on a clean checkout.
      #   d757820  pdb_divergence itself.
      #   c5a4d0f  C++ `private:`/`protected:`/`public:` section labels for
      #            class members (CV_access_t), narrowing `/* no source */`.
      #   89d3a1e  demangles `??__E`/`??__F` thunks to the target PDB's
      #            `` `dynamic initializer for 'X'' `` form so objdiff pairs them.
      #   b6159cc  emits the engine's own vostok/scaleform/sources compilands.
      #   #28      the structure-builder (extract-all-enums-and-unions).
      url = "github:srp-survarium/vostok-pdb-parser/1eee4a0a155f9ec911638f639c00818b2a84070e";
      flake = false;
    };
    vcproj2ninja-src = {
      url = "github:srp-survarium/vcproj2ninja";
      flake = false;
    };
    vostok-delinker-src = {
      # The measured code-project delinker. Keep this immutable: changing its
      # relocation materialization changes the established function ledger.
      url = "github:srp-survarium/vostok-delinker/d7e9292c5d6ddc07bd62894b2bc49334ed1c7321";
      flake = false;
    };
    vostok-data-delinker-src = {
      # Gruntz's reviewed data-topology base. It writes a separate objdiff
      # project and therefore cannot perturb the code/MAX ledger.
      url = "github:srp-survarium/vostok-delinker/25719787a8b3d2dd5d461a6af3af837c45353079";
      flake = false;
    };
    objdiff-src = {
      url = "github:encounter/objdiff/v3.7.3";
      flake = false;
    };
    vostok-resources-db-src = {
      url = "github:srp-survarium/vostok-resources-db";
      flake = false;
    };
    # pdb_fetch.nvim - the in-editor match views (:Vostok). Auto-loaded into nvim
    # by the dev shell's shim (see shellHook). Bump with `nix flake update
    # pdb-fetch-nvim-src` to pick up new plugin versions on the next `nix develop`.
    pdb-fetch-nvim-src = {
      url = "github:srp-survarium/pdb_fetch.nvim";
      flake = false;
    };
    bullet-2_79-src = {
      # The first official 2.79 revision. Multiple stock target-PDB MD5s land
      # exactly on this commit after the repository's LF -> CRLF conversion.
      url = "github:bulletphysics/bullet3/ce62d7615ecb1faa876d11172c418244c7246b5c";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, rust-overlay, vostok-pdb-parser-src, vcproj2ninja-src, vostok-delinker-src, vostok-data-delinker-src, objdiff-src, vostok-resources-db-src, pdb-fetch-nvim-src, bullet-2_79-src }:
    let
      system = "x86_64-linux";

      pkgs = import nixpkgs {
        inherit system;
        overlays = [ rust-overlay.overlays.default ];
      };

      # Nightly Rust toolchain for all Rust tools.
      # Includes the Windows GNU target for cross-compiling vcproj2ninja.exe.
      rust = pkgs.rust-bin.nightly.latest.default.override {
        extensions = [ "rust-src" "rustfmt" "clippy" ];
        targets = [ "x86_64-pc-windows-gnu" ];
      };

      # rustPlatform backed by our nightly toolchain.
      nightly-rustPlatform = pkgs.makeRustPlatform {
        cargo = rust;
        rustc = rust;
      };

      mingw = pkgs.pkgsCross.mingwW64;

      # ---------------------------------------------------------------------------
      # vostok-pdb-parser - Linux binary, generates C++ stubs from PDB files.
      # Run: vostok-pdb-parser --pdb-path survarium.pdb --output-path ../vostok-structure
      #
      # cargoHash: update by running `nix build .#vostok-pdb-parser` after bumping
      # the input (nix flake update vostok-pdb-parser-src) - Nix reports the new hash.
      # ---------------------------------------------------------------------------
      vostok-pdb-parser = nightly-rustPlatform.buildRustPackage {
        pname = "vostok-pdb-parser";
        version = "0.1.0";
        src = vostok-pdb-parser-src;
        # Keep the campaign-specific raw CodeView topology query beside this
        # repository while it is evaluated for upstreaming into the parser.
        cargoHash = "sha256-Rz5KvSEfVJS55aj08X86LkPTfggLKqGsaD1nynxVhFM=";
      };

      # ---------------------------------------------------------------------------
      # vostok-delinker - Linux binary, splits an EXE into per-unit COFF .obj files
      # for objdiff. Run natively (no Wine): vostok-delinker --pdb-path ... --exe-path ...
      #
      # cargoHash: update by running `nix build .#vostok-delinker` after bumping
      # the input (nix flake update vostok-delinker-src) - Nix reports the new hash.
      # ---------------------------------------------------------------------------
      vostok-delinker = nightly-rustPlatform.buildRustPackage {
        pname = "vostok-delinker";
        version = "0.1.0";
        src = vostok-delinker-src;
        cargoHash = "sha256-ZwFdbqUyh4b0S+fUYKGMN1fWaxRu1zU2ozKpe7CbcYs=";
      };

      # Data topology deliberately uses its own binary and output tree. The
      # newer relocation/data model is evidence for data matching, never a
      # migration of the already-measured code matcher.
      vostok-data-delinker = nightly-rustPlatform.buildRustPackage {
        pname = "vostok-data-delinker";
        version = "0.1.0";
        src = vostok-data-delinker-src;
        # The data lane needs the identities and type-derived extents that the
        # delinker already reads from the PDB.  Exporting them is opt-in and
        # exits before normal COFF emission, so function pairing is unchanged.
        cargoHash = "sha256-ry3TH1fz7Aj/JdbmlgQFFn29m8E7EQHyGaVXnZTEcXo=";
        postInstall = ''
          mv $out/bin/vostok-delinker $out/bin/vostok-data-delinker
        '';
      };

      # ---------------------------------------------------------------------------
      # vostok-resources-db - Linux binary, packs/unpacks the engine's resources.db
      # VFS archive (the "FAT" pack format). Run natively (no Wine):
      #   resources-db extract <resources.db> <out_dir>
      #   resources-db list    <resources.db>
      # Used below by survarium-resources-unpacked.
      #
      # cargoHash: update by running `nix build .#vostok-resources-db` after bumping
      # the input (nix flake update vostok-resources-db-src) - Nix reports the new hash.
      # ---------------------------------------------------------------------------
      vostok-resources-db = nightly-rustPlatform.buildRustPackage {
        pname = "vostok-resources-db";
        version = "0.1.0";
        src = vostok-resources-db-src;
        cargoHash = "sha256-+UUfJJxuiMsB6JIia1MGvp7/sbzWMsOTvrB6AtTJlu4=";
      };

      # ---------------------------------------------------------------------------
      # vcproj2ninja - Windows .exe, cross-compiled with MinGW.
      # Converts VS2008 .vcproj/.sln to Ninja build files, run under Wine.
      # Run: wine $VCPROJ2NINJA_EXE <args>
      #
      # cargoHash: update by running `nix build .#vcproj2ninja` after bumping the input.
      # ---------------------------------------------------------------------------
      vcproj2ninja = nightly-rustPlatform.buildRustPackage {
        pname = "vcproj2ninja";
        version = "0.1.0";
        src = vcproj2ninja-src;
        cargoHash = "sha256-Fc30XVO4LYQT5HHHXm0J99QZgYXh4VNzeNukWV4sFeg=";

        CARGO_TARGET_X86_64_PC_WINDOWS_GNU_LINKER =
          "${mingw.stdenv.cc}/bin/${mingw.stdenv.cc.targetPrefix}cc";

        nativeBuildInputs = [ mingw.stdenv.cc ];

        # Two library tests fail on Linux due to Windows path-separator assumptions.
        # The cross-compiled .exe is the deliverable; host tests aren't relevant.
        doCheck = false;

        # buildRustPackage's cargoBuildHook hard-codes `--target <host>`, which
        # overrides CARGO_BUILD_TARGET and produces a native Linux ELF (renamed
        # .exe) - Wine then runs it natively, so std::path uses Unix semantics and
        # mangles the vcproj's backslash paths. We must emit a genuine Windows PE.
        # So we keep cargoSetupHook (vendoring via cargoHash) but replace the
        # build/install phases with an explicit cross-build to the Windows target.
        #
        # nightly's x86_64-pc-windows-gnu std links `-l:libpthread.a` (mcfgthread);
        # shim a libpthread.a -> libmcfgthread.a so the MinGW linker finds it.
        buildPhase = ''
          runHook preBuild
          shim="$(mktemp -d)"
          ln -s ${mingw.windows.mcfgthreads}/lib/libmcfgthread.a "$shim/libpthread.a"
          export RUSTFLAGS="-L $shim ''${RUSTFLAGS:-}"
          cargo build --release --offline --target x86_64-pc-windows-gnu --bin vcproj2ninja
          runHook postBuild
        '';

        installPhase = ''
          runHook preInstall
          install -Dm755 \
            target/x86_64-pc-windows-gnu/release/vcproj2ninja.exe \
            "$out/bin/vcproj2ninja.exe"
          runHook postInstall
        '';
      };

      # ---------------------------------------------------------------------------
      # vostok-toolchain - VS2008 SP1 + WinSDK 6.0A + DXSDK Jun2010 + ninja.exe.
      # Produced by: nix-shell scripts/create-toolchain-release.nix  (reproducible -
      # a fresh rebuild yields a byte-identical tarball).
      # Uploaded to: gh release upload v0.100b binaries/vostok-toolchain-v0.100b.tar.xz \
      #                --repo srp-survarium/vostok-build-env --clobber
      # Replace sha256 with the value printed by create-toolchain-release.py.
      # ---------------------------------------------------------------------------
      vostok-toolchain = pkgs.runCommand "vostok-toolchain" {
        src = pkgs.fetchurl {
          name = "vostok-toolchain-v0.100b.tar.xz";
          url = "https://github.com/srp-survarium/vostok-build-env/releases/download/v0.100b/vostok-toolchain-v0.100b.tar.xz";
          sha256 = "c9c5c17a51739472e11e9071b0274c3278930ddda0a8478858d81612bf895d99";
        };
        nativeBuildInputs = [ pkgs.gnutar pkgs.xz ];
      } ''
        mkdir -p "$out"
        tar xf "$src" -C "$out" --strip-components=1
      '';

      # ---------------------------------------------------------------------------
      # dxsdk-shader-compiler - fxc.exe + D3DCompiler_43.dll from the June 2010
      # DirectX SDK. The shipped shader blobs in resources.db carry the RDEF
      # creator string "Microsoft (R) HLSL Shader Compiler 9.29.952.3111", which
      # is exactly this SDK's compiler; the toolchain tarball carries only the
      # SDK's Include/Lib. Run under Wine. Used by the shader roundtrip harness.
      # ---------------------------------------------------------------------------
      dxsdk-shader-compiler = pkgs.runCommand "dxsdk-shader-compiler" {
        src = pkgs.fetchurl {
          name = "DXSDK_Jun10.exe";
          url = "https://download.microsoft.com/download/A/E/7/AE743F1F-632B-4809-87A9-AA1BB3458E31/DXSDK_Jun10.exe";
          hash = "sha256-cFJx3IO/7lTZuU4ChCbiiNXwcHhLdEbRZPSOz7sqAss=";
        };
        nativeBuildInputs = [ pkgs.cabextract ];
      } ''
        set -euo pipefail
        # The installer's payload is one embedded cabinet; cabextract scans for
        # it. The compiler DLL sits in a nested redist cab whose member carries
        # an FL_-mangled name.
        cabextract -q -F 'DXSDK/Utilities/bin/x86/fxc.exe' -d extract "$src"
        cabextract -q -F 'DXSDK/Redist/Jun2010_D3DCompiler_43_x86.cab' -d extract "$src"
        cabextract -q -d cabout "extract/DXSDK/Redist/Jun2010_D3DCompiler_43_x86.cab"
        dll=$(find cabout -iname '*D3DCompiler_43*' -type f | head -1)
        test -n "$dll"
        mkdir -p "$out/bin"
        cp "extract/DXSDK/Utilities/bin/x86/fxc.exe" "$out/bin/fxc.exe"
        cp "$dll" "$out/bin/D3DCompiler_43.dll"
      '';

      # ---------------------------------------------------------------------------
      # vostok-libs - proprietary third-party DLLs and import libraries.
      # Pre-packaged as a zip; the archive's top-level directory `vostok-libs/`
      # is stripped on unpack so $out exposes `sources/...` directly.
      # Uploaded to: gh release upload v0.100b vostok-libs-v0.100b-gfx422.zip --repo srp-survarium/vostok-build-env
      # gfx422: foreign 4.0.15 GFx libs replaced by our from-source 4.2.22 suite
      # (built per the shipped PDB recipe; see docs + vostok/build/gfx.py).
      # ---------------------------------------------------------------------------
      vostok-libs = pkgs.runCommand "vostok-libs" {
        # gfx421r2: the GFx Shipping suite rebuilt from source with the 4.2.21
        # reconstructions (sockets config, pointers_to_members pragma,
        # hash-verified header/TU rollbacks) plus libgfxexpat.lib and pcre.lib
        # the exe pragma-links - cut by `vostok tool libs-release`, compiled
        # through the C:\survarium\gfx-sdk alias so the objects record that
        # neutral prefix (paths.GFX_RELEASE_PREFIX). Same machine code as r1.
        src = pkgs.fetchurl {
          name = "vostok-libs-v0.100b-gfx421r2.zip";
          url = "https://github.com/srp-survarium/vostok-build-env/releases/download/v0.100b/vostok-libs-v0.100b-gfx421r2.zip";
          sha256 = "3cd445a2c04518c14cbee924b597c672a747582646d76430abe878b94ccb4df2";
        };
        nativeBuildInputs = [ pkgs.unzip ];
      } ''
        mkdir -p "$out" unpacked
        unzip -q "$src" -d unpacked
        mv unpacked/vostok-libs/* "$out"/
      '';

      # Survarium v0.100b InnoSetup installer, shared by the game/resources/keys
      # derivations below. innoextract places everything under app/:
      #   binaries/win32/{survarium.exe,survarium.pdb,bugtrap.dll}
      #   resources.db    - packed game data (~1.5 GiB)
      #   resources/ssl/* - lobby/login server certificates + private keys
      survarium-installer = pkgs.fetchurl {
        name = "survarium_setup_v0100b.exe";
        url = "https://archive.org/download/vostok_engine_v0.1_build_802_internal_id_489_may_9_2013/survarium_setup_v0100b.exe";
        sha256 = "16aassxvbbhqx9czfvsl3zynl41n2xa7xf9n0l1aip07qgfz2l24";
      };

      # Survarium v0.100b, extracted once from the installer into three outputs.
      # The single innoextract pass already unpacks everything, so resources and
      # keys cost no extra download or extraction beyond getting the exe/pdb:
      #   out       - game binaries: survarium.exe, survarium.pdb, DLLs (SURVARIUM_BIN)
      #   resources - resources.db + resources/ tree (packed game data, ~1.5 GiB)
      #   keys      - lobby/login server SSL certificates + private keys
      survarium = pkgs.runCommand "survarium" {
        src = survarium-installer;
        outputs = [ "out" "resources" "keys" ];
        nativeBuildInputs = [ pkgs.innoextract ];
      } ''
        mkdir extract
        innoextract -d extract "$src"

        surv_exe=$(find extract -iname "survarium.exe" ! -path "*uninstall*" \
          -printf "%s %p\n" | sort -rn | head -1 | awk '{print $2}')
        if [ -z "$surv_exe" ]; then
          echo "ERROR: survarium.exe not found in extracted installer"
          find extract -maxdepth 4 -name "*.exe" | head -10
          exit 1
        fi

        mkdir -p "$out" "$resources" "$keys"
        cp -r "$(dirname "$surv_exe")"/.            "$out"/
        cp -r extract/app/resources.db extract/app/resources "$resources"/
        cp -r extract/app/resources/ssl/.           "$keys"/
      '';

      # ---------------------------------------------------------------------------
      # Scaleform GFx 4.2.22 SDK - full source, from the DuckTales Remastered
      # source release (its only public copy). vostok.build.gfx compiles the
      # libgfx suite from this tree; retail's own gfx_4.2.21 tree is
      # byte-identical for 806 of its 1,128 files (proven against the retail
      # PDB's per-file MD5s: `pdb_diff --source-dir`), so it is the
      # reconstruction baseline, not a lookalike. 5.1 GiB download - opt-in
      # via `nix develop .#with-scaleform-sdk`; a local checkout via
      # $SCALEFORM_SDK keeps working without realizing this.
      # ---------------------------------------------------------------------------
      ducktales-src = pkgs.fetchurl {
        name = "ducktales_r326558.7z";
        url = "https://archive.org/download/ducktales-remastered-src/ducktales_r326558.7z";
        sha1 = "542945ecbba4dea4118ca9845130999fbf08af36";
      };

      scaleform-sdk = pkgs.runCommand "scaleform-sdk-4.2.22" {
        nativeBuildInputs = [ pkgs.p7zip ];
      } ''
        # The SDK root inside the archive is not a path we control - find it
        # as the directory holding Include/GFxVersion.h (either separator).
        marker=$(7z l -slt ${ducktales-src} | sed -n 's/^Path = //p' \
                 | grep -iE 'Include[\\/]GFxVersion\.h$' | head -1)
        if [ -z "$marker" ]; then
          echo "ERROR: Include/GFxVersion.h not found in archive listing"
          exit 1
        fi
        root=$(printf '%s' "$marker" | sed 's![\\/]Include[\\/]GFxVersion\.h$!!I')
        echo "SDK root in archive: $root"
        7z x -oextract ${ducktales-src} "$root/*" > /dev/null
        src_dir="extract/$(printf '%s' "$root" | tr '\\' '/')"
        [ -d "$src_dir" ] || { echo "ERROR: extraction missing $src_dir"; exit 1; }
        mkdir -p "$out"
        cp -r "$src_dir"/. "$out"/
      '';

      # ---------------------------------------------------------------------------
      # Unpacked game resources: the packed resources.db (from survarium.resources)
      # expanded into its file tree with the vostok-resources-db unpacker. ~12.5k
      # files; useful for inspecting/diffing game assets without the engine. The
      # extractor preserves the archive's raw byte paths verbatim.
      #   nix build .#survarium-resources-unpacked
      # ---------------------------------------------------------------------------
      survarium-resources-unpacked = pkgs.runCommand "survarium-resources-unpacked" {
        nativeBuildInputs = [ vostok-resources-db ];
      } ''
        mkdir -p "$out"
        resources-db extract "${survarium.resources}/resources.db" "$out"
      '';

      # ---------------------------------------------------------------------------
      # objdiff - upstream's prebuilt Linux binaries (not in nixpkgs, no flake).
      # These are foreign ELF binaries built for a normal FHS distro: their ELF
      # interpreter (/lib64/ld-linux-*) and library search paths don't exist on
      # Nix, so autoPatchelfHook rewrites the interpreter + RPATH to point into the
      # store. buildInputs below is just the *pool of libraries* autoPatchelf links
      # against - not a compile step (the binaries are already built).
      # `objdiff` is the GUI (interactive matching), `objdiff-cli` the CLI differ.
      # ---------------------------------------------------------------------------
      objdiffVersion = "3.7.1";
      objdiffUrl = name:
        "https://github.com/encounter/objdiff/releases/download/v${objdiffVersion}/${name}";
      objdiffGuiLibs = with pkgs; [
        libGL libxkbcommon wayland fontconfig freetype
        libx11 libxcursor libxi libxrandr libxcb
      ];

      # Keep the established code ledger on its measured CLI.
      objdiff-cli = pkgs.stdenv.mkDerivation {
        pname = "objdiff-cli";
        version = objdiffVersion;
        src = pkgs.fetchurl {
          url = objdiffUrl "objdiff-cli-linux-x86_64";
          hash = "sha256-QNhW2gHgpnbA8zr1NOVi8JjNUORey2Tzs0ZBjHsmSuY=";
        };
        dontUnpack = true;
        nativeBuildInputs = [ pkgs.autoPatchelfHook ];
        buildInputs = [ pkgs.stdenv.cc.cc.lib ];
        installPhase = "install -Dm755 $src $out/bin/objdiff-cli";
      };

      # The parallel data project needs the Gruntz BSS and DIR32-addend fixes.
      vostok-data-objdiff-cli = nightly-rustPlatform.buildRustPackage {
        pname = "vostok-data-objdiff-cli";
        version = "3.7.3";
        src = objdiff-src;
        patches = [
          ./tools/objdiff-bss-inferred-extent.patch
          ./tools/objdiff-score-reloc-addend.patch
        ];
        cargoHash = "sha256-Z9vyUj35nrHuUoOYM54RLCn7CzcQ6k3A6FsDYKCVqVM=";
        cargoBuildFlags = [ "-p" "objdiff-cli" ];
        cargoTestFlags = [ "-p" "objdiff-core" "-p" "objdiff-cli" ];
        cargoInstallFlags = [ "-p" "objdiff-cli" ];
        postInstall = ''
          mv $out/bin/objdiff-cli $out/bin/vostok-data-objdiff-cli
        '';
      };

      objdiff = pkgs.stdenv.mkDerivation {
        pname = "objdiff";
        version = objdiffVersion;
        src = pkgs.fetchurl {
          url = objdiffUrl "objdiff-linux-x86_64";
          hash = "sha256-LpBPYyWPzuX5jm02WUovzqJQyqz+l8SbRURHDWgFqq8=";
        };
        dontUnpack = true; # a bare binary; nothing to unpack
        # autoPatchelfHook: fix the foreign binary's interpreter + RPATH so it runs
        # on Nix. makeWrapper: provides the wrapProgram used in installPhase.
        nativeBuildInputs = [ pkgs.autoPatchelfHook pkgs.makeWrapper ];
        # Libraries autoPatchelf rewrites the RPATH against: the C++ runtime plus
        # the X/GL/Wayland libs that appear in the binary's DT_NEEDED list.
        buildInputs = [ pkgs.stdenv.cc.cc.lib ] ++ objdiffGuiLibs;
        installPhase = ''
          install -Dm755 $src $out/bin/objdiff
          # The GUI *also* dlopen's GL/Wayland/X lazily at runtime; those are NOT in
          # DT_NEEDED, so autoPatchelf can't see them. Expose them on LD_LIBRARY_PATH
          # so the runtime dlopen()s resolve.
          wrapProgram $out/bin/objdiff \
            --prefix LD_LIBRARY_PATH : "${pkgs.lib.makeLibraryPath objdiffGuiLibs}"
        '';
      };

      # The lean default dev shell. `with-resources` (below) extends it with the
      # heavy game-data outputs; everything common lives here.
      defaultDevShell = pkgs.mkShell {
        name = "surv-decomp";

        packages = [
          # Nightly Rust (for manual cargo builds of vostok-delinker etc.)
          rust

          # Wine - runs cl.exe / link.exe / ninja.exe / vcproj2ninja.exe.
          # MUST be staging (>= 10.20): cl /Zi spawns mspdbsrv.exe (the PDB
          # server) over RPC, and wine-10.0 stable fails to start RpcSs so
          # mspdbsrv never spawns -> "fatal error C1902: Program database manager
          # mismatch". wine-staging 10.20 spawns mspdbsrv correctly. See
          # docs/build/toolchain-build.md.
          pkgs.wineWow64Packages.staging

          # Scripts + handy tools
          pkgs.python3
          pkgs.ruff
          pkgs.ripgrep
          pkgs.libfaketime
          pkgs.file
          pkgs.xxd
          pkgs.jq
          pkgs.llvmPackages.bintools
          # sqlite3 CLI - handy for ad-hoc inspection; the matching record itself is
          # the text ledger (config/match_state.tsv), there is no database any more.
          pkgs.sqlite

          # clangd - source navigation/LSP over the generated
          # compile_commands.json (clang is a READER here; MSVC8 under Wine
          # stays the only build truth)
          pkgs.clang-tools

          # objdiff - GUI + CLI for comparing base vs target objects
          objdiff
          objdiff-cli
          vostok-data-objdiff-cli

          # Nix-built tools and assets - all evaluated when entering the shell.
          vostok-pdb-parser
          vostok-delinker
          vostok-data-delinker
          vostok-resources-db
          vcproj2ninja
          vostok-toolchain
          vostok-libs
          survarium
        ];

        shellHook = ''
          export VOSTOK_DIR="$PWD"
          # scripts/ is THE package root: on PYTHONPATH so `python3 -m vostok`
          # and every `python3 -m vostok.<pkg>` work from anywhere in the tree.
          # There are no flat scripts left, so this is the ONLY way the package
          # is found - a shell entered in one worktree keeps resolving `vostok`
          # to THAT worktree even after cd'ing into another.
          export PYTHONPATH="$VOSTOK_DIR/scripts''${PYTHONPATH:+:$PYTHONPATH}"
          export WINEPREFIX="$VOSTOK_DIR/binaries/.wineprefix"
          export WINEDLLOVERRIDES="mscoree,mshtml="
          # Silence Wine's unactionable debug spam during builds: all fixme
          # stubs (e.g. RtlSetHeapInformation HEAP_INFORMATION_CLASS) and the
          # kerberos err channel (no Kerberos support - expected). Genuine
          # err/warn from other channels stay visible.
          export WINEDEBUG="fixme-all,err-kerberos"

          export VOSTOK_TOOLCHAIN="${vostok-toolchain}"
          export MSVC_DIR="${vostok-toolchain}/msvc"
          export WINSDK_DIR="${vostok-toolchain}/winsdk"
          export DXSDK_DIR="${vostok-toolchain}/dxsdk"
          # fxc 9.29.952.3111 + native D3DCompiler_43.dll - the exact compiler
          # behind the shipped shader blobs; vostok.shaders runs it under Wine.
          export DXSDK_SHADER_COMPILER="${dxsdk-shader-compiler}/bin"
          export NINJA_DIR="${vostok-toolchain}/ninja"
          export VOSTOK_LIBS_DIR="${vostok-libs}"
          export VCPROJ2NINJA_EXE="${vcproj2ninja}/bin/vcproj2ninja.exe"
          export SURVARIUM_BIN="${survarium}"
          export BULLET_2_79_SOURCE="${bullet-2_79-src}/src"

          # Pin large fetched packages with indirect gcroots so `nix-store --gc`
          # doesn't delete them between dev shells. Symlinks live in
          # binaries/nix-store/<name> (e.g. binaries/nix-store/survarium-game).
          # The lean shell pins only what matching needs: the toolchain, libs,
          # the game binaries (survarium `out` = exe/pdb), and the small SSL keys.
          # The heavy ~1.5 GiB packed resources and ~1.6 GiB unpacked tree are
          # deliberately NOT realized/pinned here - they're opt-in via the
          # `with-resources` shell (`nix develop .#with-resources`).
          mkdir -p "$VOSTOK_DIR/binaries/nix-store"
          for pair in \
              "vostok-toolchain:${vostok-toolchain}" \
              "vostok-libs:${vostok-libs}" \
              "vcproj2ninja:${vcproj2ninja}" \
              "survarium-game:${survarium}" \
              "survarium-keys:${survarium.keys}" \
              "bullet-2.79-source:${bullet-2_79-src}" \
              "dxsdk-shader-compiler:${dxsdk-shader-compiler}"; do
            name="''${pair%%:*}"
            path="''${pair#*:}"
            nix-store -r "$path" \
              --add-root "$VOSTOK_DIR/binaries/nix-store/$name" \
              --indirect >/dev/null
          done

          python3 -m vostok.tool.toolchain

          # Wrap nvim to auto-load pdb_fetch.nvim (:Vostok match views), leaving the
          # user's own config intact. A wrapper SCRIPT on PATH (not a shell function)
          # survives `nix develop --command fish`; the real nvim is resolved before we
          # shadow it, VOSTOK_NVIM_WRAPPED guards nested shells, and rtp points at the
          # flake-pinned plugin so `nix flake update pdb-fetch-nvim-src` ships new
          # versions on the next `nix develop`.
          if [ -z "''${VOSTOK_NVIM_WRAPPED:-}" ] && command -v nvim >/dev/null 2>&1; then
            _vnv_bin="$VOSTOK_DIR/binaries/nvim-shim"
            mkdir -p "$_vnv_bin"
            # Resolve the REAL nvim with the shim dir off PATH. VOSTOK_NVIM_WRAPPED
            # alone is not enough: lose the variable but keep the PATH (env -u, a
            # shell that drops it, a reattached multiplexer) and `command -v nvim`
            # finds the shim, which then execs itself forever.
            _vnv_real="$(PATH="$(printf %s "$PATH" | tr ':' '\n' \
              | grep -vxF "$_vnv_bin" | paste -sd:)" command -v nvim)"
          fi
          # Empty means the only nvim on PATH was the shim: wrap nothing rather
          # than write a shim that execs "".
          if [ -n "''${_vnv_real:-}" ]; then
            printf '#!/bin/sh\nexec "%s" --cmd "set rtp^=%s" "$@"\n' \
              "$_vnv_real" "${pdb-fetch-nvim-src}" > "$_vnv_bin/nvim"
            chmod +x "$_vnv_bin/nvim"
            export PATH="$_vnv_bin:$PATH"
            export VOSTOK_NVIM_WRAPPED=1
            echo "[vostok] nvim       : WRAPPED -> auto-loads pdb_fetch.nvim (:Vostok, vbs/vts/vds, vo, V). Plain nvim is unchanged outside this shell." >&2
          fi
        '';
      };

      # Opt-in shell that adds the heavy game data on top of the lean default:
      #   nix develop .#with-resources
      # Realizes and pins the ~1.5 GiB packed resources + ~1.6 GiB unpacked tree
      # and exports VOSTOK_RESOURCES_DIR / VOSTOK_RESOURCES_UNPACKED. The bare
      # `nix develop` stays lean and never touches these.
      withResourcesDevShell = pkgs.mkShell {
        name = "surv-decomp-with-resources";

        # Reuse the entire default shell (packages + its shellHook run first).
        inputsFrom = [ defaultDevShell ];

        packages = [ vostok-resources-db ];

        shellHook = ''
          export VOSTOK_RESOURCES_DIR="${survarium.resources}"
          export VOSTOK_RESOURCES_UNPACKED="${survarium-resources-unpacked}"

          # Pin the heavy resource outputs so `nix-store --gc` keeps them.
          mkdir -p "$VOSTOK_DIR/binaries/nix-store"
          for pair in \
              "survarium-resources:${survarium.resources}" \
              "survarium-resources-unpacked:${survarium-resources-unpacked}"; do
            name="''${pair%%:*}"
            path="''${pair#*:}"
            nix-store -r "$path" \
              --add-root "$VOSTOK_DIR/binaries/nix-store/$name" \
              --indirect >/dev/null
          done

          echo "[vostok] resources  : REALIZED -> VOSTOK_RESOURCES_DIR + VOSTOK_RESOURCES_UNPACKED (opt-in shell)." >&2
        '';
      };

      # Opt-in shell for GFx lib rebuilds from a fresh clone:
      #   nix develop .#with-scaleform-sdk
      # Realizes and pins the 5.1 GiB DuckTales source fetch that carries the
      # Scaleform SDK, and exports SCALEFORM_SDK from the store. With a local
      # SDK checkout, the plain default shell + $SCALEFORM_SDK works instead
      # (paths.py falls back to ~/Projects/survarium/scaleform_sdk).
      withScaleformSdkDevShell = pkgs.mkShell {
        name = "surv-decomp-with-scaleform-sdk";
        inputsFrom = [ defaultDevShell ];
        shellHook = ''
          export SCALEFORM_SDK="${scaleform-sdk}"
          mkdir -p "$VOSTOK_DIR/binaries/nix-store"
          nix-store -r "${scaleform-sdk}" \
            --add-root "$VOSTOK_DIR/binaries/nix-store/scaleform-sdk" \
            --indirect >/dev/null
          echo "[vostok] scaleform  : REALIZED -> SCALEFORM_SDK (opt-in shell)." >&2
        '';
      };

    in {
      packages.${system} = {
        inherit vostok-pdb-parser vostok-delinker vostok-data-delinker vostok-resources-db vcproj2ninja
          vostok-toolchain vostok-libs survarium
          objdiff objdiff-cli vostok-data-objdiff-cli dxsdk-shader-compiler;
        # The heavy unpacked resource tree (~1.6 GiB) is kept buildable on demand
        # (`nix build .#survarium-resources-unpacked`), but the default devShell
        # does NOT realize it - see the `with-resources` shell below.
        inherit survarium-resources-unpacked;
        # The Scaleform GFx SDK (5.1 GiB fetch) - on demand / with-scaleform-sdk.
        inherit scaleform-sdk;
        # Convenience aliases for the individual survarium outputs:
        #   nix build .#survarium-game  /  .#survarium-resources  /  .#survarium-keys
        survarium-game = survarium;            # default `out` = game binaries
        survarium-resources = survarium.resources;
        survarium-keys = survarium.keys;
      };

      devShells.${system} = {
        default = defaultDevShell;
        with-resources = withResourcesDevShell;
        with-scaleform-sdk = withScaleformSdkDevShell;
      };
    };
}
