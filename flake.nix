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
      url = "github:srp-survarium/vostok-pdb-parser";
      flake = false;
    };
    vcproj2ninja-src = {
      url = "github:srp-survarium/vcproj2ninja";
      flake = false;
    };
    vostok-delinker-src = {
      url = "github:srp-survarium/vostok-delinker";
      flake = false;
    };
    vostok-resources-db-src = {
      url = "github:srp-survarium/vostok-resources-db";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, rust-overlay, vostok-pdb-parser-src, vcproj2ninja-src, vostok-delinker-src, vostok-resources-db-src }:
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
        cargoHash = "sha256-XUF9ca0D1k5NhR6tZth2/yactZ1NyWc8W9voWNRXcDI=";
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
        cargoHash = "sha256-ry3TH1fz7Aj/JdbmlgQFFn29m8E7EQHyGaVXnZTEcXo=";
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
        cargoHash = "sha256-SKEVJ/2wEmEfevCJe8WtVief3BL25K2OmsYjWv9SSC4=";

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
      # vostok-libs - proprietary third-party DLLs and import libraries.
      # Pre-packaged as a zip; the archive's top-level directory `vostok-libs/`
      # is stripped on unpack so $out exposes `sources/...` directly.
      # Uploaded to: gh release upload v0.100b vostok-libs-v0.100b.zip --repo srp-survarium/vostok-build-env
      # ---------------------------------------------------------------------------
      vostok-libs = pkgs.runCommand "vostok-libs" {
        src = pkgs.fetchurl {
          name = "vostok-libs-v0.100b.zip";
          url = "https://github.com/srp-survarium/vostok-build-env/releases/download/v0.100b/vostok-libs-v0.100b.zip";
          sha256 = "0rr63ifgv0mkpwy2acm6zrn7ni4qk3ls5gy52fmizf02yr8jivb2";
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

      # CLI: autoPatchelf + the C++ runtime is enough - it dlopen's nothing, so no
      # LD_LIBRARY_PATH wrapper (and hence no makeWrapper) is needed.
      objdiff-cli = pkgs.stdenv.mkDerivation {
        pname = "objdiff-cli";
        version = objdiffVersion;
        src = pkgs.fetchurl {
          url = objdiffUrl "objdiff-cli-linux-x86_64";
          hash = "sha256-QNhW2gHgpnbA8zr1NOVi8JjNUORey2Tzs0ZBjHsmSuY=";
        };
        dontUnpack = true; # a bare binary; nothing to unpack
        nativeBuildInputs = [ pkgs.autoPatchelfHook ];
        buildInputs = [ pkgs.stdenv.cc.cc.lib ]; # libstdc++ / libgcc_s (its DT_NEEDED deps)
        installPhase = "install -Dm755 $src $out/bin/objdiff-cli";
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

    in {
      packages.${system} = {
        inherit vostok-pdb-parser vostok-delinker vostok-resources-db vcproj2ninja
          vostok-toolchain vostok-libs survarium survarium-resources-unpacked
          objdiff objdiff-cli;
        # Convenience aliases for the individual survarium outputs:
        #   nix build .#survarium-game  /  .#survarium-resources  /  .#survarium-keys
        survarium-game = survarium;            # default `out` = game binaries
        survarium-resources = survarium.resources;
        survarium-keys = survarium.keys;
      };

      devShells.${system}.default = pkgs.mkShell {
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
          pkgs.file
          pkgs.xxd
          pkgs.jq

          # objdiff - GUI + CLI for comparing base vs target objects
          objdiff
          objdiff-cli

          # Nix-built tools and assets - all evaluated when entering the shell.
          vostok-pdb-parser
          vostok-delinker
          vostok-resources-db
          vcproj2ninja
          vostok-toolchain
          vostok-libs
          survarium
        ];

        shellHook = ''
          export VOSTOK_DIR="$PWD"
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
          export SURVARIUM_BIN="${survarium}"

          # Pin large fetched packages with indirect gcroots so `nix-store --gc`
          # doesn't delete them between dev shells. Symlinks live in
          # binaries/nix-store/<name> (e.g. binaries/nix-store/survarium-game).
          # The survarium outputs all come from one build, so pinning resources
          # and keys here costs nothing beyond getting the game binaries.
          # survarium-resources-unpacked is the resources.db expanded into its
          # file tree (built once with vostok-resources-db), pinned the same way.
          mkdir -p "$VOSTOK_DIR/binaries/nix-store"
          for pair in \
              "vostok-toolchain:${vostok-toolchain}" \
              "vostok-libs:${vostok-libs}" \
              "vcproj2ninja:${vcproj2ninja}" \
              "survarium-game:${survarium}" \
              "survarium-resources:${survarium.resources}" \
              "survarium-resources-unpacked:${survarium-resources-unpacked}" \
              "survarium-keys:${survarium.keys}"; do
            name="''${pair%%:*}"
            path="''${pair#*:}"
            nix-store -r "$path" \
              --add-root "$VOSTOK_DIR/binaries/nix-store/$name" \
              --indirect >/dev/null
          done

          python3 "$VOSTOK_DIR/scripts/setup-toolchain.py"
        '';
      };
    };
}
