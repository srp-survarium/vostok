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
