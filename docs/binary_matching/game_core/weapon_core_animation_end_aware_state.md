# weapon_core_animation_end_aware_state (group: initialize, finalize, set_animation_to_wait, on_animation_end)

The BASE of the weapon-state hierarchy (parent of weapon_core_show/hide/aimed/idle
state bases, already matched on the stack). Four members in one unit.

## Branch
Stacked on `match/game_core-weapon_core_hide_state_base` (PR #126, the stack tip).
```
git checkout match/game_core-weapon_core_hide_state_base
git checkout -b match/game_core-weapon_core_animation_end_aware_state
```

## Target lookups (verbatim)
```
nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_animation_end_aware_state --list
```
RVAs (target):
- on_animation_end       0x6ed050
- finalize               0x6ed0c0
- set_animation_to_wait  0x6ed0e0
- initialize             0x6ed130

```
nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target
```

## Mangled access chars (target COFF weapon_core_animation_end_aware_state.cpp.obj)
- on_animation_end       `?on_animation_end@...@@IAE?AW4callback_return_type_enum...`  -> I = protected, non-virtual
- finalize               `?finalize@...@@MAEXXZ`                                       -> M = protected virtual
- set_animation_to_wait  `?set_animation_to_wait@...@@IBEXAB...@Z`                     -> I = protected, B = const
- initialize             `?initialize@...@@MAEXXZ`                                     -> M = protected virtual
Header had initialize/finalize/on_animation_end/set_animation_to_wait under `public:`
(base produced `UAE` = public virtual; mismatch). Moved all four to `protected:`.
on_animation_end_impl stays pure-virtual private (`= 0`), not in this unit.

## Bodies (from target asm)

### finalize (0x6ed0c0)  source line 24
```
push <this>; push 1; mov ecx,[this+128h]; call weapon_core::remove_animation_callback
```
-> `m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );`
(channel_id_on_animation_end = 1; callback_uid = this. m_weapon @ 0x128.)

### initialize (0x6ed130)  source lines 18,19
- 0x10 `mov byte[this+135h],0` = m_animation_has_been_ended = false   (line 18)
- line 19: boost::bind(&on_animation_end, this, _1) -> boost::function1, then
  `set_animation_callback(channel_id=1, callback_uid=this, function&)`.
-> ```
   m_animation_has_been_ended = false;
   m_weapon.set_animation_callback(
       animation::channel_id_on_animation_end, this,
       boost::bind( &weapon_core_animation_end_aware_state::on_animation_end, this, _1 ) );
   ```
The body inlines boost::bind temp construction + function1 ctor + clear (dtor of temp).

### set_animation_to_wait (0x6ed0e0)  source lines 29,30  const
- 0x09..0x12 `mov eax,[this+128h]; call weapon_core::get_user`  (m_weapon.get_user())
- 0x1a..0x25 virtual call on base_player at [vtbl+0x5c] returning bool const -> is_replaying_history()
- 0x2c `jne .1` skip when true; if (!...) { m_animation_to_wait_for = animation; }
- 0x32 `add ecx,138h; call intrusive_ptr::operator=`  (m_animation_to_wait_for @ 0x138)
-> ```
   if ( !m_weapon.get_user()->is_replaying_history() )
       m_animation_to_wait_for = animation;
   ```
CALL SITE INFO confirms `bool <unknown>() const` at vtbl+0x5c.

### on_animation_end (0x6ed050)  source lines 35-46  returns callback_return_type_enum
- 0x0c `mov byte[params+16h],0` = params.interrupt_animation_player_tick = false   (line 35)
- 0x18 `cmp [params],[this+128h]` = params.animated_object == &m_weapon ; jne .1     (line 37)
- 0x20 `mov byte[ebp-1],0; lea eax; call finalize_impl` = COMPILED-OUT ASSERT (corrected
  guidelines: empty_stub == recoverable ASSERT, delinker misnames it `finalize_impl`).
  Recovered as `ASSERT( UNKNOWN_EXPRESSION );` -- the first statement inside the
  `if ( params.animated_object == &m_weapon )` block (source line 38), NOT an LTCG artifact.
- 0x3b operator== between m_animation_to_wait_for (@0x138) and params.animation (@0x4)  (line 40)
- 0x4a `mov byte[this+135h],1` = m_animation_has_been_ended = true                    (line 41)
- 0x58 virtual call on this [vtbl+0x20] passing &params.interrupt_animation_player_tick:
  on_animation_end_impl( params.interrupt_animation_player_tick )                      (line 42)
- 0x65 `xor eax,eax; ret 4` = return callback_return_type_call_me_again (value 0)      (line 46)
-> ```
   params.interrupt_animation_player_tick = false;
   if ( params.animated_object == &m_weapon )
   {
       ASSERT( UNKNOWN_EXPRESSION );   // line 38: compiled-out ASSERT (+0x0c empty_stub)
       if ( m_animation_to_wait_for == params.animation )
       {
           m_animation_has_been_ended = true;
           on_animation_end_impl( params.interrupt_animation_player_tick );
       }
   }
   return animation::callback_return_type_call_me_again;
   ```

## Anchor
Mirror of show/hide: `use_game_core_weapon_core_animation_end_aware_state` in
temp_include_all.cpp -- concrete derived stub overriding the pure
weapon_and_hands_expression AND on_animation_end_impl, ctor (weapon, true).
Qualified non-virtual calls to initialize/finalize/set_animation_to_wait/on_animation_end,
escape &state / &anim / &params through example_callback. Add include + IncludeAll call.

## Iterations
1. INPUT: all four bodies + header access fix (protected) + anchor.
   BUILD: FAILED to compile - `weapon_core_animation_end_aware_state.cpp(46) : error
   C2678: binary '=' : no operator found which takes a left-hand operand of type
   'const vostok::resources::managed_resource_ptr'`. set_animation_to_wait is a const
   method, so m_animation_to_wait_for is const inside it; but the target DOES assign
   (`add ecx,138h; call intrusive_ptr::operator=`). FIX: declare m_animation_to_wait_for
   `mutable` (a const setter mutating a cache member). Rebuild.
2. INPUT: m_animation_to_wait_for declared `mutable` (the C2678 fix). Touched the
   .cpp + temp_include_all.cpp so ninja recompiled with the changed header.
   BUILD: compiled + linked OK; report.json fuzzy_match_percent:
     - initialize            100.0   DONE
     - finalize              100.0   DONE
     - on_animation_end       78.03  PARTIAL
     - set_animation_to_wait  77.33  PARTIAL
   DIFF (set_animation_to_wait, --view diff): body correct; the only real divergence is
   LTCG inline-vs-call of weapon_core::get_user() - target keeps get_user standalone
   (@0x9b330, `call get_user`), base /GL-inlined the trivial `{ return m_user; }` accessor
   whole-program (`mov eax,[m_weapon+44Ch]`), shifting frame slots too. `pdb_rich_query
   base --function weapon_core::get_user` -> "no function matched" confirms the inline.
   Unsteerable from this function's source.
   DIFF (on_animation_end, --view diff via --rva 0x6ed050): body matches byte-for-byte
   EXCEPT the documented LTCG-folded empty out-of-line call at line 38 (`mov byte[ebp-1],0;
   lea eax,[ebp-1]; call <empty/finalize_impl>`, assembly_patterns.md line 462 - same
   artifact as show/hide), plus register/[ebp-XX] slot noise. Not reproducible. STOP.

## Outcome (FINAL, after ASSERT recovery)
- initialize            100%   DONE
- finalize              100%   DONE
- set_animation_to_wait 77.33% PARTIAL  (inline-vs-call of trivial get_user(); re-diff, see review note)
- on_animation_end      90.92% PARTIAL  (was 78.03 pre-ASSERT; recovered -> residual is inline-vs-call frame shape)

## Review note (new guidelines)
The updated MATCHING.md narrows the LTCG excuse to *function arguments only*. The two PARTIAL
residuals here (set_animation_to_wait's get_user() inline-vs-call; on_animation_end's post-ASSERT
inline-vs-call frame shape) are therefore downgraded from "LTCG" to matching problems to re-diff
against source on a future rebuild. on_animation_end's empty_stub WAS correctly recovered as an
ASSERT (78.03 -> 90.92). This review did NOT rebuild; the body shape is unchanged.
Regressions caused: NONE. report-changes lists 59 "regressed" 100->0 entries, all of the
empty-function ICF / strip visibility-flip stale-baseline class (scalar deleting destructors,
interlocked_inc/dec, empty_stub, size_policy, intrusive_ptr/CRT/Scaleform/bullet/ai symbols)
- matched 1:1 by the `improved` list flipping the same symbols 0->100. My git diff touches
only this class's .cpp/.h + the temp_include_all anchor; none of the regressed symbols are
reachable through it. All previously-matched siblings kept their exact documented scores
(hide/show init 75.1 + on_animation_end_impl 69.9, aimed finalize 83.3, idle 100/100/100,
base_state ctor 100). finalize itself appears as 0->100 in `improved`.

SWEEP RESULT (rebuild after ASSERT recovery): on_animation_end 78.03% -> 90.92%
(PARTIAL; the recovered ASSERT closed most of the gap, residual is the inline-vs-call
frame shape). set_animation_to_wait stays 77.33% (genuine LTCG get_user() inline).
