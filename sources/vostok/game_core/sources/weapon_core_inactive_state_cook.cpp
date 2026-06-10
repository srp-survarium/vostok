////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_inactive_state_cook.h>

#include <vostok/game_core/weapon_core_inactive_state.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include "game_core_memory.h"

namespace survarium {

// STATE[79.07%|PARTIAL]: structure matches (unmanaged_cook ctor + register_cook(this); both thread
// ids = use_current_thread_id, flags 0). Two residuals, both unsteerable from this file:
// (a) class id - target passes 0x12C but our resources_classes.h enum gives weapon_inactive_state_class
//     = 0x103 (the engine enum is missing ~41 entries before it); fixing the enum is engine-wide.
// (b) the LTCG register calling convention of the unmanaged_cook ctor (target ecx=class, eax/edi=-3,
//     push 0 vs base stack pushes). Inline-vs-call/convention residual. See md.
weapon_core_inactive_state_cook::weapon_core_inactive_state_cook( ) :
	resources::unmanaged_cook( resources::weapon_inactive_state_class, reuse_false, use_current_thread_id, use_current_thread_id )
{
	resources::register_cook( this );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts - no diverging rows
	// VERDICT: STRUCTURE MATCH - residuals: target ctor passes class id 0x12C in ecx (our enum gives 0x103, engine-wide enum gap) and the unmanaged_cook ctor's LTCG register convention (edi/eax=-3) vs our stack pushes; both unsteerable from this file.
}

// STATE[100%|DONE]
weapon_core_inactive_state_cook::~weapon_core_inactive_state_cook( )
{
}

// STATE[87.45%|PARTIAL]: statement 28 (the ASSERT) is now byte-identical. It is the U-form
// `ASSERT_T_U( in_query, raw_file_data, file_exist )`: under NDEBUG/MASTER_GOLD this expands to
// `if(identity(false)){ expression_eater(in_query, raw_file_data, file_exist); }` - the first
// identity(false) gives the `bool x=false; check; test; je` guard, and BEHIND it the body copies
// in_query BY VALUE (96h dwords = 0x258 bytes) plus raw_file_data + file_exist and calls the eater
// (`add esp,264h`). RESIDUAL (statement 29, the malloc+return): the target inlines the
// mutable_buffer(pvoid,u32) ctor as two field stores into the sret (with an extra [ebp-8] temp,
// sub esp,0Ch) while the base COMDAT-folds it to an out-of-line uint2::uint2 call (sub esp,8,
// extra `push 138h`). Inline-depth/COMDAT-fold wall - same residual class as the shotgun cook
// allocate and weapon_core_cook. See md + weapon_core_shotgun_reload_state_cook.md.
mutable_buffer weapon_core_inactive_state_cook::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	ASSERT_T_U( in_query, raw_file_data, file_exist );
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( weapon_core_inactive_state ), "weapon_core_inactive_state" ), sizeof( weapon_core_inactive_state ) );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE -0x6 | 50 | return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( weapon_core_inactive_state ), "weapon_core_inactive_state" ), sizeof( weapon_core_inactive_state ) );
	// VERDICT: STRUCTURE MATCH (2/2) - the ASSERT_T_U eater is byte-identical; sole SIZE is the mutable_buffer(pvoid,u32) ctor inlined as field stores in target vs COMDAT-folded out-of-line uint2::uint2 in base, inline-depth wall, non-steerable.
}

// STATE[55.64%|PARTIAL]: byte-identical to weapon_core_shotgun_reload_state_cook::deallocate_resource.
// Allocator is the g_allocator POINTER (no deref); the reference-cast gives free_helper(A&,T*&) with
// T=resource_base, lea edi,[ebp+8]=&buffer. RESIDUAL: target calls free_helper OUT-OF-LINE; base
// INLINES the wrapper (inlines the null check + calls free_helper_impl, spilling allocator to an extra
// temp). Inline-depth divergence; same class as weapon_core_cook::delete_resource. See md.
void weapon_core_inactive_state_cook::deallocate_resource( void* buffer )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0xf | 67 | VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );
	// VERDICT: STRUCTURE MATCH (1/1) - target calls free_helper OUT-OF-LINE, base inlines the wrapper (null check + free_helper_impl call + allocator spill), inline-depth wall, non-steerable.
}

// STATE[91.97%|PARTIAL]: base asm (0x457500) is instruction-for-instruction identical to target
// 0x58efe0 (placement-new weapon_core_inactive_state(params->weapon) into the buffer, the two-vtable
// init, the null-checked downcast, set_unmanaged_resource(memory_usage_type(nocache,0x138)), and
// finish_query(result_success) with `push 1; push 3`). Sole residual is the inline-fold of the
// memory_usage_type ctor (base folds to uint2::uint2 vs the target's const_buffer-named COMDAT) +
// the operator c_ptr COMDAT - the documented inline-vs-call fold class. See md.
void weapon_core_inactive_state_cook::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );

	weapon_core_inactive_state*	object_to_cook	= new ( in_out_unmanaged_resource_buffer.c_ptr( ) ) weapon_core_inactive_state( params->weapon );

	parent.set_unmanaged_resource(
		resources::unmanaged_resource_ptr( object_to_cook ),
		resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_core_inactive_state ) )
	);
	parent.finish_query( resources::cook_base::result_success );

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x5 | 82 | weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );
	// SIZE -0xa | 84 | weapon_core_inactive_state*	object_to_cook	= new ( in_out_unmanaged_resource_buffer.c_ptr( ) ) weapon_core_inactive_state( params->weapon );
	// VERDICT: STRUCTURE MATCH (4/4) - L82: target FOLDS const_buffer::c_ptr() to a direct member read (mov eax,[ebp+0Ch]) while base keeps the accessor call; L84: the inverse inline-depth on the placement-new path (mutable_buffer c_ptr/ctor fold). Per-site LTCG inline-vs-call, non-steerable (NOT a static_cast_checked site - target has no temps, it is SHORTER).
}

// STATE[100%|DONE]
void weapon_core_inactive_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	weapon_core_inactive_state*	wpn_state	= static_cast< weapon_core_inactive_state* >( resource );
	wpn_state->~weapon_core_inactive_state( );
}

} // namespace survarium
