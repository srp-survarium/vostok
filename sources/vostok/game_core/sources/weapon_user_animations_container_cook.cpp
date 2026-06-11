////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_user_animations_container_cook.h>

#include <vostok/game_core/weapon_user_animations_container.h>
#include <vostok/configs_binary_config.h>

namespace survarium {

// STATE[100%|DONE]
weapon_user_animations_container_cook::weapon_user_animations_container_cook( )
	: resources::translate_query_cook( resources::animation_container_class, reuse_true, use_current_thread_id )
{
}

// STATE[33.38%|PARTIAL]: same delete_helper inline-shape wall as weapon_core_cook /
// victory_item_core_cook / weapon_ammunition_cook - target out-of-lines delete_helper
// <doug_lea_allocator,resource_base> with 2 cdecl args + `mov ecx,[g_allocator]`, base
// inlines strip_pointer and passes &resource in a register. Source is the canonical
// VOSTOK_DELETE_IMPL( g_allocator, resource ); divergence is pure LTCG inline choice.
void weapon_user_animations_container_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x3 | 26 | VOSTOK_DELETE_IMPL( g_allocator, resource );
	// VERDICT: STRUCTURE MATCH (1/1) - delete_helper inline-vs-call LTCG (target out-of-lines delete_helper, base inlines strip_pointer), non-steerable; the low % is reloc pairing on a tiny body.
}

// STATE[99.82%|DONE]: only the frame-size imm + trailing alignment nop differ
// (boost::function/bind temp materialization, same wall as booby_trap_set_core_cook).
void weapon_user_animations_container_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &weapon_user_animations_container_cook::on_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (0x15b both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is the frame-size imm + trailing alignment nop (boost::function/bind temp materialization), non-steerable.
}

// STATE[None|LTCG]: EMITTED (on_config_loaded keeps it) but reads None in report.json
// because this .cpp-local free fn does not pair at the per-obj scoring level (objdiff
// measures empty) - a tooling/pairing limit, not a body defect.
void create_requests_for_animations(
	configs::binary_config_value const&		cfg,
	const u32								requests_count,
	buffer_vector< resources::request >&	requests
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( requests_count ) );
	for ( u32 i = 0; i < requests_count; ++i )
		requests.push_back( resources::create_request( cfg[i], resources::animation_class ) );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x1 | 77 | requests.push_back( resources::create_request( cfg[i], resources::animation_class ) );
	// VERDICT: STRUCTURE MATCH (3/3) - sole SIZE +0x1 is the create_request call-boundary LTCG arg (target passes animation_class in EDX, base pushes), non-steerable.
}

// STATE[92.91%|DONE]: residual is the trailing query_resources statement (see embed);
// same boost::function-by-value + size() inline-vs-call walls as booby_trap_set_core_cook.
void weapon_user_animations_container_cook::on_config_loaded( resources::queries_result& data )
{
	if ( !data.is_successful( ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		data.get_parent_query( )->finish_query( result_error );
		return;
	}

	buffer_vector< resources::request > requests( ALLOCA( 468 * sizeof( resources::request ) ), 468 );

	configs::binary_config_ptr 			config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const&	root = config->get_root( );

	create_requests_for_animations( root["stand_hud"], 27, requests );
	create_requests_for_animations( root["stand"], 27, requests );
	create_requests_for_animations( root["stand_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["stand_hands_only"], 6, requests );
	create_requests_for_animations( root["aimed_stand_hud"], 27, requests );
	create_requests_for_animations( root["aimed_stand"], 27, requests );
	create_requests_for_animations( root["aimed_stand_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["aimed_stand_hands_only"], 6, requests );
	create_requests_for_animations( root["crouch_hud"], 27, requests );
	create_requests_for_animations( root["crouch"], 27, requests );
	create_requests_for_animations( root["crouch_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["crouch_hands_only"], 6, requests );
	create_requests_for_animations( root["aimed_crouch_hud"], 27, requests );
	create_requests_for_animations( root["aimed_crouch"], 27, requests );
	create_requests_for_animations( root["aimed_crouch_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["aimed_crouch_hands_only"], 6, requests );
	create_requests_for_animations( root["sprint_hud"], 2, requests );
	create_requests_for_animations( root["sprint"], 2, requests );
	create_requests_for_animations( root["jump_hud"], 100, requests );
	create_requests_for_animations( root["jump"], 100, requests );

	ASSERT( UNKNOWN_EXPRESSION );

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &weapon_user_animations_container_cook::on_animations_loaded, this, _1 ),
		g_allocator,
		NULL,
		data.get_parent_query( )
	);

	// STRUCTURE DIFF: target 29 stmts / base 29 stmts
	// SIZE +0x25 | 136 | );
	// VERDICT: STRUCTURE MATCH (29/29) - sole SIZE is the query_resources statement: target copy-constructs the boost::function temp in one templated ctor call while base default-constructs + assign_to (extra spills), and target keeps buffer_vector::size() out-of-line while base inlines it (sub/sar + temp); both whole-program LTCG walls, non-steerable.
}
// STATE[None|WALL]: reads None because the 4 instantiations (<2>,<6>,<27>,<100>) are
// byte-identical and our linker ICF-folds them to ONE symbol while the target keeps 4
// distinct addresses - 3 of 4 cannot pair, and the survivor does not pair at the
// per-obj scoring level (free template fn).
template < u32 count >
void get_animations_from_request_results(
	resources::queries_result const&	data,
	const u32							animations_count,
	u32&								resource_index,
	resources::managed_resource_ptr		(&result)[count]
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	for ( u32 i = 0; i != animations_count; ++i )
		result[i] = static_cast_resource_ptr< resources::managed_resource_ptr >( data[resource_index++].get_managed_resource( ) );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (0x86 both, <100> instantiation) - no diverging rows
	// VERDICT: STRUCTURE MATCH - byte-equal bodies; the None is the ICF-fold pairing artifact above, not a body defect.
}
// STATE[99.97%|DONE]: only the frame-size imm + a trailing nop differ; the get_animations
// call targets resolve to the ICF-folded helper (operand-only, ignored by report fuzzy).
void weapon_user_animations_container_cook::on_animations_loaded( resources::queries_result& data )
{
	if ( !data.is_successful( ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		data.get_parent_query( )->finish_query( result_error );
		return;
	}

	weapon_user_animations_container* container = VOSTOK_NEW_IMPL( g_allocator, weapon_user_animations_container );
	u32 resource_index = 0;

	get_animations_from_request_results( data, 27, resource_index, container->m_stand_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_stand_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_stand_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_stand_hands_only_animations[1] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_stand_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_stand_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_stand_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_stand_hands_only_animations[1] );
	get_animations_from_request_results( data, 27, resource_index, container->m_crouch_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_crouch_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_crouch_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_crouch_hands_only_animations[1] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_crouch_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_crouch_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_crouch_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_crouch_hands_only_animations[1] );
	get_animations_from_request_results( data, 2, resource_index, container->m_sprint_animations[0] );
	get_animations_from_request_results( data, 2, resource_index, container->m_sprint_animations[1] );
	get_animations_from_request_results( data, 100, resource_index, container->m_jump_animations[0] );
	get_animations_from_request_results( data, 100, resource_index, container->m_jump_animations[1] );

	ASSERT( UNKNOWN_EXPRESSION );

	data.get_parent_query( )->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( container ),
		resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_user_animations_container ) )
	);
	data.get_parent_query( )->finish_query( result_success );

	// STRUCTURE DIFF: target 29 stmts / base 29 stmts
	// SIZE -0x6 | 257 | );
	// VERDICT: STRUCTURE MATCH (29/29) - sole SIZE is the set_unmanaged_resource statement's unmanaged_resource_ptr temp shape (target keeps it in a named slot + destroys, base elides), MSVC8 copy-elision/temp class, non-steerable.
}

} // namespace survarium