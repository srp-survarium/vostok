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
- **A header-only edit may NOT retrigger .obj recompiles under this ninja setup -
  touch the dependent .cpp.** After fixing an inline body in a header, `rebuild.py`
  ran `[1/1]` (link only) and the LTCG codegen kept failing on the STALE obj IL
  (the obj's depfile didn't list the header). The error even reported the *new*
  source line while using the old IL. FIX: `touch` every .cpp that emits the
  affected symbol (here the vtable-emitting TUs) so ninja recompiles them with the
  fixed header. Cost 1 wasted rebuild on `movement_animation_index`'s
  `get_attachment_transform` stub.
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

- **Generated ninja files have NO header-dependency tracking - edit a header and the
  dependent `.cpp` TUs are NOT recompiled.** A pure header edit (e.g. `animation_playback_state.h`,
  `weapon_core_base_state.h`) can produce a rebuild report of `0 regressed / 0 improved` even though
  you changed the class - the build reused the stale `.obj`s, so your % won't move and you'll waste a
  ~20-min relink chasing a ghost. FIX: after any header-only change, `touch` the `.cpp` TUs that
  include it (or `ninja_build.py -t clean`) BEFORE `rebuild.py`, so ninja recompiles them. Tell-tale:
  a rebuild whose target functions are unchanged AND whose only churn is the rotating baseline dtors
  means nothing of yours recompiled - touch and rebuild. (Found while moving a class between headers
  for `weapon_core_aimed_state_base::finalize`.)
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
  `selected_animations`) -> C4716/LNK1257 link failure (cost me one full ~20-min relink).
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
  one wasted ~20-min rebuild discovering it.
- **For trivial virtual overrides, read the access char from the delinked TARGET `.h`-unit's OWN
  recovered symbol (report.json), not the ICF fold representative's rich-index mangling - and anchor with
  a QUALIFIED CALL, never address-of.** Two facts each cost a ~20-min rebuild on the
  jump_logic_state_{landing,start} overrides: (1) `landing::is_ready_for_transition`'s rich-index fold
  rep was `UBE` (public) but its OWN recovered symbol in report.json is `EBE` (private) - I declared it
  public, scored None, rebuilt. Read `report.json`'s function list for the unit FIRST. (2) Address-of a
  VIRTUAL member is a vtable thunk - it emits NO body (obj had zero member symbols). Use
  `Class& s=*(Class*)NULL; s.Class::member();` (qualified devirtualized call) to emit the body without the
  vtable. Both fixable up-front from the obj symbol dump + report.json with ZERO extra rebuilds if you
  check them before the first full relink. (Note: under /GL the qualified call still gets LTCG-inlined into
  the anchor so the standalone never reaches the EXE - the override stays ICF/LTCG-unscorable like #148;
  prove byte-correctness by disassembling the anchor, and mark DONE.)
