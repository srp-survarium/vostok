# Structure verification: body_part_parameters::fill_new_stats_item

Function: `void survarium::body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(vostok::ai::statistics_item<46,16>&, const unsigned int) const`
Target rva: 0x0ba3c0    Base rva: 0x085aa0
report.json fuzzy_match_percent: 91.78519%   STATE: BLOCKED (#119)

## Verdict

STRUCTURE MATCH.

The body is a statement-for-statement match against the target. The audit did NOT
catch any new structure divergence the % hid: the residual is a single
whole-program COMDAT inline decision on the `fixed_string<46>` type, already
correctly recorded as BLOCKED. STATE left at BLOCKED (not downgraded, not banked).

## Condensed structure-diff (--view structure-diff --condensed)

```
target: 0xba3c0            base: 0x85aa0
; ... ; target 22 stmts / base 23 stmts
.. same ..
0x010 <0x11> | 0x010 <0x12> | new_stats_item.caption = m_name;                                  SIZE
.. same ..
0x1f0 <0x31> | 0x1f1 <0x19> | new_stats_item.content.push_back( fixed_string<46>( "none" ) );   SIZE
--          | <0>          |                                                                     EMPTY only base
; aligned 20, size-diffs 2, quantity-diffs 1
```

## Divergences

1. QUANTITY (22 target / 23 base) -- `EMPTY only base`. This is a collapsed
   source-line blank-line gap present on the base side only, NOT a control-structure
   divergence. No extra/missing `[n]` block, no extra statement, no missing early
   return. Benign; not a real quantity diff.

2. SIZE @ L306 (root cause) -- `new_stats_item.content.push_back( fixed_string<46>( "none" ) )`,
   target 0x31 vs base 0x19. `--view diff` at the `je short` (m_affects.empty) branch:
   - base: `lea ecx,[ebp-80h]; call vostok::fixed_string<46>::fixed_string<46>` -- the
     `fixed_string<46>(char const*)` ctor is called OUT-OF-LINE (it exists out-of-line
     in base @0x030ae0).
   - target: INLINES that ctor: `mov dword ptr [ebp-0CCh], 2Eh` (capacity 46),
     `push "none"`, `lea/push` temps, `call vostok::buffer_string::buffer_string`.
   This is the cause of the +0x10 frame delta (sub esp, 0E8h vs 0D8h).

3. SIZE @ L286 (cascade) -- `new_stats_item.caption = m_name;`, target 0x11 vs base
   0x12. Pure register/stack-slot allocation cascade off the wider frame from (2):
   target `mov esi,[ebp+8]` (operator= -> buffer_string::operator=) vs base
   `push eax; mov ecx,[ebp+8]` (-> fixed_string<32>::operator=). 1-byte allocation
   noise, not a source-shape diff.

## Source-shape cause and fix

No source-shape divergence (no body-assigns-vs-member-init, no brace mismatch, no
early-return-vs-if, no definition-order problem). The control structure (the `for`
loop `[1]` block at L297 and the brace-less `if` at L305) matches the target exactly.

The sole residual is whether the toolchain inlines `fixed_string<46>(char const*)`
at the `"none"` leaf. That is a whole-program COMDAT/inline decision on the
`fixed_string<46>` type, not steerable from this function's source. Nothing to
restructure here. BLOCKED on `fixed_string<46>`'s emission (out-of-line ctor present
in base but inlined in target), consistent with the existing #119 BLOCKED label.

No rebuild performed; no compiled logic changed. Carcass in the .cpp replaced with
the condensed structure-diff per MATCHING.md.
