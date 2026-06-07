# damage_model::damage_model - structure verification

Verdict: STRUCTURE MATCH (shape ok). fuzzy 98.91% (report.json, post-rebuild).
Marker was stale at 97.55%; corrected to 98.91%.

## Structure diff (target 0x6efeb0 | base 0x45d970), target 11 / base 11 stmts

    .. same ..
    ; aligned 11, size-diffs 0, quantity-diffs 0

Member-init list + body assignments all match in count and size.

## Cause: boost::bind/function temporary cleanup + ICF call offsets

All divergences are:
- extra `push esi`/`pop esi` and two `lea ecx,[..] -> lea esi,[..]` spots in the
  cleanup of the boost::bind temporaries built for the two
  `m_*_damaged_subscriber.subscription_callback = boost::bind( ... )` assignments;
- ICF-folded `call` target-offset differences (same resolved symbol, different
  fold address) - already scored by objdiff.

Register-allocation choice (esi as scratch for the boost temp cleanup) inside the
boost::bind/function header inlining is not steerable from C++ source. Non-
steerable. STRUCTURE MATCH.
