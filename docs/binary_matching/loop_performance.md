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
- **A getter whose carcass shows `return 0.0f` + a collapsed FUNCTION BODY can still be a
  big function.** The base-build STUB compiled tiny, so its carcass addresses/source-lines
  are sparse (`<0> <1> ...` placeholders) and look trivial. Always pull the *target* asm
  (`pdb_fetch --view target --rva <target rva>`) - the `--list` rva (target) differs from the
  carcass addresses (base build). `get_dispersion`'s carcass implied ~3 statements; the target
  was a 0x194-byte function with a smart-ptr temp, a ternary, and a 5-term FPU expression.
- **Read ALL member offsets straight from the target asm before writing - one pass, zero
  re-rebuilds.** Cross-reference each `[reg+off]` against the relevant `*.h` member layout
  comments (`/* 0xNN */`). Getting every offset right up front (base_dispersion@0, aim@8,
  m_dispersion@0x11c, m_value@0x2C, m_shooting_skill_coeff@0x44) made the first rebuild land
  at 87.49% with nothing left but LTCG noise - no second rebuild needed.
- **An empty-body inline getter (`{ /* no source */ }`) used by your target expr must be
  filled in (`{ return m_x; }`) or the member read won't compile to the right offset.** Fix it
  in the same edit pass as the body (here `character_dispersion_calculator::get_value`); it's a
  header change but costs no extra rebuild if batched.
- **`fuzzy_match_percent: None` in report.json = mangled-name mismatch, usually
  access specifier.** Before assuming the function is unreachable, dump the target
  obj symbol table and compare the mangled name to base: a `QBE` (public) vs `ABE`
  (private) leading-letter difference makes objdiff unable to pair them and it
  reports `None`. Fix the access specifier in the header to match the target's
  mangling and the score appears. Caught on `get_target_koef` (cost one rebuild,
  but the COFF symbol dump would have caught it pre-build: read the target obj's
  `?fn@...@@` prefix in the same first pass you read the float constants).
- **A sibling getter's whole SCAFFOLDING may live in an OPEN, unmerged PR - check
  before your first rebuild.** `get_broken_hands_penalty` needed the private-move +
  `#include params.h` + `tick`-stub-calls-the-getters anchor + `temp_include_all`
  anchor that PR #110 (`get_target_koef`) built. The orchestrator forward-ported only
  the *ledger/docs* to the main branch; the *source* sat on PR #110's branch (OPEN).
  Building on the bare branch dead-stripped all four functions (278-byte obj, score
  None) - one wasted rebuild. FIX: `git branch -a | grep <sibling-fn>`; if its PR is
  open, `git checkout -b match/<yours> match/<sibling-branch>` so the scaffolding
  exists, then add only your body. Cost me one rebuild before I found PR #110.
- **Distinguish real regressions from baseline-artifact regressions in
  `report-changes.json`.** `report.json` is overwritten by every rebuild regardless
  of which branch produced it. If the previous report was generated on a tree that had
  *other* open-PR branches' scaffolding merged in, a clean rebuild on your bare branch
  will show those other functions dropping to 0.0 - they were never reachable on YOUR
  branch's committed source. Triage: (1) does your `git diff` touch that function or its
  reachability? (2) `git show HEAD:<that file>` - is it still a `STATE[STUB]`/unanchored?
  If both say no, it's a stale-baseline artifact, not your regression. (Saw 4
  `character_dispersion_calculator` fns drop 88/82/25/22 -> 0 on a fresh build of a branch
  where they are all STUBs; my change only touched `weapon_dispersion_calculator`.)
- **A trivial member getter with NO callees needs exactly one rebuild.** Read the
  `fld dword ptr [eax+0xNN]` offset, map it to the `/* 0xNN */` member, write
  `return m_member;`, anchor it (instantiate + call) in temp_include_all, rebuild once
  -> 100%. No `--view diff` round trip needed (`get_value` @ 0x18 = m_current_coeff).
- **`pdb_fetch --view diff` silently refuses ("needs both indexes") when the
  `--function` substring matches >1 index entry** - notably it also matches a
  *caller* whose `callees` field contains your function name (e.g. `get_target_koef`
  also matches `tick`). Workaround: byte-diff the two COFF `.text` regions directly
  with the COFF parser (locate the fn symbol's `.text` offset on each side, slice
  and `.hex()` compare). The score/report-changes remain authoritative for the number.
- **A trivial setter (`m_x = arg;`) needs its store OBSERVED or LTCG DSE's it - one
  rebuild if you anchor it right the first time.** Calling `calc.set_x(v)` in the
  anchor is NOT enough: LTCG can inline the setter into the anchor and then prove the
  object dead and delete the store (same elision as the constant-only ctor). Anchor
  that lands the setters at 100% on the first build: (1) put the setters' member-fn
  addresses in a local table and escape `&table` through an opaque sink
  (`example_callback`) so the standalone bodies are kept un-inlined; (2) call the
  setters on a local `calc`, then escape `&calc` through the same opaque sink so the
  stores are observed. Got all three `weapon_dispersion_calculator` setters to 100%
  in a single rebuild this way. (A reader that touches the *same* members would also
  work, but these members had no getter.)
- **A POD-struct `operator=` (member-wise scalar/byte copy) + its empty default ctor
  both hit 100% in ONE rebuild from a single anchor.** For a 3-`u8` `struct
  weapon_state` (no copy ctor), the anchor `weapon_state a, b; b = a; example_callback(&a);
  example_callback(&b);` landed `operator=` AND the empty default ctor at 100% in one
  build. Two notes: (1) read the member order straight from the `mov [eax+0xNN], dl`
  byte copies and match declaration order; (2) reproduce EXACTLY - this target had **no**
  `if (this != &other)` self-guard (the carcass FUNCTION BODY starts with the copies at
  line 27, no `if` line), unlike `player_stamina`/`player_stealth` which did guard. Don't
  add the guard by reflex; the asm decides.
- **A trivial copy ctor + operator= pair (member-wise scalar copy) needs ONE rebuild
  if you (a) copy the existing 100% sibling's shape (`player_stamina`: ctor `*this =
  other;`, operator= self-guard + decl-order member copies + `return *this`) and (b)
  anchor with default-construct + copy-construct + direct `b = a` + escape `&a`/`&b`
  through the `example_callback` opaque sink.** Both `player_stealth` members hit 100%
  first try, no `--view diff` round trip. Bonus: anchoring also pulled the (empty)
  default ctor to 100% for free. Read the member offsets straight from the operator='s
  `fld/fstp [reg+0xNN]` and map to the header `/* 0xNN */` comments in the first pass.
