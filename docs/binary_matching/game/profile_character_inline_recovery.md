<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Profile-character inlines and asynchronous preview ownership

PR 571 source-only audit covers **18 declared/generated operations**: ten
ordinary methods, three generated outer-class operations and five generated
preview_weapon operations. Six ordinary methods initially had literal
placeholders. Three now have explicit models; the destructor and two query
producers remain open. The already-bodied update is also an incomplete model,
not a recovered animation/render update. No build or score refresh.

## Individual register

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `profile_character(items_dictionary&, scene_renderer&, scene_ptr const&)` | Preserve the three borrowed references and implicit construction of resource members. Add identity-matrix/false-visibility initialization as an explicit model; original transform, construction consumer and animation-query timing remain unknown. Do not add an owning scene copy or guessed bone-index sentinel. |
| 2 | `~profile_character()` | Explicit public nonvirtual destructor, not compiler-generated. Keep its literal placeholder: automatic member destruction releases handles and destroys the animation player, but does not establish the original destructor body or scene detachment. A call to clear_resources is one hypothesis, not a verified action. |
| 3 | `update(u32)` | Existing unmarked body constructs/adds two animation lexemes and discards the expression; current_time is unused. It was introduced as a COMDAT-owner reconstruction in 06be11f82. Mark original readiness guards, tick/transform/bone publication and weapon-visibility logic open. Do not mistake an emitted helper family for verification of the complete parent body. |
| 4 | `clear_resources()` | New callback-derived model: reset animation player with true, remove the character model, detach visible weapon models/addons, clear owned handles and flags, then release the two character animations and skeleton. Exact reset policy/order, calling seam and asynchronous lifetime are open. It is not a query-cancellation implementation. |
| 5 | `profile_changed(player_profile const*)` | New forwarding model to the private query_profile_contents, supported by the retained corresponding profile_player_character method. Preserve the pointer argument; original caller, null contract, eager clearing and animation-query scheduling remain open. The callee itself is still unfinished. |
| 6 | `query_profile_contents(player_profile const*)` | Open producer: choose a character/combined-skin request plus two model/animation/addon triplets from the profile and dictionary, binding this class's callbacks. Result schema is constrained; exact paths, slot/optional-item rules, user data, profile lifetime and request timing are not. Do not substitute the sibling player_class request. |
| 7 | `query_character_animations()` | Open producer: request at least the two managed clips consumed by character_animation_ready and bind that callback. Clip identities, configuration source and constructor-versus-profile-change scheduling remain unknown. The class has no stored player profile from which this parameterless method can silently select current equipment. |
| 8 | `character_model_ready(queries_result&)` | Retail RVA 0x72f8f0, eight statements, 0x1b0 bytes. Verify old-model detach/reset before the aggregate-success test, result-zero skeleton model, its skeleton, Weapon bone index minus root count, and scene insertion using m_initial_matrix. No failure guard is added around unrelated fields. |
| 9 | `character_animation_ready(queries_result&)` | Retail RVA 0x72f370, six statements, 0x246 bytes. Log failed query elements, then unconditionally assign the first two managed results. Preserve logging rather than turning failure into an early return. Remove the obsolete base-layout/structure-closure comment; the frozen base currently lacks this function. Pin its real log line argument to retail's 302 with one function-top directive. |
| 10 | `weapon_resources_ready(queries_result&)` | Retail RVA 0x72f5d0, sixteen statements, 0x31b bytes. Verify two fixed slots, visible-only scene detachment, unconditional handle/flag clearing, model-success-gated animation assignment, and independent addon success followed by scope_point lookup on the model. Keep the potentially null-model addon path and unchecked locator result. |
| 11 | `profile_character` generated copy constructor | Raw 0x0103 record, not a missing user body. Leave implicit; the private noncopyable base restricts viable copying. Do not invent a public hand-written copy that bypasses it. |
| 12 | `profile_character` generated assignment | Raw 0x0103 record; leave compiler-owned, including reference/noncopyable restrictions. No fabricated assignment declaration or implementation. |
| 13 | `profile_character::__vecDelDtor` | Compiler-generated nonvirtual deletion helper, not a source method to author or a reason to make the destructor virtual. No standalone matching claim. |
| 14 | `preview_weapon` generated default constructor | Raw 0x0103 record. Let the three resource handles and locator subobjects construct normally; the scalar visibility flag is not automatically false. The outer constructor's new false stores are a separate explicit model. |
| 15 | `preview_weapon` generated copy constructor | Preserve memberwise resource ownership and locator/flag copying. Do not define an extra body or claim an observed copy consumer. |
| 16 | `preview_weapon` generated destructor | Preserve member destruction; no scene renderer or scene is stored in this record, so it cannot implement scene removal by itself. Parent cleanup has that context. |
| 17 | `preview_weapon` generated assignment | Preserve memberwise handle/locator/flag assignment and generated metadata. No new custom ownership or deleted-copy rule. |
| 18 | `preview_weapon::__vecDelDtor` | Compiler deletion machinery for the generated destructor. Keep it implicit; do not add a source declaration. |

Already reviewed profile_player_character ownership methods are supporting
evidence, not counted again. This register includes all methods in the raw
profile_character and nested preview_weapon records, not just marked lines.

## Raw layout and source ownership

Retail profile_character is complete TPI 0x189ee, field list 0x189ed,
size 0x8690. Its noncopyable base at zero has **private** attributes 0x0001;
the generated header's public rendering is not the raw access. The mutable
renderer and dictionary references and const scene-handle reference are at
+0x8684/+0x868c/+0x8688. The scene handle remains borrowed by reference, not
an independently owned resource_ptr. No data/member/access order changes.

Constructor overload list 0x189e3 contains generated copy 0x189e0/0x0103
and explicit constructor 0x189e2/0x0003. The destructor is explicit
0x189e4/0x0003. update, clear_resources and profile_changed are public;
both query helpers and all three callbacks are private. Assignment
0x189eb and vector-deleting destructor 0x189ec are generated 0x0103.

Nested preview_weapon is complete 0x189fb, field list 0x189fa, size 0x74.
Its model/addon/animation handles are public at 0/4/8, locator at 0xc and
visibility byte at 0x70. Both constructors in overload list 0x189f5, its
destructor, assignment and vector-deleting destructor are generated 0x0103.
The outer array is at 0x8598; the two visibility bytes are 0x8608/0x867c.
Do not recreate the previously removed explicit nested constructors/dtor.
Target/frozen-base topology reports identical semantic class variants.

The active lobby m_character is **profile_player_character**, an eight-byte
wrapper, not this 0x8690-byte class. The live sibling constructor at 0x72f5c0
zeros the player handle and stores a borrowed lobby reference; its update
calls player::tick, and its query helper creates a player resource with
player_initial_info. Current on_render_scenes_ready constructs that wrapper;
profile selection and lobby tick call its methods. Do not replace that live
path with this different class merely to make callbacks pair.

## Reference checks and their limits

The retail rich-index name search finds only this class's three callbacks,
not update or its six literal-placeholder methods. The frozen base contains
none of the three callbacks, so two-sided structure requests cannot establish
a current byte or statement comparison. Historical scores/comments are not
current evidence of a layout wall.

Current source references and clangd locate the unfinished query declaration,
but not its original consumer. Direct-call xrefs for model/weapon callbacks
are empty. A separate raw-byte check scans 135949 indexed E8/E9 instruction
rows and finds no direct calls/tail jumps to any of the three callback entry
RVAs. All 170827 HIGHLOW relocation sites are checked for their absolute VAs;
none refer to these entries. Searching every raw PE section for those 32-bit
absolute VAs, including unaligned occurrences, also finds none.

These checks cover ordinary entry-point calls and stored absolute addresses,
not arbitrary computed addresses, missing index extents or fully inlined
expansions. They are not proof of editor/server-only use or no original source.
The retained bodies still provide concrete field/result/ownership constraints.
The old temp/game_legacy lobby has no profile-character preview implementation;
local history introduces this header as carcass, not a recovered query producer.

## Callback contracts constrain the unfinished producers

character_model_ready removes the existing render model before checking the
new result's aggregate success. On success it copies result zero into +0x8588,
copies that model's skeleton into +0x8594, computes the named Weapon bone's
non-root index into +0x8680, and adds the model with the matrix at offset zero.
On failure it does not reset the retained skeleton or bone index. Preserve
this asymmetry; the broader clear_resources model is not a replacement for
the callback's exact replacement policy.

weapon_resources_ready starts at outer+0x859c and iterates twice with 0x74
record stride and 0x870 query-result stride (three times 0x2d0). Its data
layout is [model, managed animation, static addon] per slot. A producer that
compresses away empty slots would break this fixed indexing. The callback
needs six result positions; original placeholder requests/failure policy are
unknown. The model's success gate controls both model and animation assignment;
addon success is independent, and its scope_point lookup dereferences the
model without another null guard. Do not improve that behavior while matching.

character_animation_ready logs failures across data.size(), then copies
managed results zero and one into +0x858c/+0x8590 even after a logged failure.
Two output fields do not establish exact clip paths or prove no extra query
results existed. Retail uses query count +0x38, 0x2d0 result stride and managed
pointer sources +0x128/+0x3f8. The current frozen base has no callback to
support the removed claim of an eight-byte candidate layout difference.

The target log appends the immediate 0x12e (302) at callback offset +0xc7.
One `#line 298` at the function top makes the existing LOG_ERROR expansion's
line argument 302 without per-statement directives or tuning whitespace.
This anchors actual logged data, not a claimed whole-function source partition.
No callback algorithm or current-time behavior is otherwise changed here.

## Models versus unresolved source

The cleanup model reuses the verified remove-before-release and
visible-only weapon-detach rules. Its broader animation-player reset(true),
release order, skeleton/clip clearing and any required asynchronous coordination
are not observed as one retail clear_resources expansion. Resource-pointer
assignment owns reference counting; no raw deletes, scene destruction or
query-cancellation behavior is invented. reset(true) is a real public API,
also used by animation_player's destructor, but that does not prove the original
clear helper selected true or called it first.

The constructor's identity matrix and initial false visibility flags are
provisional defaults, not target constants recovered from a construction site.
The retained model callback supplies the bone index later; no invented sentinel
is added. Generated handles initialize themselves. Locator bytes remain the
locator's own concern; no whole-object memset or custom nested constructor.

The live sibling profile_changed at 0x72ff10 forwards its pointer argument
to its query helper, supporting the same minimal forwarding hypothesis here.
That sibling's replacement callback clears the old player after the new query
returns; it does not justify an eager clear or new null policy in this class.
Do not invoke either unfinished query helper from the constructor merely to
fabricate reachability. The proper original scheduling site remains open.

For query_profile_contents, dictionary_item provides configuration and path
information, and the profile enum identifies weapon slots 7 and 10. The current
weapon cook shows model and preview-animation configuration accesses, but it
requests variable animation counts and a rifle_scope resource, whereas this
callback expects exactly positioned model/animation/static-addon results.
Likewise, the player's combined-skin request uses a profile pointer in user
data and a broader player ownership pipeline. Those are useful priors, not a
drop-in request list or proof of this producer's profile-copy lifetime.

For query_character_animations, recover actual managed clip identities and
their source before adding a pair of arbitrary paths. For update, the existing
addition-expression body in 06be11f82 explains a chosen template-emission
context but does not prove any original parent control flow; its real state
publication remains open. The two unfinished producers, explicit destructor
and partial update are individually indexed alongside the three new models.

## Deferred checks

No C++/tool compilation, generated artifact refresh or match claim. Static
checks cover layout/signature preservation, detach-before-release sequences,
both visibility resets, complete register/TODO indexing and the log-line value.
The later batched build must check this header's users, any legitimately
recovered callback producers, class topology and resulting statement/data
comparisons. The models do not close this class or the engine-wide goal.
