<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Stamina inline and subscriber reconstruction

Scope: all 12 `player_stamina` inlines, both subscriber constructors and the
depletion predicate's implicit inline call operator: 15 declarations. No
build or score refresh ran. The serializer and supplied-callback constructor
are models; their remaining distinctions are not marked verified.

## Missing subscriber initialization is visible in a live constructor

Target `player_logic_sprint_state` construction at RVA 0x6e9510 performs:

1. Base-state construction and the two ordinary callback members.
2. Subscriber construction at state +0x68: push null into the callback
   constructor, then write null to subscriber +0x20 (function +0x4c).
3. The existing body assignment of `bind(on_stamina_depleted, this)` to that
   callback, beginning at function +0x53.

The independently fetched frozen candidate default-constructs the callback
without the explicit null argument and never initializes `next`. Its body
assignment begins at +0x47 instead. Restore `subscription_callback(NULL)` and
`next(NULL)` in the default subscriber constructor. The pinned Boost header
supports the null `clear_type*` overload; no vendor code is changed.

The supplied-callback overload retains its callback copy and initializes its
own link to null as the corresponding fresh-node model. No consumer for that
overload was found, so that link policy still has a source TODO. Do not move
the sprint state's bind into a member initializer: retail explicitly performs
the empty callback construction first and assigns the bind in the body.

The previous constructor comment attributed the whole difference to Boost
assignment inlining. That misses the earlier source initialization defect.
Remove the blanket wall claim; the remaining assignment expansion is a
deferred verification question, not an excuse to omit member initialization.

The subscriber is embedded, registered on sprint initialization, and erased
on finalization. Depletion dispatch uses the existing locked intrusive-list
`for_each` specialization. Target iteration saves `next` at +0x20 before
calling the callback at +0 and then advances through that saved pointer.
Keep the predicate as callback invocation, without deleting the subscriber,
clearing its link, or adding an empty-callback guard absent from the target.

## Serializer: observed field order, modeled clock conversion

Retail `player_stamina::deserialize`, RVA 0x59b030, reads exactly:

| Wire order | Type | Destination |
|---:|---|---|
| 1 | float, 4 bytes | current value at +0x34 |
| 2 | u32, 4 bytes | last-spending timestamp at +0x54 |
| 3 | u32, 4 bytes | last-tick timestamp at +0x58 |
| 4 | bool, 1 byte | lower-threshold flag at +0x5c |

The target `r<bool>` directly loads one byte and increments its pointer once.
The model therefore writes this 13-byte payload, without configuration,
subscriber pointers, padding or a fabricated framing header.

The second signed parameter is modeled as `client_offset`. That is not merely
a name guess: the retained `body_part_parameters::serialize(packet, s32
client_offset)` at RVA 0x5871f0, statement 2, subtracts the offset from a
nonzero timestamp and preserves zero. Other retained serializers likewise
subtract client offsets from event times. Stamina's reset/first-tick paths
use zero timestamps as sentinels, so the model preserves zero for both clock
fields and subtracts the offset otherwise, with existing unsigned wraparound.

No retained stamina writer or current source caller establishes that **both**
timestamps used this guarded policy rather than unconditional subtraction,
or that its producer adds no surrounding context. Those remain explicit
questions. The body is a reconstruction model, not a byte-verified writer or
a reason to fabricate an outbound call just to make it observable.

Twelve model-only packing cases check the 13-byte layout, zero sentinels,
positive/negative offsets and unsigned wraparound. They do not compile or
test the C++ implementation against a retail writer.

## Every stamina inline (12)

| Function | Disposition and evidence |
|---|---|
| `serialize` | Four-field/clock-offset model above; zero-sentinel and producer distinctions remain open. |
| `set_max_value` | Retain direct +0x30 assignment model. Do not silently recompute current stamina as a setter side effect; load/reset perform that separately. |
| `set_spending_speed` | Retain +0x40 assignment model; sprint multiplies it by the separate spending factor and elapsed time. |
| `get_regeneration_speed` | Retain +0x44 getter, not the factor-adjusted value. Target medkit activation work adds its bonus; deactivation work subtracts it before calling the existing setter. |
| `set_regeneration_threshold` | Retain direct +0x38 threshold model, distinct from the spending threshold at +0x3c. |
| `set_max_value_factor` | Retain +0x48 multiplier assignment model; no invented current-value clamp or reset. |
| `set_spending_speed_factor` | Retain +0x4c multiplier assignment model, distinct from regeneration factor at +0x50. |
| `current_value` | Retain +0x34 getter; the target jump predicate reads it before comparing jump cost. |
| `max_value` | Retain raw +0x30 getter model. The effective max also uses +0x48; do not quietly change this API to return that product. |
| `amount_to_jump` | Retain max-value * max-factor / 5.0 in that order. Both target jump eligibility and jump-event spending expand those operations; do not replace division with multiplication by 0.2 or consume stamina inside the getter. |
| `get_max_carried_weight` | Retain +0x60 getter. Target parameter-modifier apply reads it, adds the configured bonus, then stores through the setter. |
| `set_max_carried_weight` | Retain the corresponding +0x60 store without changing stamina itself. |

The pure field setters without a current external caller remain field models,
not separately proven retail expansions. Their existing bodies are accounted
for rather than ignored because they lack no-source markers.

## Every subscriber/dispatch inline (3)

| Function | Disposition |
|---|---|
| Subscriber supplied-callback constructor | Retain callback copy; add fresh null link as a model pending an original consumer. |
| Subscriber default constructor | Restore explicit null callback construction and null link from the live sprint-state expansion. |
| `stamina_depletion_predicate::operator()` | Retain callback invocation; observed inside the retained list traversal specialization. No ownership or extra guard added. |

Both raw class variants match the frozen candidate, including access and
layout. Stamina's existing constructor constants were also read directly
from the PE: all three factors are 1.0 and maximum carried weight starts at
0.0, matching source. No blanket float-value replacement was applied.

Patch and declaration-count checks pass. No C++/tool builds, generated
artifacts, ledger rows or README scores were changed. Deferred checks must
verify default subscriber codegen and the remaining Boost assignment boundary;
the unobserved writer and supplied-callback overload still need original
consumer evidence beyond a new candidate build.
