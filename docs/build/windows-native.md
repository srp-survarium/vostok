# Native Windows build

`scripts/windows/` builds the exe with the same VS2008 toolchain directly on Windows,
without Wine. It compiles much faster than under Wine, which makes it the quick route to a
runnable `survarium-dx11-win32-gold.exe`.

The committed scores, ledger and README block come only from `python3 -m vostok build`
(Linux/WSL with Nix); every measured commit is built there. `score.ps1` adds an optional fast
**preview** of a native build's scores ([Scoring preview](#scoring-preview-optional)) for quick
feedback while editing.

There are two ways to set it up:

- **Native**: no WSL. Setup downloads the toolchain and third-party libraries from the release
  archives `flake.nix` pins, and builds vcproj2ninja from the rev `flake.lock` pins. The graph is
  regenerated on Windows before every build. You edit and commit in this checkout.
- **WSL mirror**: a WSL checkout stays where you edit, commit and run `vostok build`; this
  checkout mirrors it (`sync.ps1`) and only builds.

Either way the checkout is junctioned to `C:\survarium`. That path is required: retail objects
record `c:\survarium\sources`, and the graph is rooted there (`paths.NATIVE_BUILD_ROOT`). Only one
checkout can own the junction at a time.

Generated state stays under the gitignored `binaries/`:

| Path | Contents |
|---|---|
| `binaries\windows\toolchain` | The staged toolchain, about 1 GiB. Set `VOSTOK_WIN_TOOLCHAIN` to keep it elsewhere. |
| `binaries\windows\vcproj2ninja`, `binaries\windows\rust` | Native mode: the generator, and the private nightly Rust that built it. |
| `binaries\windows\downloads` | Native mode: the hash-checked release archives. |
| `binaries\windows\logs` | Build logs. |
| `binaries\windows\scoring\bin`, `binaries\windows\llvm-mingw` | Scoring preview: the tools and the C toolchain they were built with. |
| `binaries\windows\preview` | Scoring preview: the last two preview ledgers. |
| `binaries\ninja`, `binaries\Win32` | The graph and the build outputs. |

## Native setup (once)

You need Windows 10/11, Git for Windows, Python 3.11+ on `PATH`, and about 10 GB free.

```powershell
git clone -c core.autocrlf=false https://github.com/srp-survarium/vostok F:\vostok
cd F:\vostok
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\setup.ps1 -Native
```

`core.autocrlf=false` keeps the sources byte-identical to the Linux checkouts. `setup.ps1 -Native`:

1. **Toolchain.** Downloads `vostok-toolchain-v0.100b.tar.xz` and checks it against the sha256 in
   `flake.nix`, then extracts it into `binaries\windows\toolchain`.
2. **Third-party libraries.** Downloads `vostok-libs-v0.100b-pc-only.zip` (also hash-checked) and
   stages it into `binaries.prebuilt` with `vostok.tool.libs`, the same step `nix develop` runs.
3. **vcproj2ninja.** Builds it at the `flake.lock` rev with `cargo install`.
   - The crate needs nightly Rust. Setup installs a private rustup under `binaries\windows\rust`
     (GNU host, so no Visual Studio is needed) and leaves your `PATH` and profile alone.
   - The GNU toolchain's own `dlltool` needs an assembler that rustup does not ship. Setup
     passes `-Cdlltool=` pointing at a copy of `llvm-ar` from `llvm-tools`, which works as a
     self-contained `dlltool`.
   - If the latest nightly breaks the build, pass `-RustToolchain nightly-YYYY-MM-DD`.
   - `-Vcproj2NinjaExe <exe>` uses an existing build instead.
4. **CRT, junction, graph.** Installs the VC90 CRT beside `cl.exe`, creates the junction, and
   generates the graph.

Each step is skipped when its output exists. `-Force` redoes them and repoints a junction owned by
another checkout. After `flake.lock` moves vcproj2ninja, `build.ps1` warns until you rerun
`setup.ps1 -Native -Force`.

## WSL-mirror setup (once)

You need a WSL2 distro whose checkout has completed `nix develop` and one
`python3 -m vostok build`. That build gives it `binaries.prebuilt/`, the `vostok-toolchain`
out-link and `binaries/ninja/`.

```powershell
git clone -c core.autocrlf=false https://github.com/srp-survarium/vostok F:\vostok-win
cd F:\vostok-win
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\setup.ps1 -Distro <distro> -WslRepo ~/vostok
```

This copies the toolchain and `binaries.prebuilt` out of the WSL checkout, adds the WSL checkout as
the `wsl` remote, installs the CRT and creates the junction.

## The loop

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\sync.ps1     # WSL mirror only
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\build.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\deploy.ps1 -GameDir <game>\binaries\win32
```

- **`build.ps1`** builds the exe.
  - With vcproj2ninja installed, it first regenerates the graph (`vostok.build.ninja_regen`), as
    `vostok build` does. Only the graph files whose contents changed are rewritten, so new
    `#include`s and `.vcproj` edits take effect, and an unchanged tree rebuilds nothing.
  - It then runs the toolchain's `ninja.exe` with the same PATH/INCLUDE/LIB that `vostok tool
    toolchain` puts in the Wine registry.
  - It prints `native build: rc=… min steps= errors= link stalls= log=` and exits with ninja's status.
  - Options: `-Clean` runs `ninja -t clean` first; `-NoRegen` keeps the current graph; `-Target`
    builds another ninja target.
- **`sync.ps1`** (WSL mirror) mirrors the WSL checkout into this one.
  - It checks out the WSL branch tip, applies the uncommitted diff under `sources/`, and copies
    untracked source files.
  - It then copies `binaries/ninja`, rewriting Wine's `Z:<wsl repo>` root to `C:\survarium`.
  - It discards the previous sync's changes, and refuses if anything else here is uncommitted
    (`-Force` discards that too).
  - `-SourcesOnly` or `-GraphOnly` runs one half.
- **`deploy.ps1`** copies the exe into a game install as `survarium_rebuilt.exe`, next to the retail exe.
  - It also copies the PDB under its linked name, so crash reports symbolize.
  - Run it with `-no_splash_screen -client=<host:port>`.
  - Add `-autologin[=name:password]`, a dev-only switch in `login_menu.cpp`, to sign in without clicking.

## Scoring preview (optional)

`score.ps1` scores a native build in minutes, against the hour-plus Wine run. It runs the base
side of `vostok build` on the exe `build.ps1` produced:

- PDB evidence, the code objdiff report, and the structure stubs;
- the ledger, re-derived exactly as `vostok build` does.

It then prints how the result differs from the committed `config/match_state.tsv`. It writes
nothing that is committed: the working ledger is restored, and the preview is kept in
`binaries\windows\preview`.

It is a **preview, not a measurement**. The native link folds identical COMDATs slightly
differently from the Wine link the ledger is measured with:

- about 2% of functions score differently between the two (95.11% vs 95.19% fuzzy on the same
  sources);
- both are deterministic.

So:

- **Commit only Wine measurements.** Commits stay measured by `python3 -m vostok build`.
- **Compare preview to preview.** Each run keeps the previous preview, and two native previews
  of the same sources are identical. The diff `score.ps1` prints against the previous preview is
  therefore exactly what an edit changed. The first run can only compare with the committed Wine
  ledger, folding noise included.

### Setup

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\setup.ps1 -Native -Scoring
```

`-Scoring` works with either setup mode. It adds:

1. **llvm-mingw** (`20260922`, UCRT, hash-checked) in `binaries\windows\llvm-mingw`. `vostok-pdb`
   bundles SQLite, which needs a C compiler; llvm-mingw's clang also links the tools.
2. **Rust's `x86_64-pc-windows-gnullvm` target** in the private rustup.
3. **The scoring tools**, built with `cargo install` and copied to `binaries\windows\scoring\bin`:
   - `vostok-pdb` from `tools/vostok-pdb`;
   - `vostok-delinker` and `vostok-data-delinker` at their `flake.lock` revs;
   - `objdiff-cli` at the `flake.lock` objdiff rev.

   They import only system DLLs and the Universal C Runtime, which ships with Windows 10/11.
   Building them takes a few minutes; afterwards the Rust build cache is deleted.

Toolchain details, in case a nightly or llvm-mingw update breaks the build:

- **Target choice.** The tools must be built for `gnullvm`. Built for the plain GNU target, with
  the `llvm-ar` dlltool that vcproj2ninja uses, they link but crash at startup: the import stubs
  come out broken (a DEP violation).
- **Build scripts.** Cargo build scripts still run on the GNU host. They keep
  `-Cdlltool=<llvm-ar copy>`, passed as `CARGO_TARGET_X86_64_PC_WINDOWS_GNU_RUSTFLAGS` so that it
  does not reach the `gnullvm` binaries.
- **`dlltool.exe` on PATH.** objdiff's build scripts look for `dlltool.exe` on PATH, so setup puts
  a directory holding only that shim first. Putting all of `llvm-mingw\bin` on PATH instead makes
  those build scripts link with its `gcc` wrapper, and they fail.
- **Encoded rustflags in Windows PowerShell 5.1.** `CARGO_ENCODED_RUSTFLAGS` separates flags with
  the 0x1F character. PowerShell 5.1 has no `` `u{} `` escape, so build it with `[char]0x1f`.

### Run

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\build.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\score.ps1 -GameDir <game>\binaries\win32
```

`-GameDir`, or `SURVARIUM_BIN`, is the folder holding the original `survarium.exe` and
`survarium.pdb`. The first run generates the target side from them: the retail COFF, structure,
PDB evidence and data inventory, as `vostok tool toolchain` does. Later runs reuse it. `-Top N`
lists more of the changed functions.

Measured on one machine:

| Run | Time |
|---|---|
| First run (includes generating the target side) | about 4.5 min |
| Each later run | about 2 min |
| `vostok build` in WSL, for comparison | over an hour |

A second run on unchanged sources reports `0 functions changed`.

The data lane (data COFF, image-data ledger, data gate) is not part of the preview.

## How the native graph differs from the Wine one

`vostok.build.ninja_regen` runs vcproj2ninja without `--wine` when it runs on Windows, against
`C:\survarium\sources\vostok v2.0.sln`. It applies the same retail corrections as under Wine: the
link library order, the sound archive member order, and the `c:/survarium/sources` compile
directory. It roots them at `C:/survarium` instead of `Z:<repo>`.

The graph is read and written as bytes, with universal-newline reads, so it stays LF on both
hosts. Apart from ninja pool names, which are derived from the root path, the native graph is
identical to a Wine graph rewritten to `C:\survarium`. The clangd inputs
(`compile_commands.json`) are not generated natively.

## Caveats

- **The LTCG link can stall.**
  - VS2008's code generator (`c2.dll`) sometimes waits forever on a stale handle: Windows has
    already reused that handle value for a thread-pool IoCompletion object.
  - `link.exe` then sits at "Generating code" with no CPU use.
  - `build.ps1` kills a link that makes no CPU progress for `-StallSeconds` (90) and reruns ninja,
    up to `-Attempts` (3). Only the link step reruns.
  - `relink.ps1` reruns just the exe link with `/ERRORREPORT:NONE` and prints its CPU every 30 s,
    for when you want to look at the link directly.
- **Moving the toolchain.** Precompiled headers record the include paths they were built
  with; after moving `binaries\windows\toolchain` (or changing `VOSTOK_WIN_TOOLCHAIN`), run
  `build.ps1 -Clean` or every TU fails with `sourceannotations.h` redefinitions.
- **mspdbsrv.** `build.ps1` stops the toolchain's `mspdbsrv.exe` when it finishes, so the next
  build does not inherit a server that holds stale PDB handles.
- **VC90 CRT.** `cl.exe`, `c1xx.dll`, `c2.dll` and `link.exe` request `Microsoft.VC90.CRT`
  9.0.21022.8. Setup copies the toolchain's redist into `msvc\VC\bin` as a private assembly, so a
  machine without the VC++ 2008 runtime can still start them.
