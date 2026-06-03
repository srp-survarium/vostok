# Structure verification: weapon_core_aimed_state_base {ctor, initialize, finalize}

Unit: `sources/vostok/game_core/sources/weapon_core_aimed_state_base.cpp` (PR #124).
Verified at HEAD 9e929f8c against the locally-built target/base rich indexes.
This report checks SOURCE STRUCTURE only (statement QUANTITY + SIZE), independent
of the byte/fuzzy %.

Target rvas (resolved via `pdb_rich_query --index binaries/rich/target/index.jsonl
--function weapon_core_aimed_state_base --list`):
  ctor 0x6ea600, initialize 0x6ea590, finalize 0x6ea5c0.
Base rvas (`--index binaries/rich/base/index.jsonl`):
  ctor 0x44af30, initialize 0x44aeb0, finalize 0x44aee0.

report.json fuzzy %: ctor 100.0, initialize 100.0, finalize 54.39.

=====================================================================
VERDICTS
=====================================================================
ctor       -> STRUCTURE MATCH      (2 vs 2 statements, every size identical)
initialize -> STRUCTURE MATCH      (3 vs 3 statements, every size identical)
finalize   -> STRUCTURE MATCH      (4 vs 4 statements; carcass skeleton intact.
                                    Residual is a codegen inline-vs-call SIZE
                                    delta on the reset() statement, NOT a
                                    structure defect. STATE stays 54.39% PARTIAL.)

No mislabeled-DONE found. Neither 100% DONE (ctor, initialize) sits over a wrong
structure - both are clean. finalize is already honestly non-100% with its
carcass kept; left untouched.

=====================================================================
1. ctor  weapon_core_aimed_state_base(weapon_core&)
=====================================================================
TARGET ; 2 statements, 0x31 bytes        BASE ; 2 statements, 0x31 bytes
  0x00  <0x28>  L15                          0x00  <0x28>  {
  0x28  <0x9>   L16                          0x28  <0x9>   }

L15 is the ctor decl line carrying the base-init delegation
`weapon_core_base_state( weapon, false )` + the two compiler-emitted vtable
stores (fsm_state @0x00, unmanaged_resource @0x18); base attributes the same to
the opening brace. L16/`}` is the empty-body close. QUANTITY 2 == 2; both sizes
identical (0x28, 0x9); total 0x31 == 0x31. Member-initializer-list shape is
correct - the single base delegation collapses to one decl-line statement, no
body assignments leaking extra statements. STRUCTURE MATCH. 100% is honest.

=====================================================================
2. initialize()
=====================================================================
TARGET ; 3 statements, 0x29 bytes        BASE ; 3 statements, 0x29 bytes
  0x00  <0x7>   L19                          0x00  <0x7>   {
  0x07  <0x1e>  L21                          0x07  <0x1e>  m_weapon.instant_aim_start( );
  0x25  <0x4>   L24                          0x25  <0x4>   }

One body statement (`m_weapon.instant_aim_start();`, L21) between prologue and
epilogue. QUANTITY 3 == 3; every offset and size identical (0x7 / 0x1e / 0x4);
total 0x29 == 0x29. The 0x1e body statement is the virtual dispatch through
weapon_core vtable slot 0x8c - shape matches exactly. STRUCTURE MATCH. 100% is
honest.

=====================================================================
3. finalize()  (already STATE[54.39%|PARTIAL] - confirming, not re-grading)
=====================================================================
TARGET ; 4 statements, 0x36 bytes        BASE ; 4 statements, 0x4f bytes
  0x00  <0x7>   L27                          0x00  <0x9>   {
  0x07  <0xd>   L28 reset()                  0x09  <0x24>  m_animation_playback_state.reset( );
  0x14  <0x1e>  L29 instant_aim_end()        0x2d  <0x1e>  m_weapon.instant_aim_end( );
  0x32  <0x4>   L32                          0x4b  <0x4>   }

QUANTITY: 4 == 4. The statement SKELETON is identical in count and order:
prologue, `m_animation_playback_state.reset();`, `m_weapon.instant_aim_end();`,
epilogue. The source body (`m_animation_playback_state.reset();
m_weapon.instant_aim_end();`) is the correct two-statement carcass. No control
structure was added or dropped: no extra `[n]` block-open, no brace `+0x002`, no
missing early-return guard. The member-init / body-statement split is correct.

SIZE divergences (codegen, NOT structure):
  - Stmt1 reset(): TARGET 0xd vs BASE 0x24. The target keeps reset out-of-line
    (`add eax,120h; call reset`, ~0xd bytes); our /Od /Ob2 /GL link INLINES
    reset's member-zeroing body (`mov [ecx],0; movss [edx+4],xmm0`), ~0x24 bytes.
    This is the documented inline-vs-call residual, exhaustively shown
    non-steerable from this function's source (decl/def split, own-header,
    __declspec(noinline), &reset address-escape, and multiple real callers all
    re-inline under /GL - see weapon_core_aimed_state_base.md iterations 6-10).
    The target's reset itself uses a frameless this-in-EAX LTCG custom convention
    not emittable from /Od source.
  - Stmt0 prologue: TARGET 0x7 vs BASE 0x9 (+2). Frame growth caused by the
    inlined reset needing a stack slot - a downstream effect of the same
    inline-vs-call, not an independent statement.
  - Stmts 2 and 3 (instant_aim_end, epilogue) are byte-for-byte size-identical
    (0x1e, 0x4).

The ~0x19 total-byte delta (0x4f vs 0x36) is entirely the inlined reset body.
The fuzzy 54.39% is a faithfully-implemented-reset trade-off (the empty-stub
elision scored 83.33% only by eliding a call the target genuinely makes - a
coincidental %, not a real match). Per the structure-verifier scope: the
STATEMENT STRUCTURE / carcass skeleton MATCHES the target; the residual is
codegen inline-vs-call, not a source-structure defect. STATE correctly stays
54.39% PARTIAL with the carcass kept inline. NOT a STATE mislabel.

=====================================================================
SUMMARY
=====================================================================
No source change required. All three functions reproduce the target's statement
skeleton (QUANTITY matches on all three; SIZE matches on ctor and initialize).
finalize's only divergence is a per-statement SIZE delta from an inline-vs-call
that is codegen, not structure - so its 54.39% PARTIAL with the carcass retained
is the honest, correct state. No 100% DONE sits over a wrong structure.
