# Structure verification: weapon_core_aimed_state_base::finalize

Unit: `sources/vostok/game_core/sources/weapon_core_aimed_state_base.cpp` (PR #124).
Re-audited with `--view structure-diff` against the current rich indexes
(target Jun 3, base Jun 7) at detached HEAD origin/chore/structure-diff-tool.
This report checks SOURCE STRUCTURE only (statement QUANTITY + SIZE), independent
of the byte/fuzzy %.

Resolution (single, non-overloaded):
  target rva 0x6ea5c0, base rva 0x44f5e0.
  `void survarium::weapon_core_aimed_state_base::finalize()`.

report.json fuzzy %: this object currently scores 83.33% (mangled
`?finalize@weapon_core_aimed_state_base@survarium@@MAEXXZ`, size 54). The STATE
line in the .cpp reads 54.39% PARTIAL (the faithful-reset variant deliberately
kept over the empty-stub elision; see the .cpp note). I do not touch report
numbers and did not rebuild - the % is recorded here only for context.

=====================================================================
VERDICT
=====================================================================
finalize -> STRUCTURE MISMATCH (quantity) -> target 4 stmts / base 3 stmts.
            The base build DROPS the `m_animation_playback_state.reset();`
            statement entirely. STATE already PARTIAL (not mislabeled DONE);
            no downgrade needed, carcass replaced with the condensed diff.

=====================================================================
CONDENSED STRUCTURE-DIFF (embedded in the .cpp)
=====================================================================
target: 0x6ea5c0            base: 0x44f5e0
; void survarium::weapon_core_aimed_state_base::finalize() ; target 4 / base 3 stmts
.. same ..
0x007 <0xd> | --          | L28   ONLY target   ; m_animation_playback_state.reset()
0x014 <0x1e>| 0x007 <0x1e>| m_weapon.instant_aim_end( );
.. same ..
; aligned 3, size-diffs 0, quantity-diffs 1

=====================================================================
WHAT THE AUDIT CAUGHT (NEW vs the prior report)
=====================================================================
The previous report (HEAD 9e929f8c, weapon_core_aimed_state_base.md) graded
finalize STRUCTURE MATCH "4 vs 4 statements", base size 0x4f: the base INLINED
reset's member-zeroing body (`mov [ecx],0; movss [edx+4],xmm0`), prologue grown
to 0x9, a per-statement SIZE delta on a statement that was still PRESENT.

The CURRENT base index tells a different story: base is now 3 statements, 0x29
bytes, prologue back to 0x7, and reset's body is GONE - not inlined, ELIDED.
The reset statement (target L28, 0xd bytes: `add eax,120h; call reset`) has NO
counterpart on the base side at all. The inline-vs-call SIZE divergence has
become a QUANTITY divergence: a whole statement missing.

Raw confirmation:
  TARGET 0x6ea5c0 (4 stmts):
    0x07 <0xd>  mov eax,[ebp-4]; add eax,120h; call reset        ; L28 reset()
    0x14 <0x1e> ... call [edx+90h]                               ; L29 instant_aim_end() (virtual)
  BASE 0x44f5e0 (3 stmts):
    0x07 <0x1e> ... call [edx+90h]                               ; instant_aim_end() only
    (no reset code, no sub esp,8, no member-zeroing - reset compiled to nothing)

This matches report.json's 83.33% empty-stub variant: the base object built from
the current tree elides reset to a no-op, so the call the target genuinely makes
is simply absent.

=====================================================================
CAUSE AND FIX
=====================================================================
The SOURCE is faithful: `m_animation_playback_state.reset();` is written and is
the correct first statement. The divergence is pure codegen - under /Od /Ob2 /GL
the link either inlines reset's body or (now) elides it entirely, because
`animation_playback_state::reset()` zeroes members that the optimizer can prove
dead here. The target instead keeps reset out-of-line via a frameless
this-in-EAX LTCG custom convention (@0x087f60) not emittable from /Od source.

No source restructure is available from this function (the .cpp note documents
the exhausted attempts: own-header decl/def split, __declspec(noinline), &reset
address-escape, multiple real callers - all re-inline/elide under /GL). The
honest carcass is kept. A genuine fix would have to come from how reset itself
is matched, not from finalize - that is a matcher's job, named here, not applied.

STATE stays PARTIAL with the carcass (now the condensed structure-diff) retained.
This is NOT a mislabeled-DONE: the function is already honestly non-100%.

=====================================================================
SUMMARY
=====================================================================
finalize STRUCTURE MISMATCH (quantity): target 4 / base 3 statements, the base
dropping `m_animation_playback_state.reset();`. The audit caught a real change
since the prior report - the inline-vs-call SIZE delta has degraded into a
missing-statement QUANTITY delta as the current build elides reset entirely.
Source is faithful and not steerable here; STATE correctly stays PARTIAL. No
logic changed, no rebuild run.
