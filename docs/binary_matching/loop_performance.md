# Loop performance log

Shared, append-only notes on **avoiding wasted rebuilds** — getting the body and
its wiring right the first time so a rebuild cycle is never spent on a reachability
or compile bug. This is the process analogue of
[`assembly_patterns.md`](assembly_patterns.md) (which is for asm→source mappings).

When a worker discovers anything that lets a future match converge in **fewer
iterations** or with less wasted work — a wiring trick, a step that turned out
unnecessary, a cheaper way to get the same signal — it appends a one- to three-line
entry here. Keep it concrete and actionable.
When a worker discovers anything that lets a future match need **fewer rebuilds** —
a wiring trick, a step that turned out unnecessary, a cheaper way to get the same
signal — it appends a one- to three-line entry here. Keep it concrete and
actionable.

## What a rebuild does

- The dominant cost is now **token consumption** (agent context re-read on each
  iteration), NOT the rebuild. A full `rebuild.py` (recompile the changed module
  under Wine + rerun the delinker over the EXE to regenerate `binaries/objdiff/base`
  + `binaries/rich/base`) is ~10 min and runs in the **background** — it is not the
  thing to obsess over minimizing.
- Therefore: **get the function RIGHT in as few iterations as possible** — each wrong
  iteration means re-reading the target/context (tokens) and another wait. Reading
  target asm, writing the body, diffing are all cheap; a wasted *iteration* is the
  expensive part. (The tips below still apply: they were written as "saves a rebuild"
  but the real win is saving an iteration.)
- **`rebuild.py` is the dominant cost, and it is ~fixed per call** regardless of
  how small the function is. One invocation recompiles the whole changed module
  under Wine *and* reruns the delinker over the entire EXE to regenerate
  `binaries/objdiff/base` + `binaries/rich/base`.
- Still worth **minimizing wasted `rebuild.py` calls** — a cycle spent on a
  reachability or compile bug teaches nothing. Everything else (reading target asm,
  writing the body, diffing) is cheap by comparison.

## How to converge in fewer iterations

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
  score (build finishes in ~1 min instead of ~10, and `report-changes.json` shows
  score (the build finishes fast but does NOT relink, and `report-changes.json` shows
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
- **A 3-member ctor/setter/dtor group landed in ONE rebuild (2x 100%, 1x PARTIAL) by
  reading the target asm + COFF constants up front and writing all three + the anchor
  before the first build.** Notes that saved rebuilds: (1) the ctor's "trivial" carcass
  (one source line at 0x9c) is a *member-init-list* + `initialize_logic()` call - write
  the init list in declaration order, NOT body assignments, or the store sequence won't
  match. (2) The dtor's two trailing masked `finalize_impl` calls are the *implicit*
  `ai::fsm` destructor - don't write them, the compiler emits them. (3) The setter
  iterated the fsm and called a virtual at `[vtbl+0x14]` on each state, which forces
  pulling in the derived type (`breath_state`, layout from `structure/target`) so the
  `static_cast` reaches the right vtable slot - create that header in the SAME edit pass.
  (4) The setter's residual diff was 100% the documented inline-vs-call of trivial COMDAT
  accessors (`states()/front()/current_state()/get_multiplier()`) - recognized it from
  assembly_patterns.md and stopped at PARTIAL with no second rebuild.
- **Before changing a correct body to chase a `call X` -> `inline of X` diff, DISASSEMBLE the
  base out-of-line `X` (zero rebuilds) and confirm its body == the target's inlined sequence.**
  If a WIP handoff says "the residual is the N-arg form of ctor X, rewrite the source to that form",
  first check: does the source's chosen overload ALREADY delegate to X? Query both rich indexes for
  X's symbol - if TARGET has no out-of-line X (inlined whole-program) but BASE keeps it at a real rva,
  it is the unsteerable LTCG inline-vs-call class. Then `pdb_fetch --view target --rva <base X rva>`:
  if base-X's body is exactly the target's inlined instructions, the source is already correct and NO
  rewrite/rebuild can help -> mark PARTIAL. Saved a wasted rebuild on
  `fill_new_stats_item`'s `fixed_string<46>("none")` (the const-char* ctor already delegates to the
  3-arg `buffer_string::buffer_string`; the "rewrite to 3-arg" handoff hint was moot).
- **A trivial copy ctor + operator= pair (member-wise scalar copy) needs ONE rebuild
  if you (a) copy the existing 100% sibling's shape (`player_stamina`: ctor `*this =
  other;`, operator= self-guard + decl-order member copies + `return *this`) and (b)
  anchor with default-construct + copy-construct + direct `b = a` + escape `&a`/`&b`
  through the `example_callback` opaque sink.** Both `player_stealth` members hit 100%
  first try, no `--view diff` round trip. Bonus: anchoring also pulled the (empty)
  default ctor to 100% for free. Read the member offsets straight from the operator='s
  `fld/fstp [reg+0xNN]` and map to the header `/* 0xNN */` comments in the first pass.
- **Anchoring an ABSTRACT, fsm_state-derived class costs a wasted link unless you provide the
  pure-virtual base dtor body.** To observe a protected ctor + a getter of an abstract class you
  need a concrete derived stub in temp_include_all (override the pure virtual with
  `VOSTOK_UNREACHABLE_CODE();` - no return value needed, it's `[[noreturn]]`-like). But constructing
  ANY fsm_state-derived instance pulls `??1fsm_state@ai@vostok@@UAE@XZ` into the vtable, and
  `~fsm_state()` is declared pure (`= 0`) with NO body in sources (and none on origin/.../match-fsm
  either) -> LNK2001. FIX in the SAME edit pass: define `vostok::ai::fsm_state::~fsm_state(){}` in
  temp_include_all.cpp (a non-target TU, so it can't regress a matched obj). A protected base getter
  also needs a public `call_x()` forwarder on the derived stub (free-fn anchor can't reach protected).
  Cost me one link failure on `weapon_core_base_state::{ctor,deserializing}`. Read the ctor/getter
  mangled access letters up front (IAE/IBE = protected) and set the decl's access specifier BEFORE
  the first build (else objdiff scores None - cost a 2nd rebuild here).
- **A free-function pair that returns a by-value struct (float4x4) hit 100% for BOTH
  in ONE rebuild** by reading the full target asm up front (pointer-diff index,
  operator* push order, the misnamed `float4x4()` ctor, member offsets) and writing
  both bodies + one anchor before the first build. Anchor that worked first try for a
  file-local `survarium` free function: forward-declare both fns in temp_include_all.cpp,
  add a `use_*` that (a) CALLS the wrapper on `*bone`/`*skeleton`/matrices and escapes
  the returned `float4x4` via `example_callback( reinterpret_cast<pcstr>(&result) )`,
  and (b) takes the impl's function-pointer and escapes that too. The wrapper's real
  `call` keeps the impl alive transitively, and escaping the by-value result keeps the
  observed body from being DSE'd. No `--view diff` round trip needed. Confirmed on
  `get_bone_matrix_in_object_space{,_impl}`. (The inline helper accessors it calls -
  `skeleton::get_root`, `get_root_bones_count` - also went 0->100 for free.)
- **Anchoring a STATIC member of an fsm_state-derived class pulls the WHOLE vtable
  chain - budget the stopgaps + a valid get_attachment_transform up front.** Even a
  pure static (no instance) anchor keeps the class's .obj, whose ctor emits the
  abstract vtable -> needs `vostok::ai::fsm_state::~fsm_state(){}` AND the class's own
  `~<derived>(){}` (both undefined in sources -> LNK2001), AND every inline virtual in
  the vtable with a non-void return must actually return (the `{ /* no source */ }`
  stub for `get_attachment_transform` -> C4716/LNK1257 at codegen). Write all three
  stopgaps in the SAME first edit pass (dtors in the non-target temp_include_all TU,
  the virtual's return in the header). Cost 2 rebuilds discovering them one at a time
  on `player_logic_base_state::movement_animation_index`.
- **A private STATIC member can't be anchored by a free `use_*` - befriend the anchor
  (free decls/friends don't change bytes).** When the target wants the static
  private/protected (mangling `C`/`K`), the temp_include_all anchor can't call it.
  Forward-declare the anchor in its namespace before the class and add
  `friend void ::ns::use_*();` in the class. Set the access specifier to match the
  target's storage-class char in the SAME pass (read it from the target COFF symbol:
  C=private, K=protected, S=public static) or objdiff scores None.
- **A target callee kept OUT-OF-LINE (`call X`) that our /GL LTCG INLINES at the caller is
  unsteerable - and the header decl/def split does NOT help.** `weapon_core_aimed_state_base::finalize`
  calls `animation_playback_state::reset()`; the target keeps reset standalone (@0x087f60, `call reset`)
  but our base has NO standalone reset (`pdb_rich_query base --function reset` -> "no function matched")
  because LTCG folds its tiny body into every caller whole-program. I burned 2 rebuilds trying to
  force a call: (a) filling reset's body IN-CLASS in the shared header -> /Od inlined it into finalize
  (54%, wrong frame); (b) splitting decl (in type_definitions.h) from an out-of-class `inline` def in a
  new header included only by temp_include_all.cpp -> /Od per-TU inline stopped, but `/GL` LTCG still
  inlined at LINK time (54%), AND an `inline` fn is only emitted by a TU that ODR-USES it, so I also hit
  LNK2001 until the anchor actually CALLED reset. NET: the decl/def split is useless against LTCG. Pick
  whichever CALLEE body makes the CALLER match best and stop: here the original empty `{}` stub ELIDES
  the no-op call (finalize 83%, the only diff is the 3 missing call instrs, frame correct) which beats
  the real-body inline (54%). Mark the caller PARTIAL [LTCG inline-vs-call], leave the callee stub
  untouched. Recognize this pattern from the asm BEFORE editing the shared header - a 0x97f60-class tiny
  member-zeroing reset called once is a textbook LTCG fold; verify with `pdb_rich_query base` (no standalone
  symbol) and stop at the empty-stub 83% in ONE rebuild.

- **Confirmed dead ends for the LTCG inline-vs-call of a tiny member (don't re-burn rebuilds on
  these).** For `animation_playback_state::reset()` folded into `weapon_core_aimed_state_base::finalize`,
  ALL of these still inlined under `/Od /Ob2 /GL`: (a) decl/def split into the class's OWN header
  compiled as its own TU; (b) `__declspec(noinline)` on the callee (LTCG ignored it - byte-identical
  output); (c) escaping `&reset` (member-fn-ptr) through the opaque sink - this even BACKFIRES, forcing
  the standard __thiscall ECX convention whereas the target callee uses an LTCG custom `this`-in-EAX
  convention; (d) adding multiple real same-module callers (gave a sibling finalize the same
  `reset()` body) - every site still inlined, and the sibling got WORSE. If the asm shows a tiny
  member-zeroing callee that the target keeps out-of-line (often with a non-__thiscall register
  convention = proof LTCG kept it standalone), accept the empty-stub elision at the caller and stop -
  none of these levers move it. (PR #124.)
- **A `boost::bind(&this_class::method, this, _1)` -> `set_X_callback(...)` whole statement
  matched 100% in ONE rebuild** - the bind temp / boost::function1 ctor / clear() dtor all
  inline byte-for-byte under /Od. No special anchoring beyond the standard observed-instance
  escape. The callback method it binds (here `on_animation_end`) only needs to EXIST (it does,
  it's another member of the unit). Confirmed on `weapon_core_animation_end_aware_state::initialize`.
- **A 4-member same-class group can split DONE/PARTIAL cleanly in ONE rebuild if you read the
  COFF access chars + member offsets + callee inline-vs-call up front.** Of
  `weapon_core_animation_end_aware_state::{initialize,finalize,set_animation_to_wait,on_animation_end}`:
  the two that only touch members + call NON-inlined methods hit 100%; the two that call a
  trivial /GL-inlined accessor (get_user) or carry the folded-empty-call artifact capped at
  ~77-78% PARTIAL. Both PARTIALs were recognizable from the asm pre-build (target keeps the
  callee standalone, base doesn't), so no second rebuild was spent chasing them.
- **A const method that ASSIGNS to a member needs the member `mutable` - catch it pre-build
  (it's a guaranteed C2678 otherwise).** When the target's `... const` method takes `&member`
  and calls a mutating op (`operator=`/byte store), declare the member `mutable` in the SAME
  edit pass as the body; otherwise the first rebuild is wasted on the compile error. Cost me
  one rebuild on `set_animation_to_wait` (the body was right, just needed `mutable`).
- **A trivial fsm_state-style virtual override (empty `{}` / `{ return true; }`) is
  BYTE-CORRECT but objdiff-UNSCORABLE - confirm it in ONE rebuild and stop, don't chase
  the `None`.** These header one-liners get `/OPT:ICF`-folded whole-program: the surviving
  representative symbol at the folded address differs build-to-build and side-to-side, so
  neither delinker re-attaches the body to `your_class::method` - the `.h` unit reads
  `fuzzy: None` with stray *other-class* fold-siblings (e.g. `breath_state*`) listed, and
  `binaries/rich/base/index.jsonl` has no standalone symbol for your function (same class as
  the documented `game_core_initialize` gap). PROVE the bytes instead of chasing a symbol:
  read the target fold body (`pdb_rich_query target --rva`), then scan
  `binaries/rich/base/index.jsonl` for functions whose `instructions` are byte-identical -
  if the empty-thiscall fold (push ebp;mov ebp,esp;push ecx;mov [ebp-4],ecx;mov esp,ebp;pop
  ebp;ret) and the return-true fold (...mov al,1...) both have a populated family (~100 / ~30
  members on game_core), your bytes are emitted and correct. Mark 100%|DONE. The 50-ish
  100<->0 rows in report-changes are symmetric fold-representative churn (dtors/thunks/refcount
  trivials), NOT regressions - none touch your source. ANCHOR via member-fn ADDRESS-OF only
  (`&Class::method`), NEVER construct an instance: instantiating an fsm_state-derived class
  emits its vtable and forces codegen of any still-STUB non-void virtual (e.g.
  `selected_animations`) -> C4716/LNK1257 link failure (cost me one full ~10-min relink).
  `selected_animations`) -> C4716/LNK1257 link failure (cost me one full relink).
  Whole unit (2 trivial overrides) done in effectively one productive rebuild this way.
- **The inherited stack tip's `temp_include_all.cpp` may not even COMPILE - brace-balance-check it
  BEFORE your first rebuild (zero cost).** On `weapon_core_hide_state_base` the tip branch
  (`match/game_core-weapon_core_show_state_base`) had FIVE anchor functions each missing their closing
  `}` (use_dispersion_calculator, use_character_dispersion_calculator, use_weapon_dispersion_calculator,
  use_client_player_update, use_game_core_weapon_state) - `void use_X(){...` ran straight into
  `void use_Y(){`, so MSVC failed with `C2601 local function definitions are illegal` + `C1075 EOF`. The
  prior report.json was therefore generated on a DIFFERENT (older, compiling) tree, which makes nearly
  every `100->0` row in report-changes a stale-baseline artifact, not your regression. Catch it for free
  before building: `python3 -c "s=open('.../temp_include_all.cpp').read(); d=0; [exec('global d')]"` - or
  just iterate chars counting `{`/`}` and assert final depth 0, min depth never negative. Fixing the
  braces is safe (the anchor TU emits no matched bytes) and re-enabled ~89 dead-stripped anchors. Cost me
  one wasted ~10-min rebuild discovering it.
- **For trivial virtual overrides, read the access char from the delinked TARGET `.h`-unit's OWN
  recovered symbol (report.json), not the ICF fold representative's rich-index mangling - and anchor with
  a QUALIFIED CALL, never address-of.** Two facts each cost a ~10-min rebuild on the
  one wasted rebuild discovering it.
- **For trivial virtual overrides, read the access char from the delinked TARGET `.h`-unit's OWN
  recovered symbol (report.json), not the ICF fold representative's rich-index mangling - and anchor with
  a QUALIFIED CALL, never address-of.** Two facts each cost a rebuild on the
  jump_logic_state_{landing,start} overrides: (1) `landing::is_ready_for_transition`'s rich-index fold
  rep was `UBE` (public) but its OWN recovered symbol in report.json is `EBE` (private) - I declared it
  public, scored None, rebuilt. Read `report.json`'s function list for the unit FIRST. (2) Address-of a
  VIRTUAL member is a vtable thunk - it emits NO body (obj had zero member symbols). Use
  `Class& s=*(Class*)NULL; s.Class::member();` (qualified devirtualized call) to emit the body without the
  vtable. Both fixable up-front from the obj symbol dump + report.json with ZERO extra rebuilds if you
  check them before the first full relink. (Note: under /GL the qualified call still gets LTCG-inlined into
  the anchor so the standalone never reaches the EXE - the override stays ICF/LTCG-unscorable like #148;
  prove byte-correctness by disassembling the anchor, and mark DONE.)

## temp_include_all anchors: DEFINE *and* DISPATCH, or the rebuild is wasted
A new `use_game_core_<x>()` anchor must be (a) defined, (b) befriended on the class/cook
template, AND (c) CALLED from the dispatcher aggregator (`IncludeAll::IncludeAll()` in
`game/sources/temp_include_all.cpp`, the ctor that lists `vostok::use_game_core_...();`). If you only do (a)+(b) but forget (c),
nothing references the anchor, so the linker folds away your target functions; they vanish from
the base side and `report.json` scores them 0 (shown as `fuzzy_match_percent: null` AND missing
from the diff `right`/base side). That costs a full relink to discover. Grep the
dispatcher for an existing sibling anchor call and add yours right next to it BEFORE the first
build. (Hit while matching pistol_/double_barreled_weapon_core_idle_state - build #1 wasted.)

## New .cpp: `git status` it before the rebuild (verify the path)
A cpp belongs under `sources/vostok/<module>/SOURCES/`, the header under `sources/vostok/<module>/`.
If a `Write` drops the `/sources` segment it silently creates the cpp NEXT TO the header; ninja
then compiles the UNTOUCHED stub in `sources/` and the whole relink is against stubs (looks like
"my change didn't show"). One `git status` catches it: a `??` cpp directly under the header dir
(not under `.../sources/`) is misplaced - `mv` it before building. (Cost one full relink while
matching pistol_/double_barreled_weapon_core_aimed_idle_state.)

## A function scores `fuzzy: None` (unpaired) when its access-mangling differs
objdiff pairs base<->target by full mangled symbol. The access char is mangled in
(`Q`=public, `I`=protected, `A`=private for `?...@@?AE`). If the target symbol is `AAE`
(private) but your header declares the method `public` (`QAE`), the names don't match and
objdiff reports `fuzzy: None` (NOT 0%) even though the body may be byte-identical and the
size matches. Read the target's mangled access char (`grep -o '<name>@...@@[A-Z]AE'
binaries/rich/target/index.jsonl`) and set the header's access specifier to match BEFORE
the first build. (initialize_weapon_logic: target `AAE`, declared public -> `None`.)

## A fake-observation direct anchor is RARELY the cap on a private method's %
A private method kept alive by a DIRECT `temp_include_all` anchor that observes a fabricated
object (`*reinterpret_cast<T*>(NULL)`, escaped result, address-taken setters) does NOT
generally distort that method's own scored bytes. Codegen for the method body is driven by the
method's own source, not by what its anchor does at the call site - so the anchor's fake
observation changes the *anchor* function's bytes, not the matched method's. Verified on the
`legs_ik_processor` deep pass: removing the fake-observation anchors for `get_foot_fixed_transform`
/ `process_leg` / `get_additional_length` / `leg_params::*` (so the real `process()` call chain
keeps them alive transitively) left every % byte-for-byte UNCHANGED (`--view diff` identical).
LESSON: do NOT spend a rebuild "removing fake observation to flip LTCG" hoping a capped private
method jumps - the cap is almost always genuine (a missing brace/structure, an inline-vs-call
COMDAT, a call-boundary arg). Removing redundant anchors is worthwhile for source HYGIENE (the
private method should be reached transitively, not fake-observed), but budget it as cleanup, not
as a %-recovery move. If you DO remove an anchor, the one thing to re-verify is that every fn
still SCORES in report.json (not dead-stripped) - that is the only real risk.

## A batch of ~18 trivial one-liners across 10 headers in ONE rebuild - read access + fold-class up front
A large one-liner batch (getters, empty virtual overrides, a ctor/dtor pair, map/vector registrars)
matched in a SINGLE rebuild (706s) by classifying each function BEFORE building, from the delinked
TARGET obj symbols + asm, with zero re-rebuilds:
- **Read the access char from the delinked TARGET obj symbol table** (`strings
  binaries/objdiff/target/vostok/game_core/<unit>.h.obj | grep <fn>`), NOT the rich-index fold rep.
  collision_sensor's four on_* overrides mangled `MAE` (protected) while the header had them
  `public:` -> would be unpairable; moved them to `protected:` in the same pass.
- **Recognize the THREE unscorable-but-byte-correct classes up front so you don't chase the None:**
  (1) ICF-fold trivial overrides (`return this`/`return true`/empty/`return NULL`) - PROVE bytes by
  counting the fold family in `binaries/rich/base/index.jsonl` (mov al,1;ret / ret 4 / xor eax,eax;ret
  / empty-this-frame families were 25/68/16/100 members), mark None|DONE, do NOT anchor (a folded body
  can't be made scorable). (2) trivial-accessor LTCG inline-vs-call - a getter/setter/registrar
  anchored only by a synthetic caller gets INLINED whole-program into the anchor; verify by
  disassembling the anchor (the member read lands at the right `[this+off]`), mark None|PARTIAL.
  (3) frameless-target vs /Od-framed (the `this`-unused-leaf entry in assembly_patterns.md) - None|PARTIAL.
- **Only the ctor/dtor that USE `this` and have a real game consumer score a real % (100%).** The
  damage_protector ctor/dtor were already 100% (a `new booster_damage_protector` real consumer keeps
  them standalone). Everything anchored ONLY synthetically is None.
- **Net: of 18 one-liners, expect ~2 real-% (the real-consumer ctor/dtor) + the rest None** (DONE for
  folds, PARTIAL for inline-vs-call / frameless). This is the realistic ceiling for trivial members
  under /Od+/GL with no matched real consumers - budget the batch as "confirm byte-correctness + mark",
  not "drive each to 100%".

## The "rebuild stuck for minutes with nothing happening" is a Wine zombie, not real work
A full relink's real cost is ~1.5 min (≈37s whole-module recompile + ≈55s LTCG link + ≈5s delink),
measured. But intermittently `rebuild.py` appears to hang for 10+ minutes at 0% CPU: Wine leaves a
finished `cl.exe`/`link.exe` child that already wrote its output and SUCCEEDED but never exits, so
ninja blocks on it (you'll see dozens of stale `cl` procs parented to `wineserver` in htop). That dead
wait, not compute, is what made rebuilds feel like ~12-20 min. `scripts/ninja_build.py` now carries a
**watchdog** for the full-game build: it runs `wine ninja` as a child and, once the EXE+PDB mtimes have
advanced past build-start AND the whole wine compiler/linker tree has been idle (<0.15 cores) for 60s
while ninja still hasn't returned, it concludes the link is done-but-zombied, reaps the wine children,
and returns success. A normal build never trips it (ninja exits ~1s after the link, long before the
idle timer fills), so there's zero added latency on the common path. Safety: it only proceeds once BOTH
link outputs are freshly written (never on a half-written EXE - that would show as a blown-up diff), and
a real LTCG link keeps a core busy so it never reads as idle. If you still see a multi-minute 0%-CPU
wait, the watchdog's 60s idle window has not yet elapsed - that's the worst case now (60s, not 10 min).

## The OTHER 10-minute stall: mspdbsrv.exe holds the build's output pipe (constant ~600s)
The watchdog above didn't end the 10+ minute rebuilds in matcher worktrees: a fresh-worktree rebuild
still took 11-15 min wall-clock while `.ninja_log` showed only ~100-325s of edges and the EXE+PDB were
written on time. mtime forensics pinned the gap: a constant ~604s between the link finishing and the
delink/structure/rich steps starting. Cause: `cl.exe`/`link.exe` spawn `mspdbsrv.exe` (the PDB-writer
daemon), which idles for **~10 minutes** before exiting on its own and inherits the build's stdout fd.
Anything reading the build through a pipe - `rebuild.py`'s module-counting reader, an agent's shell
capture - gets EOF only when mspdbsrv dies, long after everything has exited. The main repo dodged it
by luck (its long-lived mspdbsrv predated the pipe), which is why only fresh worktrees seemed slow.
Two fixes, both in place: `ninja_build.py` kills its prefix's `mspdbsrv.exe` after every build
(prefix-scoped by matching WINEPREFIX in `/proc/<pid>/environ` - a global pkill could corrupt a
sibling worktree's in-flight link), and `rebuild.py` reads the pipe via `select()`, letting go once
the child has exited and the pipe stays silent for 2s - so any future leaked fd-holder costs at most
2s, not 10 min. Measured steady state after the fix (2-module recompile + relink): ~2 min wall-clock,
of which ~60s is the LTCG link and ~5s the parallel structure/COFF/rich regeneration. If rebuilds
ever creep back to a constant ~10-min overhead, compare `.ninja_log` edge spans against the
rebuild.log wall-clock and look for a new pipe-holder via `/proc/<pid>/environ` WINEPREFIX matches.

## The exe LINK rsp goes stale in worktrees: regen_ninja never compares it (OpenSSL LNK2001 trap)
A fresh worktree branched after a `.vcproj` LINKER change (e.g. PR-stack commit be4e9a5e adding
`libeay32/ssleay32` + the openssl LIBPATH to the exe's VCLinkerTool) can fail its first rebuild with
38 unresolved `_SSL_*`/`_BIO_*` externals even though `regen_ninja.py` reports "ninja graph already
matches the .vcprojs". Cause: regen compares only each module's *compile-source union*
(`<module>_cl_*.rsp`) and exact-compares the top-level `.ninja` files - the link flags live in
`rsp/<exe>_link.rsp`, which is NEVER diffed, so a stale link rsp survives regen. Fix: diff the
worktree's `<exe>_link.rsp` against a known-good worktree's (path-normalized) and hand-patch the
missing `/LIBPATH:`/libs into the flags line (or delete the rsp dir + force a full vcproj2ninja
regen). Symptom signature to remember: linker UNRESOLVED EXTERNALS for symbols a vcproj
`AdditionalDependencies` should provide + a no-op regen_ninja.
## An include ADDED to temp_include_all.cpp after the graph regen is dep-invisible - `touch` the TU when you edit that header
vcproj2ninja computes header deps at regen time. If your unit's flow is (regen) ->
(add `#include <vostok/<module>/sources/<x>.h>` to `temp_include_all.cpp`) -> build ->
(edit `<x>.h`) -> rebuild, the second rebuild recompiles the module TUs that always
included `<x>.h` but NOT `temp_include_all.cpp` (its dep list predates your include) -
and when the anchor TU holds the ONLY instantiation of the class, the linked COMDAT is
the STALE one and your header edit silently "does not take" (score frozen). `touch
sources/vostok/game_core/sources/temp_include_all.cpp` before the rebuild (do NOT
re-run the regen mid-run - it rewrites the rsp files and loses hand patches like the
OpenSSL exe-link libs). Cost one relink on string_response::~string_response.

## A stale incrementally-linked base PDB can crash pdb_parser ("Enums cannot be of different length")
The base link UPDATES `survarium-dx11-win32-gold.pdb` incrementally, so a TYPE record from an
earlier build of the same worktree (e.g. an enum whose enumerators a later commit changed) can
survive in the exe PDB even after every obj that defined it was recompiled - and
`pdb_parser`'s header generation then panics merging the old and new same-name enums
(gen_headers.rs `Data::add`). Symptom: `[rebuild] base structure: FAILED ... Enums cannot be
of different length` while report.json/rich index still regenerate fine. FIX: move aside
`binaries/Win32/survarium-dx11-win32-gold.{exe,pdb}` and relink - the fresh PDB carries only
current types. (Hit on the login-stack match after the #303 lobby-enum values changed; the
4-entry `connection_allowed` era record lingered through every clean module rebuild.)
Diagnosis trick: build a scratch copy of vostok-pdb-parser with the `unreachable!` swapped for
an `eprintln!` of `e.name` + both lengths - it names the colliding enum immediately.

## Header edits to vostok/network or vostok/login_server headers do NOT recompile temp_include_all
The game_core ninja dep list for `temp_include_all.cpp` misses cross-module headers (the
regen-ninja-blind-to-include-changes trap): after flipping access specifiers or signatures in
`network/login_client.h`, the link fails with stale-anchor LNK2001/LNK2005 (the obj still holds
the OLD manglings/COMDATs). `touch sources/vostok/game_core/sources/temp_include_all.cpp`
before the rebuild whenever a network public header changes.

## Macro-dispatched platform headers (`VOSTOK_RENDER_CORE_FILE(x.h)`) are dep-INVISIBLE - edits to `render/core/dx11/*.h` do not rebuild anything
`sources/vostok/render/core/backend.h` (and `res_xs.h`, ...) are 4-line shims whose only
content is `#include VOSTOK_RENDER_CORE_FILE(backend.h)`, i.e. a MACRO include that expands to
`vostok/render/core/dx11/backend.h`. vcproj2ninja's scanner cannot resolve a macro include, so
the ninja implicit-dep list for every render TU contains `render/core/backend.h` but NOT
`render/core/dx11/backend.h` (`grep -c "dx11.backend.h" binaries/ninja/render_core_pc_dx11.ninja`
returns only unrelated hits). Editing the dx11 header therefore leaves ninja with nothing to do -
and if some *other* file in the same project changed, the project's batch `cl` still runs, the
`.obj` mtimes still advance, and the link still happens, so every freshness check you can think of
(obj newer than header, lib newer than header, exe relinked, `report-changes.json` non-empty)
says "fresh" while the codegen is the OLD header. Symptom: a header edit that provably compiles
produces byte-identical output - e.g. new `++num_ps_changes` / early-out guards in
`backend::set_ps*` never appeared in `res_xs<T>::apply`.
FIX: `touch sources/vostok/render/core/<name>.h` (the tracked SHIM) after editing
`render/core/dx11/<name>.h`, then rebuild; the shim is in every dep list, so this forces the
recompile. Applies to any `VOSTOK_*_FILE(...)` platform-dispatch header in the tree.
Cost when missed: one full rebuild cycle plus a bogus "the change had no effect" conclusion.
