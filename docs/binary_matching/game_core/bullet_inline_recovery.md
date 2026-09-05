<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Bullet manager, callback pool and bullet inlines

Scope: nine manager inlines; seven explicit callback-pool methods and its
implicit destructor; the callback record's generated constructor/destructor;
two redundant-bullet predicate methods; seven bullet accessors:
**28 declarations**. Related retained producer/cleanup operations are checked
as consumers, not counted again. No builds or score refresh.

## Real producer bug: normal was copied over direction

Retail `bullet_manager::play_particle`, RVA `0x5916e0`, copies the supplied
direction to callback +0x2c and normalizes it. At +0xb5 it separately copies
the supplied normal to **+0x38**, then normalizes that normal. Independent
base-only inspection shows our second copy instead targets +0x2c: direction
is overwritten and the subsequent normal normalization reads uninitialized
pool contents. Correct that one destination.

The frozen two-sided structure verdict is already MATCH: ten statements and
0x29f bytes on each side. The wrong field offset changes neither statement
count nor size. This is a data/semantic correction, not an inferred inline
boundary. Target PDB also records `bullet_functor* const functor` in this
producer, `add_decal` and `update_tracer`; restore all three pointer const
qualifiers. Leave the separate unemitted play_sound local unchanged rather
than claiming the three records prove its original qualification.

## Restore the declared tracer and cleanup boundaries

`emit_bullet`, `0x591ba0 + 0xeb`, synchronously dispatches engine vtable +0x10
with the newly created bullet after its tracer/engine guards. `free_bullet`,
`0x5913f0 + 0x21`, synchronously dispatches +0x14 after checking engine and
bullet tracer index != 0xffff. Implement the declared private bool
`attach_tracer_impl`/`detach_tracer_impl` as direct forwarding, and use them
at those two existing calls. Do not add callback allocation, move the guards,
rewrite tracer ownership, or descend into render. The concrete game-world
overrides already own assigning/releasing the tracer association.

The call operations and arguments are established. Their exact original
helper boundary and returned-bool policy are inferred: these callers discard
the result, though both manager declarations and engine interface are bool.
No standalone helper procedure is needed to establish that the operation is
live; absence of one does not justify the old empty bodies.

`destroy_redundant_bullets` receives the complete remove-if/erase expression
from tick's final statement, `0x591d80 + 0x19e`, and is called at that same
point after child tasks and queued callbacks. Its definition stays in the
implementation file where the existing predicate is complete; the private
declaration is visible in the header. No new predicate or replacement
algorithm is invented. The predicate's retained operator constructs a zero
vector, compares **start velocity** (+0x24), returns false if nonzero,
otherwise calls free_bullet and returns true. Preserve that exact criterion.

This cleanup source boundary remains inferred. Frozen tick already has an
identical 17-statement/0x212-byte structure; the deferred build must preserve
its partition, not claim a percentage gain from old artifacts.

## Callback pool: typed/raw forwarding and the missing assertion use

Retained allocator `malloc_impl`, `0xae5c0`, has two statements: the size
assertion, then pop/result/assertion-use/return. At +0x2f it calls try_pop,
saves the pointer, materializes a false assertion-use bool and invokes the
helper before returning that pointer. The frozen candidate has no separate
malloc_impl procedure: its new_helper expansion performs the size assertion
and try_pop, then returns immediately. It lacks the post-pop operation.

Restore typed `allocate` as pop, assertion use and return, then have
`malloc_impl` delegate to it. The assertion's original expression and macro
spelling remain open; `UNKNOWN_EXPRESSION_T(result)` is an explicit model,
not proof of the original exhaustion policy. The placement in allocate is a
helper-boundary inference guided by the raw/typed API, not a named local
recovered from malloc_impl's PDB record (which has none).

The reverse path supplies stronger call evidence. Retained generic deletion
releases callback-record resource +0x4c and clears its boost function, then
passes a temporary typed pointer by reference to the retained `deallocate`
at `0xae530`. That method pushes the raw slot on the free stack and nulls
the reference; it does not run the destructor itself. Implement free_impl
through that declared deallocate, leaving object lifetime to the existing
VOSTOK_NEW_IMPL/VOSTOK_DELETE_IMPL wrappers. Do not turn typed allocate into
an object constructor or double-destroy in deallocate.

Constructor `0xae450` initializes the empty stack and buffer pointer, asserts,
then pushes each 0x58-byte raw slot. Its loop locals i/e are mutable. Use the
direct raw-slot push there; passing iterator i itself to deallocate would
clear the iterator. Swap is independently expanded in memory replacement as
`std::swap` of stack then buffer pointer. The unused buffer getter remains
the existing +8 field model. Implicit allocator destruction owns no buffer;
the enclosing unmanaged allocation resource owns it.

Callback-record constructor `0xae670` constructs the boost function, three
float3 members and resource pointer. Keep generated construction/destruction,
without adding zero initialization to size/next/face or the vector contents.
The record's resource/function destructor operations are verified in generic
deletion above.

## Decal-clear model and accessor coverage

There is no current source caller or retained procedure for clear_decals.
Its model guards engine, then removes every ring slot from zero to
max_bullets_decals_count-1. Retained decal insertion increments current ID and
wraps it at the maximum: removing only IDs below the current cursor would
miss slots after wrap. Whether clear originally resets the cursor, uses a
different guard, or coordinates queued work remains unknown. The model
preserves the cursor and is not inserted into a new live lifecycle.

Existing manager getters agree with retained bullet consumers: material
manager +0x5c, physics world +0x60, gravity reference +8. The time-factor
getter really uses **global s_bm_bullet_time_factor**, observed in bullet
tick's absolute load, not the similarly named manager member at +0x74.
The engine predicate agrees with retained guard +0x58.

Bullet's start-velocity getter is expanded in the redundant predicate.
Last-hit getter/setter read/write +0x44 in retained damage-model hit_body_part
(`0x6efaf0`). The start-position/current-position/current-velocity/initiator
getters remain existing field models with original consumers not found.
Include the existing hit_initiator declaration for the inline id member
access; do not depend on a lucky PCH include order or invent a new id field.

## Preserve two target oddities; do not silently repair retail

- Tick stores granularity 0x100, but its spawned range endpoints are shifted
  by four bits (16-element spans). Both operations are in retail. This pass
  does not replace those multipliers based on a sensible-looking intention.
- Nonempty memory replacement repeatedly reads bullet-vector element zero,
  copies it into the replacement list, deletes the old bullet and branches
  back without erasing that element. The retained bullet destructor does not
  unlink it. This hazardous path is preserved, with its supported lifetime/
  reallocation preconditions left open, not classified as a candidate-only
  bug or “fixed” by inventing an erase.

Frozen raw topology reports identical manager, allocator, bullet and predicate
records. The later [raw-base audit](../pdb_base_access_rendering.md) resolves
the allocator's apparent public/private discrepancy: both actual PDBs encode
private inheritance. The generator hardcodes public; keep the private source.

## Every declaration

| # | Declaration | Disposition |
|---:|---|---|
| 1 | `bullet_manager::has_engine` | Existing pointer predicate agrees with live guards. |
| 2 | `clear_decals` | New full-ring removal model; original guard/reset/consumer open. |
| 3 | `get_material_manager` | Existing reference getter verified in bullet material lookup. |
| 4 | `get_physics_world` | Existing +0x60 getter verified in collision query. |
| 5 | `get_gravity` | Existing +8 reference verified in bullet tick. |
| 6 | `get_bullet_time_factor` | Existing global read verified; do not replace with member. |
| 7 | `attach_tracer_impl` | Restore/propagate engine forwarding; original boundary/result policy inferred. |
| 8 | `detach_tracer_impl` | Restore/propagate independent detach slot; preserve caller guards. |
| 9 | `destroy_redundant_bullets` | Restore/propagate retained remove/erase sequence; boundary inferred. |
| 10 | Allocator constructor | Raw-slot stack initialization/push loop verified. |
| 11 | Allocator `buffer` | Existing +8 getter retained; no original consumer identified. |
| 12 | `allocate` | Restore post-pop assertion use; expression/local/boundary model open. |
| 13 | `deallocate` | Retained raw push plus caller-pointer clear verified. |
| 14 | `swap` | Existing stack-then-buffer swaps verified in memory replacement. |
| 15 | `malloc_impl` | Size assertion then typed allocation, reproducing missing post-pop operation. |
| 16 | `free_impl` | Restore raw-to-typed forwarding seen in generic deletion. |
| 17 | Implicit allocator destructor | Preserve non-owning member/base destruction; no raw buffer free. |
| 18 | Implicit callback-record constructor | Retained member construction verified; no invented scalar initialization. |
| 19 | Implicit callback-record destructor | Resource/function destruction verified before slot recycling. |
| 20 | Redundant-predicate constructor | Existing manager-pointer capture verified in tick. |
| 21 | Redundant-predicate operator | Existing zero-start-velocity/free/return sequence verified. |
| 22 | `bullet::get_start_position` | Existing +0x18 reference model; original consumer open. |
| 23 | `get_start_velocity` | Existing +0x24 reference verified in predicate. |
| 24 | `get_position` | Existing +0 reference model; original consumer open. |
| 25 | `get_velocity` | Existing +0xc reference model; original consumer open. |
| 26 | `get_initiator_id` | Existing initiator/id field model; original consumer open. |
| 27 | Last-hit getter | Existing +0x44 read verified in damage consumer. |
| 28 | Last-hit setter | Existing +0x44 write independently verified in damage consumer. |

Deferred checks: corrected normal destination, three producer const locals,
allocator assertion and generic new/delete partitions, tracer caller guards
and cleanup helper inlining. No fresh byte score or compile result is claimed.
