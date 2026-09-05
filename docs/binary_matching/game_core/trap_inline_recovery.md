<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Trap-family inline review

Scope: all 19 inlines of `booby_trap_set_core` (including its configuration
constructor), and all five `booby_trap_core` inlines. Both frozen raw target/base
PDB class comparisons are identical. Nine literal no-source markers are
resolved into bodies/models, and two additional unsupported true returns are
removed. No C++ or tool build was run.

## Existing operations recovered behind the declared helpers

`remove_traps` now owns the `std::for_each` over the complete trap vector,
using the existing private `remove_trap_if_active` predicate. The retained
`remove()` delegates to it. Target `remove` at RVA 0x6ed8b0 has one statement:
construct the bound member predicate, traverse the +0x118/+0x11c range, and
invoke it on each element. This removes active world objects; it is **not** a
container `clear`, a resource release or a loop shrinking the trap vector.

`load_collision` now owns the existing `booby_trap_core::load` configuration
block: five assert-shaped operations, collision-sensor geometry, usable-object
geometry, and optional hittable-object configuration. The retained virtual
loader delegates to it. Target load at RVA 0x58bf60 has 13 statements covering
those operations. The helper boundary is inferred and awaits the deferred
structure check; no geometry behavior is invented. `load_aabb` is separately
confirmed as a framed empty target method, but no extra call is added.

The derived `booby_trap` needed its invented friendship only to access the
base's private matrix. Replace its three matrix reads with the existing
const-reference `transform()` accessor and remove that friendship. Its state
and owner fields are already protected and remain directly accessible. No
field access, vtable slot or signature is broadened; the separate cook friend
is left unchanged. Direct target member loads do not establish friendship.

## Two explicit models, not recovered packet boundaries

- `count_active_traps`: count elements satisfying the already retained
  `trap_is_active` predicate and narrow to the PDB-declared `u8` result.
- `append_inactive_trap_index_to_packet`: if a trap is inactive, append its
  existing `trap_index` result, whose type is `u8`.

The names, existing active predicate, and index-serialization sibling support
these models. No target procedure is retained for either helper, and the
direct-xref scan of retained `trap_is_active` (RVA 0x6ed820) reports no direct
callers. That does not prove no inlined or indirect use. Current source has no
consumer that establishes the original filtering, null/sentinel treatment,
assertions or packet ordering, so those questions remain open. No invented
caller is added to make the models appear in the binary.

Their definitions stay in the owning TU, after the complete trap type and
existing predicate are available. `remove_traps` is likewise defined before
its real consuming `remove()`; `load_collision` before its consuming `load()`.

## Non-returning defaults, not safe fallback results

The trap set is an inventory item with `use_silent` action behavior. Seven
active-object virtual defaults share the one-byte target trap record at
0x112e20: deactivate, transform, tick, readiness, both model hooks, and
sprinting. Preserve the existing unreachable representation for the first two
and use it for the five unsupported empty/true defaults. This is a consistent
source model, not proof of the original macro spelling or of all vtable call
paths. The earlier claim that these methods had no target symbols was false.

Two other defaults have more discriminating target operations:

| Method / target RVA | Actual operations |
|---|---|
| `activate` / 0x9ea00 | Push the two reference pointers, call the argument helper, clean eight stack bytes, then padding. |
| `selected_animations` / 0x86a30 | Push the bool and the two words of the buffer value, call the helper, clean 12 bytes, then padding. |

Restore direct unguarded argument use followed by the unreachable macro.
Neither method has a returning epilogue; selected animations produces no
expression result. Do not use the plural guarded macro, whose guard was absent
from the inspected target.

This also resolves the preceding inventory-hook extent question: raw bytes at
0x869f0+0x36 and 0x9b250+0x13 are `cc` padding, not clipped register restores or
returns. Their direct argument-use bodies now end in the same non-returning
source model. The state/object register and review TODO were corrected, not
left asserting an unresolved epilogue cutoff.

The broader shared-trap warning still applies: 29 different methods share the
0x112e20 record, including an SDK method with a substantive source body. Do not
generalize this class-level model into proof of every alias's original source.

## Every trap-set inline

| Function | Disposition |
|---|---|
| `config_params::config_params` | Retain all-zero fields. Raw constructor operands at RVA 0xb23a0+0xa/+0x19 both read RVA 0x964044: bytes 00000000, float 0.0. The disassembler's generic `offset` label does not identify the value. |
| `on_trap_fired` | Target `ret 4`; empty default confirmed. |
| `on_trap_disarmed` | Target `ret 4`; empty default confirmed. |
| `damage_parameters` | Retain const reference to owned damage vector. |
| `config` | Retain const configuration reference. |
| `activate` | Restore two reference-pointer argument uses, then unreachable source form. |
| `deactivate` | Retain unreachable model for shared trap record. |
| `transform` | Retain non-value-producing unreachable model; do not return a fabricated matrix. |
| `tick` | Replace empty return with the class's unreachable model. |
| `is_ready_to_be_deactivated` | Remove unsupported true result; use unreachable model. |
| `selected_animations` | Direct buffer-value/bool argument use, then unreachable form; no invented expression result. |
| `on_player_model_added` | Replace empty return with unreachable model. |
| `on_player_model_removed` | Same shared target record/model. |
| `is_sprinting` | Remove unsupported true result; use unreachable model. |
| `remove_traps` | Extract existing active-world-object removal traversal; propagate to `remove`. |
| `traps` nonconst | Target adds +0x118 to this; retain vector reference. |
| `traps` const | Retain corresponding const reference and distinct interface. |
| `count_active_traps` | Explicit existing-predicate count model; original consumer open. |
| `append_inactive_trap_index_to_packet` | Explicit inactive-index filter model; packet contract open. |

## Every individual-trap inline

| Function | Disposition |
|---|---|
| `load_collision` | Extract existing collision configuration operations and propagate to loader; boundary check deferred. |
| `transform` | Retain const matrix reference; used instead of private-member friendship in derived rendering. |
| `set_owner` | Retain non-owning owner pointer store. |
| `is_active` | Retain state-not-removed predicate; used by placement/removal and retained active predicate. |
| `get_speed` | Retail `fldz; ret`; retain zero float default. |

All 24 methods are accounted for. Count/filter models, unreachable original
spelling/vtable use, and extracted caller partitions remain explicit questions.
Patch checks and evidence review are complete; byte/structure verification is
deferred with the rest of the reconstruction campaign.
