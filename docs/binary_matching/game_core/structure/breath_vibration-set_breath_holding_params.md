# structure verification: breath_vibration_calculator::set_breath_holding_params

target rva 0x5834d0  base rva 0x4574a0
report.json fuzzy_match_percent = 76.803276 (INPROGRESS, issue #117)

## Verdict

STRUCTURE MISMATCH (both quantity + size).

The 76.80% fuzzy score is over the WRONG structure. The setter has TWO
source-shape divergences that the byte/instruction view hides because the bulk
residual is an unrelated inlining artifact (fsm::states()/front() inlined in base
vs out-of-line in target). Statement-shape: target 8 body statements vs our base 8
body statements, but NOT the same 8 - the base is missing one and has a spurious
extra one, with two SIZE diffs on top.

## Skeletons (condensed structure-diff)

```
target: 0x5834d0            base: 0x4574a0
; target 10 stmts / base 13 stmts
.. same ..
--          | <0>         |    EMPTY only base
0x012 <0x21> | 0x012 <0x1a> | for ( ai::fsm_state* it = m_logic.states( ).front( ); it; it = it->next )   SIZE
0x033 <0x17> | 0x02c <0x13> | static_cast< breath_state* >( it )->set_breath_holding_params( params );   SIZE
--          | <0>         |    EMPTY only base
0x04a <0x2> | --          | L37   ONLY target
.. same ..
--          | <0>         |    EMPTY only base
.. same ..
0x063 <0x16> | 0x056 <0x15> | m_logic.set_initial_state( m_logic.states( ).front( ) );   SIZE
0x079 <0x3b> | 0x06b <0x23> | m_target_multiplier = static_cast< breath_state* >( m_logic.current_state( ) )->get_multiplier( );   SIZE
--          | 0x08e <0xc> | m_current_multiplier = m_target_multiplier;   ONLY base
.. same ..
; aligned 5, size-diffs 4, quantity-diffs 5
```

Target body statement list (authoritative, from the generated target skeleton):
L34 m_params=params; / L35 for(...) [block-open 1] / L36 for-body /
L37 (+0x002, back-edge) / L38 if(m_params) / L39 reserve= / L40 set_initial_state /
L41 multiplier (0x3b). No L42.

## Divergence A - QUANTITY + control structure (for-loop body braces)

target: `0x04a <0x2>  L37  ONLY target`

In the target the for-loop body's back-edge `jmp short` (target 0x4a) is attributed
to its OWN source line, L37 - the classic `+0x002` closing-`}` marker. That only
happens when the for body is a BRACED block:

```cpp
for ( ai::fsm_state* it = m_logic.states( ).front( ); it; it = it->next )
{
    static_cast< breath_state* >( it )->set_breath_holding_params( params );
}
```

Our base writes the for body brace-less (single statement), so the back-edge jmp
(base 0x3d) folds into L36 and L37 never appears. SOURCE FIX: add `{ }` around the
for-loop body.

## Divergence B - QUANTITY + size (chained multiplier assignment)

target: `0x079 <0x3b>` is ONE statement (0x3b bytes) and there is NO L42.
base:   `0x06b <0x23>` (L41) PLUS `0x08e <0xc>` (`m_current_multiplier = m_target_multiplier;`, ONLY base).

Target asm computes the multiplier, `fstp [+0x24]` (m_target_multiplier), then
`fld [+0x24]; fstp [+0x20]` (m_current_multiplier = m_target_multiplier) - all under
the single source line L41. That is a CHAINED assignment:

```cpp
m_current_multiplier = m_target_multiplier = static_cast< breath_state* >( m_logic.current_state( ) )->get_multiplier( );
```

Our base writes them as two separate statements. SOURCE FIX: collapse the two
assignments into one chained assignment (drop the separate
`m_current_multiplier = m_target_multiplier;` line).

## NOT a divergence - the `if ( m_params )` braces

The earlier INPROGRESS note suspected the `if ( m_params ) { ... }` block (or an
early-return guard) as the problem. That is WRONG. In both target and base the
`if` body is the function's tail; MSVC debug codegen emits no scope-close jump for
it (`je .4` jumps straight to the epilogue on both sides) and statements 38/39/40/41
align. The `if` braces are structurally neutral here. The earlier note has been
corrected in the .cpp.

## Other SIZE diffs (residual, not structure)

The `for` (0x21 vs 0x1a), the `set_breath_holding_params` body call (0x17 vs 0x13),
and `set_initial_state` (0x16 vs 0x15) SIZE diffs are all the same root cause:
`m_logic.states( ).front( )` is OUT-OF-LINE (call) in the target build but INLINED
in the base build. That is an LTCG/inlining residual blocked on the ai::fsm type,
not a source-shape divergence; the call-target symbol names shown in the assembly
view (fixed_size_allocator / boost::_bi::list3) are just whatever symbol the index
resolved at that rva and are not the real callees.

## Action taken

- STATE downgraded note rewritten in
  sources/vostok/game_core/sources/breath_vibration_calculator.cpp: kept INPROGRESS
  (was already INPROGRESS), corrected the cause to A (for-body braces) + B (chained
  assignment), removed the wrong if-brace/early-return hypothesis.
- One-sided `// FUNCTION BODY` carcass replaced with the embedded condensed
  structure-diff.
- No logic changed, no rebuild. Applying A+B and re-diffing is a matcher's job.
