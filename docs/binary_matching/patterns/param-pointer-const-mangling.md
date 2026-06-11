# Top-level const on a by-value pointer parameter gates pairing (QAV vs PAV)
tags: cpp:const | topic:mangling
symptoms: QAV, PAV, QBV, PBV, not found in BASE index, unpaired with identical body
confidence: 9/10
variants: mangled-join-failure.md, template-param-byvalue-vs-constref.md

Top-level `const` on a BY-VALUE parameter is dropped for overload resolution but MSVC
encodes it in the decorated name (`T* const` = QAV vs `T*` = PAV); objdiff pairs by
mangled symbol, so the mismatch makes the pair unpairable regardless of byte identity.
TELL: a sibling overload/functor in the same file already uses `T* const` and matches.
RULE: read the P/Q letter off the target's mangled name for EVERY pointer parameter
(`PAV` = `order*`, `QAV` = `order* const`); only the parameter spelling is mangled -
locals keep whatever `--view info` records (a `response* const` LOCAL is fine).

```cpp
bool operator( )( damage_protector* const protector );   // match the target's const exactly
```
Evidence: protect_affect_predicate::operator() in body_part_parameters.cpp 0.00 -> 100.00 by adding `const` (twin protect_damage_predicate was already `* const`); network_world::add_order/add_response None -> 100 with zero body edits by DROPPING a `* const` the target mangles `PAV`.
