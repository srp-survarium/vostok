# Assembly patterns - low-confidence / residual / tooling notes

The overflow of [`assembly_patterns.md`](assembly_patterns.md). These are NOT trusted,
actionable asm->source patterns - they are: low-confidence or NEGATIVE results (a thing
that did NOT work), unsteerable LTCG / `/Od` frame-slot residual notes (mark PARTIAL and
move on, do not chase), and high-confidence-but-not-a-pattern TOOLING / build / compile
caveats (delinker quirks, RVA offsets, LNK1257, the LOG `__FILE__` wall). Useful to KNOW,
but they do not tell you what source to write, so they stay out of the main list.

## Notes

### stripped/folded empty out-of-line call can appear MID-BODY with a real source line
SYMPTOM: `mov byte[ebp-1],0; lea eax,[ebp-1]; call <fn>` mid-function (a 1-byte local set to
false, its address taken, then a `call`), where the call target is a single `ret` (delinker
COMDAT-misnames it `fixed_size_allocator<...>::finalize_impl`), the arg is passed in EAX with
NO push and NO stack cleanup (matches no writable C++ calling convention), and NO nameable
symbol for the callee exists in EITHER rich index. This is the same class as the prologue
`mov byte[ebp-1],0; ...; call empty_stub` artifact (above) - a call to a helper that is
empty/stripped/COMDAT-folded whole-program - but here it carries a genuine source-line number
(it IS a real source statement) rather than being a prologue artifact. NOT reproducible from
the function's own source. Match every other statement and mark PARTIAL. Verify it is this
class (not a real callee) by: (1) the call target disassembles to a bare `ret`; (2) the arg
register/convention is non-standard; (3) `pdb_rich_query` finds no standalone symbol in target
OR base. Confirmed in `game_core/weapon_core_show_state_base::{initialize (line 28, 75.13%),
on_animation_end_impl (line 42, 69.93%)}` - every other statement (base init/finalize call,
instant_toggle_start/end, *m_is_shown=true, result=true) is byte-exact.

### LTCG custom `this`-in-EAX convention = proof the target callee is out-of-line (don't escape its address)
SYMPTOM: a member call the source writes as `member.reset()` appears in the target as
`add eax,120h; call reset` (object pointer in **EAX**, no `lea ecx,...`), and the callee
`reset` itself has NO `push ebp` frame and reads `[eax]/[eax+4]`:
    xorps xmm0,xmm0; mov dword[eax],0; movss [eax+4],xmm0; ret
This non-`__thiscall` (this-in-EAX, frameless) convention is an LTCG optimization MSVC applies
ONLY to functions it decided to keep OUT-OF-LINE whole-program. So seeing EAX-this is itself the
tell that the TARGET kept the callee standalone - and that our build, which instead INLINES the
tiny body at every caller (`add eax,120h; mov [ebp-4],eax; mov [ecx],0; movss...`, frame grows from
`push ecx`/`[ebp-4]` to `sub esp,8`/`[ebp-8]`), is fighting the LTCG inliner.
WHAT DOES NOT WORK (verified, PR #124, `weapon_core_aimed_state_base::finalize` -> `animation_playback_state::reset`):
decl/def split into the class's own header-TU; `__declspec(noinline)`; multiple real same-module
callers. CRITICAL: do NOT try to force the out-of-line call by escaping `&callee` (member-fn-ptr)
through an opaque sink - taking the address FORCES the standard `__thiscall` ECX convention with a
full frame, which diverges from the target's EAX form AND still gets inlined. The closest reproducible
shape is the EMPTY-stub callee, which lets `/Od` cleanly ELIDE the call (caller PARTIAL, the only diff
is the N call instrs). This is a genuine inline-vs-call LTCG residual (the narrowed MATCHING.md rule
allows stopping here - it is codegen, not a wrong member/branch in source; the `member.reset()` call
IS written, only its inline-vs-call lowering differs).

### const setter/getter that calls a TRIVIAL inline accessor is the get_user() inline-vs-call class
A `m_ref.get_x()` where `get_x` is a one-line header accessor (`{ return m_member; }`)
compiles in the TARGET to an out-of-line `call get_x` (the accessor kept standalone), but
our `/GL` LTCG inlines it whole-program to a direct `mov eax,[m_ref+offset_of_m_member]`
(no standalone symbol in base; `pdb_rich_query base --function ...::get_x` -> "no function
matched"). The inline also adds a frame temp, so the surrounding `[ebp-XX]` slots shift -
all unsteerable from the caller's source. Same class as the documented trivial-accessor
inline-vs-call; recognize it (target standalone getter at a real rva + base has none) and
stop at PARTIAL, do not rewrite. Caught on
`weapon_core_animation_end_aware_state::set_animation_to_wait` (target `call
weapon_core::get_user` @0x9b330; base inlined `mov eax,[m_weapon+44Ch]` -> 77.33%).

### Explicit-specialization DECLARATION does NOT force a call to a VISIBLE inline template (INEFFECTIVE)
SYMPTOM: inline-vs-call wall where the TARGET keeps `call operator+<T,T>` and OUR BASE inlines it; the
specialization is standalone in BOTH indexes. TEMPTATION: add `template<> R operator+<T,T>(...);` in
the consuming .cpp. RESULT: NO EFFECT under MSVC8 (`weapon_and_hands_expression` 85.65 -> 85.65) - when
the primary inline template's DEFINITION is in scope (via the included `_inline.h`), MSVC still inlines
it; a spec-DECL does not suppress that. The forward-decl device works ONLY when the inline DEFINITION is
OUT of the consuming TU (drop the inline-header include + forward-decl the function AND its types) - a
TU restructure, often shared across many sibling .cpp in an out-of-scope module (collateral risk).

### compiler-generated dtor: missing member-dtor `this`-pointer setup is ICF folding
A destructor where the target sets `mov ecx,[this]; add ecx,0xNN` before each trivial member
`~T()` call but the base omits those `add ecx` setups (still issuing the calls) is ICF/codegen
folding the identical member-dtor `this` adjustments - not source-steerable. The only real source is
the explicit body (e.g. `DELETE(m_drawer)`); the member-dtor epilogue is auto-emitted.
(game_core/legs_ik_processor::~legs_ik_processor, 85.71%.)

### per-call `get_skeleton()`/reference-return spill = uniform `[ebp-N]` shift, not a brace bug
When `f(*ptr, accessor_returning_ref(), ...)` is called repeatedly and the BASE spills the
ref-returning accessor into a fresh stack temp per call while the TARGET inlines the deref
(`mov ecx,[this]; mov edx,[ecx]; push edx`), the base frame grows by 4 bytes per spilled site and
EVERY later `[ebp-N]` slot shifts by that total - a uniform slot-rename storm with NO `[n]`
block-open / `+`/`-` control-flow divergence. Distinguish from the genuine brace-scope storm
(§2a): if the diff has zero target-only/base-only rows and the slot deltas are all the SAME
constant, it is a temp-spill/LTCG artifact, not a missing brace. (game_core/legs_ik_processor::process.)

### thin forwarder `m_renderer.draw_X(m_scene, ...)`: float/int arg passed in xmm0/eax vs spilled to stack = LTCG call-boundary residual, NOT a source bug
A one-line debug-draw forwarder `member_ref.method( other_member_ref, a, b, c )` (member_ref at
this+0, scene_ptr at this+4 via `add ecx,4`) can match 100% for some overloads and stall at
60-80% for sibling overloads with the SAME forwarding shape. The divergence is purely at the
call boundary: e.g. the TARGET passes a `const float` arg in `movss xmm0,[ebp+..]` (register)
while the BASE passes it on the stack (`fld [ebp+..]; fstp [esp]`), or which integer arg ends in
`eax` vs gets pushed differs. This is whole-program LTCG calling-convention specialization
dictated by the (possibly unmatched) callee; it cannot be steered from the forwarder's source -
the two 100%-matching siblings prove the source is right. Stop at PARTIAL and name the cause; do
NOT chase it. (game_core/legs_ik_drawer: draw_cross/draw_line_capsule 100%, draw_origin 62.88%,
draw_solid_capsule 79.43%, draw_leg 73.36% - same draw_origin xmm0-vs-fld/fstp residual x4.)

## A file-static `cc_bool` console command -> `dynamic initializer` is objdiff-UNSCORABLE (None)
A `static console_commands::cc_bool s_x_cc( "name", s_x_value, serializable, command_type )` at
file scope compiles its construction into a `dynamic initializer for 's_x_cc'` (plus a matching
`dynamic atexit destructor`). Both score **None** in report.json and have **no standalone symbol**
in `binaries/rich/base/index.jsonl` - the per-TU init/atexit thunks are LTCG/ICF-folded so the
delinker can't re-attach them. The body is still emitted and byte-correct. Recognize the asm:
`push 1`(serializable) / `push s_x_value` / `push "name"` ; `mov eax,<command_type>` ; `xor ecx,ecx`
(execution_filter_general default) ; `mov esi,s_x_cc`(this) ; `call cc_bool::cc_bool` ; then
`push <atexit dtor> ; call atexit`. Mirror an existing matched sibling
(dispersion_calculator's `s_dispersion_enabled_cc`, also None) and mark None|DONE - do not chase
the symbol. command_type values: engine_internal=0 (eax=0), user_specific=1 (eax=1).

## A `tick`/update with FPU vibration math hit 94% with structure 1:1; residual is /Od frame-slot churn
A member `tick` that reads `[ebp+8]`(time arg)/`[ebp+0Ch]`(scale) and does `fild qword; fmul
[epsilon_3]; fmul scale` (= `(a-b)*0.001f*scale`), `fsm::tick()`, a `static_cast<derived*>(
m_logic.current_state())` (`mov [+10h]`), a `[vtbl+N]` virtual tick, a `cond ? math::max(...) :
math::min(...)` clamp store, and a `[vtbl+M]` virtual returning u32 feeding a `sin(phase/period)*
amp*...` FPU chain - all reproduce 1:1 from source (member offsets straight off the asm,
math::max/min/sin stay out-of-line). The remaining ~6% is `sub esp,38h` (target) vs `30h` (base):
2 extra temp dword slots shift the saved-`this` slot ([ebp-24h] vs [ebp-1Ch]) and swap which
register holds the vtable at the second virtual call. Pure /Od register/slot allocation, NOT a
missing local/brace/ASSERT/statement - the LOCALS all map, the carcass structure matches. Stop at
PARTIAL. (breath_vibration_calculator::tick, 94.23%.)

### a `this`-UNUSED trivial member (`return literal`/`return 0.0f`) is FRAMELESS in the target, framed under /Od
SYMPTOM: a trivial member fn that NEVER references `this` (returns a string literal, `return 0.0f`,
`return NULL` that ignores members) has a TARGET obj body with NO ebp frame - e.g. `get_speed` =
`d9 ee c3` (`fldz; ret`), `use_info` = `b8 <reloc> c2 0400` (`mov eax,lit; ret 4`) - while our /Od
BASE emits the full frame (`55 8bec 51 894dfc <body> 8be5 5d c3`, i.e. `push ebp; mov ebp,esp; push
ecx; mov [ebp-4],ecx; ...`). The original build applied frame-pointer omission for `this`-unused
leaves; `/Od` ALWAYS emits the frame + the `mov [ebp-4],ecx` save-this. The 3-5 vs 11+ byte gap is
too large for objdiff to pair -> `fuzzy: None` even though the semantic body (the fldz / literal /
ret N) is correct. NOT source-steerable under /Od (frame omission is a build flag). DISTINGUISH from
ICF-fold None: these survive STANDALONE in the EXE (qualified-call anchor) at a real rva - they are
present-but-divergent, mark None|PARTIAL. CONTRAST: a member that USES `this` (reads a member, the
ctor/dtor storing into `this`) keeps its frame in BOTH and matches 100% (e.g. damage_protector
ctor/dtor). So frame-presence tracks `this`-usage. Confirmed: artefact_container_core::use_info,
booby_trap_core::get_speed (both None|PARTIAL, frameless target vs /Od frame).

### a member `tick`/update whose body is byte-identical but with `sub esp` off by 4 = one extra unused /Od frame slot (DONE-quality PARTIAL)
When the base disasm matches the target instruction-for-instruction, member-offset-for-offset,
call-for-call, constant-for-constant, but the TARGET reserved 4 more stack bytes
(`sub esp,1Ch` vs base `sub esp,18h`) so the saved-`this` slot and every `[ebp-N]` shifts by 4
(target this@[ebp-10h] vs base this@[ebp-0Ch]), that is a single UNUSED /Od frame slot the
target build allocated - pure stack-allocation noise, NOT a missing local/brace/ASSERT/statement.
The PDB local set maps, the carcass statement structure matches 1:1. Non-steerable under /Od.
Mark PARTIAL at the resulting % (here 99.67%), not chase. Same class as
`breath_vibration_calculator::tick` (94%, frame-slot churn). Confirmed in
`game_core/character_dispersion_calculator::tick` (16 stmts, only diff = sub esp 1Ch vs 18h).

### two trivial accessors in ONE `&&` can split inline-vs-call (one matches, one is the residual)
A `if ( a() && getter_x() && getter_y() )` where BOTH `getter_x`/`getter_y` are one-line
header accessors (`{ return m_member; }`) can lower DIFFERENTLY under /GL: the linker keeps
ONE of them out-of-line (the TARGET emits `call ...getter_x`, standalone symbol present in the
target rich index) while INLINING the other (`mov al,[this+off]`, no standalone in either index).
Our /GL LTCG inlines BOTH, so the inlined-on-both-sides one matches byte-for-byte and the
target-kept-standalone one is the lone residual (`call` vs inlined member read), shifting the
trailing `[ebp-N]` slot/frame by the one inline. This is the same unsteerable trivial-accessor
inline-vs-call class as is_aimed()/get_user() - it is a per-method whole-program decision, NOT
steerable from the caller's source. Confirm which is the residual by querying BOTH rich indexes
per accessor (target standalone + base absent = the diverging one). Mark the caller PARTIAL at the
resulting %. Confirmed on `game_core/weapon_core_reload_state_base::initialize` (92%): in
`!deserializing() && chamber_a_round_on_reload() && round_is_chambered()`, chamber_a_round_on_reload
(@0x48F) inlined on both sides (matches), round_is_chambered (standalone target @0x09b360, base
inlines `mov cl,[+48Eh]`) is the residual.

### empty base virtual called via qualified `Base::method()` - LTCG inlines the empty body at the call site (PARTIAL)
SYMPTOM: a derived override calls the EMPTY base implementation, `Base::execute();` (base decl is
`virtual void execute() override { /* no source */ }`). TARGET emits `mov ecx,[ebp-4]; call
Base::execute` (the empty body kept out-of-line @ its rva). BASE (/GL LTCG) INLINES the empty `{}` at
this call site - the `call` simply vanishes, so the diff shows `+ call <addr>` present only on the
target side and the following member store with a different scratch reg. Both rich indexes STILL list
a standalone `Base::execute` (base @0x012c20, target @0x087f80), so it is NOT "inlined everywhere in
base" - it is the documented PER-CALL-SITE whole-program inline decision (same class as
animation_playback_state::reset() in weapon_core_aimed_state_base::finalize). Filling the empty body
would make /Od inline REAL bytes (worse); the empty stub elides one no-op call cleanly. Leave it,
mark PARTIAL [LTCG empty-callee inline-vs-call]. Confirmed on
`game_core/weapon_core_fire_state_base::execute` (80.91%): only the `call execute` (3 bytes) differs.

### a `boost::bind(&Derived::virtual_method, this, _1)` ICF-folds onto a SIBLING class's bind<> rep - don't be misled by the delinker name
SYMPTOM: `set_animation_callback("ch", this, boost::bind(&weapon_core_fire_state_base::on_shot_event,
this, _1))` - the `call boost::bind<...>` at the bind site is delinker-named with a DIFFERENT class
(`...weapon_core_animation_end_aware_state...`) than the actual bound method. The `boost::bind<>`
helper packs only {member-fn-ptr, this, arg} and is byte-identical across sibling state classes, so
/OPT:ICF folds them and the delinker prints whichever fold representative it picked. The TRUE class
shows on the un-folded `assign_to<bind_t<...weapon_core_fire_state_base...>>` and the
`Derived::vcall'{36}'` member-pointer (a vcall thunk because on_shot_event is VIRTUAL). Source is
`&weapon_core_fire_state_base::on_shot_event` - the mismatched bind<> name is an ICF artifact, not a
wrong source type. Confirmed in `game_core/weapon_core_fire_state_base::initialize` (99.71%).

### module-only ninja_build does NOT relink -> base index is stale for new symbols
`scripts/ninja_build.py <module>` (and `rebuild.py <module>`) compiles the module's objs but
skips the EXE/PDB link (watchdog note in ninja_build.py). The base structure/rich index is
generated from the PDB, so brand-new symbols (e.g. freshly-anchored template instantiations in
game_core/temp_include_all) won't appear in `binaries/rich/base/index.jsonl` until a FULL
relink: run `python3 scripts/rebuild.py` with NO module arg (~10 min) before scoring anchors
you just added.

### carcass `// FUNCTION BODY[0xVA]` RVAs are +0x10000 off the rich index RVA
The structure-carcass comment addresses are offset +0x10000 from the rva keys in
`binaries/rich/target/index.jsonl` (e.g. carcass 0xa72f0 == index 0x972f0, carcass 0x8d690 ==
index 0x7d690). Subtract 0x10000 when fetching a carcass function's disasm by `--rva`, or just
look it up by `--function`/name.

### delinker MASKS base immediate operands but keeps target's -> address-range seed/const is unmatchable
A `mov ecx, imm32` whose `imm32` falls in (or even near) the image address range is treated by
the delinker as a maybe-relocation: on the BASE side the operand is masked (zeroed), on the
TARGET side the literal bytes survive. objdiff then compares target `mov ecx, 0x995a34` vs base
`mov ecx, 0` -> permanent mismatch even when the SOURCE constant is byte-correct. Verified on
`udp_network_flow_emulator::ctor` (87.59%): the three `random32(0x995a34/35/36)` seeds are bare
immediates in BOTH target+base ninja objs (no DIR32 reloc - llvm-objdump `-r` shows none, the
raw bytes `34 5a 99 00` appear once each), yet the BASE *delinked* obj has them zeroed while the
TARGET delinked obj keeps them. Proof it is the delinker, not codegen: ANY seed literal is
stripped from base - tried `0xDEADBEEF` and `0x12345` (non-address), both present in the ninja
obj, both gone from the delinked base obj. The rich/structure STATEMENT view masks operands too,
so it scores these as IDENTICAL (0 size-diffs) - only the operand-aware objdiff % exposes the gap.
RECOGNIZE: a residual that is purely N `mov reg, <image-range-imm>` operands, structure otherwise
perfect, % stuck. Write the source so it emits the matching ctor/call SHAPE (here: explicit
`m_x( seed )` init -> the `??0random32@@QAE@I@Z(u32)` ctor instead of the default-seed path) and
keep the literal at the target value, but do NOT expect the % to close - it is a delinker
asymmetry, not a source miss.

### post-increment returning by value must have a `return` or you get LNK1257
`sequence_number operator++(s32)` declared to return BY VALUE with an empty `/* no source */`
body crashes the LTCG linker with LNK1257. Give it the real temp-copy body
(`T r(*this); ++m_number; return r;`); this also unblocks every caller that post-increments.
