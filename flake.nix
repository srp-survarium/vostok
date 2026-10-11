# SPDX-License-Identifier: GPL-3.0-or-later

{
  description = "Vostok Engine decompilation - Linux build environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    rust-overlay = {
      url = "github:oxalica/rust-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    vcproj2ninja-src = {
      url = "github:srp-survarium/vcproj2ninja";
      flake = false;
    };
    vostok-resources-db-src = {
      url = "github:srp-survarium/vostok-resources-db";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, rust-overlay, vcproj2ninja-src, vostok-resources-db-src }:
    let
      system = "x86_64-linux";

      pkgs = import nixpkgs {
        inherit system;
        overlays = [ rust-overlay.overlays.default ];
      };

      # Nightly Rust toolchain for the Rust tools below (vcproj2ninja,
      # vostok-resources-db). Includes the Windows GNU target for vcproj2ninja.exe.
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
      # Produced on v0.10b by: nix-shell scripts/create-toolchain-release.nix  (reproducible -
      # a fresh rebuild yields a byte-identical tarball).
      # Uploaded to: gh release upload v0.100b binaries/vostok-toolchain-v0.100b.tar.xz \
      #                --repo srp-survarium/vostok --clobber
      # Replace sha256 with the value printed by vostok.tool.toolchain_release.
      # ---------------------------------------------------------------------------
      vostok-toolchain = pkgs.runCommand "vostok-toolchain" {
        src = pkgs.fetchurl {
          name = "vostok-toolchain-v0.100b.tar.xz";
          url = "https://github.com/srp-survarium/vostok/releases/download/v0.100b/vostok-toolchain-v0.100b.tar.xz";
          sha256 = "c9c5c17a51739472e11e9071b0274c3278930ddda0a8478858d81612bf895d99";
        };
        nativeBuildInputs = [ pkgs.gnutar pkgs.xz ];
      } ''
        mkdir -p "$out"
        tar xf "$src" -C "$out" --strip-components=1
      '';

      # ---------------------------------------------------------------------------
      # vostok-libs - proprietary third-party DLLs and import libraries.
      # Pre-packaged as a zip; the archive's top-level directory `vostok-libs/`
      # is stripped on unpack so $out exposes `sources/...` directly.
      # Uploaded to: gh release upload v0.100b vostok-libs-v0.100b-pc-only.zip --repo srp-survarium/vostok
      # gfx422: foreign 4.0.15 GFx libs replaced by our from-source 4.2.22 suite
      # (built per the shipped PDB recipe; see vostok/build/gfx.py on v0.10b).
      # ---------------------------------------------------------------------------
      vostok-libs = pkgs.runCommand "vostok-libs" {
        # PC-only package: console SDKs and console library builds removed.
        # GFx Shipping suite rebuilt from source with the 4.2.21
        # reconstructions (sockets config, pointers_to_members pragma,
        # hash-verified header/TU rollbacks) plus libgfxexpat.lib and pcre.lib
        # the exe pragma-links - cut by `vostok tool libs-release`, compiled
        # through the C:\survarium\gfx-sdk alias so the objects record that
        # neutral prefix (paths.GFX_RELEASE_PREFIX). Same machine code as r1.
        src = pkgs.fetchurl {
          name = "vostok-libs-v0.100b-pc-only.zip";
          url = "https://github.com/srp-survarium/vostok/releases/download/v0.100b/vostok-libs-v0.100b-pc-only.zip";
          sha256 = "9c990d54992b864d58908525b05ed88aecc8f71e76f1c9a44112b3295fea4836";
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

      # The lean default dev shell. `with-resources` (below) extends it with the
      # heavy game-data outputs; everything common lives here.
      defaultDevShell = pkgs.mkShell {
        name = "surv-decomp";

        packages = [
          # Wine - runs cl.exe / link.exe / ninja.exe / vcproj2ninja.exe.
          # MUST be staging (>= 10.20): cl /Zi spawns mspdbsrv.exe (the PDB
          # server) over RPC, and wine-10.0 stable fails to start RpcSs so
          # mspdbsrv never spawns -> "fatal error C1902: Program database manager
          # mismatch". wine-staging 10.20 spawns mspdbsrv correctly. See
          # docs/build/toolchain-build.md on v0.10b.
          pkgs.wineWow64Packages.staging

          # Scripts + handy tools
          pkgs.python3
          pkgs.ruff
          pkgs.ripgrep
          pkgs.gh
          pkgs.libfaketime
          pkgs.file
          pkgs.xxd
          pkgs.jq
          pkgs.llvmPackages.bintools

          # clangd - source navigation/LSP over the generated
          # compile_commands.json (clang is a READER here; MSVC8 under Wine
          # stays the only build truth)
          pkgs.clang-tools

          # Nix-built tools and assets - all evaluated when entering the shell.
          vcproj2ninja
          vostok-toolchain
          vostok-libs
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
          export NINJA_DIR="${vostok-toolchain}/ninja"
          export VOSTOK_LIBS_DIR="${vostok-libs}"
          export VCPROJ2NINJA_EXE="${vcproj2ninja}/bin/vcproj2ninja.exe"

          # Pin large fetched packages with indirect gcroots so `nix-store --gc`
          # doesn't delete them between dev shells. Symlinks live in
          # binaries/nix-store/<name> (e.g. binaries/nix-store/vostok-toolchain).
          # The lean shell pins only what the build needs: the toolchain, libs
          # and vcproj2ninja. The original game (installer, ~1.5 GiB packed
          # resources, ~1.6 GiB unpacked tree) is NOT realized here - it's
          # opt-in via the `with-resources` shell (`nix develop .#with-resources`).
          # Read-only tools need the flake environment without worktree setup.
          if [ "''${VOSTOK_SKIP_SETUP:-0}" != 1 ]; then
            mkdir -p "$VOSTOK_DIR/binaries/nix-store"
            for pair in \
                "vostok-toolchain:${vostok-toolchain}" \
                "vostok-libs:${vostok-libs}" \
                "vcproj2ninja:${vcproj2ninja}"; do
              name="''${pair%%:*}"
              path="''${pair#*:}"
              nix-store -r "$path" \
                --add-root "$VOSTOK_DIR/binaries/nix-store/$name" \
                --indirect >/dev/null
            done

            python3 -m vostok.tool.toolchain || exit $?
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
          if [ "''${VOSTOK_SKIP_SETUP:-0}" != 1 ]; then
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
          fi

          echo "[vostok] resources  : REALIZED -> VOSTOK_RESOURCES_DIR + VOSTOK_RESOURCES_UNPACKED (opt-in shell)." >&2
        '';
      };

    in {
      packages.${system} = {
        inherit vostok-resources-db vcproj2ninja vostok-toolchain vostok-libs survarium;
        # The heavy unpacked resource tree (~1.6 GiB) is kept buildable on demand
        # (`nix build .#survarium-resources-unpacked`), but the default devShell
        # does NOT realize it - see the `with-resources` shell below.
        inherit survarium-resources-unpacked;
        # Convenience aliases for the individual survarium outputs:
        #   nix build .#survarium-game  /  .#survarium-resources  /  .#survarium-keys
        survarium-game = survarium;            # default `out` = game binaries
        survarium-resources = survarium.resources;
        survarium-keys = survarium.keys;
      };

      devShells.${system} = {
        default = defaultDevShell;
        with-resources = withResourcesDevShell;
      };
    };
}
