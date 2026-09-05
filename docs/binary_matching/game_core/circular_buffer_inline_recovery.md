<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Circular-buffer inline review

Scope: all 22 declarations on `survarium::circular_buffer<T>`, including the
19 that still carried `/* no source */` at the `855dc1283` baseline. The shipped
instantiation is `circular_buffer<client_player_history_item>` (0x14 bytes),
embedded as `player::m_history` at offset 0x10e1c.

Raw target/base `pdb_topology --classes --class` reports one identical semantic
variant on each side and no access, declaration or layout differences. The
generated target header's displayed `public boost::noncopyable` is therefore
not a reason to change the existing private source base. No visibility change
is needed to restore these seams.

## Consumer evidence

- `new_item`, target RVA `0x7f000`: construct at head, advance head modulo the
  allocation count, and advance tail if head catches it. **One slot is reserved**;
  usable capacity is `m_max_count - 1`, not `m_max_count`.
- `player::remove_oldest_history_item`: one modulo increment/store of tail;
  no item destructor. Do not add destruction to `pop_tail` as a generic container
  improvement.
- `player::history_lower_bound_index`: head/tail loads, modulo decrement, and
  physical indexing with a 0x60-byte item stride. `operator[]` is not an index
  relative to the oldest item.
- `player::remove_oldest_history_items`: head/tail equality test, oldest-item
  timestamp load and the same modulo tail increment.
- `player::remote_input`: head/tail equality followed by `(head + count - 1) %
  count` and physical indexing to obtain the newest input.
- `player::time_warp` and `replay_history` already use the named helpers;
  there is no open-coded access to the container's private indices to propagate.

No source consumer was found for `resize`, `size`, `insert`, `full`, `index`,
or `is_valid`. The last three newly supplied bodies are models constrained by
the observed ring representation, not additional matched procedures.

## Every declaration

| Function | Disposition |
|---|---|
| constructor | Existing allocator/count/zero-index initialization retained. |
| destructor | Existing pop-to-empty/free body retained; committed ledger records exact max. |
| `resize` | Open: allocation growth/shrink, element preservation and index rebasing are not established. A raw realloc and a copy-to-linear-order implementation are not equivalent. |
| `size` | New model: modular head-minus-tail distance, consistent with the one-reserved-slot invariant. |
| `new_item` | Existing retained body and caller seam unchanged. Its committed register-allocation residual is outside this source-only pass. |
| `insert` | Open: argument could be a physical insertion position or a logical offset; shifting direction and full-buffer eviction policy need a consumer. It cannot safely be replaced with `new_item`. |
| `pop_tail` | Existing body confirmed by the target's modulo tail store; stale marker removed. |
| `oldest()` | Existing physical-tail accessor retained; mutable overload matches the removal consumer. |
| `oldest() const` | Same accessor with const return; no distinct standalone procedure. |
| `newest()` | Existing previous-head accessor retained; live history update code uses it. |
| `newest() const` | Same operation visible in `remote_input`. |
| `empty` | Existing head/tail equality is visible in removal and remote-input consumers. |
| `full` | New model: `next(head) == tail`, not `head == tail` (which denotes empty). |
| `next` | Existing modulo increment is present in both insertion and removal. |
| `previous` | Existing modulo decrement is present in history search and newest-item access. |
| `operator[](u32)` | Existing raw physical index retained, not an offset from tail. |
| `operator[](u32) const` | Same raw physical-index operation visible in history search. |
| `head` | Existing direct field load retained; history-search consumer. |
| `tail` | Existing direct field load retained; history-search consumer. |
| `max_count` | Existing allocation-count getter retained, not changed to usable capacity. |
| `index` | New model: pointer difference from `m_history`. Requires an element of this allocation; this is not a general pointer-validity check. |
| `is_valid` | Open: allocated-range membership and live-ring membership give different answers for evicted/free slots. Do not select one solely from the name. |

Result: thirteen stale markers removed without changing their bodies, three
new models, three still-open bodies with explicit alternatives; constructor,
destructor and `new_item` also reviewed. All 22 declarations are accounted for,
not all implementations proven. No C++ build or ledger refresh was run.
An independent arithmetic check over every head/tail pair for allocation counts
1 through 32 passed 22,880 size/full/overwrite invariant assertions. This checks
the ring model, not compilation or retail byte equivalence.

Before this edit, `new_item` has four statements on each side; the structure
diff reports the known boundary-size/assignment-scheduling residual (target
0x4a, base 0x46). It is not a newly introduced regression or a reason to rewrite
the exact-max destructor or the existing callers during this pass.
