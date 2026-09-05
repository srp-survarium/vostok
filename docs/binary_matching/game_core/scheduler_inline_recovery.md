<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Scheduler inline reconstruction

Scope: all **ten inline methods**, including the unmarked empty
`change_status` and methods whose `inline` appears only on their definitions,
plus the two retained `on_frame` consumers: **12 declarations reviewed**.
No C++/tool builds or score refresh. Raw target/frozen-base class topology is
identical; private helper and public lifecycle access remain unchanged.

## Real bug: per-frame registration retained an indeterminate mode bit

The packed word at record +0x28 is a 31-bit update interval and a one-bit mode.
Retail `register_on_frame`, RVA `0x86a60`, statement 1 after the registration
call, writes the **whole word** as `0x7fffffff`, followed by zero maximum count
and zero last-update time. Independent base-only inspection shows our existing
source instead emits `or [eax+28h], 0x7fffffff`, leaving the mode bit unchanged.

Both retained `register_object` and its frozen candidate initialize the
temporary callback's empty-state word, but do not initialize the scheduler
metadata before copying that temporary into the vector. `record()` in this
toolchain/context is therefore not evidence of zero-initialized mode. The
candidate can retain the fixed-interval bit from prior stack contents; the
retail per-frame caller explicitly clears it. Per-record `on_frame` at
`0xa8dd0 + 0x26` branches on exactly that high bit. With it set, a supposed
per-frame callback instead enters periodic scheduling with the huge interval.

Restore `record.m_type = type_on_frame` alongside the existing interval and
maximum-count assignments in their shared retail statement. Do not change
the record's layout, bit widths, value-initialization expression, or all
registration paths to steer this one missing store. Damage zones, traps and
anomalies already use the per-frame API; medkit/oxygen use periodic
registration/unregistration. No new use of the modeled transition API is
inserted into those lifecycles.

## Models: activation, deactivation and the previously unmarked empty helper

The existing `change_status(id, dest, src)` was empty even though it had no
`no source` marker. Restore a bounded transfer model:

1. Copy the indexed source record into the destination vector.
2. Reuse retained `unregister` to perform source swap/pop and repair the moved
   record's identifier index.
3. Retag the transferred identifier with its new destination index/state.

`activate` and `deactivate` guard an already-requested state, then call the
private helper with the appropriate active/inactive vectors. These are
**models**, not verified original bodies. Search of current source and the
retained index found no transition consumer/procedure; absence of those
records is not proof of server/editor-only ownership or original emptiness.
Do not rewrite existing register/unregister calls to manufacture consumers.

The shared helper preserves callback, interval, mode and timestamp rather
than inventing a reset/re-registration policy. Repeated calls are modeled as
no-ops, but an original assertion or different contract remains possible.
Copy/move order and the original helper boundary remain unverified.

One important caveat is deliberately not hidden: retained `unregister` at
`0x82de0` decrements `m_current_index` whenever it is at/after the removed
identifier's index, **without checking the active bit or the -1 sentinel**.
The new model reuses this actual behavior, including for inactive-source
activation. Whether original transition code had a different callback-time
index policy is open. No live caller is switched to this model before that
consumer is recovered. Do not silently "fix" the retained unregister itself.

A transient Python bookkeeping model exhaustively checked 3,852 cases: one
through six records, every active/inactive placement, each selected record,
both requested states, repeated transitions and three current-index values.
All records/payloads remain present once, and each identifier agrees with its
vector/index. This validates the proposed transfer bookkeeping only; it does
**not** validate C++ codegen, callback-time visitation, or retail semantics.

## Existing bodies and source constraints

- Construction is expanded in `game::game`, RVA `0x5d7a70`, before the first
  body statement: scheduler at game +0x380, allocator-backed vectors at +0/+0x10,
  current index **zero** at +0x28, and object-table pointers at +0x20/+0x24.
  Keep zero initialization; don't infer -1 just from the post-frame sentinel.
- Destructor `0x7d8c0` destroys/deallocates active then inactive vectors. The
  existing empty destructor body correctly leaves that to member destruction.
- `objects` expands in `register_object` and `unregister` as the active high
  bit selecting `m_objects[0 or 1]`. Keep the private accessor.
- Registration `0x82cf0` assigns the active bit, obtains that vector, derives
  the identifier index, appends a default record and assigns its identifier
  and callback. The named local references may disappear under optimization;
  this is not a reason to remove the declared helper or bypass its visibility.
- Periodic registration `0x82da0` sets the high type bit and supplied interval,
  maximum count and start time. Preserve its existing implementation.
- Unregister `0x82de0` copies the last record over the removed one, repairs the
  moved identifier's index, destroys the popped callback and clears active.
  Keep its observed unusual unconditional current-index policy above.
- Per-record tick's PDB names `update_delta` as **const u32**; restore that
  qualification. Its frozen structure already matches (20 statements, 0x15a
  bytes), so statement equality alone did not expose the qualifier omission.
  The periodic branch advances last-update time by the full elapsed interval
  count, caps callback count afterward, copies the callback, and checks the
  identifier's active bit on every iteration. Preserve these mutation-safe
  callback operations rather than reading a vector reference after a callback.
- Outer tick `0x77de80` uses active-vector +0x10 for **both** size and indexing
  and stores -1 after the loop. Remove the stale claim that it mixed inactive
  size with active indexing. The field `m_last_update_time` is advanced by
  elapsed intervals, not simply the next firing time; remove that old label.

Previous source comments called these methods generally "not steerable" or
claimed matching had failed. They are not body evidence and are removed.
Known frozen call-vs-inline differences in vector operations can be revisited
only after the deferred build; they do not excuse missing assignments or
empty lifecycle helpers.

## Every declaration

| # | Declaration | Disposition |
|---:|---|---|
| 1 | `scheduler(allocator*)` | Existing allocator/vector/table and zero-index initialization verified in game construction. |
| 2 | `~scheduler()` | Existing empty body; retained member destruction verified. |
| 3 | `objects(id*)` | Existing private active-bit lookup verified in two retained consumers. |
| 4 | `change_status(id*,dest&,src&)` | New transfer model for an unmarked empty body; original order/index policy open. |
| 5 | `register_object(id*,callback const&,bool)` | Existing retained append/bind/index operations verified. |
| 6 | `register_on_frame(id*,callback const&,bool)` | Real missing mode-bit clear restored. |
| 7 | `register_for_update(id*,callback const&,bool,u32,u32,u32)` | Existing periodic metadata stores verified. |
| 8 | `unregister(id*)` | Existing swap/pop/index/callback destruction verified, including unusual index policy. |
| 9 | `activate(id*)` | New guarded inactive-to-active model; repeat-call and callback-time contracts open. |
| 10 | `deactivate(id*)` | New guarded active-to-inactive model; repeat-call and callback-time contracts open. |
| 11 | `on_frame(record&,u32,u32)` | Retained consumer checked; restore const interval local, preserve callback-copy/active checks. |
| 12 | `on_frame(u32,u32)` | Retained active-only iteration and final sentinel verified; implementation unchanged. |

Deferred checks: per-frame packed store and its statement partition, existing
registration consumers, unchanged record-tick partition and newly modeled
transition bodies if original consumers become available. No fresh byte-match
claim is made from this source-only batch.
