# Structure audit: weapon_core_animation_end_aware_state::on_animation_end

- target rva: 0x6ed050   base rva: 0x44f850
- sig: vostok::animation::callback_return_type_enum
  survarium::weapon_core_animation_end_aware_state::on_animation_end(vostok::animation::animation_callback_params&)
- report.json fuzzy_match_percent: 90.916664 (label "90.92%|PARTIAL", #127)

## Verdict

STRUCTURE MATCH.

The condensed structure-diff prints `target 11 / base 12 stmts ; aligned 9,
size-diffs 0, quantity-diffs 5`, but ALL 5 reported quantity-diffs are an
ALIGNMENT ARTIFACT, not a real divergence. They are NOT a structure mismatch.

## Side-by-side skeletons

```
0x09 <0x7>  L35 params.interrupt_animation_player_tick = false;   ==  0x09 <0x7>  same
0x10 <0x10> L37 if ( params.animated_object == &m_weapon )        ==  0x10 <0x10> if ( params.animated_object == &m_weapon )
0x20 <0xc>  L38 ASSERT( UNKNOWN_EXPRESSION ); (compiled-out)       ==  0x20 <0xc>  same
0x2c <0x1b> L40 if ( m_animation_to_wait_for == params.animation )  ~  0x2c <0x1c> if ( m_animation_to_wait_for == params.animation )
0x47 <0xa>  L41 m_animation_has_been_ended = true;                 ==  0x48 <0xa>  same
0x51 <0x14> L42 on_animation_end_impl( params.interrupt_... );     ==  0x52 <0x14> same
0x65 <0x2>  L46 return animation::callback_return_type_call_me_again; == 0x66 <0x2> same
```

target 11 / base 12 statements. The 1-statement count gap is the base PDB's
trailing `EMPTY only base` (`<0>`) run; it carries no instructions.

## Why the 5 "quantity-diffs" are not real

The target PDB exposes the two `if` statements only as bare source-line markers
(`L37`, `L40`); the base PDB exposes them as resolved C++ text. The diff tool
therefore paired:

- base `if ( params.animated_object == &m_weapon )` (ONLY base) against
  target `L37` (ONLY target) - SAME offset 0x010, SAME size 0x10.
- base `if ( m_animation_to_wait_for == params.animation )` (ONLY base) against
  target `L40` (ONLY target) - SAME offset 0x02c, sizes 0x1b vs 0x1c.
- plus one trailing `EMPTY only base` `<0>`.

Same offsets + same sizes => these are the SAME statements, mislabeled because
the two sides spell the statement differently. Both `if`s are brace-blocks whose
`jne short .1` / `je short .1` share the single `.1` return label (base disasm
0x1e and 0x46 both target 0x66); the nested-`if`-with-braces source shape in the
.cpp reproduces this exactly. No statement is added, dropped, or re-scoped.

## What the 90.92% residual actually is (SIZE, not structure)

The byte gap is confined to the second `if`'s `intrusive_ptr::operator==`
argument setup, offsets +0x2c..+0x38:

```
base:    mov edx,[ebp+8]; mov eax,[edx+4]; push eax; mov ecx,[ebp-8]; add ecx,138h   (push params.animation first, then this+0x138)
target:  mov edx,[ebp-8]; add edx,138h;   mov eax,[ebp+8]; mov eax,[eax+4]           (compute this+0x138 first, then load params.animation)
```

This is an operand evaluation-ORDER / inline-vs-call frame difference inside the
comparison, costing the 1-byte size diff on the `L40` statement (0x1b vs 0x1c)
and the residual %. It does not change the statement set or control structure.

## Source fix

None required for structure. The .cpp already matches the target shape (nested
braced `if`s, compiled-out ASSERT, single return). The remaining % is a
genuine matcher problem (force the `operator==` argument evaluation order /
inline shape) and is left for a matcher; it needs a rebuild to confirm. STATE
remains PARTIAL with its existing inline-vs-call note - no downgrade, no logic
change made by this audit.

## Audit caught new

No new structural divergence. Confirms the 90.92% is a SIZE/operand-order
artifact, not the "high % over wrong structure" trap.
