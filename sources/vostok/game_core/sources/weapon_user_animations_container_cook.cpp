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

	// FUNCTION BODY
	// <0x7629b9>|0x009|+0x013:'20'	VOSTOK_DELETE_IMPL( g_allocator, resource )
	// ******
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

	// FUNCTION BODY[0x7632e0]: 10
	// <0x7632ef>|0x00f|+0x00b:'25'
	// <0x7632fa>|0x01a|+0x01d:'26'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x763317>|0x037|+0x11e:'34'
	// ******
}

// STATE[0%|LTCG]: now EMITTED (on_config_loaded keeps it). EXE-level body matches
// the target except the call-boundary LTCG arg: target passes animation_class (0x3d)
// to the noinline create_request in EDX, base pushes it on the stack (and the 8-byte
// frame-size knock-on). That is the allowed LTCG-argument divergence. Reads 0% in
// report.json because this .cpp-local free fn does not pair at the per-obj scoring
// level (objdiff measures empty) - a tooling/pairing limit, not a body defect.
void create_requests_for_animations(
	configs::binary_config_value const&		cfg,
	const u32								requests_count,
	buffer_vector< resources::request >&	requests
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( requests_count ) );
	for ( u32 i = 0; i < requests_count; ++i )
		requests.push_back( resources::create_request( cfg[i], resources::animation_class ) );

	// FUNCTION BODY
	// <0x762956>|0x006|+0x00c:'41'	ASSERT( ... )
	// <0x762962>|0x012|+0x01a|[1]:'42'	for ( u32 i = 0; i < requests_count; ++i )
	// <0>
	// <0x76297c>|0x02c|+0x02d:'44'	requests.push_back( create_request( cfg[i], animation_class ) )
	// <0>
	// ******
}

// STATE[91.05%|PARTIAL]: probable MSVC8 copy-elision wall (contested between audit lenses).
// At 0x67 the target materializes data[0].get_unmanaged_resource()'s by-value
// unmanaged_resource_ptr into a NAMED slot [ebp-18h] and destroys it (intrusive_ptr::dec)
// after static_cast_resource_ptr; our base elides it. The reviewer pass suspected a
// source-steerable named-local hoist; the structure-verifier then found the source skeleton
// already matches (31==31 statements), so a hoist would ADD a statement and likely worsen it
// - the [ebp-18h]/dec is codegen copy-elision, not a missing source statement. Treat as a
// wall (same class as booby_trap_set_core_cook::on_config_ready ~92%) unless a future attempt
// proves otherwise; see weapon_user_animations_container_cook.md.
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

	// FUNCTION BODY[0x762f50]: 52
	// <0x762f60>|0x010|+0x00f:'50'
	// <0>
	// <0x762f6f>|0x01f|+0x00c:'52'
	// <0x762f7b>|0x02b|+0x013:'53'
	// <0x762f8e>|0x03e|+0x005:'54'
	// <0>
	// <1>
	// <2>
	// <0x762f93>|0x043|+0x027:'58'
	// <0>
	// <0x762fba>|0x06a|+0x022:'60'
	// <0x762fdc>|0x08c|+0x010:'61'
	// <0>
	// <0x762fec>|0x09c|+0x01c:'63'
	// <0x763008>|0x0b8|+0x01c:'64'
	// <0>
	// <0x763024>|0x0d4|+0x01c:'66'
	// <0x763040>|0x0f0|+0x01c:'67'
	// <0>
	// <0x76305c>|0x10c|+0x01c:'69'
	// <0x763078>|0x128|+0x01c:'70'
	// <0>
	// <0x763094>|0x144|+0x01c:'72'
	// <0x7630b0>|0x160|+0x01c:'73'
	// <0>
	// <0x7630cc>|0x17c|+0x01c:'75'
	// <0x7630e8>|0x198|+0x01c:'76'
	// <0>
	// <0x763104>|0x1b4|+0x01c:'78'
	// <0x763120>|0x1d0|+0x01c:'79'
	// <0>
	// <0x76313c>|0x1ec|+0x01c:'81'
	// <0x763158>|0x208|+0x01c:'82'
	// <0>
	// <0x763174>|0x224|+0x01c:'84'
	// <0x763190>|0x240|+0x01c:'85'
	// <0>
	// <0x7631ac>|0x25c|+0x01c:'87'
	// <0x7631c8>|0x278|+0x01c:'88'
	// <0>
	// <0x7631e4>|0x294|+0x01c:'90'
	// <0x763200>|0x2b0|+0x01c:'91'
	// <0>
	// <0x76321c>|0x2cc|+0x00c:'93'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x763228>|0x2d8|+0x08d:'101'
	// ******
}
// STATE[0%|WALL]: EXE-level body matches the target except the get_managed_resource()
// temp materialization (target binds it into [ebp-0Ch] + destroys; base elides via push
// esp) - same MSVC8 copy-elision wall. Additionally reads 0% because the 4 instantiations
// (<2>,<6>,<27>,<100>) are byte-identical and our linker ICF-folds them to ONE symbol
// (base 0x090b40) while the target keeps 4 distinct addresses - so 3 of 4 cannot pair at
// all, and the survivor does not pair at the per-obj scoring level (free template fn).
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

	// FUNCTION BODY[0x762b90]: 3
	// <0x762b97>|0x007|+0x00c:'107'
	// <0x762ba3>|0x013|+0x01a|[1]:'108'
	// <0x762bbd>|0x02d|+0x054:'109'
	// ******
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

	// FUNCTION BODY[0x762c20]: 43
	// <0x762c2c>|0x00c|+0x00f:'114'
	// <0>
	// <0x762c3b>|0x01b|+0x00c:'116'
	// <0x762c47>|0x027|+0x013:'117'
	// <0x762c5a>|0x03a|+0x005:'118'
	// <0>
	// <1>
	// <0x762c5f>|0x03f|+0x051:'121'
	// <0x762cb0>|0x090|+0x007:'122'
	// <0x762cb7>|0x097|+0x01c:'123'
	// <0x762cd3>|0x0b3|+0x01c:'124'
	// <0>
	// <0x762cef>|0x0cf|+0x01c:'126'
	// <0x762d0b>|0x0eb|+0x01c:'127'
	// <0>
	// <0x762d27>|0x107|+0x01c:'129'
	// <0x762d43>|0x123|+0x01c:'130'
	// <0>
	// <0x762d5f>|0x13f|+0x01c:'132'
	// <0x762d7b>|0x15b|+0x01c:'133'
	// <0>
	// <0x762d97>|0x177|+0x01c:'135'
	// <0x762db3>|0x193|+0x01c:'136'
	// <0>
	// <0x762dcf>|0x1af|+0x01c:'138'
	// <0x762deb>|0x1cb|+0x01c:'139'
	// <0>
	// <0x762e07>|0x1e7|+0x01c:'141'
	// <0x762e23>|0x203|+0x01c:'142'
	// <0>
	// <0x762e3f>|0x21f|+0x01c:'144'
	// <0x762e5b>|0x23b|+0x01c:'145'
	// <0>
	// <0x762e77>|0x257|+0x01c:'147'
	// <0x762e93>|0x273|+0x01c:'148'
	// <0>
	// <0x762eaf>|0x28f|+0x01c:'150'
	// <0x762ecb>|0x2ab|+0x01c:'151'
	// <0>
	// <0x762ee7>|0x2c7|+0x00c:'153'
	// <0>
	// <0x762ef3>|0x2d3|+0x038:'155'
	// <0x762f2b>|0x30b|+0x013:'156'
	// ******
}

} // namespace survarium