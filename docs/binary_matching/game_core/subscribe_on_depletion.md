# player_stamina::subscribe_on_depletion - structure verification

Verifier-matcher v2 pass on chain/game_core-sv-player_stamina.

## Target
target rva 0x59b0b0 / base 0x45c020
`void player_stamina::subscribe_on_depletion( player_stamina_subscriber* const )`

Body is one statement: `m_subscribers.push_back( subscriber );`

## Diagnosis (structure-diff)
`target 1 stmts / base 1 stmts ; aligned 1, size-diffs 0, quantity-diffs 0`
Source shape matches exactly.

## Byte diff (--view diff) - objdiff fuzzy 85.31% (marker was a stale 99.75%)
```
~ 0x03: sub esp, 14h        -> sub esp, 10h
~ 0x06: mov [ebp-14h], ecx  -> mov [ebp-10h], ecx
~ 0x0f: mov ecx, [ebp-14h]  -> mov ecx, [ebp-10h]
~ 0x12: call 0000041Bh      -> call 00000467h
```
Target reserves a larger frame (0x14 vs 0x10, one extra 4-byte slot) and calls a
different (reloc-different) `push_back`.

`m_subscribers` is `player_stamina_subscriber_list`, an
`intrusive_list< player_stamina_subscriber, ..., threading::mutex, size_policy,
no_debug_policy >` (player_stamina_subscriber.h). The inlined `push_back` takes a
mutex scope-lock; under LTCG the target's inlined body keeps an extra stack slot
for the lock guard and resolves the inner call to a different relocation than our
build does.

## Verdict
STRUCTURE MATCH. Residual is the inlined intrusive_list `push_back` with the
threading::mutex policy: frame-size + reloc-different inner call. This is
inline/LTCG, non-steerable from the single-statement source - the statement count
and shape already match the target. Marker synced to 85.31%, carcass deleted,
condensed structure-diff embedded.

The earlier 99.75% marker predates the toolchain/delinker state that now inlines
push_back differently; the divergence is genuine and not regressable from source.
