# scheduler::on_frame(u32, u32) - structure audit

Target rva 0x77de80 / base rva 0x573f20.
Signature: `void survarium::scheduler::on_frame(const unsigned int, const unsigned int)`
(the public overload; the `(record&, u32, u32)` overload at target 0x0a8dd0 is a
separate function, not audited here).

fuzzy_match_percent (report.json): 46.38889% - STATE already `46.39%|PARTIAL`.

## Verdict: STRUCTURE MATCH

The label is NOT mislabeled-DONE; it is correctly PARTIAL. The low % is an LTCG
inline-vs-out-of-line codegen decision, already documented inline, NOT a source
shape divergence. No downgrade needed (it was never banked as DONE). The audit
caught nothing new; it confirms the existing PARTIAL note.

## Condensed structure-diff (target 5 stmts / base 6 stmts)

```
.. same ..
0x00f <0x3d> | 0x009 <0x41> | for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )   SIZE
0x04c <0x2d> | 0x04a <0x28> | on_frame( m_active_objects[m_current_index], frame_delta, current_time );   SIZE
--          | <0>         |    EMPTY only base
--          | 0x072 <0xa> | m_current_index = u32(-1);   ONLY base
.. same ..
0x079 <0xd> | --          | L52   ONLY target
; aligned 2, size-diffs 2, quantity-diffs 3
```

## Divergence analysis

Two source statements, both present on both sides, in the correct order:
1. the `for` loop header, and
2. the recursive `on_frame(...)` call in the loop body,
then the `m_current_index = u32(-1)` reset.

### SIZE diffs (2) - NOT a source-shape cause

- `for` header: target 0x3d vs base 0x41. Target emits a CALL to
  `vostok::vectora<record>::size` (out-of-line); base inlines `.size()` as raw
  pointer arithmetic (`mov;add 10h;mov;mov;sub;idiv 38h`). Same source expression.
- loop body call: target 0x2d vs base 0x28. Target emits a CALL to
  `vector<record>::operator[]` (out-of-line); base inlines `[]` as
  `imul edx, 38h; add edx, [ebp-8]`. Same source expression.

These are the inline/out-of-line LTCG decisions already documented in the source
`claude@NOTE`. They cannot be steered from this function's source: the body, loop,
member access (m_active_objects at offset 0x10) and call args all match.

### Quantity diffs (3) - all artifacts, NOT real

- `EMPTY only base` and `ONLY base m_current_index = u32(-1)` vs `ONLY target L52`:
  this is ONE statement double-attributed. The `--view diff` output shows the final
  `mov dword ptr [eax+28h], 0FFFFFFFFh` (the `m_current_index = -1` reset) present in
  BOTH disassemblies, just charged to a different source line on each side (base to
  its own srcline, target to L52). It is the same statement, same bytes, same place;
  the aligner could not pair the differing srcline labels. Not a control-structure or
  count divergence.

## Source fix

None required. The source already writes the carcass-correct shape:
- `for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )`
  brace-less single-statement body (matches target),
- followed by `m_current_index = u32(-1);`.

The residual % is template-COMDAT inline-vs-call (LTCG), a matcher concern, not a
structure-verifier restructure. Leave at PARTIAL.

## Note on the trap

report.json shows 46.39% here - a LOW number that, unlike the usual high-%-wrong-
structure trap, correctly reflects real instruction divergence. The structure is
nonetheless sound; the byte gap is pure codegen (inline vs out-of-line), so raising
the % is not achievable by reshaping the source.
