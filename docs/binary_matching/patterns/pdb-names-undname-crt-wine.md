# Retail PDB type names are `__unDName` renderings — Wine's builtin msvcr90 spells them differently

- confidence: 10
- cpp tags: cpp:template cpp:enum cpp:const
- asm tags: asm:none
- topic tags: topic:tooling topic:pdb-records topic:wine-environment topic:mangling

## Symptom

Retail `survarium.pdb` spells template-instance UDT records with elaborated
enums and postfix-const-plus-space —

    flags_type<enum vostok::resources::cook_base::flags_enum,vostok::threading::single_threading_policy>
    pair<unsigned int const ,vostok::render::shader_configuration>

— while our base gold PDB spells the SAME types bare:

    flags_type<vostok::resources::cook_base::flags_enum,...>
    pair<unsigned int const,...>

Uniformly, both sides: ~1,700 elaborated records in retail, zero in ours.
Not full undname (no `class`/`struct` elaboration), so it looks like a third
mystery printer. `pdb_divergence` drowns in the spelling delta unless its
canon normalization fences it.

## Cause (proven 2026-08-25, probe A/B)

MSVC composes these display names by **undecorating the mangled name through
the CRT's `__unDName`** — `c1xx.dll`, `c2.dll` and `link.exe` all import it
from `msvcr90.dll`. Under Wine, msvcr90 resolves to Wine's **builtin**
reimplementation, whose undecorator renders enums bare and const without the
trailing space. Retail compiled on real Windows with Microsoft's msvcr90.

Proof chain:
- Same probe TU, same `cl.exe 15.00.30729.01`, same flags, same machine —
  swapping only the CRT flips every spelling to retail's exactly
  (compile-time `.debug$T`, `/Zi` and `/GL`+LTCG link paths all confirmed).
- The GSC-era Windows-built boost vc90 libs in vostok-libs are full of
  elaborated names; Rich headers of retail and base exes carry identical
  toolchain fingerprints (all build 30729), ruling out a version difference.
- Both name-authoring stages render: struct records bake at COMPILE time
  (into /GL IL or `.debug$T`), method records render at LINK time — so
  changing the CRT needs a full clean rebuild to take effect everywhere.

## Resolution

**Where the DLL comes from:** it ships INSIDE the toolchain -
`$MSVC_DIR/VC/redist/x86/Microsoft.VC90.CRT/msvcr90.dll` (Microsoft's genuine
"Visual C++ Runtime Library", 9.00.21022.8, 655,872 B). Nothing is downloaded;
the correct CRT was always present, just never loaded.

**Why the wrong one loaded:** Wine ships a from-scratch reimplementation of
`msvcr90` (a ~1 MB Winelib stub). cl.exe's embedded SxS manifest requests the
assembly `Microsoft.VC90.CRT`; Wine resolves it to a fake winsxs assembly
(`…_deadbeef`) that maps to the builtin, unless told to prefer native. So the
builtin always won. It went unnoticed because the CRT is functionally fine for
RUNNING the compiler - only `__unDName`'s cosmetic name rendering differs.

**The fix is prefix SETUP, not a per-build step** (`native_crt.py`,
`provision()`): (1) copy the redist CRT into the winsxs assembly dir
(`winsxs/x86_microsoft.vc90.crt_*_deadbeef/`), and (2) write a persistent
`msvcr90=native` DllOverride into the prefix's Wine registry
(`HKCU\Software\Wine\DllOverrides`). Then every `wine` in that prefix prefers
the native CRT with NO `WINEDLLOVERRIDES` on the command line (proven: env var
unset, cl still emits the elaborated spelling). `toolchain.py` provisions in
the wine + registry stages; `ninja.py` only self-heals a pre-existing prefix
once (no-op afterward), never touching the environment.

Traps, all hit while landing this:
- **wineboot pre-creates Wine STUB PEs at the winsxs paths** (~1MB vs native
  655,872B). Copy-if-missing keeps the stub and then every `cl` dies at load
  (`err:module:import_dll Library MSVCR90.dll ... not found`) because the
  override forbids the builtin and the "native" file found is a stub.
  Install must compare sizes and overwrite.
- app-dir and system32 copies do NOT satisfy the loader: cl.exe's embedded
  manifest forces SxS resolution to the winsxs assembly.
- `wine reg add` flushes user.reg only on wineserver shutdown, so verify the
  override with a LIVE `wine reg query`, not a disk read of user.reg.
- An old prefix that still misbehaves is cheapest to reinitialize
  (`python3 -m vostok tool toolchain --force wine registry`).

The pdb-parser canon normalization (branch divergence-canon) compensated for
this on the comparison side; with builds on native msvcr90 it becomes
redundant rather than wrong.
