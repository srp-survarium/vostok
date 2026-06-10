# ASSERT eater's COMDAT-fold misname is scored MATCHED by report.json
tags: cpp:assert | asm:call | topic:scoring-artifact topic:fold-icf
symptoms: dummy::nonnull vs finalize_impl, --view diff flags the eater call, N/M instructions equal line ~85-88%
confidence: 9/10
variants: assert-eater-recover.md, fold-misname-empty-fn.md

`--view diff` flags the compiled-out ASSERT's call target as a mismatch (base
`boost::function1<void,char const*>::dummy::nonnull` vs target
`vostok::memory::fixed_size_allocator<...>::finalize_impl` - arbitrary fold names), but
report.json pairs the reloc at the fold representative and scores it MATCHED. A function
whose ONLY diff flag is this misname is 100%.

```asm
call boost::function1<...>::dummy::nonnull       ; base fold name
call fixed_size_allocator<...>::finalize_impl    ; target fold name - SAME folded empty fn
```
Read the TRUE % from report.json `fuzzy_match_percent` (top-level field), not the `N/M instructions equal` footer; STATE markers quoting the footer are stale.
Evidence: game_core/collision_geometry.cpp (get_overlapping_objects et al. - all 100% despite the misname).
