# Loop performance log

Shared, append-only notes on **making the match loop faster** — so each run waits
less than the last. This is the process/time analogue of
[`assembly_patterns.md`](assembly_patterns.md) (which is for asm→source mappings).

When a worker discovers anything that lets a future match need **fewer rebuilds**
or less waiting — a wiring trick, a step that turned out unnecessary, a cheaper
way to get the same signal — it appends a one- to three-line entry here. Keep it
concrete and actionable.

## What costs the time

- **`rebuild.py` is the dominant cost, and it is ~fixed per call** regardless of
  how small the function is. One invocation recompiles the whole changed module
  under Wine *and* reruns the delinker over the entire EXE to regenerate
  `binaries/objdiff/base` + `binaries/rich/base`. Budget ~10–15 min each.
- Therefore: **minimize the number of `rebuild.py` calls.** Everything else
  (reading target asm, writing the body, diffing) is cheap by comparison.

## How to need fewer rebuilds

- **Get the body as right as you can before the *first* rebuild.** Read the target
  with `pdb_fetch --view structure/target/callees/info` and wire reachability in
  `temp_include_all.cpp` in the *same* edit pass, so the first build already
  produces a diffable base object. A wasted first build (function stripped because
  it was unreachable) is a whole rebuild cycle lost.
- **The `--view diff` inspection step does NOT need a rebuild.** It reads the
  existing `binaries/rich/base` vs `target`. Only re-run `rebuild.py` after you
  have actually *changed source*; never rebuild just to "look again".
- **Batch source edits between rebuilds.** If the diff shows two independent fixes,
  apply both, then rebuild once.
- **An empty / trivial body should need one real rebuild** to confirm. If you find
  yourself rebuilding an empty function repeatedly, the problem is reachability
  wiring, not the body — fix the `temp_include_all.cpp` wiring and rebuild once.

_(Append new findings below this line.)_

- **Check the callee headers compile before the first rebuild.** A `*::serialize`
  taking `udp_match_packet&` looks trivial, but `network_core/udp_match_packet.h`
  (and its `sequence_number` / `boost::array` / `base_packet` deps) are
  never-compiled stubs that only break when a `.cpp` first `#include`s them. Grep
  `rg -l "<the type's header>" sources/` first: if *no* built `.cpp` includes it,
  expect a standalone-compile blocker and budget for it (or park BLOCKED) instead
  of spending a rebuild discovering it. Cost me one rebuild on
  `client_player_update::serialize`.

- **A correct pre-existing STUB body needs zero rebuilds.** If the function's symbol
  is already in `binaries/rich/base/index.jsonl` (the STUB compiled and is reachable)
  and `--view diff` shows the body is already byte-exact (remaining `~` rows are only
  register / stack-slot LTCG noise, or a cross-function inlining diff you can't steer
  from this function's source), the score in `report.json` is already final. Updating
  the STATE marker / adding a `claude@NOTE:` is a comment-only edit that cannot change
  compiled bytes, so don't rebuild to "confirm" - the existing number stands. Saved a
  full rebuild on `inventory_item::inventory_item`.

- **A constant-only default ctor reachable only via the temp_include_all anchor
  will compile EMPTY under /Od+/GL** - LTCG dead-store-eliminates every member
  store because no real consumer observes the object (an anchor that discards or
  even escapes `&obj` to an opaque external does not count). Before spending
  rebuilds trying to defeat it: such a ctor is a PARTIAL until its real game
  callers are matched. Write the (correct) body once, anchor it once to confirm
  the symbol survives + the score, then STOP - don't iterate the anchor.
  Cost me 2 wasted rebuilds chasing it on `weapon_recoil_params::weapon_recoil_params()`.
- **`rebuild.py <module>` does NOT relink the EXE - it only builds that ninja
  sub-target (the module's `.lib`).** `rebuild.py` forwards its args to
  `ninja_build.py`, which passes them through as ninja *targets*; with no arg it
  builds the full game (`survarium_-_PC_-_DirectX_11`) and relinks the EXE. The
  delinker/rich index read the linked **EXE**, so if you pass a module name the
  EXE is stale and your source change does not show up in `--view diff` or the
  score (build finishes in ~1 min instead of ~20, and `report-changes.json` shows
  `+0.00 / 0 changed` - the tell). Run **`python3 scripts/rebuild.py`** with no
  module arg so the EXE actually relinks. Cost me one wasted rebuild on
  `scheduler::on_frame`. (Note: the per-function loop doc's `rebuild.py <module>`
  is misleading on this point.)
- **MSVC LTCG objects are ANONYMOUS OBJECTs (IL, not machine code).** `dumpbin
  /DISASM` on `binaries/Win32/intermediates/.../*.obj` prints nothing useful -
  codegen happens at link. The only post-codegen disassembly is the delinked
  base obj / `binaries/rich/base`, which requires the full EXE relink (above).
- **Read the actual float/struct constants straight out of the target `.obj`**
  instead of guessing - rich/objdiff mask rdata operands as `[0]`/`[offset]`.
  A ~30-line Python COFF parser (section headers -> dump `.rdata` as f32 ->
  walk `.text` type-20 relocations for each `movss xmm0,[offset]` site) gives you
  every constant *and* the member->value map with **zero rebuilds**. Do this in
  the first pass so the very first body you write already has the right literals.
