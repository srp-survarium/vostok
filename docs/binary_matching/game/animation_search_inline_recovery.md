<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Animation-space search inlines and their A* consumers

PR 571 source-only audit covers **70 declaration/consumer anchors**: graph
wrapper 10, heuristics 12, restrictor 7, search-service/nested operations 20,
graph inlines/generated operations 11, and vertex/edge/comparison/hash
operations 10. Counts include existing bodies, generated methods and the
explicitly unresolved passability body; they are not new-implementation or
byte-match counts. No build or score refresh.

The substantive change is the failed-search fallback: retail keeps the
heuristic's best ID on both branches; the reconstruction selected the start
ID on the false branch. Restore the existing queue-to-manager forwarding
seam and retain the original source/statement boundary as an open question.
Twelve stale literal markers disappear after checking actual consumers.
The independent is_passable helper remains unresolved, not editor/server-only.

## Individual register

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `animation_space_graph_wrapper(graph&, start&)` | Initializer-only body verified in search: borrow the graph handle and start ID by reference. Remove stale marker; no handle copy/refcount or reset. |
| 2 | Wrapper `on_before_search()` | A* invokes the hook, but retail's complete pre-search expansion contains no wrapper work. Preserve empty body; do not confuse it with the queue/manager reset. |
| 3 | Wrapper `on_after_search(bool)` | Used through A* on every outcome; retail find returns directly without wrapper cleanup. Empty shipped behavior, not absence of a consumer. |
| 4 | Wrapper `edge_iterators<VertexType>(...)` | Retail step starts at zero and ends at graph edge count +0x11c. Preserve input-independent enumeration of all graph edges. |
| 5 | Wrapper `vertex_id(...)` | Verify retained transform composition: current rotation matrix plus translation, edge translation transformed by it, edge rotation multiplied by current rotation. Preserve the existing body; its frozen statement projection is not closed. |
| 6 | Wrapper `edge_id(...)` | Identity on the iterator; process_neighbour stores that u32 as the parent edge. No vertex-dependent remapping. |
| 7 | Wrapper generated copy constructor | Raw 0x0103; leave implicit and preserve private noncopyable base/borrowed references. No fabricated copying consumer. |
| 8 | Wrapper generated destructor | Compiler-owned; references are not owned graph/vertex objects and do not release them. |
| 9 | Wrapper generated assignment | Keep compiler-owned, including reference/noncopyable restrictions. |
| 10 | Wrapper `__vecDelDtor` | Generated nonvirtual deletion helper; no explicit source method. |
| 11 | `animation_space_heuristics(graph&, target&, max_speed)` | Retained constructor initializes borrowed references, positive infinity minimum, supplied max speed, identity best rotation and zero translation. No start-ID initializer. |
| 12 | Heuristics `on_before_search()` | Actual A* hook emits no reset; constructor owns initial minimum/best setup. Do not reset m_min_heuristics_value at every invocation based on the name. |
| 13 | Heuristics `on_after_search(bool)` | Used but empty in the retained find outcome paths. No best-ID clearing before failed-search fallback reads it. |
| 14 | Heuristics `on_start_iteration(vertex_id)` | Used by step; no callback work between edge bounds and neighbor iteration in retail. Preserve by-value const parameter and empty body. |
| 15 | Heuristics `estimate(current*, neighbour&) const` | Retail computes translation distance/max_speed, updates minimum and best ID only on strict improvement, returns distance. Current pointer and rotation do not enter the estimate. |
| 16 | Heuristics `evaluate<VertexType>(...) const` | Both new/existing-neighbor branches add current g to edge.animation_length (+0x24); no neighbor g or geometric length term. |
| 17 | Heuristics `metric_euclidian()` | Preserve true. Retail process_neighbour immediately rejects closed vertices; no reopen/update-successors branch remains. |
| 18 | Heuristics `best_vertex_id() const` | Const reference to embedded +8 ID, used by failed-search visited/lookup. Remove stale marker; no temporary or value copy. |
| 19 | Heuristics generated copy constructor | Raw 0x0103; no manual copy bypassing private noncopyable base. |
| 20 | Heuristics generated destructor | Compiler-owned; no owned graph handle, target ID or heap storage. |
| 21 | Heuristics generated assignment | Preserve generated constraints from references, const speed and base. |
| 22 | Heuristics `__vecDelDtor` | Generated nonvirtual deletion machinery; leave implicit. |
| 23 | `animation_space_search_restrictor(...)` | Verify generic base constructor's range/iteration/visited/start/target mapping and borrowed graph reference. Remove marker and stale request to verify those stores. |
| 24 | Restrictor `accessible<VertexType>(...) const` | Preserve true: retail goes directly from vertex_id construction to visited lookup, without inspecting navigation world or radius. Do not wire the unrelated unresolved is_passable into this live path. |
| 25 | Restrictor `get_start_vertices_count()` | Preserve one; retained pre-search constructs precisely one start vertex and does not loop over multiple roots. |
| 26 | Restrictor generated copy constructor | Raw 0x0103; leave implicit with inherited/reference restrictions. |
| 27 | Restrictor generated destructor | No graph ownership or navigation cleanup; preserve compiler-owned body. |
| 28 | Restrictor generated assignment | Do not add custom reference rebinding/copy policy. |
| 29 | Restrictor `__vecDelDtor` | Generated nonvirtual helper, not a source declaration. |
| 30 | `animations_search_service()` | Verify 1024 vertices, 4096 hash buckets, 1024 lookup cells and 1024 queue entries through retained allocations. Existing initializer-only source is correct. |
| 31 | Search-service `search(...)` | Restore best-ID lookup in both failed-search branches, directly through m_vertex_manager or the queue's existing vertex forwarder. Keep visited evaluation, construction order, budgets and always-true final result. Original source boundary and frozen 8-vs-5 statement projection stay open. |
| 32 | Search-service generated copy constructor | Raw 0x0103; preserve implicit generation/member restrictions, not a new owning copy implementation. |
| 33 | Search-service generated destructor | Retained 0x9d150 frees queue, hash table, lookup-cell storage and vertices in reverse ownership order. Do not replace it with manual destructor source. |
| 34 | Search-service generated assignment | No hand-written shallow copy or new deleted-copy declaration. |
| 35 | Search-service `__vecDelDtor` | Generated nonvirtual machinery, not a reason to add virtuality. |
| 36 | Nested `vertex_allocator_impl_type(allocator&, count)` | Forward allocator address and count to its public base. Parent constructor's 0xc000-byte allocation proves 1024 × 0x30 vertex storage. Remove stale marker. |
| 37 | Vertex-allocator adapter generated destructor | Base frees vertex storage; preserve automatic cleanup. |
| 38 | Vertex-allocator adapter `__vecDelDtor` | Raw generated helper. No copy/assignment records in this adapter are invented or counted. |
| 39 | Nested `vertex_manager_impl_type(allocator&, hash_count, cell_count)` | Forward all three arguments. Retained base constructor records 0x1000 buckets/0x400 cells and allocates/zeros 0x4000/0x6000 bytes. Remove stale marker. |
| 40 | Vertex-manager adapter generated copy constructor | Preserve implicit base behavior/restrictions; not an implemented owning duplication policy. |
| 41 | Vertex-manager adapter generated destructor | Base destroys its two buffers; no separate adapter cleanup. |
| 42 | Vertex-manager adapter generated assignment | Compiler-owned; no extra field or reallocation routine. |
| 43 | Vertex-manager adapter `__vecDelDtor` | Generated nonvirtual helper. |
| 44 | Nested `priority_queue_impl_type(manager&, count)` | Forward manager reference and count. Parent constructor stores manager pointer at +0x2c and allocates/zeros 0x1000 bytes for 1024 pointers. Remove stale marker. |
| 45 | Priority-queue adapter generated copy constructor | Preserve implicit metadata/restrictions; no custom queue copy. |
| 46 | Priority-queue adapter generated destructor | Base frees heap backing storage through manager's allocator. |
| 47 | Priority-queue adapter generated assignment | Do not invent a safe/deep-copy policy absent from retail. |
| 48 | Priority-queue adapter `__vecDelDtor` | Generated nonvirtual helper. |
| 49 | Nested `vertex_type` generated default constructor | Raw no-argument generated method; leave compiler-owned. Retained allocation does not zero or individually initialize the 0xc000-byte vertex buffer; queue/search initializes relevant fields later. |
| 50 | Graph `get_animations_count()` | Preserve const u32 +0x114 getter, consumed in cook allocation/destruction and graph array address calculation. |
| 51 | Graph `get_mixes_count()` | Preserve const u32 +0x118 getter and mix/edge array boundary. |
| 52 | Graph `get_edges_count()` | Preserve const u32 +0x11c getter; all-edge iteration and search iteration budget consume it. |
| 53 | Graph `get_animations()` | Preserve trailing array at this+0x120, element stride 0x124. Not a separately allocated pointer field. |
| 54 | Graph `get_mixes()` | Preserve following pair array at this+0x120+0x124*animation_count, pair stride eight. |
| 55 | Graph `get_edges()` | Preserve following 0x28-byte edge array after eight bytes per mix. Retained edge() and wrapper/evaluate expansions independently agree. |
| 56 | Graph `agent_radius()` | Getter at +0x10c appears in selected_animations' navigation find_path call. Remove stale marker; do not substitute the fixed global even though the observed graph constructor uses 0.35f. |
| 57 | Graph `is_passable(from&, to&) const` | Still open. Hypothesis: map start translation to a nav node, advance to destination, reject invalid node(s), and test destination clearance with agent_radius. Exact swept/endpoint checks, guard policy and original consumer remain unknown; false is still a placeholder, not retail behavior. |
| 58 | Graph generated copy constructor | Raw 0x0103; preserve private noncopyable second base and const counts. No trailing-array copy invented. |
| 59 | Graph generated assignment | Compiler-owned restrictions; not an unimplemented custom graph copy. |
| 60 | Graph `__vecDelDtor` | Generated introducing-virtual 0x0113. Explicit virtual destructor remains separate and does not free the trailing allocation by itself. |
| 61 | `animation_space_vertex_id` generated default constructor | Raw no-argument 0x0103 record. Preserve aggregate fields and compiler construction; do not add identity/zero initialization globally. Heuristics explicitly performs its own initialization. |
| 62 | `animation_space_edge(...)` | Retained generate_graph_edges copies all 28 movement bytes, mix pointer, first weight and derived animation length into +0/+0x1c/+0x20/+0x24. Preserve existing initializer-only constructor. |
| 63 | `animation_space_vertex(resource&, caption)` | Verify owned managed handle, copied virtual path, clip frame duration divided by 30, and u32(-1) group/interval sentinels. Existing explicit constructor is not a missing body. |
| 64 | Animation vertex generated copy constructor | Preserve memberwise resource/path copying and scalars; no new custom source. |
| 65 | Animation vertex generated destructor | Cook's retained loop destroys the managed handle at each 0x124 stride. Do not independently free each trailing vertex. |
| 66 | Animation vertex generated assignment | Compiler-owned resource/path/scalar assignment. |
| 67 | Animation vertex `__vecDelDtor` | Generated nonvirtual helper; leave implicit. |
| 68 | Vertex-ID `operator!=` | Retained 0x9d210 compares quaternion components exactly and translation with 0.3 tolerance, then negates. Keep existing logic despite standalone base being inlined away. |
| 69 | Vertex-ID `operator==` | Same positive comparison appears in A* target_reached expansion. Preserve exact rotation/componentwise strict translation tolerance, including unordered-float behavior. |
| 70 | `ai::vertex_manager::hash_value(vertex_id const&)` | Retained visited/vertex process all 0x1c raw bytes through Boost CRC32 and use checksum modulo hash count. Do not quantize coordinates to repair the mismatch with tolerant equality. |

Empty wrapper vertex_impl/look_up_cell_impl and service look_up_cell_type
have no method records to count. Other retained graph/cook/controller and
shared A* methods supply evidence here, not extra count inflation.

## Failed-search fallback: key versus manager selection

Retail search is RVA 0x7a62a0. With the post-find stack fixed, the heuristic
starts at [esp+0x34]; its best ID is [esp+0x3c]. Start ID is [esp+0x68].
The fallback does the following, confirmed against raw PE bytes:

| Offset | Operation |
|---|---|
| +0xa0/+0xa4 | Address best ID and direct manager (this+0x10). |
| +0xa8 | Call visited with that ID. |
| +0xad/+0xaf | Test result and branch. |
| +0xb1 | True: push direct manager pointer. |
| +0xb4/+0xb6 | False: load manager reference from queue at this+0x2c, then push it. |
| +0xb7 | Both paths: address [esp+0x40], the same best ID adjusted for the push. |
| +0xbb | Call vertex; its actual convention takes key in EDI, manager on stack. |
| +0xc0/+0xc4 | Pass returned vertex to construct_path. |

There is **no start-ID address selection** in either branch. Frozen base
instead has a false-branch lea of the start ID before vertex. This is a
real operand/behavior difference, not a harmless register rename.

The queue already declares public vertex(id) const, forwarding to its
private m_manager.vertex(id). Restoring the direct-manager/queue ternary
uses existing interfaces and reproduces the observed manager selection
without fabricating a helper or changing shared AI code. Exact original
source spelling is inferred; the observed key is not. Preserve the visited
call and the odd lookup-after-failed-visited behavior rather than making
the fallback safer. search returns true even if A* failed.

Target records eight statements/0xd5 bytes versus frozen base five/0xc8.
Its recorded locals are graph, path, graph_wrapper, path_heuristics and
search_restrictor. Current source also has a path-constructor local; this
audit does not certify that original local/source boundary or claim the
statement discrepancy is solved without a later build.

## Hooks, heuristics and restriction evidence

Read the shared A* source and full retained specializations:

- on_before_search 0x9d900, six statements/0x158 bytes: queue/manager reset,
  one start vertex, estimate, g=0, parent assignment and queue insertion.
  No extra graph or heuristic reset. Frozen structure agrees.
- find 0x9d7d0, six statements/0x77 bytes: before hook, limits, step and
  direct true/false outcomes. No graph/heuristic after-hook work survives
  on any exit. Frozen structure agrees.
- step 0x9da60, eight statements/0xcb bytes: target comparison, close best,
  zero-to-edge-count iteration. No on_start_iteration work between bounds
  and neighbor processing. Frozen structure agrees.
- process_neighbour 0x9ddc0, fourteen statements/0xfe bytes: construct
  neighbor, immediately test visited, then new/existing handling. No
  navigation lookup or passability rejection. Closed vertices return
  immediately, supporting metric_euclidian=true. Both g calculations load
  edge.animation_length and add current g. Its frozen structure still
  differs (17 statements/0x101); do not claim shared-template closure.

These are real used hooks with empty **observable shipped behavior**.
Compiled-out assertions/debug-only work cannot be reconstructed from those
empty expansions. No missing-standalone-symbol argument is needed.

Heuristics constructor 0x9d4a0 has two statements/0x9c bytes and estimate
0x9d540 four/0x8e on both frozen sides. The constructor pool operands were
read from the PE: +0x33 resolves to 1.0f at RVA 0xa6b6c4; its initial minimum
is positive infinity at 0xa4c684. Estimate ignores current pointer and
rotation, updates on strict less-than, and has no zero-max-speed guard.
Wrapper vertex_id 0x9d280's observed transform/rotation order is preserved;
its six-vs-five frozen statement discrepancy is not waved away as LTCG.

## Ownership, raw methods and constants

Complete raw TPI records / field lists / sizes:

| Class | Type / field list | Size |
|---|---|---:|
| graph wrapper | 0x48e83 / 0x48e82 | 0x8 |
| heuristics | 0x48e9d / 0x48e9c | 0x2c |
| restrictor | 0x48eaf / 0x48eae | 0x18 |
| search service | 0x48ee4 / 0x48ee3 | 0x3c |
| allocator / manager / queue adapters | 0x48eeb / 0x48eea; 0x48f12 / 0x48f11; 0x48f49 / 0x48f48 | 0x10 / 0x1c / 0x10 |
| service vertex / lookup cell | 0x48f02 / 0x48f01; 0x48f3a / 0x48f39 | 0x30 / 0x18 |
| graph | 0xa4c8 / 0xa4c7 | 0x120 |
| vertex ID / edge / animation vertex | 0xa48d / 0xa48c; 0xa4aa / 0xa4a9; 0xa4d5 / 0xa4d4 | 0x1c / 0x28 / 0x124 |

Wrapper/heuristics noncopyable bases are raw private 0x0001, despite the
generated headers rendering public. Restrictor and adapter bases are
public. Graph has public unmanaged_resource at zero and private
noncopyable at +0x108. Ordinary helper access is public; helper reference/
cache fields are private. Service's three owning members are public.
Static metric method is 0x000b and precedes best_vertex_id in raw order;
do not follow the renderer's relocated static-method display order.

The generated methods listed above have 0x0103 attributes, except graph's
introducing-virtual deletion helper 0x0113. Vertex ID and service vertex
generated constructors have zero arguments in their actual LF_MFUNCTION
records. Do not infer explicit source constructors/destructors just from
the rendered header. Target/frozen-base topology agrees for graph,
wrapper, heuristics, restrictor and service. No declaration/access/field
changes are made in this batch.

Service constructor 0x9d000 (0x55 bytes) and generated destructor 0x9d150
(0x60) have matching zero-statement frozen structures. The retained manager
constructor 0x9d0f0 has four statements/0x5e bytes. The allocator adapter
borrows an allocator and owns its vertex buffer; manager owns hash/cell
buffers; queue owns its heap buffer. Destruction frees heap, hash, cells,
vertices in that order. The graph wrapper/heuristics/restrictor borrow the
caller's handle/IDs; they do not extend resource lifetimes with handle copies.

Graph edge() at 0x7558c0 independently proves all trailing-array offsets.
Graph constructor 0x755960 initializes radius (observed 0.35f pool at
0x9305a4), max-speed cache -1 and all counts. The controller's
selected_animations 0x766bc0+0x10a loads the radius member, then passes it
to navigation find_path. No passability helper is called in this path.
Existing mutable-cache spelling question remains separately indexed.

generate_graph_edges 0x757980+0x12e copies the 28-byte movement and the three
remaining edge values. Preserve its supplied animation-length formula,
not a guessed physical distance. Animation vertex constructor 0x7559c0
pins the animation, reads frame duration, scales by 1/30, and writes both
u32(-1) sentinels. Cook delete_resource 0x757920 destroys trailing vertex
handles, invokes graph destruction without deallocation, and frees the
whole allocation. No separate vertex/edge allocation owner is introduced.

Raw CRC32 bytes and tolerant equality are intentionally different notions
here. visited 0x9d5d0 and vertex 0x9d650 process all 0x1c bytes; operator!=
0x9d210 and step compare quaternion floats exactly and translation using
0.3f from RVA 0x92d484 (9a 99 99 3e). Do not replace CRC with quantized
coordinates, exact translation equality, a Euclidean-radius test or
quaternion-sign equivalence to improve mathematical consistency.

## Still-open passability and verification boundary

is_passable has no named source caller beyond its declaration in the
current engine, and clangd's current partial reference index also returns
only that declaration. The local history search finds its carcass import,
not an earlier implementation; the parked legacy game tree has no matching
animation-space files. These checks do not prove unshipped-only use.

The stored navigation world, radius and two vertex IDs constrain a model:
get_node_id_at(start.translation), get_node_id_after_move(start node,
start.translation, destination.translation), then can_stand(destination,
node, radius), possibly with invalid-node guards. Endpoint-only clearance,
a full find_path test, and swept movement have different semantics. No
retained helper/caller chooses among them. Keep the body visibly unfinished
and do not replace accessible=true with any of these guesses.

Static checks cover declaration/access/field preservation, all 70 register
rows, two paired TODOs and literal-marker counts. They do not establish
post-edit buildability, statement matching or byte scores. Remaining scan
after this batch: 177 marked lines (game 56, game_core 52, inactive SDK
mirrors 57, physics 6, animation 5, render 1), or 120 excluding SDK mirrors.
That is not a remaining-function count; open bodies/models stay in scope.
