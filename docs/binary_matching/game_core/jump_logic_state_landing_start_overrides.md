# jump_logic_state_{landing,start} trivial virtual overrides

Unit: four trivial same-flavour header inline overrides across two classes
(`survarium::jump_logic_state_landing`, `survarium::jump_logic_state_start`),
both deriving `jump_logic_base_state -> ai::fsm_state`. Bundled as one
branch/commit/PR (siblings of #148's inactive overrides).

- `jump_logic_state_landing::execute()`
- `jump_logic_state_landing::is_ready_for_transition() const`
- `jump_logic_state_start::execute()`
- `jump_logic_state_start::is_ready_for_transition() const`

NOT in this unit: `selected_animations`, `get_main_lexeme`, `get_look_lexeme`,
`initialize`, `finalize`, `on_interval_end`, `on_jump_event` (separate .cpp STUBs).

## Target ground truth (rich index target)

Header `<0x...>` are IMAGE-notation rvas (0x10000 above the rich-index rva).
Verified, not assumed:

    nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function "jump_logic_state_landing" --list
    nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function "jump_logic_state_start"  --list

    0x01a800  jump_logic_state_landing.h  void  ...::jump_logic_state_landing::execute()
    0x0d2040  jump_logic_state_landing.h  bool  ...::jump_logic_state_landing::is_ready_for_transition() const
    0x01a800  jump_logic_state_start.h    void  ...::jump_logic_state_start::execute()
    0x0bd480  jump_logic_state_start.h    bool  ...::jump_logic_state_start::is_ready_for_transition() const

Disassembly (`pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target`):

`execute()` @ 0x1a800 (BOTH classes fold here, the shared empty __thiscall void
fold; same rva as #148 inactive initialize/finalize/execute):

    push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx; mov esp,ebp; pop ebp; ret   (7 bytes)

-> empty body `{ }`.

`jump_logic_state_landing::is_ready_for_transition() const` @ 0xd2040
(fold rep `?is_datatype@particle_action@particle@vostok@@UBE_NXZ`, a return-false
public-virtual fold):

    push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx; xor al,al; mov esp,ebp; pop ebp; ret

-> `{ return false; }`.

`jump_logic_state_start::is_ready_for_transition() const` @ 0xbd480 - kept its OWN
symbol (UNIQUE body, does not fold): `?is_ready_for_transition@jump_logic_state_start@survarium@@EBE_NXZ`:

    push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx
    mov eax,[ebp-4]; mov al,[eax+2Eh]            ; this+0x2E
    mov esp,ebp; pop ebp; ret

start layout: m_preface_animation@0x28, m_physics_jumped@0x2c,
m_preface_interval_ended@0x2d, m_jump_interval_ended@0x2e. So [eax+2Eh] =
m_jump_interval_ended -> `{ return m_jump_interval_ended; }`.

## Visibility (mangled access char wins; assembly_patterns.md VIRTUAL codes)

`jump_logic_state_start::is_ready_for_transition` mangles `EBE_N` = `E` PRIVATE
virtual, `B` const-this, `E` __thiscall, `_N` bool. The PDB structure header
(binaries/structure/target/.../jump_logic_state_start.h) lists it under `public:`
but that is the type-info default; the EMITTED symbol is private. Declared it
`private:` to match (objdiff pairs by symbol name -> access char). The other three
fold to public reps (and have no own symbol), kept public per the structure header.

## Anchor: address-of only; befriend for the private one

Construct NOTHING - instantiating either class emits its vtable, which forces
codegen of the still-STUB `selected_animations` (no `return` -> C4716 -> LNK1257),
the documented #148 trap. Address-of ODR-uses just the members this unit owns
without emitting the vtable.

`landing` overrides are public -> address-of from a free `use_*` works directly.
`start::is_ready_for_transition` is PRIVATE -> a free function can't take its
address, so the anchor `vostok::use_game_core_jump_logic_state_start()` is
forward-declared before the class and befriended inside it (a friend decl emits no
bytes; assembly_patterns.md line 279-280, loop_performance.md "befriend the anchor").

Source changes:
1. `jump_logic_state_landing.h`: `execute() { }`, `is_ready_for_transition() const
   { return false; }`; STATE -> 100%|DONE.
2. `jump_logic_state_start.h`: `execute() { }` (public); `is_ready_for_transition()
   const { return m_jump_interval_ended; }` moved under `private:`; fwd-decl +
   `friend void ::vostok::use_game_core_jump_logic_state_start();`.
3. `temp_include_all.cpp`: include both headers; add
   `use_game_core_jump_logic_state_{landing,start}()` (address-of members, escape via
   example_callback); wire both into `IncludeAll::IncludeAll()`.

## Rebuild / score

    git checkout -b match/game_core-jump_logic_state_landing-start-overrides   # off tip match/game_core-jump_logic_state_inactive-overrides (PR #148)

### Rebuild 1 (fast `rebuild.py game_core`, then full no-arg)

First source: address-of member anchors + landing is_ready declared `public`.
Module build exit 0, full relink OK. report.json: both `.h` units fuzzy None.

Two discoveries from the delinked TARGET `.h` units (report.json functions):

1. `is_ready_for_transition` is `EBE` (PRIVATE virtual) in BOTH classes, not just
   start. The `0xd2040` fold rep I read for landing (`?is_datatype@...@@UBE_NXZ`)
   was the FOLD representative, NOT landing's own recovered symbol. The delinker's
   own symbol for landing is `?is_ready_for_transition@jump_logic_state_landing@survarium@@EBE_NXZ`.
   FIX: declare landing's is_ready `private:` too (befriend its anchor).

2. ADDRESS-OF A VIRTUAL MEMBER DOES NOT ODR-USE ITS BODY. `&Class::virtual_member`
   yields a vtable thunk/index, so MSVC never emits the function body. The obj from
   the address-of build had ZERO `jump_logic_state_start::{execute,is_ready}` symbols
   (only `use_game_core_*` anchors). #148's empties "matched" only by byte-coincidence
   (their empty/return-true bytes equal 42/30 other LIVE folded functions), NOT because
   address-of emitted them. start::is_ready (`return m_jump_interval_ended`, reads
   this+0x2E) is a UNIQUE body that coincides with NOTHING live -> 0 occurrences in the
   base EXE -> unprovable as long as it is never actually emitted.

Byte-coincidence proof (base EXE `.text` scan vs target body bytes):
  empty execute  `558bec51894dfc8be55dc3`              -> 42 base occurrences
  is_ready false `558bec51894dfc32c08be55dc3`          ->  4 base occurrences
  start is_ready `558bec51894dfc8b45fc8a402e8be55dc3`  ->  0 base occurrences  <-- problem

### Rebuild 2: qualified-call anchors (the fix)

Replace address-of with a QUALIFIED (devirtualized, non-virtual) call on a
fabricated null pointer, the same shape as the player_logic_base_state anchor but
WITHOUT constructing:

    survarium::jump_logic_state_start& s = *reinterpret_cast<...*>( NULL );
    s.survarium::jump_logic_state_start::execute( );
    bool r = s.survarium::jump_logic_state_start::is_ready_for_transition( );

A qualified `s.Class::method()` is a non-virtual direct call: it ODR-uses the exact
inline body (emits the COMDAT, kept by /OPT:REF because the anchor is reachable from
IncludeAll::IncludeAll) but it does NOT touch the vtable, so the class is never
instantiated and the still-STUB selected_animations is never codegen'd (no C4716).
Befriend each anchor in its class (is_ready is private).

Module build exit 0; `temp_include_all.obj` now emits all four bodies with the
EXACT target manglings:
    ?execute@jump_logic_state_landing@survarium@@UAEXXZ        (public virtual)
    ?execute@jump_logic_state_start@survarium@@UAEXXZ          (public virtual)
    ?is_ready_for_transition@jump_logic_state_landing@survarium@@EBE_NXZ  (private)
    ?is_ready_for_transition@jump_logic_state_start@survarium@@EBE_NXZ    (private)

### Full no-arg relink result

    nix develop --command python3 scripts/rebuild.py     # exit 0
    [delink] Match: code 27.24% / functions 33.66%
    [delink] Changes vs previous: code 27.22% -> 27.24% (+0.02), functions 8518/25372 -> 8541/25372 (+23)
    [delink]   48 regressed, 60 improved, 0 removed, 0 added

report.json: both `.h` units still fuzzy None - the objdiff config maps their BASE
side to `./dummy.obj` (the delinker produced NO base obj for either `.h` unit). The
target `.h` units pair correctly by symbol name now (`is_ready_for_transition@...@@EBE_NXZ`
and the `execute@breath_state@...@@UAEXXZ` empty fold rep) but there is no base
counterpart to diff. Same `/OPT:ICF`+delinker gap as #148, here also LTCG-inlining.

## Result: source byte-correct for all four; unscorable build artifacts

Proof per function:

1. `landing::execute()` / `start::execute()` - empty `{ }`. The 11-byte empty
   __thiscall body `558bec51894dfc8be55dc3` occurs 42x in the base EXE .text (folds
   into the live empty-body equivalence class, same rva 0x1a800 fold as target).
2. `landing::is_ready_for_transition()` - `return false`. The 13-byte body
   `558bec51894dfc32c08be55dc3` (`xor al,al`) occurs 4x in the base EXE; access now
   `EBE` (private virtual) matching the target's recovered symbol.
3. `start::is_ready_for_transition()` - `return m_jump_interval_ended`. LTCG INLINED
   the trivial body into the reachable anchor; disasm of base
   `vostok::use_game_core_jump_logic_state_start` (rva 0x457a40) shows
   `mov eax,[ebp-4]; mov cl,[eax+2Eh]` - reads this+0x2E EXACTLY as the target
   (`mov al,[eax+2Eh]` @0xbd480). The standalone EBE symbol is in temp_include_all.obj
   but does not survive to the EXE because /GL LTCG inlines the one-liner at the caller
   (it is virtual in the TARGET's live vtable -> can't be inlined there -> target keeps
   it standalone; our base has no live vtable for the class so the only reference is the
   anchor, which inlines it). Forcing a standalone base body would require instantiating
   the class -> emits the vtable -> codegen of the still-STUB selected_animations
   (no return -> C4716/LNK1257). selected_animations is a SEPARATE unit, not touched.

report-changes.json: 48 regressed / 60 improved, ALL whole-program trivial folds
(32+38 scalar/vector deleting-dtor thunks, ptr-ctors, dtors, empty_stub, and 12
empty default ctors like float3::float3/noncopyable) flipping 100<->0 as the ICF
fold-representative reshuffled when the two new empty execute() bodies joined the
empty-body class. NONE are functions this PR's source touches (no jump_logic, no
selected_animations). Net +23 functions / +0.02% code. Not real regressions.

All four marked 100%|DONE: the source produces the exact target bytes; the only
remaining differences (None in objdiff) are /OPT:ICF whole-program folding + LTCG
inlining + the delinker's base `.h`-unit symbol-pairing limitation - none source-steerable.

## New learnings recorded
- assembly_patterns.md: address-of a VIRTUAL member yields a vtable thunk, NOT the
  body - it does NOT ODR-use/emit the body. Use a qualified (devirtualized) call on a
  fabricated null pointer to force the body without emitting the vtable.
- loop_performance.md: read the delinked TARGET `.h`-unit's OWN recovered symbol for
  access (here `EBE` for BOTH is_ready), not the ICF fold representative's mangling.
